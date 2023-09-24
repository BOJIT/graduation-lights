/**
 * @file server.cpp
 * @author James Bennion-Pedley
 * @brief Server for handling ESP8266 onboarding
 * @date 24/09/2023
 *
 * @copyright Copyright (c) 2023
 *
 */

/*--------------------------------- Includes ---------------------------------*/

#include <Arduino.h>

#include <DNSServer.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <FS.h>

/*---------------------------- Macros & Constants ----------------------------*/

/*----------------------------------- State ----------------------------------*/

static ESP8266WebServer m_espServer(80); // Server for when in AP mode
static DNSServer m_dnsServer;
static IPAddress m_apIP(192, 168, 1, 1);

// clang-format off
static const char *m_index = "<!DOCTYPE HTML><html><body> <h1>AP Credentials</h1> <form action='/creds' method='post'> <input name='ssid' placeholder='SSID' autocapitalize='none'></input> <input name='psk' placeholder='Password' autocapitalize='none'></input> <button type='submit'>Submit</button> </form></body><style>body *{font-family: 'Courier New', Courier, monospace;}form{display: flex; flex-direction: column; gap: 1rem;}input{height: 4rem; font-size: 2.5rem;}button{height: 6rem; font-size: 2.5rem;}h1{text-align: center; font-size: 5rem;}</style></html>";
// clang-format on

/*------------------------------ Private Functions ---------------------------*/

static void
handleAppleCaptivePortal(void)
{
    const char *metaRefreshStr = "<head><meta http-equiv=\"refresh\" content=\"0; url=http://192.168.1.1/index.html/?venue-info-url=http:192.168.1.1/index.html\" /></head><body><p>redirecting...</p></body>";

    m_espServer.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
    m_espServer.sendHeader("Pragma", "no-cache");
    m_espServer.sendHeader("Expires", "-1");
    m_espServer.send(200, "text/html", metaRefreshStr);
    return;
}

static void handleIndex(void)
{
    m_espServer.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
    m_espServer.sendHeader("Pragma", "no-cache");
    m_espServer.sendHeader("Expires", "-1");
    m_espServer.send(200, "text/html", m_index);

    // Sends SSID and creds as plaintext!
}

static void handleCredentials()
{
    String ssid = m_espServer.arg("ssid");
    String psk = m_espServer.arg("psk");
    Serial.printf("POST received: %s, %s\r\n", ssid.c_str(), psk.c_str());

    m_espServer.sendHeader("Location", String("/"), true);
    m_espServer.send(302, "text/plain", "");
}

/*------------------------------- Public Functions ---------------------------*/

void server_loop(void)
{
    // Handle webserver
    m_dnsServer.processNextRequest();
    m_espServer.handleClient();
}

const char *server_get_ssid(void)
{
    // Return stored creds, or hardcoded defaults
    return nullptr;
}

const char *server_get_psk(void)
{
    // Return stored creds, or hardcoded defaults
    return nullptr;
}

void server_initialise(const char *ap_prefix)
{
    WiFi.mode(WIFI_AP);
    WiFi.softAPConfig(m_apIP, m_apIP, IPAddress(255, 255, 255, 0));

    uint8_t mac[WL_MAC_ADDR_LENGTH];
    WiFi.macAddress(mac);

    char ap_ssid[100];
    sprintf(ap_ssid, "%s-%02X%02X%02X%02X%02X%02X", ap_prefix, mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

    WiFi.softAP(ap_ssid);

    // Create captive portal and default routes
    m_dnsServer.start(53, "*", m_apIP);
    m_espServer.on("/", handleIndex);
    m_espServer.on("/index.html", handleIndex);
    m_espServer.on("/hotspot-detect.html", handleAppleCaptivePortal);
    m_espServer.on("/creds", HTTP_POST, handleCredentials);

    // redirect all 404 traffic to index.html with android captive portal
    m_espServer.onNotFound([]()
                           {
        const char *metaRefreshStr = "<head><meta http-equiv=\"refresh\" content=\"0; url=http://192.168.1.1/index.html?venue-info-url=http:192.168.1.1/index.html\" /></head><body><p>redirecting...</p></body>";
        m_espServer.send(200, "text/html", metaRefreshStr); });

    m_espServer.begin();
}

/*----------------------------------------------------------------------------*/
