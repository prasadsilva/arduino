// Rainbow color cycle for Adafruit GEMMA + 16 LED NeoPixel ring

#include <Adafruit_NeoPixel.h>

const uint8_t   OUTPUT_PIN   = 0;
const uint8_t   NUM_PIXELS   = 16;

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUM_PIXELS, OUTPUT_PIN);

uint8_t brightness = 255;
uint8_t hue = 0, saturation = 255, value = 255;
uint8_t red, green, blue;

// Modified HSV->RGB function
// Original: http://stackoverflow.com/questions/3018313/algorithm-to-convert-rgb-to-hsv-and-hsv-to-rgb-in-range-0-255-for-both
//
void ComputeNextRgb()
{  
    uint8_t region, p, q, t;
    uint32_t h, s, v, remainder;

    // converting to 16 bit to prevent overflow
    h = hue;
    s = saturation;
    v = value;
    
    if (hue == 255) hue = 0;
    else hue++;

    if (saturation == 0)
    {
        red = value;
        green = value;
        blue = value;
        return;
    }

    region = h / 43;
    remainder = (h - (region * 43)) * 6; 

    p = (v * (255 - s)) >> 8;
    q = (v * (255 - ((s * remainder) >> 8))) >> 8;
    t = (v * (255 - ((s * (255 - remainder)) >> 8))) >> 8;

    switch (region)
    {
        case 0:
            red = v;
            green = t;
            blue = p;
            break;
        case 1:
            red = q;
            green = v;
            blue = p;
            break;
        case 2:
            red = p;
            green = v;
            blue = t;
            break;
        case 3:
            red = p;
            green = q;
            blue = v;
            break;
        case 4:
            red = t;
            green = p;
            blue = v;
            break;
        default:
            red = v;
            green = p;
            blue = q;
            break;
    }
}

void setup() 
{
  pixels.begin();
}

void loop() 
{
  ComputeNextRgb();
  
  pixels.setBrightness(brightness);
  for (uint8_t ledNumber = 0; ledNumber < NUM_PIXELS; ledNumber++) {
    pixels.setPixelColor(ledNumber, red, green, blue);
  }
  pixels.show();
  delay(200);
}
