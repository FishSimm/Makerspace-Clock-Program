//
//acrylic is at 400 dpi, k=50, brighten as texture    15.73
#include <DS1307RTC.h>
#include <TimeLib.h>
#include <FastLED.h>

#define NUM_LEDS 28*4+4
#define LED_PIN 4

CRGB leds[NUM_LEDS];

int prev_time;
void setup() 
{
  Serial.begin(9600);
  setSyncProvider(RTC.get);
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(50); //0-255
  for (int i = 0; i < NUM_LEDS; i++)
  {
    leds[i] = CRGB::Black;
    FastLED.show();
  }
 
  prev_time = minute()-1;
}

//This is the layout of a 7-seg display. I set up the strips in this order, F -> A -> B -> C -> D -> E -> G
//    ---A---           --->
//   |       |        ^     |
//   F       B    in->|     v
//   |       |         --------->out
//    ---G---         ^     |
//   |       |        |     v
//   E       C          <---
//   |       |
//    ---D---

//What leds should be on or off for each number
byte DigitStates[10] [28] = 
{
// F F F F A A A A B B B B C C C C D D D D E E E E G G G G
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0}, //0
  {0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0}, //1
  {0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1}, //2
  {0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1}, //3
  {1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1}, //4
  {1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1}, //5
  {1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}, //6
  {0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0}, //7
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}, //8
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1}  //9
};


CRGB makerspaceBlue = (0x0096d6);
//Rainbow list
CRGB colorCode [32] = {
    0xFF0000, 0xD52A00, 0xAB5500, 0xAB7F00, 
    0xABAB00, 0x56D500, 0x00FF00, 0x00D52A,
    0x00AB55, 0x0056AA, 0x0000FF, 0x2A00D5,
    0x5500AB, 0x7F0081, 0xAB0055, 0xD5002B, 
    0xD5002B, 0xAB0055, 0x7F0081, 0x5500AB, 
    0x2A00D5, 0x0000FF, 0x0056AA, 0x00AB55, 
    0x00D52A, 0x00FF00, 0x56D500, 0xABAB00, 
    0xAB7F00, 0xAB5500, 0xD52A00, 0xFF0000
    };

void SetDigit(int digit, int place)
{
  //Middle dot lights
  leds[112] = makerspaceBlue;
  leds[113] = makerspaceBlue;
  leds[114] = makerspaceBlue;
  leds[115] = makerspaceBlue;

  //pay no attention to this....
  int hour12 = hour();
  int curtime = minute();
  int minOne = curtime%10;
  int minTen = curtime/10;
  int hourOne = hour12%10;
  int hourTen = hour12/10;

  // offset the place of the lights to the next number, 28 leds away
  int led_offset = place*28;
 
  for (int seg = 0; seg < 28; seg++)
    {
      if ((hour() < 9) || (hour() >= 16))
      {
        // Between 4 and 9 it turns purple
        leds[seg+led_offset] = DigitStates[digit][seg]? CRGB::Purple : CRGB::Black;
      }
     
     else if (((hour() == 16) && (minute() == 19)) || ((hour() == 16) && (minute() == 21)))
      { 
        //Turns green at 4:19 and 4:21, but NOT at 4:20
        //                                  Both colors have to be the same type, CRGB or Palette16
        //                                             Color to turn   Off color
        leds[seg+led_offset] = DigitStates[digit][seg]? CRGB::Green : CRGB::Black;
        //If its a one, turn it on, otherwise dont
      }
      else if ((hour() == 13) && (minute() == 23))
      {
        //Rainbow at 1:23 
        leds[seg+led_offset] = DigitStates[digit][seg]? colorCode[seg] : CRGB::Black;
      }
      else if ((hour() == 12) && (minute() == 34))
      {
        //Rainbow at 12:34
        leds[seg+led_offset] = DigitStates[digit][seg]? colorCode[seg] : CRGB::Black;
      }
      else if ((hour() == 17) && (minute() == 38))
      {
        //Turns purple at 17:38, ayy
        leds[seg+led_offset] = DigitStates[digit][seg]? CRGB::Purple : CRGB::Black;
      }
      else if ((hour() == 13) && (minute() == 11))
      {
        //Turns gold at 1:11
        leds[seg+led_offset] = DigitStates[digit][seg]? CRGB::Gold : CRGB::Black;
      }
       else if ((hour() == 14) && (minute() == 22))
      {
        //Turns gold at 2:22
        leds[seg+led_offset] = DigitStates[digit][seg]? CRGB::Gold : CRGB::Black;
      }
       else if ((hour() == 15) && (minute() == 33))
      {
        //Turns gold at 3:33
        leds[seg+led_offset] = DigitStates[digit][seg]? CRGB::Gold : CRGB::Black;
      }
       else if ((hour() == 16) && (minute() == 44))
      {
        //Turns gold at 4:44
        leds[seg+led_offset] = DigitStates[digit][seg]? CRGB::Gold : CRGB::Black;
      }
       else if ((hour() == 17) && (minute() == 55))
      {
        //Turns gold at 5:55
        leds[seg+led_offset] = DigitStates[digit][seg]? CRGB::Gold : CRGB::Black;
      }
      else if ((hour() == 11) && (minute() == 11))
      {
        //Turns gold at 11:11
        leds[seg+led_offset] = DigitStates[digit][seg]? CRGB::Gold : CRGB::Black;
      }
      else if ((hour() == 10) && (minute() == 11))
      {
        //Turns red at 10/11
        leds[seg+led_offset] = DigitStates[digit][seg]? CRGB::Red : CRGB::Black;
      }
      else
      {
        //Normal blue color when not at a special time
        leds[seg+led_offset] = DigitStates[digit][seg]? makerspaceBlue : CRGB::Black;
      }

      FastLED.show(); 
    }
  
}


void loop()
{
  int curtime = minute();
  int hour12 = hour();
  
  //Turns RTC 24 hour time into 12 hour time
  if ((hour() == 5) && (minute() == 38))
    //make clock show 17:38 at 5:38
    hour12 = 17;
  if (hour() > 12)
    hour12 = hour() - 12;
  

  if (curtime != prev_time)
  {
    //create tens and ones place for minute
    int minOne = curtime%10;
    int minTen = curtime/10;

  //SetDigit(what number, where);
    SetDigit(minOne, 3);
    SetDigit(minTen, 2);

    //create tens and ones place for hour
    int hourOne = hour12%10;
    int hourTen = hour12/10;

    SetDigit(hourOne, 1);
    SetDigit(hourTen, 0);

    prev_time = curtime;
  }
}



