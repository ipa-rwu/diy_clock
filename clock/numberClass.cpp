#include "numberClass.hpp"

NumberClass::NumberClass()
{

}

NumberClass::~NumberClass()
{
}

void NumberClass::setup_led()
{
  FastLED.addLeds<APA102, DATA_PIN, CLOCK_PIN, BGR, DATA_RATE_MHZ(12)>(_leds, NUM_LEDS_ALL);
  LEDS.setBrightness(100);
  fill_solid( &(_leds[0]), NUM_LEDS_ALL /*number of leds*/, CRGB( 255, 0, 0) );
  FastLED.show();
  delay(2000);
}

void NumberClass::choose_line(int line_num,int num_number, colorHSV color)
{
  fill_solid( &(_leds[num_number + line_num*showled_num]), showled_num /*number of leds*/, CRGB(color.h, color.s, color.v) );
}


void NumberClass::blink_dots(int led_num, time_t current_time, colorHSV color)
{
  // Serial.print(current_time);
  // Serial.print("\n");
  //Serial.print(on_time);
  //Serial.print("\n");
  //Serial.print(off_time);
  //Serial.print("\n");
  if (on_time == 0 && off_time == 0)
  {
    fill_solid( &(_leds[led_num]), NUM_DOTS /*number of leds*/, CHSV(color.h, color.s, color.v));
    FastLED.show();
    _flag = 1;
    on_time = current_time;
  }
  
  if (current_time - on_time >=1 && _flag == 1)
  {
    fill_solid( &(_leds[led_num]), NUM_DOTS /*number of leds*/, CHSV( 0, 0, 0) );
    FastLED.show();
    off_time = current_time;
    _flag = 0;
  }

   if (off_time !=0 && current_time - off_time >=1 && _flag == 0)
  {
    fill_solid( &(_leds[led_num]), NUM_DOTS /*number of leds*/, CHSV(color.h, color.s, color.v));
    FastLED.show();
    on_time = current_time;
    _flag = 1;
  }
  
}



void NumberClass::show_number(int number, int num_number, colorHSV color)
{
  switch(number)
  {
    case 0:
      choose_line(0, num_number, color);
      choose_line(1, num_number, color);
      choose_line(2, num_number, color);
      choose_line(6, num_number, color);
      choose_line(5, num_number, color);
      choose_line(4, num_number, color);
      FastLED.show();
      break;

    case 1:
      choose_line(2,num_number, color);
      choose_line(6, num_number, color);
      FastLED.show();
      break;

    case 2:
      choose_line(0, num_number, color);
      choose_line(1, num_number, color);
      choose_line(6, num_number, color);
      choose_line(5 ,num_number, color);
      choose_line(3, num_number, color);
      FastLED.show();
      break;

    case 3:
      choose_line(2, num_number, color);
      choose_line(1, num_number, color);
      choose_line(6, num_number, color);
      choose_line(5, num_number, color);
      choose_line(3, num_number, color);
      FastLED.show();
      break;

    case 4:
      choose_line(4, num_number, color);
      choose_line(3, num_number, color);
      choose_line(6, num_number, color);
      choose_line(2, num_number, color);
      FastLED.show();
      break;

    case 5:
      choose_line(5, num_number, color);
      choose_line(4, num_number, color);
      choose_line(3, num_number, color);
      choose_line(2, num_number, color);
      choose_line(1, num_number, color);
      FastLED.show();
      break;

    case 6:
      choose_line(5, num_number, color);
      choose_line(4, num_number, color);
      choose_line(3, num_number, color);
      choose_line(0, num_number, color);
      choose_line(1, num_number, color);
      choose_line(2, num_number, color);
      FastLED.show();
      break;

    case 7:
      choose_line(5, num_number, color);
      choose_line(6, num_number, color);
      choose_line(2, num_number, color);
      FastLED.show();
      break;
        
    case 8:
      choose_line(0, num_number, color);
      choose_line(1, num_number, color);
      choose_line(2, num_number, color);
      choose_line(6, num_number, color);
      choose_line(5, num_number, color);
      choose_line(4, num_number, color);
      choose_line(3, num_number, color);
      FastLED.show();
      break;

    case 9:
      choose_line(1, num_number, color);
      choose_line(2, num_number, color);
      choose_line(6, num_number, color);
      choose_line(5, num_number, color);
      choose_line(4, num_number, color);
      choose_line(3, num_number, color);
      FastLED.show();
      break;
  }
}

void NumberClass::fade_all()
{
  fill_solid( &(_leds[0]), NUM_LEDS_ALL /*number of leds*/, CRGB( 0, 0, 0) );
  FastLED.show();
}

void NumberClass::fade_number(int start_num, int num_number)
{
  fill_solid( &(_leds[start_num]), num_number /*number of leds*/, CRGB( 0, 0, 0) );
  FastLED.show();
}

void NumberClass::show_test() { 

    fill_solid( &(_leds[0]), 21 /*number of leds*/, CRGB( 0, 0, 0) ); 
    FastLED.show(); 
}
