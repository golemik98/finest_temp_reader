import paho.mqtt.client as mqtt
import time
import requests

print()

def on_log(client, userdata, level, buff):
    print("log: ",buff)


def on_connect(client, userdata, flags, rc):
    if rc == 0:
        print("Connected OK")
    else:
        print("Bad connection return code=", rc)


def on_message(client, userdata, message):
    # print("Topic: ", message.topic, ", ", str(message.payload.decode("utf-8")))
    # messages_array.append(str(message.payload.decode("utf-8")))
    msg = str(message.payload.decode("utf-8"))
    msg = msg.split(" : ")
    # timeStamp = msg[0]
    # sensorName = msg[1]
    # temperature = msg[2]
    data_post["timeStamp"] = msg[0]
    data_post["sensorName"] = msg[1]
    data_post["temperature"] = msg[2]

    r = requests.post("https://httpbin.org/post", json=data_post)
    print(r.json()['data'])
    


def on_disconnect(client, userdata, flags, rc=0):
    print("Disconnected result code=", str(rc))


#messages_array = []
data_post = {
    'timeStamp': 0, 
    'sensorName': 0, 
    'temperature': 0
    }

client = mqtt.Client("P1")
client.on_connect = on_connect
# client.on_log = on_log
client.on_disconnect = on_disconnect
client.on_message = on_message

print("Connecting to the broker ...")
client.connect("localhost", keepalive=10)


#THE LOOP
client.loop_start()
client.subscribe("dev/temp")
input(" ")
client.loop_stop()
#THE LOOP


# for message in messages_array:
#     print(message)



client.disconnect()


