#!/bin/bash
nohup /mnt/partage/tunnel46 &
sleep 1
ip route add 172.16.2.176/28 via 172.16.2.2
