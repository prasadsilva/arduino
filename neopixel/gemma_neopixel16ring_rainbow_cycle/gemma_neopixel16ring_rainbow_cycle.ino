// Rainbow color cycle for Adafruit GEMMA + 16 LED NeoPixel ring

#include <Adafruit_NeoPixel.h>

const uint8_t   OUTPUT_PIN   = 0;
const uint8_t   NUM_PIXELS   = 16;

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUM_PIXELS, OUTPUT_PIN);

uint8_t brightness = 255;
uint32_t hue = 0, saturation = 255;

const uint8_t CYCLE_DELAY = 50;

uint8_t pixel_shift = 0;

// Modified HSV->RGB function
// Original: http://stackoverflow.com/questions/3018313/algorithm-to-convert-rgb-to-hsv-and-hsv-to-rgb-in-range-0-255-for-both
//
uint32_t ComputeColor(uint32_t h, uint32_t s, uint32_t v)
{  
  uint8_t region, p, q, t;
  uint32_t remainder;

  if (saturation == 0)
  {
    return pixels.Color(v, v, v);
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
  uint8_t curr_pixel = pixel_shift;
  for (uint8_t i = 0; i < NUM_PIXELS; i++) {
    pixels.setPixelColor(curr_pixel, ComputeColor(hue, saturation, (i * 16) + 8));
    if (curr_pixel++ > NUM_PIXELS) {
      curr_pixel = 0;
    }
  }
  pixels.show();
  
  if (pixel_shift++ > NUM_PIXELS) {
    pixel_shift = 0;
  }
  
  if (hue++ > 256) {
    hue = 0;
  }
  
  delay(CYCLE_DELAY);
}
