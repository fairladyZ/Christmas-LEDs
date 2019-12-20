/*
  Digispark_Christmas_String_2019-12-19.ino
  
  Merry Christmas!!!
  
  Christmas is a very special time of year where we get to gather together with Family and friends
  and celebrate the birth of Jesus. 

  This code contains a few animations that can join in the festivities.
  
  Copyright (c) 2019, 87fairlady300zx@gmail.com
  All rights reserved.
  Redistribution and use in source and binary forms, with or without modification, 
  are permitted with the following conditions:
  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND 
  CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, 
  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR 
  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
  NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, 
  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
  A special thanks to Mark Kriegsman for offering the FastLED 
  library to the world making fun things like this possible
  
  */
#include <FastLED.h>

FASTLED_USING_NAMESPACE

#if defined(FASTLED_VERSION) && (FASTLED_VERSION < 3001000)
#warning "Requires FastLED 3.1 or later; check github for latest code."
#endif

#define DATA_PIN    0
#define INDICATOR   1
#define LED_TYPE    WS2811
#define COLOR_ORDER RGB
#define NUM_LEDS    50
#define STAR_START  150
#define STAR_STOP   160
#define Softwhite   0xDFAF4F
#define Purple      0xAF00FF

CRGB leds[NUM_LEDS];

#define BRIGHTNESS          75  // Brightness can go from 1 to 254
#define FRAMES_PER_SECOND   60

void setup() {
  pinMode(INDICATOR, OUTPUT);
  digitalWrite(INDICATOR, HIGH);
  delay(300); // 0.3 second delay for recovery
  
  // FastLED LED strip configuration
  FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);

  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);
}


// List of patterns to cycle through.  Each is defined as a separate function below.
typedef void (*SimplePatternList[])();
SimplePatternList gPatterns = { colors, PopChristmas, shimmer, ChristmasWithGlitter, gingerbreadWithGlitter, forestWithGlitter }; //Christmas, gingerbread, forest, ChristmasWithGlitter,

uint8_t gCurrentPatternNumber = 0; // Index number of which pattern is current
uint8_t gHue = 0; // rotating "base color" used by some of the patterns
  
void loop()
{
  // Illuminate the onboard LED to show the system is running
  digitalWrite(INDICATOR, HIGH);
  // Call the current pattern function once, updating the 'leds' array
  gPatterns[gCurrentPatternNumber]();

  // send the 'leds' array out to the actual LED strip
  FastLED.show();
  digitalWrite(INDICATOR, LOW);  
  // insert a delay to keep the framerate modest
  FastLED.delay(1000/FRAMES_PER_SECOND); 
  

  // do some periodic updates
  EVERY_N_MILLISECONDS( 40 ) { gHue++; } // slowly cycle the "base color" through the rainbow
  EVERY_N_SECONDS( 90 ) { nextPattern(); } // change patterns periodically
}

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

void nextPattern()
{
  // add one to the current pattern number, and wrap around at the end
  gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE( gPatterns);
}

void shimmer() 
{
  //Show something other than green and red, this is white, red, and yellow with random sparkles
  for( int i = 0; i < NUM_LEDS; i++) {
    leds[i] = Softwhite;
    i+=1;
    leds[i] = CRGB::Red;
    i+=1;
    leds[i] = CRGB::Yellow;
  }
  addGlitter(80);
}

void Christmas() 
{
  //Show Christmas colors Red, Green, White
  for( int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::Red;
    i+=1;
    leds[i] = CRGB::Green;
    i+=1;
    leds[i] = Softwhite; //Soft White
  }
  /*for( int i = STAR_START; i < STAR_STOP +1; i++) {
    leds[i] = CRGB::Yellow;
    i+=1;
    leds[i] = CRGB::White;
  }*/
}

void colors() 
{
  //Show multi colors Prurple, White, Green, Yellow, Blue, and Red
  for( int i = 0; i < NUM_LEDS; i++) {
    leds[i] = Purple;
    i+=1;
    leds[i] = Softwhite;
    i+=1;
    leds[i] = CRGB::Green;
    i+=1;
    leds[i] = CRGB::Yellow;
    i+=1;
    leds[i] = CRGB::Blue;
    i+=1;
    leds[i] = CRGB::Red;
    
  }
  /*for( int i = STAR_START; i < STAR_STOP +1; i++) {
    leds[i] = CRGB::Yellow;
    i+=1;
    leds[i] = CRGB::White;
  }*/
}

void gingerbread() 
{
  // Show alternating red and white
  for( int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::Red;
    i+=1;
    leds[i] = Softwhite;
  }
}

void gingerbreadWithGlitter() 
{
    gingerbread();
    addGlitter(80);
}

void forest() 
{
  // Show alternating Green and white
  for( int i = 0; i < NUM_LEDS; i++) {
    leds[i] = Softwhite;
    i+=1;
    leds[i] = CRGB::Green;
  }
}

void forestWithGlitter() 
{
  forest();
  addGlitter(80);
}

void ChristmasWithGlitter() 
{
  // just add sparkles
  Christmas();
  addGlitter(80);
}

void shimmerWithGlitter() 
{
  // just add sparkles
  shimmer();
  addGlitter(80);
}

void addGlitter( fract8 chanceOfGlitter) 
{
  if( random8() < chanceOfGlitter) {
    leds[ random16(NUM_LEDS) ] += Softwhite;
  }
}

void confetti() 
{
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( leds, NUM_LEDS, 10);
  int pos = random16(NUM_LEDS);
  leds[pos] += CHSV( gHue + random8(64), 200, 255);
}

void PopChristmas() 
{
  // Christmas color speckles that blink in and fade smoothly
  fadeToBlackBy( leds, NUM_LEDS, 10);
  int pos = random16(NUM_LEDS);
  leds[pos] += CRGB::Green;
  pos = random16(NUM_LEDS);
  leds[pos] += CRGB::Red;
  pos = random16(NUM_LEDS);
  leds[pos] += Softwhite;
}

void sinelon()
{
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy( leds, NUM_LEDS, 20);
  int pos = beatsin16( 13, 0, NUM_LEDS-1 );
  //leds[pos] += CHSV( gHue, 255, 192);
  leds[pos] += CRGB( random(0,2)*128, random(0,2)*85, random(0,2)*128);
  
}

void bpm()
{
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
  for( int i = 0; i < NUM_LEDS; i++) { //9948
    leds[i] = ColorFromPalette(palette, gHue+(i*2), beat-gHue+(i*10));
  }
}

void juggle() {
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy( leds, NUM_LEDS, 20);
  byte dothue = 0;
  for( int i = 0; i < 8; i++) {
    leds[beatsin16( i+7, 0, NUM_LEDS-1 )] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
}
