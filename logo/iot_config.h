#ifndef _IOT_CONFIG_H_
#define _IOT_CONFIG_H_

#include <IotWebConf.h>

//
// This drives iot_web_conf, making sure the thing can connect to the WiFi.
//
class IotConfig {
  public:
    // Constructs a new IoT configuration with the provided dependencies.
    IotConfig();
    ~IotConfig();

    IotConfig(const IotConfig&) = delete;
    IotConfig& operator=(const IotConfig&) = delete;
    IotConfig(IotConfig&&) = delete;
    IotConfig& operator=(IotConfig&&) = delete;

    // Initializes the IoT configuration portal. Expects dependencies to be
    // initialized.
    void setup(int statusPin);

    // Executes configuration portal's event loop. Expects the object itself to
    // be initialized.
    void loop();
  private:

    // Handles HTTP requests to web server's "/" path.
    void handleHttpToRoot_();

    // Handles HTTP requests to web server's "/config" path.
    void handleHttpToConfig_();

    // Handles configuration changes.
    void handleConfigSaved_();

    // Configuration portal's DNS server.
    DNSServer dns_server_;

    // Configuration portal's web server.
    WebServer web_server_;

    // IotWebConf interface handle.
    IotWebConf iot_web_conf_;
};
#endif  // _IOT_CONFIG_H_