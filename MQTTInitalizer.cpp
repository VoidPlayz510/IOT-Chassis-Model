// #include <ArduinoWiFiServer.h>
// #include <BearSSLHelpers.h>
// #include <CertStoreBearSSL.h>
#include <ESP8266WiFi.h>
// #include <ESP8266WiFiAP.h>
// #include <ESP8266WiFiGeneric.h>
// #include <ESP8266WiFiGratuitous.h>
// #include <ESP8266WiFiMulti.h>
// #include <ESP8266WiFiSTA.h>
// #include <ESP8266WiFiScan.h>
// #include <ESP8266WiFiType.h>
#include <WiFiClient.h>
// #include <WiFiClientSecure.h>
// #include <WiFiClientSecureBearSSL.h>
// #include <WiFiServer.h>
// #include <WiFiServerSecure.h>
// #include <WiFiServerSecureBearSSL.h>
// #include <WiFiUdp.h>

#include <PubSubClient.h>

// Update these with values suitable for your network.
const int Forward_IN = 14; // d5 = 14 // d6 = 12 d7 = 13
const int Stopped_IN = 13;
const int Obstacle_IN = 12;

const char* ssid = "xxxxxxxxx";
const char* password = "xxxxxxxxx";
const char* mqtt_server = "0.0.0.00";

// mqtt login info
const char* mqttPwd = "xxxxxx";
const char* mqttUser = "xxxxxxx";

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE (50)
char msg[MSG_BUFFER_SIZE];
int value = 0;


void setup_wifi()
{

    delay(10);
    // We start by connecting to a WiFi network
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

    // Switch on the LED if an 1 was received as first character
    if ((char)payload[0] == '1')
    {
        digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on (Note that LOW is the voltage level
        // but actually the LED is on; this is because
        // it is active low on the ESP-01)
    }
    else
    {
        digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off by making the voltage HIGH
    }

}

void reconnect()
{
    // Loop until we're reconnected
    while (!client.connected())
    {
        Serial.print("Attempting MQTT connection...");
        // Create a random client ID
        String clientId = "ESP8266Client-";
        clientId += String(random(0xffff), HEX);
        // Attempt to connect
        if (client.connect(clientId.c_str(), mqttUser, mqttPwd))
        {
            Serial.println("connected");
            // Once connected, publish an announcement...
            client.publish("testTopic", "connected");
            // client.publish("testTopic", "forward");
            // ... and resubscribe
            client.subscribe("testTopic");
        }
        else
        {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" try again in 5 seconds");
            // Wait 5 seconds before retrying
            delay(5000);
        }
    }
}

void setup()
{
    pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
    pinMode(Forward_IN, INPUT);       // Define forward pin as input
    Serial.begin(115200);
    setup_wifi();
    client.setServer(mqtt_server, 1883);
    client.setCallback(callback);
}

void loop()
{

    if (!client.connected())
    {
        reconnect();
    }
    client.loop();



    unsigned long now = millis();
    if (now - lastMsg > 2000)
    {
        lastMsg = now;
        ++value;
        snprintf(msg, MSG_BUFFER_SIZE, "hello world #%ld", value);
        Serial.print("Publish message: ");
        Serial.println(msg);
        client.publish("outTopic", msg);

        if(digitalRead(Forward_IN))
        {
            client.publish("testTopic", "forward");
        }

        if(digitalRead(Stopped_IN))
        {
           client.publish("testTopic", "stopping");
        }

        if(digitalRead(Obstacle_IN))
        {
           client.publish("testTopic", "obstacle");
        }
    }

    
    //digitalRead(Forward_IN)
}
