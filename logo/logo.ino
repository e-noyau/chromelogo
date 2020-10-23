
#include "display.h"
#include "iot_config.h"

// The internal LED, used to debug potentially.
#define LED_BUILTIN 2

// The PIN driving the LED strip
#define PIN 15

// Controller for the front LEDs.
Display display(PIN);
IotConfig config;

// Main setup, initializing everything that need to be.
void setup() {
  display.setup();
  config.setup(LED_BUILTIN);
}


// Magic variables
static int count = 0;
static unsigned long previousMillis = 0;
static const long interval = 30000;

void loop() {
  unsigned long currentMillis = millis();
  
  if (display.isAnimating()) {
    // If the display is still animating a transition, reset the timer to let
    // the transition finish.
    previousMillis = currentMillis;
  } else if (currentMillis - previousMillis >= interval) {
    // We waited long enough since the end of the previous animation, time to
    // switch to the next display scheme.
    count += 1;
    previousMillis = currentMillis;
    display.setSchemeAtIndex(count % display.availableSchemes().size());
  }

  display.loop();
  config.loop();
}



