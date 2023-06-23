#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <PubSubClient.h>
#include "config.h"


const int Forward_IN = 14;
const int Stopped_IN = 13;
const int Obstacle_IN = 12;

const char* ssid = "xxxxxxxxxxxxxxxxxxx";
const char* password = "xxxxxxxxxxxxxxxxxxxxxxxx";
const char* mqtt_server = "00.0.0.0.0";
const char* mqttPwd = "xxxxxxxx";
const char* mqttUser = "xxxxxxxx";
AdafruitIO_Feed *digital = io.feed("xxxxxxxx");
WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE (50)
#define LED_PIN 0
char msg[MSG_BUFFER_SIZE];
int value = 0;

void setupWiFi()
{
    delay(10);
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    randomSeed(micros());

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}

void setupMQTT()
{
    client.setServer(mqtt_server, 1883);
    client.setCallback(callback);
}

void callback(char* topic, byte* payload, unsigned int length)
{
    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("] ");
    for (int i = 0; i < length; i++)
    {
        Serial.print((char)payload[i]);
    }
    Serial.println();

    // Switch on the LED if an 1 was received as the first character
    if ((char)payload[0] == '1')
    {
        digitalWrite(BUILTIN_LED, LOW);
    }
    else
    {
        digitalWrite(BUILTIN_LED, HIGH);
    }
}

void reconnect()
{
    while (!client.connected())
    {
        Serial.print("Attempting MQTT connection...");
        String clientId = "ESP8266Client-";
        clientId += String(random(0xffff), HEX);
        
        if (client.connect(clientId.c_str(), mqttUser, mqttPwd))
        {
            Serial.println("connected");
            client.publish("testTopic", "connected");
            client.subscribe("testTopic");
        }
        else
        {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" try again in 5 seconds");
            delay(5000);
        }
    }
}

void publishMessage(const char* topic, const char* message)
{
    client.publish(topic, message);
}

void handleForward()
{
    if (digitalRead(Forward_IN))
    {
        publishMessage("testTopic", "forward");
    }
}

void handleStopped()
{
    if (digitalRead(Stopped_IN))
    {
        publishMessage("testTopic", "stopping");
    }
}

void handleObstacle()
{
    if (digitalRead(Obstacle_IN))
    {
        publishMessage("testTopic", "obstacle");
    }
}

void handleMessage(AdafruitIO_Data *data) {
  Serial.print("received <- ");

  if(data -> toPinLevel() == HIGH)
  {
    Serial.println("HIGH");
    digitalWrite(LED_PIN, HIGH);
  }
  else
  {
    Serial.println("LOW");
    digitalWrite(LED_PIN, LOW);
    publishMessage("testTopic", "stopping");
  }
  // write the current state to the led

  
  //digitalWrite(LED_PIN, data -> toPinLevel());

}

void setup()
{
    pinMode(BUILTIN_LED, OUTPUT);
    pinMode(LED_PIN, OUTPUT);
    pinMode(Forward_IN, INPUT);
    Serial.begin(115200);
    setupWiFi();
    setupMQTT();

      // wait for serial monitor to open
  while(! Serial);

  // connect to io.adafruit.com
  Serial.print("Connecting to Adafruit IO");
  io.connect();

  digital -> onMessage(handleMessage);

  // wait for a connection
  while(io.status() < AIO_CONNECTED) 
  {
    Serial.print(".");
    delay(500);
  }

  // we are connected
  Serial.println();
  Serial.println(io.statusText());
  digital -> get();
}



void loop()
{
    if (!client.connected())
    {
        reconnect();
    }
    client.loop();
    io.run();
    unsigned long now = millis();
    if (now - lastMsg > 2000)
    {
        lastMsg = now;
        ++value;

        handleForward();
        handleStopped();
        handleObstacle();
    }
}
