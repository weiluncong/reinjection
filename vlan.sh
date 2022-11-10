#! /bin/bash

sudo vconfig add enp3s0 31
sudo vconfig add enp3s0 4

sudo ifconfig enp3s0.31 172.16.31.13 netmask 255.255.255.0 up
sudo ifconfig enp3s0.4 172.16.4.123 netmask 255.255.255.0 up

sudo su

cd /proc/sys/net/ipv4

sudo echo 31>ip_forward
