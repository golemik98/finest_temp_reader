import paho.mqtt.client as mqtt
import time
print()

def on_log(client, userdata, level, buff):
    print("log: ",buff)


def on_connect(client, userdata, flags, rc):
    if rc == 0:
        print("Connected OK")
    else:
        print("Bad connection return code=", rc)


def on_message(client, userdata, message):
    print("Topic: ", message.topic, ", ", str(message.payload.decode("utf-8")))


def on_disconnect(client, userdata, flags, rc=0):
    print("Disconnected result code=", str(rc))

client = mqtt.Client("P1")
client.on_connect = on_connect
# client.on_log = on_log
client.on_disconnect = on_disconnect
client.on_message = on_message

print("Connecting to the broker ...")
client.connect("localhost", keepalive=10)

print("Connected!")

client.loop_start()

client.subscribe("dev/temp")
input(" ")
client.loop_stop()

client.disconnect()


