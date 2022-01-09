FROM debian:buster-slim

RUN apt-get update

# network stuff
RUN apt-get install -y net-tools iputils-ping

# some handy tools, and of course git
RUN apt-get install -y procps wget bzip2 nano bsdmainutils git 

# install usbip support 
RUN apt-get install -y  usbutils 

# install the arm toolchain
RUN apt-get install -y make gcc-arm-none-eabi stlink-tools

# install debugger
RUN apt-get install -y gdb-multiarch
RUN ln -s /usr/bin/gdb-multiarch /usr/bin/arm-none-eabi-gdb

# a more friendlyer shell config, as we use this in the vscod terminal
COPY .bashrc /root

# copy github access keys
RUN mkdir -p /root/.ssh && \
    chmod 0700 /root/.ssh && \
    ssh-keyscan github.com > /root/.ssh/known_hosts

COPY .ssh/* /root/.ssh/
RUN chmod -fR 600 /root/.ssh/*
