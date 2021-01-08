#include <TimeLib.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

const char ssid[] = "UPC6D53D17";
//"UPC6D53D17" "wext";
const char pass[] = "hY6yfwdhwCmt"; 
//"hY6yfwdhwCmt" "Nuwk3^CejoTexy";

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
