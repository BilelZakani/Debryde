from math import *
from influxdb_client import InfluxDBClient, Point, WritePrecision
from influxdb_client.client.write_api import SYNCHRONOUS
import time
import serial
import random
from datetime import datetime
import json


#CONNECTION TO INFLUXDB
url = 'your_url'
token = 'your_token'
bucket = 'Debryde'
org = 'MEA3'
client = InfluxDBClient(url=url, token=token)

#DEFINE PARAMETERS
kind0 = 'turbidité'
kind1 = 'temperature'
kind2 = 'accéléromètre'
host = 'host1'
device = 'Arduino'

#OPEN THE SERIAL PORT
ser = serial.Serial('/dev/ttyACM0', 115200)


while True :

    
    #READ THE SERIAL PORT
    readed = ser.readline() 

    #BRING BACK THE JSON DATA
    data = json.loads(readed) 


    #ADD THE DATE
    today = datetime.now()
    iso = today.isoformat()
    data.update({"time" : str(iso)})


    #-------------------------------------------DATABASE CONFIGURATION---------------------------------------------------#
    
    #A0
    point = Point(kind0).tag('host', host).tag('device', device).field('value', data["A0"]).time(time=datetime.utcnow())
    write_api = client.write_api(write_options=SYNCHRONOUS)
    write_api.write(bucket = bucket, org = org,record=point)
    
    #A1
    point = Point(kind1).tag('host', host).tag('device', device).field('value', data["A1"]).time(time=datetime.utcnow())

    write_api = client.write_api(write_options=SYNCHRONOUS)
    write_api.write(bucket = bucket, org = org,record=point)
    
    #A2
    point = Point(kind2).tag('host', host).tag('device', device).field('value', data["A2"]).time(time=datetime.utcnow())

    #--------------------------------------------------------------------------------------------------------------------#


    #SEND DATA TO DATABASE
    write_api = client.write_api(write_options=SYNCHRONOUS)
    write_api.write(bucket = bucket, org = org,record=point)


    #PRINT VALIDATION
    print("data ",i," envoyé\n")

    i += 1

    time.sleep(3)
