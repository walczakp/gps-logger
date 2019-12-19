#ifndef _LCD_H
#define _LCD_H
#include <Nokia_LCD.h>
#include <NMEAGPS.h>

// ------------------- Nokia 5110 LCD
extern Nokia_LCD nokiaLcd;

void lcdInit();
void lcdPrintFix(gps_fix::status_t status);
void lcdPrintSDStat(float fs, const char* sdCardType);
void lcdPrintSDFail(const char* msg);
void lcdPrint(int x, int y, const char* msg);
void lcdClearLine(int x, int y);
void lcdPrintLatLon(float lat, float lon); 
void lcdPrintSatellites(uint8_t sats);
void lcdPrintTime(NeoGPS::time_t dateTime, long msLeftToWrite);

#endif /* _LCD_H */
