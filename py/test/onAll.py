import paho.mqtt.client as mqtt
import json
import time
import sys
import os

user=os.environ.get("MQTT_USER", "nob")
passwd=os.environ.get("MQTT_PASSWD", "nob")
host= os.environ.get("MQTT_HOST", "localhost")
port=int(os.environ.get("MQTT_PORT", "1883"))
print("MQTT %s:%d - %s\n"%(host,port, user))

on_topic = "GRP/saber/TPC/on"
connected_topic = "TNG/" + user + "/LC/ON"

pong_topic = "TNG/+/TPC/PONG"
lc_topic = "TNG/+/LC/#"
state_topic = "TNG/+/STATE/#"
grp_topic = "GRP/saber/TPC/#"

# The callback for when the client receives a CONNACK response from the server.
def on_connect(client, userdata, flags, rc):
    print("Connected with result code "+str(rc))

    # Subscribing in on_connect() means that if we lose the connection and
    # reconnect then subscriptions will be renewed.
    


# The callback for when a PUBLISH message is received from the server.
def on_message(client, userdata, msg):
    print("Rcv topic=" +msg.topic+" msg="+str(msg.payload))

client = mqtt.Client(client_id=user)
client.username_pw_set(username=user, password=passwd)
client.on_connect = on_connect
client.on_message = on_message
j = {'online':0}
p = json.dumps(j)
client.will_set(connected_topic, p, qos=1, retain=False) #set will

client.connect(host, port, 60)

client.loop_start()

'''
client.subscribe( lc_topic )
client.subscribe( pong_topic )
client.subscribe( state_topic )
'''
client.subscribe( grp_topic )
    
print("publishing connect")
j = {'online':1}
p = json.dumps(j)
client.publish(connected_topic,p,retain=False,qos=1)

id = 0
j = {'id': id}
p = json.dumps(j)
print("Publishing ON %s"%p)
infot = client.publish(on_topic, p,retain=False, qos=1)
infot.wait_for_publish()


time.sleep(30)