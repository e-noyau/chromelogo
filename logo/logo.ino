
#include "display.h"

//
// Pilots a Chrome logo, composed of 3 segments of LEDs and a center ring.
//
Display display;


// The internal LED, used to debug potentially.
#define LED_BUILTIN 2


void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  display.setup();

}

// Number of times throught the loop, just used to debug
static int count = 0;

void loop() {
  display.loop();

  //Bling the LED while an animation is running.
  count += 1;
  digitalWrite(LED_BUILTIN, (count % 2) ? LOW : HIGH);
}



