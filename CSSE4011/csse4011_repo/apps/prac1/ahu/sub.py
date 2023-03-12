# python3.6
# import essentail libraries
import random

from paho.mqtt import client as mqtt_client

# set the mqtt broker to publish to broker (https://www.emqx.com/en/blog/how-to-use-mqtt-in-python)
broker = 'broker.emqx.io'
port = 1883
topic = "prac1/"
topic += input("Enter topic: ")
print(topic)
# generate client ID with pub prefix randomly
client_id = f'python-mqtt-{random.randint(0, 100)}'

"""
Function which handles connecting to the mqtt broker connection
"""
def connect_mqtt() -> mqtt_client:
    def on_connect(client, userdata, flags, rc):
        if rc == 0:
            print("Connected to MQTT Broker!")
        else:
            print("Failed to connect, return code %d\n", rc)

    client = mqtt_client.Client(client_id)
    #client.username_pw_set(username, password)
    client.on_connect = on_connect
    client.connect(broker, port)
    return client

"""
function  that handles subcribing to a topics and printing the message
"""
def subscribe(client: mqtt_client):
    def on_message(client, userdata, msg):
        print(f"Received `{msg.payload.decode()}` from `{msg.topic}` topic")

    client.subscribe(topic)
    client.on_message = on_message

"""
function that handles the connection,and subscribers to specified topics
"""
def run():
    client = connect_mqtt()
    subscribe(client)
    client.loop_forever()

"""
main function thar executes script
"""
if __name__ == '__main__':
    run()
