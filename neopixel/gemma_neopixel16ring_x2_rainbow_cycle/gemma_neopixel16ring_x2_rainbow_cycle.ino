// Rainbow color cycle for Adafruit GEMMA + 2 x 16 LED NeoPixel ring (with brightness toggle)

// The two rings side by side will display an infinity symbol tracer effect

#include <Adafruit_NeoPixel.h>

const uint8_t STRIP_PIN = 0;
const uint8_t TOGGLE_PIN = 1;

const uint8_t TOTAL_PIXELS = 32;
const uint8_t TRAIL_PIXEL_COUNT = 10;  // Make sure this is less than TOTAL_PIXELS

Adafruit_NeoPixel strip = Adafruit_NeoPixel(TOTAL_PIXELS, 0);

uint16_t brightness = 255;
uint32_t hue = 0, saturation = 255;

const uint16_t CYCLE_DELAY = 25;

const uint32_t VFACTOR = 255 / (TOTAL_PIXELS - TRAIL_PIXEL_COUNT);

// This lookup allows compensating for physical orientation of the rings
// due to installation differences
uint16_t pixel_order_idx = 0;
const uint16_t PIXEL_ORDER[32] = {
  1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 0,
  26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 31, 30, 29, 28, 27,
};

void setup() 
{
  pinMode(TOGGLE_PIN, INPUT);
  digitalWrite(TOGGLE_PIN, HIGH);
  
  strip.begin();
  strip.show();
}

void loop() 
{ 
  // Switch between the two brightness levels
  uint8_t buttonState = digitalRead(TOGGLE_PIN);
  if (buttonState == LOW) {
   brightness = 255;
  } else {
    brightness = 64;
  }
  strip.setBrightness(brightness);
 
  uint16_t curr_pixel = pixel_order_idx;
  for (uint16_t i = 0; i < TOTAL_PIXELS; i++) {
    uint32_t value = (i < TRAIL_PIXEL_COUNT ? 0 : (i - TRAIL_PIXEL_COUNT) * VFACTOR);
    strip.setPixelColor(PIXEL_ORDER[curr_pixel], ComputeColor(hue, saturation, value));
    if (curr_pixel++ >= TOTAL_PIXELS) {
      curr_pixel = 0;
    }
  }
  strip.show();
  
  if (pixel_order_idx++ >= TOTAL_PIXELS) {
    pixel_order_idx = 0;
  }
  
  if (hue++ > 255) {
    hue = 0;
  }
  
  delay(CYCLE_DELAY);
}

// Modified HSV->RGB function
// Original: http://stackoverflow.com/questions/3018313/algorithm-to-convert-rgb-to-hsv-and-hsv-to-rgb-in-range-0-255-for-both
//
uint32_t ComputeColor(uint32_t h, uint32_t s, uint32_t v)
{  
  uint8_t region, p, q, t;
  uint32_t remainder;

  if (saturation == 0)
  {
    return strip.Color(v, v, v);
  }

  region = h / 43;
  remainder = (h - (region * 43)) * 6; 

  p = (v * (255 - s)) >> 8;
  q = (v * (255 - ((s * remainder) >> 8))) >> 8;
  t = (v * (255 - ((s * (255 - remainder)) >> 8))) >> 8;

  switch (region)
  {
    case 0:
        return strip.Color(v, t, p);
    case 1:
        return strip.Color(q, v, p);
    case 2:
        return strip.Color(p, v, t);    
    case 3:
        return strip.Color(p, q, v);    
    case 4:
        return strip.Color(t, p, v);    
    default:
        return strip.Color(v, p, q);
  }
}
