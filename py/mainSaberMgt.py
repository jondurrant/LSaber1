from flask import Flask,flash, redirect, request, send_from_directory
from flask import url_for, render_template
import json
import os
import sys
import threading
import logging

from mqttAgent import  MQTTAgent
from mqttObserver import MQTTObserver
from mqttRouterPing import MQTTRouterPing
from mqttRouterTwinClient import  MQTTRouterTwinClient



import datetime



# set the project root directory as the static folder, you can set others.
app = Flask(__name__, static_url_path='/static')

@app.route('/')
def route():
    #return redirect("/static/html/index.html")
    return redirect("/static/html/Saber.html")



@app.route('/api/getSabers', methods = ['GET','POST'])
def getSabers():
    columns=[
        "drgb",
        "nrgb",
        "days",
        "daye",
        "dseq",
        "nseq",
        "timer",
        "temp",
        "on",
        "day",
        "clock"
        ]
    
    select= ["clientId"]
    asColumn = ["clientId"]
    for c in columns:
        select.append("reported.%s"%c)
        asColumn.append(c)
    
    '''
    for c in columns:
        select.append("desired.%s"%c)
        asColumn.append("desired_%s"%c)
    '''
    
    where = {'column': "reported.days", 'op': "<", 'value': 24}
    
    d = twinClient.query(select, asColumn, where, orient="records")
    #frame = pd.read_json(json.dumps(d["res"]), orient="records")
    #table = frameToTable(frame)
    if ("res" in d):
        table = recordsToTable(d["res"], "clientId")
    
        return table 
    return {}
    
@app.route('/api/setSabers', methods = ['GET','POST'])
def setSabers():
    if (request.json != None):
        targetId = request.json.get("target", None)
        delta = request.json.get("delta", {})
        if (targetId):
            targets = [targetId]
        else:
            targets = []
            
        logging.debug("Requesting %s, %s"%(json.dumps(delta), json.dumps(targets)))
        twinClient.update(delta, targets)
        
    return {}
      
@app.route('/api/saberOn', methods = ['GET','POST'])
def saberOn():
    if (request.json != None):
        on = request.json.get("on", True)
        delta = {"on": on}            
        twinClient.update(delta, [])
        
    return {}  
    

def recordsToTable(recs, indexCol):
    typeConv={ str: "string",
               int: "number",
               float:  "number",
               bool: "boolean",
               datetime: "datetime"
              }
    table = {"cols": [], "rows": []}
    
    #print("rec=%s\n"%json.dumps(recs))
    row = recs[0]
    
    for c in row:
        cell = row[c]
        t=type(cell)
        nt = typeConv.get(t, str)
        table["cols"].append({"id": c, "type": nt, "label": c})

    #print("cols=%s\n"%json.dumps(table))
    
    for r in recs:
        list=[]
        
        for ch in table["cols"]:
            c = ch["id"]
            list.append({"v": r[c]})
            
        row = {}
        row["c"]=list
        table["rows"].append(row)
        
    #print("rows=%s\n"%json.dumps(table))
    return table
        
            

def startMQTT():
    global twinClient
    
    #MQTT Credentials and targets
    mqttUser=os.environ.get("MQTT_USER")
    mqttPwd=os.environ.get("MQTT_PASSWD")
    mqttTarget= os.environ.get("MQTT_HOST")
    mqttPort=int(os.environ.get("MQTT_PORT"))
    print("MQTT %s:%d - %s\n"%(mqttTarget,mqttPort, mqttUser))
    
    #The MQTT Client Agent
    mqttAgent = MQTTAgent(mqttUser)
    mqttAgent.credentials(mqttUser, mqttPwd)
    mqttAgent.mqttHub(mqttTarget, mqttPort, True)
    
    #Consigure the observers and routers
    mqttObs = MQTTObserver()
    pingRouter = MQTTRouterPing(mqttUser)
    twinClient = MQTTRouterTwinClient(mqttUser, "saber", mqttAgent)
    
    #Add observers and reouter to client agent
    mqttAgent.addObserver(mqttObs)
    mqttAgent.addRouter(pingRouter)
    mqttAgent.addRouter(twinClient)
    
    mqttAgent.start()



if __name__ == "__main__":
    
    logging.basicConfig(level="DEBUG", 
                    format= '[%(asctime)s] {%(name)s:%(lineno)d} %(levelname)s - %(message)s')


    unifi={}
    app.secret_key = 'LCARS'
    app.config['SESSION_TYPE'] = 'filesystem'
    
    
   
    
    thread = threading.Thread(target = startMQTT)
    thread.start()
    
    
    app.run(host="0.0.0.0")
    
    
    