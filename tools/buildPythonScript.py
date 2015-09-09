#!/usr/bin/python
import sys
import os
import pdb
import json
import time
import stat
import shutil
import subprocess



blist=[]
binCoreList=[]
binPortList=[]
blackListPorts=[]
whiteListPorts=[]
whiteListCores=[]
blackListCores=[]
userFilePath=None
cpucount=0
portcount=0
portmask=None
coremask=None

def set_envir_variables():
    #currdir = os.getcwd()
    currdir = os.path.dirname(os.path.realpath(sys.argv[0]))
    rPath= os.path.join(currdir,'../../dpdk')
    APath = os.path.abspath(rPath)
    
    os.environ['RTE_SDK'] = APath
    os.environ['RTE_TARGET']="x86_64-native-linuxapp-gcc"
    #print os.environ['RTE_SDK'] 
    #print os.environ['RTE_TARGET'] 
    
def populate_lshw_paramas(): #parsing 'lshw' command in json
    global cpucount,binCoreList, binPortList, portcount,whiteListPorts
    p = subprocess.Popen(['lshw','-json'], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    out, err = p.communicate()
    sysTree = json.loads(out)
    
    for boards in sysTree["children"]:
        for devices in boards["children"]:
            if devices["class"]=="processor":
                #print devices["id"]
                cpucount = cpucount+1
                binCoreList.append('1') 
            if devices["id"]=="pci":
                for pcichild in devices["children"]:
                    if pcichild["class"]=="network":
                        tempdic={}
                        pciaddr = pcichild["handle"]
                        tempdic["pciaddress"]=pciaddr[4:]
                        try:
                            tempdic["logicalname"]=pcichild["logicalname"]
                        except KeyError:
                            tempdic["logicalname"]="eth%d"%portcount
                        whiteListPorts.append(tempdic)
                        binPortList.append('1') 
                        portcount=portcount+1
    
def handle_user_params():
    global cpucount, binPortList, portcount,whiteListPorts,blist

    print "\n----Available ports----"
    for portiter in whiteListPorts:
        print portiter["logicalname"]+" : "+portiter["pciaddress"]

    port_names=[]
    for eth_port in whiteListPorts:
        port_names.append(eth_port["logicalname"])

    print "-------------------------"
    print "Atleast one port must be associated with Linux\n"
    portParameters = raw_input('Mention the Ports separted by space to be associated with Linux ( type eth<x> ) : ')
    blackportlist = portParameters.split()
    for blackport in blackportlist:
        if blackport not in port_names:
            print "Invalid port name " , blackport
            return 1

    blackPortIndices=[]
    for i in blackportlist:
        blackPortIndices.append(int (i[3:]))        

    for ind in blackPortIndices:
        blist.append(whiteListPorts[ind])
        binPortList[ind]='0'
        
    newwhiteList = [v for i, v in enumerate(whiteListPorts) if i not in blackPortIndices]
    whiteListPorts = newwhiteList
    if len(whiteListPorts)>0:
        for portiter in range(len(whiteListPorts)):
            whiteListPorts[portiter]["logicalname"]="tw%d"%portiter

def mask_calculation():
    global binCoreList, binPortList,portmask,coremask

#Core Mask Calculation                            
    l= len (binCoreList)
    binCoreList[l-1]='0'
    joined = ''.join(binCoreList)
    coremask = hex( int(joined,2) )

#Port Mask Calculation
    binPortList = list(reversed(binPortList))
    joined = ''.join(binPortList)
    portmask= hex ( int(joined,2) )
    

def bind_ports():
    global whiteListPorts
    currdir = os.getcwd()
    nicbindpath= os.path.join(currdir,'../../dpdk/tools/dpdk_nic_bind.py')
    if len(whiteListPorts) > 0:
        for interfacedic in whiteListPorts:
            print "Assigning "+ interfacedic["logicalname"]+":"+interfacedic["pciaddress"]+" to Twister."
            nicprocess = subprocess.Popen(['python',nicbindpath,'--bind=igb_uio', interfacedic["pciaddress"]], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
            stdout = nicprocess.communicate()[0]
            print 'STDOUT:{}'.format(stdout)
    else:
        print "!!WARNING!! No Port is assigned To Twister"
        
def display_summary():
    global cpucount,whiteListPorts,portmask,coremask,blist
    time.sleep(1)
    print "*********SUMMARY*************"
    print "\nEngines Available for Twister: "
    for cpu in range(cpucount-1):
        print "engine%d"%cpu
    print "coremaskhex = "+str(coremask)
    
    print "\nPORTS Available for Twister:"
    for interfacedic in whiteListPorts:
        print interfacedic["logicalname"]+" : "+interfacedic["pciaddress"]

    print "PORTS Available for Linux"
    for blistport in blist:
        print blistport["logicalname"]+" : "+blistport["pciaddress"]
#    print "PORTmaskhex = "+str(portmask)
    
    
def output_file():
    permission = raw_input('.......................\n Do you want to generate a buildscript for an application?.\
                                         Press 1 for Yes and 0 for No : ') 
    if permission=='1':
    
        userfile = raw_input('.......................\n Enter the name of the app to genrate a build script\n') 
        global portmask,coremask,blist
        blackliststring=''
        for black in blist:        
            blackliststring=blackliststring + ' -b '+ black['pciaddress']
        
        file_string = """#!/bin/bash
        if pidof -x "{3}" >/dev/null;
        then 
            echo \"Process already running\" 
        else
            echo \"Process not running, starting...\" 
            screen -dm ./{3} -c{0} -n 4{1} -- -p {2}
            while :
            do
                sleep 1
                if pidof -x \"{3}\">/dev/null;
                then
                 break
                else
                 screen -dm ./{3} -c{0} -n 4{1} -- -p {2}
                fi
            done
        fi """.format (coremask, blackliststring, portmask, userfile )
        filename = userfile+".sh"
        srcfilepath = os.path.join( os.path.dirname(os.path.realpath(sys.argv[0])) ,  filename)     
        f = open( srcfilepath , 'w')
        f.write(file_string)
        f.close()
        st = os.stat(srcfilepath )
        os.chmod(srcfilepath, st.st_mode | stat.S_IEXEC)
        print srcfilepath
        print os.getcwd()
        if ( os.path.dirname(srcfilepath) != os.getcwd()):
            shutil.copy(srcfilepath, os.getcwd())
        destfilepath = os.path.join(os.getcwd(), filename)
        if destfilepath!=srcfilepath:
            os.remove(srcfilepath)
        print "\n A build script named "+filename+" is placed at "+ os.getcwd()    
    
        
def main():
    set_envir_variables()
    populate_lshw_paramas()
    handle_user_params()
    mask_calculation()
    bind_ports()
    display_summary()
    output_file()
    

if __name__ == "__main__":
    main()













