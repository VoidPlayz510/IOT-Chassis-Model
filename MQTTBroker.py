import random
from paho.mqtt import client as mqtt_client
from Adafruit_IO import Client, Feed
from sense_hat import SenseHat
import time

broker = 'xxxxxxxxxxx'
port = 0000
topic = "testTopic"

# Generate client ID with pub prefix randomly
client_id = f'python-mqtt-{random.randint(0, 100)}'
username = 'xxxxxxxxx'
password = 'xxxxxxxxx'

ADAFRUIT_IO_KEY = 'xxxxxxxxxxxxxxxxxxxxxxxx'
ADAFRUIT_IO_USERNAME = 'xxxxxxxxxxxxxxx'

aio = Client(ADAFRUIT_IO_USERNAME, ADAFRUIT_IO_KEY)

# Create a new feed named 'counter'
name = 'Jesus'
feed = Feed(name=name)

feedsList = []
feedsList.append(feed)
points_over_minutes = 30 / 60

time_time_pause = (len(feedsList) / points_over_minutes)
count = 0

data_limit = 100  # Specify the data limit
data_count = 0  # Initialize the data count


def connect_mqtt() -> mqtt_client:
    def on_connect(client, userdata, flags, rc):
        if rc == 0:
            print("Connected to MQTT Broker!")
            global time_time_pause
            print(f'{time_time_pause} pause time')
        else:
            print("Failed to connect, return code %d\n", rc)

    client = mqtt_client.Client(client_id)
    client.username_pw_set(username, password)
    client.on_connect = on_connect
    client.connect(broker, port)
    return client


def subscribe(client: mqtt_client):
    def on_message(client, userdata, msg):
        msg.payload = msg.payload.decode("utf-8")

        if msg.payload == "connected":
            global count, time_time_pause
            count = count + 1

            aio.send_data('jesus', count)
            time.sleep(3)

            EnvironSensors()
            time.sleep(time_time_pause)

        if msg.payload == "Hello World!":
            print("Received by Paho!")

        if msg.payload == "forward":
            print("moving forwards.")
            aio.send_data('jesus', "We are moving forwards.")

        if msg.payload == "stopping":
            print("stopping.")
            aio.send_data('jesus', "We are stopping.")

        if msg.payload == "obstacle":
            print("obstacle detected.")
            aio.send_data('jesus', "we detected something!")

        time.sleep(time_time_pause)
    client.subscribe(topic)
    client.on_message = on_message


def EnvironSensors():
    sense = SenseHat()
    print("Uploading environ sensors")

    # Humidity
    humidity = sense.get_humidity()
    print("Humidity: %s %%rH" % humidity)

    # Temperature
    temp = sense.get_temperature()
    print("Temperature: %s C" % temp)

    # Pressure
    pressure = sense.get_pressure()
    print("Pressure: %s Millibars" % pressure)

    # Upload data to ADA
    aio.send_data('jesus', humidity)
    aio.send_data('jesus', temp)
    aio.send_data('jesus', pressure)

    global data_count, data_limit

    data_count += 1

    if data_count >= data_limit:
        print("Data limit reached. Waiting until reset.")
        data_count = 0  # Reset the data count


def run():
    client = connect_mqtt()
    subscribe(client)
    client.loop_forever()


if __name__ == '__main__':
    run()
