#ifndef _GPS_SD_H
#define _GPS_SD_H

#include <SPI.h>
#include <SdFat.h>
#include <NMEAGPS.h>
#include "lcd.h"
#include <Arduino.h>

static const char* sdFetchType();
float sdGetSize();
void sdInit();
File sdOpenFile(char* filename);

#endif /* _GPS_SD_H */
