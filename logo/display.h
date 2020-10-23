#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#include <NeoPixelBrightnessBus.h>
#include <NeoPixelAnimator.h>

// This holds the various colors needed to update the Chrome logo.
typedef struct {
  const char* name;
  const RgbColor top, right, left, center;
} LogoScheme;


//
// This class Pilots a Chrome logo led display, composed of 3 segments of LEDs
// and a center ring.
//
class Display {
  public:
    Display(int displayPin);
    ~Display();

    Display(const Display&) = delete;
    Display& operator=(const Display&) = delete;
    Display(Display&&) = delete;
    Display& operator=(Display&&) = delete;

    // Call from main setup to initialize the display.
    void setup();
    // Call from the main loop to keep eventual animations running.
    void loop();

    // Returns the names of all the available color schemes to put on the
    // display.
    const std::vector<const char *> availableSchemes();
    // Select the color scheme to switch to. The transition is animated.
    void setSchemeAtIndex(int index);

    // Is there a transition currently taking place.
    boolean isAnimating();

  private:
    // Start all animations to switch a segment of the led strip to a particular
    // color. The animation changes the color from the current color to the one
    // passed as argument.
    void LightSegment_(int startIndex, int length, const RgbColor& color);

    // Switch the whole strip to the log colors passed in.
    void LightLogo_(const LogoScheme& logoColor);
  
    // Logo colors to display next
    int currentColorScheme_;
  
    // The bus for all the LEDs.
    NeoPixelBus<NeoGrbFeature, NeoEsp32I2s0Ws2812xMethod> pixels_;
    // For animation, Things are kept simple with one channel per led.
    NeoPixelAnimator animations_;
};

#endif // _DISPLAY_H_
