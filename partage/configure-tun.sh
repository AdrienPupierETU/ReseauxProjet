#!/bin/bash

ip addr add $2/28 dev $1
ip link set tun0 up
