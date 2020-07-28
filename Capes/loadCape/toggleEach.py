#!/usr/bin/env python3
import time

def loadON(i):
    sink = open("/sys/class/leds/load-sink"+str(i)+"/brightness", "w")    
    sink.write("255")
    sink.close()

def loadOFF(i):
    sink = open("/sys/class/leds/load-sink"+str(i)+"/brightness", "w")    
    sink.write("0")
    sink.close()
    
for i in range(1, 9):
    print("Toggling Sink{}".format(i))
    loadON(i)
    time.sleep(0.5)
    loadOFF(i)
    time.sleep(0.5)
