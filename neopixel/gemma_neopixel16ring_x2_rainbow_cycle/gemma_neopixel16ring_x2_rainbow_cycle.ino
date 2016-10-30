// Rainbow color cycle for Adafruit GEMMA + 2 x 16 LED NeoPixel ring (with brightness toggle)

#include <Adafruit_NeoPixel.h>

const uint8_t STRIP_PIN = 0;
const uint8_t TOGGLE_PIN = 1;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(32, 0);

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

void setup() 
{
  pinMode(TOGGLE_PIN, INPUT);
  digitalWrite(TOGGLE_PIN, HIGH);
  
  strip.begin();
  strip.show();
}

void loop() 
{ 
  uint8_t buttonState = digitalRead(TOGGLE_PIN);
  if (buttonState == LOW) {
   brightness = 255;
  } else {
    brightness = 64;
  }
  strip.setBrightness(brightness);
  
  uint32_t vfactor = 255 / strip.numPixels();
  uint8_t curr_pixel = pixel_shift;
  for (uint8_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(curr_pixel, ComputeColor(hue, saturation, (i * vfactor)));
    if (curr_pixel++ > strip.numPixels()) {
      curr_pixel = 0;
    }
  }
  strip.show();
  
  if (pixel_shift++ > strip.numPixels()) {
    pixel_shift = 0;
  }
  
  if (hue++ > 256) {
    hue = 0;
  }
  
  delay(CYCLE_DELAY);
}
