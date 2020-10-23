
#include <IotWebConf.h>

#include "iot_config.h"

// Name of this IoT object.
#define THING_NAME "Chrome Logo"

// Initial WiFi access point password.
#define INITIAL_WIFI_AP_PASSWORD "qwerty12"

// IoT configuration version. Change this whenever IotWebConf object's
// configuration structure changes.
#define CONFIG_VERSION "v1"

// Port used by the IotWebConf HTTP server.
#define WEB_SERVER_PORT 80

// HTTP OK status code.
#define HTTP_OK 200

// HTTP MIME type.
#define MIME_HTTP "text/html"


IotConfig::IotConfig()
    : web_server_(WEB_SERVER_PORT),
      iot_web_conf_(THING_NAME, &dns_server_, &web_server_,
                    INITIAL_WIFI_AP_PASSWORD, CONFIG_VERSION) {}
IotConfig::~IotConfig() {}

void IotConfig::handleHttpToRoot_() {
  if (iot_web_conf_.handleCaptivePortal())
    return;

  static const char html[] =
    "<!DOCTYPE html>"
    "<html lang='en'>"
    "<head>"
        "<meta name='viewport' content='width=device-width, "
                "initial-scale=1, user-scalable=no'/>"
        "<title>Chrome Logo</title>"
    "</head>"
    "<body>"
        "<h1>Chrome Logo</h1>"
        "<ul><li><a href='config'>Settings</a></li></ul>"
    "</body>"
    "</html>\n";
  web_server_.send(HTTP_OK, MIME_HTTP, html);
}

void IotConfig::handleHttpToConfig_() {
    iot_web_conf_.handleConfig();
}

void IotConfig::handleConfigSaved_() {
  // [TODO]
}

void IotConfig::setup(int statusPin) {
    iot_web_conf_.setStatusPin(statusPin);
    // Here we can set IotWebConf's status and config reset pins, if available.
    // iot_web_conf_.setConfigPin(CONFIG_PIN);
    iot_web_conf_.setConfigSavedCallback([this](){ handleConfigSaved_(); });
    iot_web_conf_.init();

    web_server_.on("/", [this](){ handleHttpToRoot_(); });
    web_server_.on("/config", [this](){ handleHttpToConfig_(); });
    web_server_.onNotFound([this](){ iot_web_conf_.handleNotFound(); });
}

void IotConfig::loop() {
    iot_web_conf_.doLoop();
}