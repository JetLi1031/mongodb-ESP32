#include <WiFi.h>
#include "time.h"

const char* ssid       = "Wong_2.4GHz@unifi";
const char* password   = "60127886883@60127882883";

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 3600;
const int   daylightOffset_sec = 3600;

void printLocalTime()
{
  time_t now;
  char strftime_buf[64];
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  time(&now);
// Set timezone to China Standard Time
  setenv("TZ", "CST-8", 1);
  tzset();

  localtime_r(&now, &timeinfo);
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
  strftime(strftime_buf, sizeof(strftime_buf), "%d/%b/%Y %H:%M:%S", &timeinfo);
  Serial.println(strftime_buf);
}

void setup()
{
  Serial.begin(115200);
  
  //connect to WiFi
  Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
  }
  Serial.println(" CONNECTED");
  
  //init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  printLocalTime();

  //disconnect WiFi as it's no longer needed
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
}

void loop()
{
  delay(1000);
  printLocalTime();
}
