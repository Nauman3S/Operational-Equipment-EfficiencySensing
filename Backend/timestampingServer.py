from datetime import datetime
import paho.mqtt.client as mqtt
import random
import time


msgV = ""
topicV = ""


def getTS():

    date_time = datetime.now().strftime("%S;%M;%H;%d;%m;%Y;%f")
    print(date_time)
    return str(date_time)


def on_connect(client, userdata, rc):
    print("Connected with result code "+str(rc))
    # Subscribing in on_connect() means that if we lose the connection and
    # reconnect then subscriptions will be renewed.
    client.subscribe("OEEDevice/dev/config")


# The callback for when a PUBLISH message is received from the server.
def on_message(client, userdata, msg):

    global msgV, topicV, voltsData, freqData, appState
    print(msg.topic+" "+str(msg.payload))
    topicV = str(msg.topic)
    msgV = str((msg.payload).decode('utf-8'))

    if(topicV == 'OEEDevice/dev/config'):
        print(msgV)


clientID_prefix = ""
for i in range(0, 6):
    clientID_prefix = clientID_prefix + str(random.randint(0, 99999))


client = mqtt.Client("OEEDevice"+clientID_prefix)
#client.on_connect = on_connect
client.on_message = on_message

client.connect("broker.hivemq.com", 1883, 60)
client.subscribe("OEEDevice/dev/config")


client.loop_start()

while 1:
    try:
        client.publish("OEEDevice/dev/timestamp", getTS())
        time.sleep(10)
    except:
        print('some error occurred')
