#!/usr/bin/env python3
# //////////////////////////////////////
# 	getset.py
#  Get the value of P8_11 and write it to P9_14. 
#     P8_11 is line 13 on chip 1.  P9_14 is line 18 of chip 1.
# 	Wiring:	Attach a switch to P8_13 and 3.3V and an LED to P9_14.
# 	Setup:	sudo apt uupdate; sudo apt install libgpiod-dev
# 	See:	https://github.com/starnight/libgpiod-example/blob/master/libgpiod-led/main.c
# //////////////////////////////////////
# Based on https://git.kernel.org/pub/scm/libs/libgpiod/libgpiod.git/tree/bindings/python/examples

import gpiod
import sys

CONSUMER='getset'
CHIP='1'
getoffsets=[13] # P8_11
setoffests=[18] # P9_14

chip = gpiod.Chip(CHIP)

getlines = chip.get_lines(getoffsets)
getlines.request(consumer=CONSUMER, type=gpiod.LINE_REQ_DIR_IN)

setlines = chip.get_lines(setoffests)
setlines.request(consumer=CONSUMER, type=gpiod.LINE_REQ_DIR_OUT)

print("Hit ^C to stop")

while True:
    vals = getlines.get_values()
    
    for val in vals:
        print(val, end=' ')
    print('\r', end='')

    setlines.set_values(vals)
