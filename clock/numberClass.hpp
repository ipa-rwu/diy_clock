#ifndef __NUMBERS_HPP__
#define __NUMBERS_HPP__

#include <TimeLib.h>

#define FASTLED_ESP8266_RAW_PIN_ORDER
#define FASTLED_ESP8266_NODEMCU_PIN_ORDER
#define FASTLED_ESP8266_D1_PIN_ORDER

#include <FastLED.h>

#define NUM_LEDS 21
#define NUM_LEDS_ALL 21*4+2
#define NUM_DOTS 2
#define COLOR_ORDER BGR
#define DATA_PIN 13
#define CLOCK_PIN 12
#define showled_num 3


class NumberClass
{
  public:
    NumberClass();
    ~NumberClass();
    
    time_t on_time;
    time_t off_time;
    
    struct colorHSV
    {
      int h;
      int s;
      int v;
    };
    colorHSV color_hsv;

    void setup_led();

    void show_test();
    void show_number(int number, int num_number, colorHSV color);
    void fade_all();
    void blink_dots(int led_num, time_t current_time, colorHSV color);
    void fade_number(int start_num, int num_number);
    
  private:
    bool _flag;
    int _data_pin;
    int _clock_pin;
    void choose_line(int line_num,int num_number, colorHSV color);
    CRGB _leds[NUM_LEDS_ALL];
};



#endif __NUMBERS_HPP__
