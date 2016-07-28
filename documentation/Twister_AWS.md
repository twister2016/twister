Run Twister on AWS
==================

This guide describes the process to run Twister on Amazon EC2 virtual machine. Please go through following steps to run Twister on AWS.
```
1.	Create VPC and Networks
2.	Creating an instance on Amazon Web Services EC2
3.	Accessing Twister instance
4.	Attaching interfaces to EC2 machine

Each of these parts are explained in detail below
```
 

1. Creating VPC and Networks
----------------------------

- Go to [AWS Management Console](https://aws.amazon.com/console)

 ![alt tag] (https://cloud.githubusercontent.com/assets/3003907/14815313/142f3cd2-0bc4-11e6-973f-df595a3615d7.png)

-	Click on sign in button and log in with AWS credentials.
A dashboard will open up with list of all AWS components. Click on `VPC` service in Networking section. This service is usually listed around bottom of the list, as also depicted in the figure below:

![alt tag] (https://cloud.githubusercontent.com/assets/3003907/14815377/7e7d3c88-0bc4-11e6-84b1-7afce65d3de3.png)

-	Go to `Your VPC` in the left panel and click on `Create VPC` button.

![alt tag](https://cloud.githubusercontent.com/assets/3003907/14815387/8c09c54c-0bc4-11e6-9f16-57ec7b8efade.png)

-	A pop up window will open. Provide a VPC name and a valid CIDR block. Click on `Yes, Create` button.

![alt tag](https://cloud.githubusercontent.com/assets/3003907/14815391/94f08966-0bc4-11e6-871a-a318e6ea2c3e.png)

- Check the newly created VPC, and select `Edit DNS Hostnames` button from drop down menu under `Actions`

![alt tag](https://cloud.githubusercontent.com/assets/3003907/14815430/cccfc590-0bc4-11e6-8737-c74a8c075722.png)

- Check `Yes` radio button in `Edit DNS Hostnames` window.

![alt tag](https://cloud.githubusercontent.com/assets/3003907/14815432/d19c4b3e-0bc4-11e6-8ad1-9804b48c9d60.png)

-	Select `Subnets` in left panel and click on `Create Subnet`

![alt tag](https://cloud.githubusercontent.com/assets/3003907/14815446/dddcda94-0bc4-11e6-9509-c1ddcf806853.png)




- A pop up window will open. Create a subnet for accessing Twister VMs. Provide subnet name e.g. “Twister Access”, in `VPC` box select the VPC created in last step. Select any availability zone and a CIDR block.

![alt tag](https://cloud.githubusercontent.com/assets/3003907/14815451/e3e064b0-0bc4-11e6-818c-1e60e5bbdc27.png)


**Note:** 
```
It is more convenient to choose any one of the availability zone and stick to that availability zone
for creation of rest of the resources in a VPC.
```

- Similarly create another subnet for Twister Data Network.
 
 ![alt tag](https://cloud.githubusercontent.com/assets/3003907/14815454/ec1d0f5c-0bc4-11e6-8d84-e0a8e5412777.png)

- Check “Twister Access Subnet” and select `Modify Auto Assign Public IP` from drop menu in `Subnet Actions`
 
 ![alt tag](https://cloud.githubusercontent.com/assets/3003907/14815460/f64a66a0-0bc4-11e6-9c73-c6699f36e07d.png)

- Check the box for Enable auto assign public IP.
 
 ![alt tag](https://cloud.githubusercontent.com/assets/3003907/14815462/f8d5f84e-0bc4-11e6-923f-20216b56d3b2.png)

- Go to `Internet Gateways` in left panel and click on `Create Internet Gateway`. Provide a name and click on `Yes, Create` button. 

![alt tag](https://cloud.githubusercontent.com/assets/3003907/14815472/01891214-0bc5-11e6-99c1-0abc309776df.png)
 

- Now check the newly created gateway and click on `Attach to VPC` button. Select the Twister VPC and click on `Yes, Attach`.

 ![alt tag](https://cloud.githubusercontent.com/assets/3003907/14815477/0a5da08a-0bc5-11e6-83f4-41b6fc4df524.png)

- Now go to routes table in in left panel. Check the Twister VPC and click on “Edit” button under `Routes` tab as shown in figure below.

![alt tag](https://cloud.githubusercontent.com/assets/3003907/14815486/104a633e-0bc5-11e6-8403-38603f274d2d.png)
 

- Add 0.0.0.0/0 in destination. Select ‘Twister Gateway’ for Target box and click on save button.

 ![alt tag](https://cloud.githubusercontent.com/assets/3003907/14815490/16d78326-0bc5-11e6-886a-c2e60aa2b055.png)
 
- Now go to `Subnet Associations` tab, click on `Edit` button and check the two subnets created in previous steps.

 ![alt tag](https://cloud.githubusercontent.com/assets/3003907/14815492/1a88a54a-0bc5-11e6-8401-1e23e3ecfd7c.png)

- In VPC dashboard, go to `Security Group` tab under `Security` panel. Click on `Create Security Group` button.

 ![alt tag](https://cloud.githubusercontent.com/assets/3003907/15205852/3842d576-1834-11e6-838e-59f9c6041b18.png)

- Provide name for security groug and select VPC from drop down menu. Also add rules to allow TCP, UDP and ICMP traffic as shown in figure below.
 ![alt tag](https://cloud.githubusercontent.com/assets/3003907/15205853/39332954-1834-11e6-86ed-72da6d5ec18d.png)

- Now to EC2 dashboard, go to `Key Pairs` tab in `Network & Security` panel. Click on `Create Key Pair`.

 ![alt tag](https://cloud.githubusercontent.com/assets/3003907/15205857/3a706796-1834-11e6-8d19-a08e87363443.png)

- Create a new keypair and save the ".pem" key generated. This keypair will be used to create access EC2 instances.   
 ![alt tag](https://cloud.githubusercontent.com/assets/3003907/15205858/3b525e4e-1834-11e6-83bc-6ba34a61733f.png)

2.	Creating an instance on Amazon Web Services EC2
---------------------------------------------------


- Click on `EC2` service in compute section from AWS portal. This service is usually listed as first service, as also depicted in the figure below:

 ![alt tag](https://cloud.githubusercontent.com/assets/3003907/14815818/fe4f7564-0bc6-11e6-95bf-185d285acf32.png)

- Click on Launch instance button to create a new virtual machine:
 
 ![alt tag](https://cloud.githubusercontent.com/assets/3003907/14815819/00b39f92-0bc7-11e6-8f5d-a3f49cf6b58b.jpg)

- Choose Amazon Machine Image (AMI) for launching an instance.

Twister should work on all operating systems that are supported by DPDK. 

[See list of DPDK support OS.]
(http://dpdk.org/doc/guides/rel_notes/supported_os.html)

Twister should work on any 64bit Amazon linux image which support hardware virtual machine (HVM). Following HVM images have been tested on Amazon EC2.

```
Ubuntu Server 14.04 LTS (HVM), SSD Volume Type
Amazon Linux AMI 2016.03.1 (HVM), SSD Volume Type
```
Following Amazon Images should work with Twister as they meet the requirements of Twister. But they have not been tested yet.

```
Red Hat Enterprise Linux 7.2 (HVM), SSD Volume Type
SUSE Linux Enterprise Server 12 SP1 (HVM), SSD Volume Type
```
Select `Ubuntu Server 14.04 LTS (HVM), SSD Volume Type` or `Amazon Linux AMI 2016.03.1 (HVM), SSD Volume Type` (64 bit) image from the list. 


![alt tag](https://cloud.githubusercontent.com/assets/3003907/14815821/03430ef0-0bc7-11e6-9ced-3321d83a97c6.jpg)
 

- In step 2, choose an instance type from the menu.

The NICs connected with Virtual Machine should either be virtual functions or other DPDK supported NICs. 
[See the list of DPDK supported NICS]
(http://dpdk.org/doc/nics)

In Instance Types, only `Compute Optimized` Family supports virtual functions. Other Instance Type Families connect xen based NICs which are not supported by Twister.
For ease of use `c4.xlarge` instance type is selected here.

 ![alt tag](https://cloud.githubusercontent.com/assets/3003907/14815824/05b384f8-0bc7-11e6-8cb1-76e2a82d49c1.png)

- In step 3 (configure instance details), select “Twister VPC” and “Twister Access subnet” in respective boxes. Also select `Enable` for auto assigning public IP.
 
 ![alt tag](https://cloud.githubusercontent.com/assets/3003907/14815829/08ec8a7a-0bc7-11e6-9449-d7e2cc143507.png)

- In step 3 (configure instance details), go to `Advanced Details` and add following lines in text box labelled as `User data`.
-In case of Ubuntu AMI: 

```bash
#!/bin/bash
apt-get update -y
apt-get upgrade -y
apt-get install linux-generic linux-headers-generic make gcc gdb git unzip python-netaddr -y
bash -c "echo vm.nr_hugepages = 1024 >> /etc/sysctl.conf"
mkdir -p /mnt/huge
sed -i -e '$i \mount -t hugetlbfs nodev /mnt/huge &\n' /etc/rc.local
reboot
```
-In case of Amazon Linux or CentOS AMI:

```bash
#!/bin/bash
yum update -y
yum install make gcc gdb git unzip kernel-devel redhat-lsb python-pip -y
pip install --upgrade netaddr
bash -c "echo vm.nr_hugepages = 1024 >> /etc/sysctl.conf"
mkdir -p /mnt/huge
sed -i -e '$i \mount -t hugetlbfs nodev /mnt/huge &\n' /etc/rc.local
reboot
```

These commands in user-data will install the software dependecies required for twister, configure hugepages creation at boot time and reboot the VM for the changes to take effect. 

 ![alt tag](https://cloud.githubusercontent.com/assets/3003907/14815832/0bd3bd58-0bc7-11e6-887b-36f900d36ca2.png)

- Skip step 4 (`Add storage`) and step 5 (`Tag instance`) as there are no configurations required for them.
- Go to Step 6 (`Configure Security Group`), click on `Choose an existing security group` and select the security group created in previous step.
  
   ![alt tag](https://cloud.githubusercontent.com/assets/3003907/15206071/cf30215e-1835-11e6-8e45-8b591fab7cf2.png)

- Click on Launch button. A window will pop up to select a key pair. Select `Choose an existing key pair` and select key pair created in previous section. Otherwise create a new key pair and download it.

 ![alt tag](https://cloud.githubusercontent.com/assets/3003907/15205861/3d55537c-1834-11e6-9d65-0f8e026dd026.png)

- After selecting a key pair, tick the acknowledgement check box and click on `Launch Instances` button. Dashboard will redirect to Launch Status page. Click on the instance link given on this page as shown in figure below.

 ![alt tag](https://cloud.githubusercontent.com/assets/3003907/14815843/14049420-0bc7-11e6-9b03-47c1c03107b9.png)

- Instance can be viewed in instances tab now. Hover over `Name` Column for newly created instance and click on the edit button to give it a more readable name.

![alt tag](https://cloud.githubusercontent.com/assets/3003907/14815845/166c6b52-0bc7-11e6-97a3-e6210bbe6250.png)

After creation of the machine, installation of updates may take 4-5 minutes. In order to view the progress of installation procedure, log in to the machine and run following command.
```
tail /var/log/cloud-init-output.log
```

3.	Accessing Twister instance
------------------------------

- Go to instances tab in left panel and select the Twister virtual machine. Click on `Connect` button on top.


- In order to access the instance, go to instances tab from left panel, select the virtual machine just created and click on `Connect` button. A window will pop up, showing ssh access credentials. Use the key generated in pervious steps to SSH into VM. 
 
If you are in Windows machine, [connect using PuTTY](https://docs.aws.amazon.com/AWSEC2/latest/UserGuide/putty.html).
For Linux users, change permission of key pair and SSH with the using Public DNS as shown in figure below.

![alt tag](https://cloud.githubusercontent.com/assets/3003907/16298682/27b3e50a-3950-11e6-9d55-22277cc07ba9.png)

4.	Attaching interfaces to EC2 machine
---------------------------------------

- Go to `Network Interfaces` tab under `Network & Security` section in left panel. Click on `Create Network Interface`.

![alt tag](https://cloud.githubusercontent.com/assets/3003907/14815850/1ce14b7e-0bc7-11e6-9dab-efcf0b8ef963.png)
 
- Select Twister Data subnet (created in VPC and networks section) from drop down menu. Select security group `Twister Security Group` created in previous steps. Click on `Yes, Create` button.

```Note down the subnet address of the subnet specified. It will be later used in configuring Twister ports. ```

![alt tag](https://cloud.githubusercontent.com/assets/3003907/16220914/3970949e-37a9-11e6-889b-51c17cfe45bc.png)

- Now select the newly created interface and click on `Attach` button. Select Twister virtual machine, created in previous steps. Click on `Attach` button.

![alt tag](https://cloud.githubusercontent.com/assets/3003907/14815853/21103b9c-0bc7-11e6-849d-e9088bb0afca.png)
 
- Note down the IP address of attached NIC. IP address is generally listed in `Details` tab in `Network Interfaces` panel. As shown in figure below:

![alt tag](https://cloud.githubusercontent.com/assets/3003907/16221070/2c158c0e-37aa-11e6-9304-349f46920c45.png)

__Note:__
```
* IP address, netmask and gateway would be required in next steps to configure Twister ports
* Netmask can be calculated from the subnet address to Twister data network. 
  For example, netmask for 10.0.2.0/24 subnet would be 255.255.255.0
* Generally gateway of the subnet in AWS is the first address in the network.
  For example, gateway for 10.0.2.0/24 subnet would be 10.0.2.1
```
