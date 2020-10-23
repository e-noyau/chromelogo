#include "display.h"


// Number of leds in a segment
#define N_SEGMENT 46
// Number of LEDs in the center ring
#define N_CENTER 24

// There are 3 segments and one central ring, this gives the total of LEDs to
// drive.
#define N_SEGMENT_COUNT 3
#define N_LEDS (N_SEGMENT * N_SEGMENT_COUNT) + N_CENTER 

// Animation speed when converting from one scheme to another.
#define TIME_CHANGE_ANIMATION_SPEED 800


// Only used at start to clear the strip.
RgbColor black = RgbColor(0x00, 0x00, 0x00);

// The Chrome logo can be rendered differently for Chromium, Canary and Stable.
// This is still missing the iOS developer color, a magnificent purple.
const LogoScheme colorSchemes[] = {
  { // Chrome stable
    .name = "Stable",
    .top = RgbColor(0xFF, 0x00, 0x00),     // red
    .right = RgbColor(0xFF, 0xFF, 0x00),   // yellow
    .left = RgbColor(0x00, 0xFF, 0x00),    // green
    .center = RgbColor(0x00, 0x00, 0xFF),  // blue
  },
  { // Canary
    .name = "Canary",
    .top = RgbColor(0xFF, 0x7F, 0x00),
    .right = RgbColor(0xFF, 0xFF, 0x00),
    .left = RgbColor(0xFF, 0xAF, 0x00),
    .center = RgbColor(0xFF, 0xFF, 0x00),
  },
  { // Chromium
    .name = "Chromium",
    .top = RgbColor(0x2D, 0x76, 0xD5),
    .right = RgbColor(0x9C, 0xC4, 0xF6),
    .left = RgbColor(0x50, 0x9C, 0xF1),
    .center = RgbColor(0x25, 0x8A, 0xF0),
  },
};
#define SCHEME_COUNT (sizeof(colorSchemes) / sizeof(LogoScheme))


Display::Display(int displayPin)
    : currentColorScheme_(0),
      pixels_(N_LEDS, displayPin),
      animations_(N_LEDS, NEO_MILLISECONDS) {}

Display::~Display() {}


void Display::setup() {
  pixels_.Begin();
  LightLogo_(colorSchemes[0]);
  pixels_.Show();
}

void Display::loop() {
  if (animations_.IsAnimating()) {
    animations_.UpdateAnimations();
    pixels_.Show();
  }
}

const std::vector<const char *> Display::availableSchemes() {
  std::vector<const char *> schemes;

  for (int i = 0; i < SCHEME_COUNT; i++) {
    schemes.push_back(colorSchemes[i].name);
  }
  return schemes;
}

void Display::setSchemeAtIndex(int index) {
  LightLogo_(colorSchemes[index]);
}

boolean Display::isAnimating() {
  return animations_.IsAnimating();
}

void Display::LightSegment_(int startIndex, int length, const RgbColor& color) {
  for (int led = 0; led < length; led++) {
    int ledIndex = startIndex + led;
    RgbColor currentColor = pixels_.GetPixelColor(ledIndex);
    RgbColor nextColor = color;
    
    AnimUpdateCallback animUpdate = [=](const AnimationParam& param) {
        float progress = NeoEase::QuadraticIn(param.progress);
        RgbColor intermediate =
            RgbColor::LinearBlend(currentColor, nextColor, progress);
        pixels_.SetPixelColor(ledIndex, intermediate);
    };
    animations_.StartAnimation(ledIndex, TIME_CHANGE_ANIMATION_SPEED, animUpdate);
  }
}

void Display::LightLogo_(const LogoScheme& logoColor) {
  LightSegment_(0 * N_SEGMENT, N_SEGMENT, logoColor.top);
  LightSegment_(1 * N_SEGMENT, N_SEGMENT, logoColor.right);
  LightSegment_(2 * N_SEGMENT, N_SEGMENT, logoColor.left);
  LightSegment_(3 * N_SEGMENT, N_CENTER, logoColor.center);
}

