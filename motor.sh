#!/bin/bash
insmod ./motor_control/driver/urbtc.ko
lsmod | grep urbtc
