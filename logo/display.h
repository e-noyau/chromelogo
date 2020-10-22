#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#include <NeoPixelBrightnessBus.h>
#include <NeoPixelAnimator.h>

typedef struct { RgbColor top, right, left, center; } LogoColors;

class Display {
  public:
    Display();
    ~Display();

    Display(const Display&) = delete;
    Display& operator=(const Display&) = delete;
    Display(Display&&) = delete;
    Display& operator=(Display&&) = delete;

    void setup();
    void loop();

  private:
    // Start all animations to switch a segment of the led strip to a particular
    // color. The animation changes the color from the current color to the one
    // passed as argument.
    void LightSegment_(int startIndex, int length, const RgbColor& color);

    // Switch the whole strip to the log colors passed in.
    void LightLogo_(const LogoColors& logoColor);
  
    // Logo colors to display next
    int currentColorScheme_;
  
    // The bus for all the LEDs.
    NeoPixelBus<NeoGrbFeature, NeoEsp32I2s0Ws2812xMethod> pixels_;
    // For animation, Things are kept simple with one channel per led.
    NeoPixelAnimator animations_;
};

#endif // _DISPLAY_H_
