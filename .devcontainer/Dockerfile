FROM debian:buster-slim

RUN apt-get update

# network stuff
RUN apt-get install -y net-tools iputils-ping

# some handy tools, and of course git
RUN apt-get install -y procps git wget bzip2 nano

# install usbip support 
RUN apt-get install -y  usbutils 

# install the arm toolchain
RUN apt-get install -y make gcc-arm-none-eabi stlink-tools 

COPY .bashrc /root