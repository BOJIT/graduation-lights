
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

#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

/*---------------------------- Macros & Constants ----------------------------*/

/*----------------------------------- State ----------------------------------*/

// WiFi
static const char *m_ssid = WIFI_SSID; // Enter your WiFi name
static const char *m_psk = WIFI_PSK;   // Enter WiFi password

// MQTT Broker
static const char *m_broker = "broker.emqx.io";
static const char *m_broker_username = "emqx";
static const char *m_broker_password = "public";

static const char *m_topic_command = "DIET-4073c85645649a02734/command";
static const char *m_topic_discover = "DIET-4073c85645649a02734/discover";
static const char *m_topic_state = "DIET-4073c85645649a02734/state";

// Instances
static WiFiClient m_espClient;
static PubSubClient m_client(m_espClient);

static char m_jsonBuf[512];

static char m_mode[32] = "Off";
static uint8_t m_colours[3] = {0, 0, 0};

/*------------------------------ Private Functions ---------------------------*/

static void str_to_colour(const char *str, uint8_t *cols)
{
    // Assume string takes form #00FF00, etc... and is NULL-terminated
    const char *ptr = &(str[1]);

    int r, g, b; // Will be less than uint8_t

    // NOTE this has no error handling!
    sscanf(ptr, "%02x%02x%02x", &r, &g, &b);
    cols[0] = r;
    cols[1] = g;
    cols[2] = b;
}

static void colour_to_str(uint8_t *cols, char *dest)
{
    // cols is a uint_8 array that is 3 elements long
    // Destination should be at least 8 characters long!

    sprintf(dest, "#%02x%02x%02x", cols[0], cols[1], cols[2]);
}

static void callback(char *topic, byte *payload, unsigned int length)
{
    StaticJsonDocument<256> doc;
    deserializeJson(doc, payload);

    const char *mode = doc["mode"];
    const char *colour = doc["colour"];

    if (mode == nullptr)
        return;

    if (colour == nullptr)
        return;

    strcpy(m_mode, mode); // Assumes no overflow
    str_to_colour(colour, m_colours);

    Serial.print("Message arrived in topic: ");
    Serial.println(topic);
    Serial.printf("Mode: %s\r\n", mode);
    Serial.printf("Mode: %s\r\n", colour);
    Serial.println();
}

static void compose_json(void)
{
    StaticJsonDocument<256> doc;

    doc["mac"] = WiFi.macAddress();
    doc["ip"] = WiFi.localIP().toString();
    doc["mode"] = m_mode;

    char col_string[16];
    colour_to_str(m_colours, col_string);
    doc["colour"] = col_string;

    serializeJson(doc, m_jsonBuf);
}

/*------------------------------- Public Functions ---------------------------*/

void setup()
{
    // Initialisation
    Serial.begin(115200);
    pinMode(LED_BUILTIN, OUTPUT);

    Serial.print("Device MAC Address: ");
    Serial.println(WiFi.macAddress());

    /*------------------------------------------------------------------------*/

    // connect to the WiFi network
    WiFi.begin(m_ssid, m_psk);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.println("Connecting to WiFi...");
        digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    }

    Serial.printf("Connected to the WiFi network: %s\r\n", m_ssid);

    /*------------------------------------------------------------------------*/

    // connecting to a mqtt broker
    m_client.setServer(m_broker, 1883);
    m_client.setCallback(callback);
    while (!m_client.connected())
    {
        String client_id = "esp8266-client-";
        client_id += String(WiFi.macAddress());
        if (!m_client.connect(client_id.c_str(), m_broker_username, m_broker_password))
        {
            Serial.printf("Connection Failed! Code: %d", m_client.state());
            digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
            delay(2000);
        }
        delay(1000);
    }

    /*------------------------------------------------------------------------*/

    // Subscribe to topic sets
    m_client.subscribe(m_topic_command);
    m_client.subscribe(m_topic_state);
}

void loop()
{
    uint32_t t_now = millis();

    m_client.loop();

    // Publish to discovery channel every 5 seconds
    static uint32_t t_dscvr = 0;
    if (t_now - t_dscvr > 5000)
    {
        compose_json();
        m_client.publish(m_topic_discover, m_jsonBuf);
        t_dscvr = t_now;
    }
}
