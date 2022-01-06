FROM debian:buster-slim

RUN apt-get update
RUN apt-get install -y build-essential gcc-arm-none-eabi stlink-tools usbutils git

COPY .bashrc /root