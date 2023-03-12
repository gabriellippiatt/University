import serial
import json
import os 
import paho.mqtt.client as mqtt
from random import randrange, uniform
import time

def checkforindex(st):
    for i in range(0,len(st)-2):
        if st[i + 1] == ':' and st[i+2] != '~':
            return i
    return -1

def elim(st):
    new = ""
    for i in range(31,len(st)):
        if st[i].isnumeric() or st[i] == '.' or st[i] == '-':
            new += st[i]
    return new


d = {1:[],2:[],3:[],4:[],5:[],6:[],7:[]}
ser = serial.Serial("/dev/serial/by-id/usb-FADS_DONGLE_333B04C3F2B81F5A-if00", baudrate=115200, timeout=3.0)
# if os.path.isfile("/dev/serial/by-id/usb-FADS_DONGLE_E9D853851B535197-if00"): 
#     ser = serial.Serial("/dev/serial/by-id/usb-FADS_DONGLE_E9D853851B535197-if00", baudrate=115200, timeout=3.0)
# elif os.path.isfile("/dev/serial/by-id/usb-FADS_DONGLE_333B04C3F2B81F5A-if00"):
#     ser = serial.Serial("/dev/serial/by-id/usb-FADS_DONGLE_333B04C3F2B81F5A-if00", baudrate=115200, timeout=3.0)
# print(ser.name)
while(1):
    serialString = ser.readline()
    # Print the contents of the serial data
    sw = serialString.decode('Ascii')
    print(sw)
    esc = ""
    #print(esc)
    index = checkforindex(sw)
    if index != -1:
        esc = elim(sw)
        #print(esc)
        d[int(sw[index])+1].append(esc)
        print(d)
        j = json.dumps(d,indent = 2)
        with open('data.json','w') as f:
            f.write(j)
       # d[int(sw[0])].append(serialString.decode('Ascii'))
   #print(sw)      # check which port was really used
# print(ser.readline())
ser.close()
f.close()