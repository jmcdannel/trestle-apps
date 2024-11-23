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

# i2c = busio.I2C(scl=board.GP1, sda=board.GP0)
# kit = ServoKit(channels=16, i2c=i2c)

# Load configuration from config.json
with open('config.json') as config_file:
    config = json.load(config_file)

# Set up pins
pins = {}
for pin_number, pin_name in config['pins'].items():
    print("Setting up pin", pin_name)
    print("Setting up pin #", pin_number)
    pin = digitalio.DigitalInOut(getattr(board, pin_name))
    pin.direction = digitalio.Direction.OUTPUT
    pins[int(pin_number)] = pin

broker = os.getenv('MQTT_BROKER')
layoutId = os.getenv('LAYOUT_ID')
deviceId = os.getenv('DEVICE_ID')
topicId = os.getenv('TOPIC_ID')
stopic = topicId + "/" + layoutId + "/" + deviceId
ptopic = topicId + "/" + layoutId + "/" + deviceId

pool = socketpool.SocketPool(wifi.radio)
socket = pool.socket()
socket.listen(1)
socket.setblocking(True)
print("Connected, IPv4 Addr: ", wifi.radio.ipv4_address)


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
    device = data.get("device")
    print("device")
    print(device)

    if device != deviceId:
        return
    
    if payload is not None:
        if action == "pin":
            handlePin(client, payload)
        else:
            print("Payload fail")

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

# def handleTurnout(client,payload):
#     print("handleTurnout")
#     print(payload)
#     servo = payload["servo"]
#     angle = payload["value"]
#     kit.servo[servo].angle = angle
#     client.publish(ptopic, f"Toggled servo {servo} to angle {angle}")
#     print(f"Toggled servo {servo} to angle {angle}")

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
