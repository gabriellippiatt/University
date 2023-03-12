#import essential libraries
import serial
import json
import os 
import random
import time

from paho.mqtt import client as mqtt_client

# set the mqtt broker to publish to broker (https://www.emqx.com/en/blog/how-to-use-mqtt-in-python)
broker = 'broker.emqx.io'
port = 1883

#array that stores the names of all the topics
topics = []
topics.append("prac1/0x01")
topics.append("prac1/0x02")
topics.append("prac1/0x03")
topics.append("prac1/0x04")
topics.append("prac1/0x05")
topics.append("prac1/0x06")
topics.append("prac1/0x07")
d = {1:[],2:[],3:[],4:[],5:[],6:[],7:[]}
# generate client ID with pub prefix randomly
client_id = f'python-mqtt-{random.randint(0, 1000)}'
# ask user which type of dongle is connected to serial
dongle = input("Dongle? ")
if dongle == 'f':
      ser = serial.Serial("/dev/serial/by-id/usb-FADS_DONGLE_E9D853851B535197-if00", baudrate=115200, timeout=3.0)

elif dongle == 'g':
    ser = serial.Serial("/dev/serial/by-id/usb-FADS_DONGLE_333B04C3F2B81F5A-if00", baudrate=115200, timeout=3.0)
"""
Function which handles connecting to the mqtt broker connection
"""
def connect_mqtt():
    def on_connect(client, userdata, flags, rc):
        if rc == 0:
            print("Connected to MQTT Broker!")
        else:
            print("Failed to connect, return code %d\n", rc)

    client = mqtt_client.Client(client_id)
    client.on_connect = on_connect
    client.connect(broker, port)
    return client
"""
function which checks for the starting index of the json string
skips terminal encoding
"""
def checkforindex(st):
    for i in range(0,len(st)-2):
        if st[i + 1] == ':' and st[i+2] != '~':
            return i
    return -1

"""
function that returns the specific sensor data
"""
def elim(st):
    new = ""
    for i in range(31,len(st)):
        if st[i].isnumeric() or st[i] == '.' or st[i] == '-':
            new += st[i]
    return new


"""
function that handles connecting serial and reading
as well as publishing data to the specified topics
"""
def publish(client):
    msg_count = 0
    while True:
        time.sleep(1)
        serialString = ser.readline()
        sw = serialString.decode('Ascii')
        print(sw)
        esc = ""
        index = checkforindex(sw)
        if index != -1:
            esc = elim(sw)
            esc = esc.replace("1324011110","")
            esc = esc.replace("1324011111324011110","")
            d[int(sw[index])+1].append(esc)
            j = json.dumps(d,indent = 2)
            with open('data.json','w') as f:
                f.write(j)
            msg = esc
            result = client.publish(topics[int(sw[index])], msg)
            status = result[0]
            if status == 0:
                print(f"Send `{msg}` to topic `{topics[int(sw[index])]}`")
            else:
                print(f"Failed to send message to topic {topics[int(sw[index])]}")
            msg_count += 1

"""
function that handles the connection,and publishes data to specified topics
"""
def run():
    client = connect_mqtt()
    client.loop_start()
    publish(client)

"""
main function thar executes script
"""
if __name__ == '__main__':
    run()
