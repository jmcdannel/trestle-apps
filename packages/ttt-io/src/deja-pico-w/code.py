import board
import digitalio
import os
import time
import ssl
import json
import socketpool
import wifi
import adafruit_minimqtt.adafruit_minimqtt as MQTT
import busio
from adafruit_servokit import ServoKit

i2c = busio.I2C(scl=board.GP1, sda=board.GP0)
kit = ServoKit(channels=16, i2c=i2c)

broker = os.getenv('MQTT_BROKER')
layoutId = os.getenv('LAYOUT_ID')
deviceId = os.getenv('DEVICE_ID')
topicId = os.getenv('TOPIC_ID')
stopic = topicId + "/" + layoutId + "/" + deviceId
ptopic = topicId + "/" + layoutId + "/" + deviceId

pins = {
    6: digitalio.DigitalInOut(board.GP6),
    7: digitalio.DigitalInOut(board.GP7),
    8: digitalio.DigitalInOut(board.GP8),
    9: digitalio.DigitalInOut(board.GP9)
}
pins[6].direction = digitalio.Direction.OUTPUT
pins[7].direction = digitalio.Direction.OUTPUT
pins[8].direction = digitalio.Direction.OUTPUT
pins[9].direction = digitalio.Direction.OUTPUT

def connect(client, userdata, flags, rc):
    print("Connected to MQTT Broker {}".format(broker))
    print("Flags: {0}\n RC: {1}".format(flags, rc))

pool = socketpool.SocketPool(wifi.radio)
socket = pool.socket()
socket.listen(1)
socket.setblocking(True)
print("Connected, IPv4 Addr: ", wifi.radio.ipv4_address)

mqtt_client = MQTT.MQTT(
    broker=broker,
    port=1883,
    socket_pool=pool,
)

# Define callback methods which are called when events occur
# pylint: disable=unused-argument, redefined-outer-name
def connected(client, userdata, flags, rc):
    # This function will be called when the client is connected
    # successfully to the broker.
    client.subscribe(stopic)
    client.publish(ptopic, "Connected")
    print(f"Connected to mqtt Listening for topic changes", stopic, ptopic)

def disconnected(client, userdata, rc):
    # This method is called when the client is disconnected
    print("Disconnected from mqtt!")

def message(client, topic, message):
    # This method is called when a topic the client is subscribed to
    # has a new message.
    print(f"New message on topic {topic}: {message}")
    data = json.loads(message)
    print("data")
    print(data)
    action = data.get("action")
    print("action")
    print(action)
    payload = data.get("payload")
    print("payload")
    print(payload)
    
    if payload is not None and "config" in payload:
      device = payload["device"]
      print("device")
      print(device)
      # Rest of your code here
      if device != deviceId:
          return
      elif action == "turnouts":
          handleTurnout(client, payload)
      elif action == "effects":
          handlePin(client, payload)
    else:
      print("Payload does not contain a 'config' object")
    

def handlePin(client, payload):
    print("handlePin")
    print(payload)
    pinNumber = payload["pin"]
    if payload["state"]:
        value = True
    else:
        value = False
    pins[pinNumber].value = value
    client.publish(ptopic, f"Toggled pin {pinNumber} to value {value}")
    print(pinNumber)
    print(value)
    print(pins[pinNumber].value)
    print(pins[pinNumber].value)

def handleTurnout(client,payload):
    print("handleTurnout")
    print(payload)
    servo = payload["servo"]
    angle = payload["value"]
    kit.servo[servo].angle = angle
    client.publish(ptopic, f"Toggled servo {servo} to angle {angle}")
    print(f"Toggled servo {servo} to angle {angle}")

def subscribe(client, userdata, topic, granted_qos):
    # This method is called when the client subscribes to a new feed.
    print('Subscribed to {0} with QOS level {1}'.format(topic, granted_qos))

def runMqtt():
    
    mqtt_client = MQTT.MQTT(
        broker=broker,
        port=1883,
        socket_pool=pool,
    )

    mqtt_client.on_connect = connected
    mqtt_client.on_disconnect = disconnected
    mqtt_client.on_message = message
    mqtt_client.on_subscribe = subscribe

    # Connect the client to the QTT broker.
    print("Connecting to MQTT...")
    mqtt_client.connect()

    while True:
        # Poll the message queue
        mqtt_client.loop()

runMqtt()
