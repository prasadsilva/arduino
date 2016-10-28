// Rainbow color cycle for Adafruit GEMMA + 16 LED NeoPixel ring

#include <Adafruit_NeoPixel.h>

const uint8_t   OUTPUT_PIN   = 0;
const uint8_t   NUM_PIXELS   = 16;

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUM_PIXELS, OUTPUT_PIN);

uint8_t brightness = 255;
uint8_t hue = 0, saturation = 255, value = 255;

// Modified HSV->RGB function
// Original: http://stackoverflow.com/questions/3018313/algorithm-to-convert-rgb-to-hsv-and-hsv-to-rgb-in-range-0-255-for-both
//
uint32_t ComputeColor(uint8_t hparam, uint8_t sparam, uint8_t vparam)
{  
  uint8_t region, p, q, t;
  uint32_t h, s, v, remainder;

  // Using 32 bit to support bit shift later
  h = hparam;
  s = sparam;
  v = vparam;

  if (saturation == 0)
  {
    return pixels.Color(value, value, value);
  }

  region = h / 43;
  remainder = (h - (region * 43)) * 6; 

  p = (v * (255 - s)) >> 8;
  q = (v * (255 - ((s * remainder) >> 8))) >> 8;
  t = (v * (255 - ((s * (255 - remainder)) >> 8))) >> 8;

  switch (region)
  {
    case 0:
        return pixels.Color(v, t, p);
    case 1:
        return pixels.Color(q, v, p);
    case 2:
        return pixels.Color(p, v, t);    
    case 3:
        return pixels.Color(p, q, v);    
    case 4:
        return pixels.Color(t, p, v);    
    default:
        return pixels.Color(v, p, q);
  }
}

void setup() 
{
  pixels.begin();
  pixels.setBrightness(brightness);
  pixels.show();
}

void loop() 
{ 
  for (uint8_t ledNumber = 0; ledNumber < NUM_PIXELS; ledNumber++) {
    pixels.setPixelColor(ledNumber, ComputeColor(hue, saturation, value / ledNumber);
  }
  pixels.show();
  delay(200);
  
  if (hue++) {
    hue = 0;
  }
}
