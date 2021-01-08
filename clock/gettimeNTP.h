#include <TimeLib.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>



// NTP Servers:
static const char ntpServerName[] = "de.pool.ntp.org";

const int timeZone = 2;     // Central European Time


struct TimeStruct {
  int hour_1;
  int hour_2;
  int min_1;
  int min_2;
};
extern struct TimeStruct time_array;



time_t getNtpTime();
void digitalClockDisplay();
void printDigits(int digits);
void sendNTPpacket(IPAddress &address);
TimeStruct gettime();

void SetupTime();
void ShowTime();
