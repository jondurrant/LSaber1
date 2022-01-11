import paho.mqtt.client as mqtt
import json
import time
import random



targetId = "BCFF4D195C03"
target2Id = "BCFF4D197AE6"
host = "nas3"
port = 1883
user = "mbp"
passwd = "test"

ping_topic = "TNG/" + targetId + "/TPC/PING"
connected_topic = "TNG/" + user + "/LC/ON"

pong_topic = "TNG/" + targetId + "/TPC/PONG"
lc_topic = "TNG/" + targetId + "/LC/#"
state_topics = "TNG/" + targetId + "/STATE/#"
get_topic = "TNG/" + targetId + "/STATE/GET"
set_topic = "TNG/" + targetId + "/STATE/SET"
set2_topic = "TNG/" + target2Id + "/STATE/SET"
upd_topic = "TNG/" + targetId + "/STATE/UPD"

# The callback for when the client receives a CONNACK response from the server.
def on_connect(client, userdata, flags, rc):
    print("Connected with result code "+str(rc))

    # Subscribing in on_connect() means that if we lose the connection and
    # reconnect then subscriptions will be renewed.
    


# The callback for when a PUBLISH message is received from the server.
def on_message(client, userdata, msg):
    print("Rcv topic=" +msg.topic+" msg="+str(msg.payload))
    

def on_disconnect():
    print("DISCONNNECTED")


client = mqtt.Client(client_id=user)
client.username_pw_set(username=user, password=passwd)
client.on_connect = on_connect
client.on_message = on_message
client.on_disconnect = on_disconnect
j = {'online':0}
p = json.dumps(j)
client.will_set(connected_topic, p, qos=1, retain=False) #set will

client.connect(host, port, 60)

client.loop_start()

client.subscribe( lc_topic )
client.subscribe( pong_topic )
client.subscribe( state_topics )
    
print("publishing connect")
j = {'online':1}
p = json.dumps(j)
client.publish(connected_topic,p,retain=False,qos=1)




j = {'delta': { 'trn': 5,
                'drgb':[0, 0, 255], 
                'nrgb':[255, 0, 0],
                'on': True,
                'id': 1,
                'days': 7,
                'daye': 21,
                'dseq': 4,
                'nseq': 0,
                'day': True,
                'timer': 10
               }
            }
p = json.dumps(j)
print("Publishing %s %s"%(set_topic,p))
infot = client.publish(set_topic, p,retain=False, qos=1)
infot.wait_for_publish()

j = {'delta': { 'trn': 5,
                'drgb':[0, 0, 255], 
                'nrgb':[255, 0, 0],
                'on': True,
                'id': 1,
                'days': 7,
                'daye': 21,
                'dseq': 4,
                'nseq': 0,
                'day': True,
                'timer': 10
               }
            }
p = json.dumps(j)
print("Publishing %s %s"%(set2_topic,p))
infot = client.publish(set2_topic, p,retain=False, qos=1)
infot.wait_for_publish()




j = {'GET': 1}
p = json.dumps(j)
print("Publishing  %s"%p)
infot = client.publish(get_topic, p,retain=False, qos=1)
infot.wait_for_publish()

time.sleep(30)