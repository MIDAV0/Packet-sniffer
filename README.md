# A multi-threaded packet sniffer that analyzes packets and detects SYN flood, ARP attacks, and restricted URLs.

View `Report.txt` for details

## Instructions
### Step 1 (Create the VM): Run the following command in the terminal

`/courses/cs241/coursework-multi-test2021 &`
<br>
This will create the VM and boot it. It also creates a copy-on-write (COW) file to automatically store the changes that you make to the VM. The output of the command looks something like

Creating COW file for courseworkFormatting '/dcs/acad/u1872683/cs241-qemu/cs241vm_d11.cow', fmt=qcow2 size=3221225472 backing_file=/modules/cs241/cs241vm_d11_v3.qcow2 cluster_size=65536 lazy_refcounts=off refcount_bits=16
done.
`copying SSH key...
SSH server will start on port 3187, to connect use:
    ssh -p 3187 root@localhost

VNC server will start on port 7187, to connect use ( password= Xaepu9re )
    vncviewer localhost:7187
Running VM...`
<br>
These details give you the information required for logging into the VM using SSH and VNC.

### Step 2 (Log into the VM): You can now log into the VM from the terminal using ssh by issuing the following command

`ssh -p 3187 root@localhost`
<br>
Please note that you should use the port number shown in the output of the previous step (in this case shown above it is 3187).

This command will take you to the command prompt of the VM you have just created. Remember that you have root access to this VM. This is crucial for this coursework as it allows you to access the different network interfaces in the VM. You can see the list of available network interfaces in the VM by issuing the following command

`ifconfig`
<br>
You should see two interfaces named 'eth0' and 'lo' in the VM. The interface 'eth0' is the external interface and is used to access the internet. The other interface 'lo' is the loopback interface.

### Step 3 (Create a working directory): Type the following command to create a working directory called 'cs241' in the home folder of the VM

`mkdir cs241`
<br>
This will be your working directory within the VM.

### Step 4 (Mount the directory containing the skeleton): Now, mount the '/cs241' directory in the DCS system into the '/cs241' directory of the VM using the following command

`sshfs u2113635@remote-35.dcs.warwick.ac.uk:cs241 cs241`
<br>
This command will mount the '/cs241' directory from your DCS system to the '~/cs241' in the VM. Hence, the skeleton should now be available from the VM. From this point onwards, any changes you make to the cs241 directory in the DCS system will be automatically available to the working directory of the VM and vice versa.

### Step 5 (Building the skeleton): In the VM, change to the folder '/cs241/skeleton/src' and run

`make`
<br>
to build the project. This will create a new folder '/cs241/skeleton/build' where the main executable file 'idsniff' will be stored.

### Step 6 (Running the skeleton): To run the skeleton from the 'src' directory issue the following command in the VM

`../build/idsniff -i <interface>`
<br>
Replace <interface> with the name of the interface (either eth0 or lo) at which you wish to capture packets. This will start a packet capturing session at the specified interface. If no interface is specified, i.e., if the -ioption is not used, then the program will assume the default interface name eth0 for the interface. The program will continue to run unless you kill it by pressing Cntrl+c.

The skeleton code will not show any captured packets unless you run it with the -v option to set the verbose flag to 1 as shown below

`../build/idsniff -i <interface> -v 1`
<br>
If the verbose option is used, then the program will print the contents of all packets captured at the interface. This option may be useful for debugging purposes.

The setup process is now complete. If you do not use the VM for a long time, it will shutdown automatically. You can re-boot the VM and log into it again following steps 1 and 2 above. The changes you make to the VM are not lost when the VM shuts down. So when you restart it, you should still see the folder cs241 within the VM. However, you still need to mount the '~/cs241' directory from the DCS system to the VM (Step 4) everytime the VM is restarted.

Re-creating the VM

If you need to re-create your virtual machine for whatever reason, you can do so by killing the VM process (if it is running) and removing the COW file. To kill the VM process, run

`ps -aux | grep $USER | grep kvm`
<br>
This will print the pid of the kvm process running on your account. You can kill these process using

`kill <pid>`
<br>
with <pid> replaced by the actual pid of the kvm process. Then delete the VM by using

`rm -rf ~/cs241-qemu/`
<br>
You can then follow Steps 1-6 above again to set up a fresh copy of the VM.

Note that the skeleton code only starts a packet capturing session on the specified interface. But it doesn't filter any suspicious packet. Your job will be to extend the skeleton to detect some sucpicious packets (as specified below) and prepare a summary of all suspicious activities when the program is killed using Cntrl+c.

Your solution should not require the installation of any additional packages or libraries. However, if you would like to use any additional applications to help you with this coursework, you are welcome to install them. Many common Debian applications can be installed by using apt-get as follows

apt-get install <package_name>
