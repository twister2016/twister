Run Twister on AWS
==================

This guide describes the process to run Twister on Amazon EC2 virtual machine. Please go through following steps to run Twister on AWS.

1.	Create VPC and Networks
2.	Creating an instance on Amazon Web Services EC2
3.	Accessing Twister instance
4.	Attaching interfaces to EC2 machine

Each of these parts are explained in detail below

 

1. Creating VPC and Networks
----------------------------

- Go to [AWS Management Console](https://aws.amazon.com/console)

 ![alt tag] (https://cloud.githubusercontent.com/assets/3003907/14815313/142f3cd2-0bc4-11e6-973f-df595a3615d7.png)

-	Click on sign in button and log in with AWS credentials.
A dashboard will open up with list of all AWS components. Click on “VPC” service in Networking section. This service is usually listed around bottom of the list, as also depicted in the figure below:

![alt tag] (https://cloud.githubusercontent.com/assets/3003907/14815377/7e7d3c88-0bc4-11e6-84b1-7afce65d3de3.png)

-	Go to “Your VPC” in the left panel and click on “Create VPC” button.

![alt tag](https://cloud.githubusercontent.com/assets/3003907/14815387/8c09c54c-0bc4-11e6-9f16-57ec7b8efade.png)

-	A pop up window will open. Provide a VPC name and a valid CIDR block. Click on “Yes, Create” button.

![alt tag](https://cloud.githubusercontent.com/assets/3003907/14815391/94f08966-0bc4-11e6-871a-a318e6ea2c3e.png)

- Check the newly created VPC, and select “Edit Hostnames” button from drop down menu under “Actions”

![alt tag](https://cloud.githubusercontent.com/assets/3003907/14815430/cccfc590-0bc4-11e6-8737-c74a8c075722.png)

- Check “Yes” radio button in “Edit Hostnames” window.

![alt tag](https://cloud.githubusercontent.com/assets/3003907/14815432/d19c4b3e-0bc4-11e6-8ad1-9804b48c9d60.png)

-	Select “Subnets” in left panel and click on “Create Subnet”

![alt tag](https://cloud.githubusercontent.com/assets/3003907/14815446/dddcda94-0bc4-11e6-9509-c1ddcf806853.png)




- A pop up window will open. Create a subnet for accessing Twister VMs. Provide subnet name e.g. “Twister Access”, in “VPC” box select the VPC created in last step. Select any availability zone and a CIDR block.

![alt tag](https://cloud.githubusercontent.com/assets/3003907/14815451/e3e064b0-0bc4-11e6-818c-1e60e5bbdc27.png)


**Note: It is more convenient to choose any one of the availability zone and stick to that availability zone for creation of rest of the resources in a VPC.**


- Similarly create another subnet for Twister Data Network.
 
 ![alt tag](https://cloud.githubusercontent.com/assets/3003907/14815454/ec1d0f5c-0bc4-11e6-8d84-e0a8e5412777.png)

- Check “Twister Access Subnet” and select “Modify Auto Assign Public IP” from drop menu in “Subnet Actions”
 
 ![alt tag](https://cloud.githubusercontent.com/assets/3003907/14815460/f64a66a0-0bc4-11e6-9c73-c6699f36e07d.png)

- Check the box for Enable auto assign public IP.
 
 ![alt tag](https://cloud.githubusercontent.com/assets/3003907/14815462/f8d5f84e-0bc4-11e6-923f-20216b56d3b2.png)

- Go to “Internet Gateways” in left panel and click on “Create Internet Gateway”. Provide a name and click on “Yes, Create” button. 

![alt tag](https://cloud.githubusercontent.com/assets/3003907/14815472/01891214-0bc5-11e6-99c1-0abc309776df.png)
 

- Now check the newly created gateway and click on “Attach to VPC” button. Select the Twister VPC and click on “Yes, Attach”.

 ![alt tag](https://cloud.githubusercontent.com/assets/3003907/14815477/0a5da08a-0bc5-11e6-83f4-41b6fc4df524.png)

- Now go to routes table in in left panel. Check the Twister VPC and click on “Edit” button under “Routes” tab as shown in figure below.

![alt tag](https://cloud.githubusercontent.com/assets/3003907/14815486/104a633e-0bc5-11e6-8403-38603f274d2d.png)
 

- Add 0.0.0.0/0 in destination. Select ‘Twister Gateway’ for Target box and click on save button.

 ![alt tag](https://cloud.githubusercontent.com/assets/3003907/14815490/16d78326-0bc5-11e6-886a-c2e60aa2b055.png)
 
- Now go to “Subnet Associations” tab, click on “Edit” button and check the two subnets created in previous steps.

 ![alt tag](https://cloud.githubusercontent.com/assets/3003907/14815492/1a88a54a-0bc5-11e6-8401-1e23e3ecfd7c.png)







2.	Creating an instance on Amazon Web Services EC2
---------------------------------------------------

- Click on sign in button and log in with AWS credentials.

![alt tag](https://cloud.githubusercontent.com/assets/3003907/14815313/142f3cd2-0bc4-11e6-973f-df595a3615d7.png)

- A dashboard will open up with list of all AWS components. Click on “EC2” service in compute section. This service is usually listed as first service, as also depicted in the figure below:

 ![alt tag](https://cloud.githubusercontent.com/assets/3003907/14815818/fe4f7564-0bc6-11e6-95bf-185d285acf32.png)

- Click on Launch instance button to create a new virtual machine:
 
 ![alt tag](https://cloud.githubusercontent.com/assets/3003907/14815819/00b39f92-0bc7-11e6-8f5d-a3f49cf6b58b.jpg)

- A list will be given to choose Amazon Machine image. Select “Ubuntu Server 14.04 LTS (HVM), SSD Volume Type” (64 bit) image. 
![alt tag](https://cloud.githubusercontent.com/assets/3003907/14815821/03430ef0-0bc7-11e6-9ced-3321d83a97c6.jpg)
 

- In step 2, you will be prompted choose instance type. Any instance type from “compute optimized” family can be used. For ease of use “c4.xlarge” instance type is selected here.

 ![alt tag](https://cloud.githubusercontent.com/assets/3003907/14815824/05b384f8-0bc7-11e6-8cb1-76e2a82d49c1.png)

- In step 3 (configure instance details), select “Twister VPC” and “Twister Access subnet” in respective boxes. Also select “Enable” for auto assigning public IP.
 
 ![alt tag](https://cloud.githubusercontent.com/assets/3003907/14815829/08ec8a7a-0bc7-11e6-9449-d7e2cc143507.png)

- In step 3 (configure instance details), go to “Advanced Details” and add following lines in text box labelled as “User data”. 
```bash
#!/bin/bash
apt-get update -y
apt-get upgrade -y
apt-get install linux-generic linux-headers-generic make gcc gdb git unzip -y
bash -c "echo vm.nr_hugepages = 1024 >> /etc/sysctl.conf"
reboot
```
 ![alt tag](https://cloud.githubusercontent.com/assets/3003907/14815832/0bd3bd58-0bc7-11e6-887b-36f900d36ca2.png)

- Skip step 4 (Add storage) and step 5 (Tag instance) as there is no configurations required for them.
- Go to Step 6 (Configure Security Group), create a new security group for Twister. Add rules for ICMP and SSH traffic, as shown in figure below:
  
   ![alt tag](https://cloud.githubusercontent.com/assets/3003907/14815835/0e9be79a-0bc7-11e6-9183-f22e658f3d94.png)

- Click on Launch button. A window will pop up to select a key pair. Select an existing key pair if you have created it earlier. Otherwise create a new key pair and download it.

 ![alt tag](https://cloud.githubusercontent.com/assets/3003907/14815840/1119cd66-0bc7-11e6-89db-03331d379987.png)

- After selecting a key pair, tick the acknowledgement check box and click on “Launch Instances” button. Dashboard will redirect to Launch Status page. Click on the instance link given on this page as shown in figure below.

 ![alt tag](https://cloud.githubusercontent.com/assets/3003907/14815843/14049420-0bc7-11e6-9b03-47c1c03107b9.png)

- Instance can be viewed in instances tab now. Hover over “Name” Column for newly created instance and click on the edit button to give it a more readable name.

![alt tag](https://cloud.githubusercontent.com/assets/3003907/14815845/166c6b52-0bc7-11e6-97a3-e6210bbe6250.png)

3.	Accessing Twister instance
------------------------------

- Go to instances tab in left panel and select the Twister virtual machine. Click on “Connect” button on top.


- In order to access the instance, go to instances tab from left panel, select the virtual machine just created and click on “Connect” button. A window will pop up, showing ssh access credentials. Use the key generated in pervious steps to SSH into VM using your favorite SSH client

![alt tag](https://cloud.githubusercontent.com/assets/3003907/14815848/1924ef18-0bc7-11e6-95e6-5ca584386733.png)

4.	Attaching interfaces to EC2 machine
---------------------------------------

- Go to “Network Interfaces” tab under “Network & Security” section in left panel. Click on “Create Network Interface”.

![alt tag](https://cloud.githubusercontent.com/assets/3003907/14815850/1ce14b7e-0bc7-11e6-9dab-efcf0b8ef963.png)
 
- Select Twister Data subnet (created in VPC and networks section) from drop down menu. Select security group “Twister Security Group” created in previous steps. Click on “Yes, Create” button.

 ![alt tag](https://cloud.githubusercontent.com/assets/3003907/14815851/1f172ddc-0bc7-11e6-8f48-e2f67ae4df31.png)


- Now select the newly created interface and click on “Attach” button. Select Twister virtual machine, created in previous steps. Click on “Attach” button.

![alt tag](https://cloud.githubusercontent.com/assets/3003907/14815853/21103b9c-0bc7-11e6-849d-e9088bb0afca.png)
 






