#include "lcd.h"

Nokia_LCD nokiaLcd(7 /* CLK */, 6 /* DIN */, 5 /* DC */, 4 /* CE */, 3 /* RST */);
const char * ZERO = "0";
const char * COLON = ":";

void lcdPrintFix(gps_fix::status_t status) {
  lcdClearLine(0, 1);
  nokiaLcd.setCursor(0, 1);
  nokiaLcd.print("FIX ");
  switch (status) {
    case gps_fix::STATUS_NONE:
      nokiaLcd.print("NONE");
      break;
    case gps_fix::STATUS_EST:
      nokiaLcd.print("EST");
      break;
    case gps_fix::STATUS_TIME_ONLY:
      nokiaLcd.print("TIME_ONLY");
      break;
    case gps_fix::STATUS_STD:
      nokiaLcd.print("STD");
      break;
    case gps_fix::STATUS_DGPS:
      nokiaLcd.print("DGPS");
      break;
    case gps_fix::STATUS_RTK_FLOAT:
    case gps_fix::STATUS_RTK_FIXED:
      nokiaLcd.print("RTK");
      break;
    case gps_fix::STATUS_PPS:
      nokiaLcd.print("PPS");
      break;
  }
}

void lcdPrintSDFail(const char* msg) {
  nokiaLcd.clear();
  lcdClearLine(0, 0);
  nokiaLcd.setCursor(0, 0);
  nokiaLcd.print("SD ");
  nokiaLcd.print(msg);
}

void lcdPrintSDStat(float fs, const char* sdCardType) {
  lcdClearLine(0, 0);
  nokiaLcd.setCursor(0, 0);
  nokiaLcd.print(sdCardType);
  nokiaLcd.print(" ");
  nokiaLcd.print(fs, 1);
  nokiaLcd.print("GB");
}

void lcdPrint(int x, int y, const char* msg) {
  nokiaLcd.setCursor(x, y);
  nokiaLcd.print(msg);
}

void lcdClearLine(int x, int y) {
  lcdPrint(x, y, "               ");
}

void lcdPrintLatLon(float lat, float lon) {
  lcdClearLine(0,2);
  nokiaLcd.setCursor(0, 2);
  nokiaLcd.print(lat, 5);
  nokiaLcd.print(" ");
  nokiaLcd.print(lon, 5);
}

void lcdPrintSatellites(uint8_t sats) {
  lcdClearLine(0,3);
  nokiaLcd.setCursor(0, 3);
  nokiaLcd.print("Sats: ");
  nokiaLcd.print(sats);
}

void lcdPrintTime(NeoGPS::time_t dateTime, long msLeftToWrite) {
  lcdClearLine(0, 5);
  nokiaLcd.setCursor(0, 5);
  if (dateTime.hours < 10) 
    nokiaLcd.print(ZERO);
  nokiaLcd.print((uint8_t) dateTime.hours);
  nokiaLcd.print(COLON);
  if (dateTime.minutes < 10) 
    nokiaLcd.print(ZERO);
  nokiaLcd.print((uint8_t) dateTime.minutes);
  nokiaLcd.print(COLON);
  if (dateTime.seconds < 10) 
    nokiaLcd.print(ZERO);
  nokiaLcd.print((uint8_t) dateTime.seconds);
  nokiaLcd.setCursor(60, 5);
  nokiaLcd.print(msLeftToWrite);
}

void lcdInit() {
  nokiaLcd.begin();
  nokiaLcd.setContrast(50);
  nokiaLcd.clear();
  nokiaLcd.setCursor(0, 0);
}
