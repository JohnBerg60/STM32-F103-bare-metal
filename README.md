# Bare metal STM32 blue pill, VSCode and Dockerized

&nbsp;
## Prerequisites
- STM32F103 dev board (blue pill), and a ST-LINK V2 usb programmer. ST-LINK V2 can be a cheap clone from Amazon or ebay ($ 3). Make sure the ST-LINK driver is installed, open device manager to check. Flashing over a serial link is also possible, but not recommended because there is no debugging over a serial link.
- Assuming Windows 10 with Docker desktop installed and WSL2 enabled. A Mac or Linux machine will work also, but this repo focusses on Windows 10 host. As WSL2 distro, Debian is used.
- VSCode is used as IDE, with tasks configured in .vscode/tasks.json. 
If you want keybindings, install them as a user setting (File->Preferences->Keyboard shortcuts select json file)

&nbsp;
## Checking the dev environment
To check the arm dev environment, open a terminal into the ```.devcontainer``` folder. Rebuild the arm dev environment with:  
 ```docker build -t armdev . ```   
and run it in the background by:   
```docker run -it -d --privileged armdev```  
Check if container is running by ```docker ps```, and terminal into it by using the container id:   
```docker exec -it <id> bash```

&nbsp;
## USB in WSL and Docker
We need to flash devices from the docker container, but USB on the windows machine is not passed to WSL2 or docker containers. Also Windows hyper V does not support this. Virtualbox and VMWare player claim to forward USB devices, but I found this very cumbersome and tedious.

&nbsp;
### Preparations and installing 

&nbsp;
#### Windows
- install WSL2 with your favorite distro, in this document I will use Debian.
- install ```usbipd``` from [github](https://github.com/dorssel/usbipd-win/releases/tag/v1.3.0), this is the deamon that manages sharing on the windows side.
- Prepare a usb device for sharing from windows host. Use ```usbipd list``` to view usb sharing. After disconnecting/connecting a usb device, its state is persisted, in contrast to what the docs say. For binding/unbinding usb devices, you a elevated prompt on Windows host. Binding example:   
```usbipd bind -b 2-2```
- There is a shortcut to share a device to WSL2, so you do not have to mess around with hostnames and networks:   
 ```usbipd wsl attach --busid=<BUSID>```   
 This works as long as a WSL2 session is active. ```lsusb``` on WSL2 side should now show the USB device. After removing/inserting the device, it should be attached again.

&nbsp;
#### WSL 2
- Make sure that the WSL 2 kernel is version > 5.10.60.1 To check open WSL2 terminal and ```uname -a``` If the version is lower, run from elevated prompt on Windows: ```wsl --update```
- install usbip hwdata usbutils on WSL2, if you want to use USB in WSL2. Optional stlink-tools if you want to flash STM32 devices from WSL2.   
```sudo apt install usbip hwdata usbutils stlink-tools```
- WSL comes with its own mDNS, so the Windows host can be reached as ```ping "$(hostname).local"``` There is a firewall in place, so ping does not work, but the name resolves!
- To see a list of shared devices: ```usbip list --remote="$(hostname).local"``` The resolved ip will also work, of course.
- ```sudo usbip attach --remote="$(hostname).local" --busid=<id>``` will attach a device, ```lsusb``` should now show it.   
In case of a ST-LINK V2 a:   
 ```sudo st-info --probe```  
or   
```sudo st-flash --reset write firmware.bin 0x8000000``` should work.
- As usb devices cannot be shared, it needs to be released before using on another machine. On WSL2, verify status with ```usbip port``` and   
``` sudo usbip detach -p 00``` to detach.

&nbsp;
### Using usb in Docker
Docker should be setup as ```Use the WSL 2 based engine```. In that case, all usb from WSL2 will be relayed into each docker container. To access the usb a container should be run with ```--privileged``` flag. No need for any other software or setup. 

## Setting up a development envirement
A docker container can be setup by using the files in the docker folder, by simply renaming docker to .devcontainer, and restart vscode. When asked ```open files in devconatiner``` select yes and your good to go. However, you will be limited to 1 project and have to use the terminal from vscode. Furthermore, you are limited to this project and cannot for examples test some STM32Cube output to see how things are done. It is more conveniant to convert the docker container to wsl2, and go from there.   
To convert:   
```
cd docker
docker build -t armdev .
docker run -it --privileged --entrypoint bash armdev
```
Open a new terminal and export to a tarball:   
```
cd %homepath%
mkdir D:\Distros
docker ps
docker export <container id> > armdev.tar
wsl --import ArmDev D:\Distros .\armdev.tar
wsl --list
del armdev.tar
wsl -d ArmDev
```

When a drive is not mounted, just remount it: ```mkdir /mnt/c``` and then ```mount -t drvfs C: /mnt/c``` It will be persisted through the sessions. For gitconfig, create a link from windows to wsl: ```cd ~ && ln -s /mnt/c/Users/myuser/.gitconfig```   
For ssh keys (for git etc.) you need to copy these over from windows. DO NOT SHARE!   
As for the path, in Debian it is set to a fixed value in /etc/profile. Just add :$PATH to the lines where the path is set.

The docker container is no longer needed, and should be stopped. Reclaim all used space by:   
```
docker system prune -a
```

From here, cd into a folder and start vscode from there with ```code .```. Very conveniant is to install the Terminal from the Microsoft store, and make a tab for the ArmDev wsl2.

&nbsp;
### Debugging
This repo installs the arm remote debugger, with the cortex-debug extension. Set a breakpoint in the source, and run the debugger.

&nbsp;
### Using usbip in VSCode, with devcontainer
The ```.devcontainer``` folder has al the files needed to start a Dockerized development environment.

&nbsp;
## Switching projects
As this repo contains multiple examples on the same board, just edit the Makefile to switch to another project.

&nbsp;
## Useful links
- [Installing on WSL2/Debian](https://superuser.com/questions/1686414/e-unable-to-locate-package-linux-tools-5-4-0-77-generic-on-wsl-debian-11)
- [usbipd on github](https://github.com/dorssel/usbipd-win)
- [st-flash doc and examples](https://www.mankier.com/1/st-flash)
- [Basic explanation of what is going on bare metal](https://karooza.net/going-bare-metal-on-stm32)
- [Good explanation of STM32F1](http://stefanfrings.de/stm32/stm32f1.html)
- [importing a docker images into wsl](https://docs.microsoft.com/en-us/windows/wsl/use-custom-distro)


