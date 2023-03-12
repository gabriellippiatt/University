import serial
import json
import os 
import paho.mqtt.client as mqtt
from random import randrange, uniform
import time
import math
import numpy as np
import statistics
import tkinter as tk
from turtle import color

def untilcom(st,dict):
    val = ""
    counter = 1
    for i in range(0,len(st)):
        if counter > 12:
            break
        if st[i] != ',' and st[i] != '\n' and st[i] != '\r':
            val += st[i]
        else:
            dict[counter].append((val))
            val = ""
            counter += 1
    return dict

def until_rssi(st):
    b = ""
    for i in st:
        if i != ":":
            b += i
        else:
            if b.isdigit() or (b[0] == '-' and b[1:].isdigit()):
                return int(b)
            break
    return -1

d = {1:[],2:[],3:[],4:[],5:[],6:[],7:[],8:[],9:[],10:[],11:[],12:[]}
vals = {1:[],2:[],3:[],4:[],5:[],6:[],7:[],8:[],9:[],10:[],11:[],12:[]}
counter = 0
numx = input("enter x")
numy = input("enter y")
ser = serial.Serial("/dev/serial/by-id/usb-Fardeen_DONGLE_FADS_-_Zephyr_E9D853851B535197-if00", baudrate=115200, timeout=3.0)
while(1):
    serialString = ser.readline()
# Print the contents of the serial data
    sw = serialString.decode('Ascii')
    d = untilcom(sw,d)
    print(d)
    for i in range(1,13):
        vals[i].append(until_rssi(d[i][counter]))
    pub = numx +"," + numy + ","
    for i in range(1,13):
        pub += str(vals[i][counter]) + ","
    pub += "\r\n"
    df = open("data1.txt", "a")
    df.write(pub)
    df.close()
    counter += 1
    
    