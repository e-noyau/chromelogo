#include <NeoPixelBrightnessBus.h>
#include <NeoPixelAnimator.h>

//
// Pilots a Chrome logo, composed of 3 segments of LEDs and a center ring.
//

// The PIN driving the LED strip
#define PIN    15

// Number of leds in a segment
#define N_SEGMENT 46
// Number of LEDs in the center ring
#define N_CENTER 24
// There are 3 segments and one central ring, this gives the total of LEDs to
// drive.
#define N_SEGMENT_COUNT 3
#define N_LEDS (N_SEGMENT * N_SEGMENT_COUNT) + N_CENTER 
#define TIME_CHANGE_ANIMATION_SPEED 800

// The internal LED, used to debug potentially.
#define LED_BUILTIN 2

// The bus for all the LEDs.

NeoPixelBus<NeoGrbFeature, NeoEsp32I2s0Ws2812xMethod> pixels(N_LEDS, PIN);
// For animation, I kept things simple with one channel per led.
// I could have done one channel per segment, it migh have been more efficient…
NeoPixelAnimator animations(N_LEDS, NEO_MILLISECONDS);

// Only used at start to clear the strip.
RgbColor black = RgbColor(0x00, 0x00, 0x00);

// The Chrome logo can be rendered differently for Chromium, Canary and Stable.
// This is still missing the iOS developer color, a magnificent purple.
typedef struct { RgbColor top, right, left, center; } LogoColors;
LogoColors logoColors[] = {
  { // Canary
    .top = RgbColor(0xFF, 0x7F, 0x00),
    .right = RgbColor(0xFF, 0xFF, 0x00),
    .left = RgbColor(0xFF, 0xAF, 0x00),
    .center = RgbColor(0xFF, 0xFF, 0x00),
  },
  { // Chrome stable
    .top = RgbColor(0xFF, 0x00, 0x00),     // red
    .right = RgbColor(0xFF, 0xFF, 0x00),   // yellow
    .left = RgbColor(0x00, 0xFF, 0x00),    // green
    .center = RgbColor(0x00, 0x00, 0xFF),  // blue
  },
  { // Chromium
    .top = RgbColor(0x2D, 0x76, 0xD5),
    .right = RgbColor(0x9C, 0xC4, 0xF6),
    .left = RgbColor(0x50, 0x9C, 0xF1),
    .center = RgbColor(0x25, 0x8A, 0xF0),
  },
};
#define LOGO_COUNT (sizeof(logoColors) / sizeof(LogoColors))


// Start all animations to switch a segment of the led strip to a particular
// color. The animation changes the color from the current color to the one
// passed as argument.
void LightSegment(int startIndex, int length, RgbColor color) {
  for (int led = 0; led < length; led++) {
    int ledIndex = startIndex + led;
    RgbColor currentColor = pixels.GetPixelColor(ledIndex);
    RgbColor nextColor = color;
    
    AnimUpdateCallback animUpdate = [=](const AnimationParam& param) {
        float progress = NeoEase::QuadraticIn(param.progress);
        RgbColor intermediate =
            RgbColor::LinearBlend(currentColor, nextColor, progress);
        pixels.SetPixelColor(ledIndex, intermediate);
    };
    animations.StartAnimation(ledIndex, TIME_CHANGE_ANIMATION_SPEED, animUpdate);
  }
}

// Switch the whole strip to the log colors passed in.
void LightLogo(LogoColors logoColor) {
  LightSegment(0 * N_SEGMENT, N_SEGMENT, logoColor.top);
  LightSegment(1 * N_SEGMENT, N_SEGMENT, logoColor.right);
  LightSegment(2 * N_SEGMENT, N_SEGMENT, logoColor.left);
  LightSegment(3 * N_SEGMENT, N_CENTER, logoColor.center);
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);

  pixels.Begin();
  pixels.ClearTo(black);
  pixels.Show();            // Turn OFF all pixels ASAP
  LightLogo(logoColors[0]);

}

// Logo colors to display next
static int currentColors = 1;

// Number of times throught the loop, just used to debug
static int count = 0;

void loop() {
  if (!animations.IsAnimating()) {
    delay(30000); // 30 seconds
    LightLogo(logoColors[currentColors]);
    currentColors = (currentColors + 1) % LOGO_COUNT;
  }

  //Bling the LED while an animation is running.
  count += 1;
  digitalWrite(LED_BUILTIN, (count % 2) ? LOW : HIGH);

  animations.UpdateAnimations();
  pixels.Show();
}



