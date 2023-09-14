
/**
 * @file main.cpp
 * @author James Bennion-Pedley
 * @brief Graduation Table Lights
 * @date 14/09/2023
 *
 * @copyright Copyright (c) 2023
 *
 */

/*--------------------------------- Includes ---------------------------------*/

#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

/*---------------------------- Macros & Constants ----------------------------*/

/*----------------------------------- State ----------------------------------*/

// WiFi
static const char *m_ssid = WIFI_SSID; // Enter your WiFi name
static const char *m_psk = WIFI_PSK;   // Enter WiFi password

// MQTT Broker
static const char *m_broker = "broker.emqx.io";
static const char *m_topic = "DIET-4073c85645649a02734/command";
static const char *m_broker_username = "emqx";
static const char *m_broker_password = "public";

// Instances
static WiFiClient m_espClient;
static PubSubClient m_client(m_espClient);

/*------------------------------ Private Functions ---------------------------*/

static void callback(char *topic, byte *payload, unsigned int length)
{
    Serial.print("Message arrived in topic: ");
    Serial.println(topic);
    Serial.print("Message:");
    for (unsigned int i = 0; i < length; i++)
    {
        Serial.print((char)payload[i]);
    }
    Serial.println();
    Serial.println("-----------------------");
}

/*------------------------------- Public Functions ---------------------------*/

void setup()
{
    Serial.begin(115200);

    Serial.println("Program Init");
    Serial.print("Device MAC Address: ");
    Serial.print(WiFi.macAddress());

    // connecting to a WiFi network
    WiFi.begin(m_ssid, m_psk);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.println("Connecting to WiFi...");
    }

    Serial.printf("Connected to the WiFi network: %s", m_ssid);
    // connecting to a mqtt broker
    m_client.setServer(m_broker, 1883);
    m_client.setCallback(callback);
    while (!m_client.connected())
    {
        String client_id = "esp8266-client";
        client_id += String(WiFi.macAddress());
        Serial.printf("The client %s connects to the public mqtt broker\n", client_id.c_str());
        if (!m_client.connect(client_id.c_str(), m_broker_username, m_broker_password))
        {
            Serial.print("failed with state ");
            Serial.print(m_client.state());
            delay(2000);
        }
    }
    // Subscribe to topic sets
    m_client.subscribe(m_topic);
}

void loop()
{
    m_client.loop();

    // Publish to discovery channel every 5 seconds
}
