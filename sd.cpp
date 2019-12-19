#include "sd.h"
#define DEBUG_PORT Serial

const int chipSelect = 10;
const char* sdCardType = "";
SdFat SD;

boolean sdInitialized = false;

static const char* sdFetchType() {
  auto cardSize = SD.card()->cardSize();
  if (cardSize == 0) {
    DEBUG_PORT.println(F("cardSize failed"));
    return "????";
  }

  switch (SD.card()->type()) {
    case SD_CARD_TYPE_SD1:
      return "SD1";

    case SD_CARD_TYPE_SD2:
      return "SD2";
      break;

    case SD_CARD_TYPE_SDHC:
      if (cardSize < 70000000) {
        return "SDHC";
      } else {
        return "SDXC";
      }
  }
}


float sdGetSize() {
  uint32_t volFree = SD.vol()->freeClusterCount();
  return 0.000512 * volFree * SD.vol()->blocksPerCluster() / 1024;
}

void sdInit()
{
  lcdPrint(0, 0, "Initializing...");
  DEBUG_PORT.println( F("Initializing SD card...") );

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    DEBUG_PORT.println( F("  SD card failed, or not present") );
    // don't do anything more:
    lcdPrintSDFail("NOCARD");
    // Flicker the LED
    while (true) {}
  }

  DEBUG_PORT.println( F("  SD card initialized.") );
  
  sdCardType = sdFetchType();
  lcdClearLine(0, 0);
  float s = sdGetSize();
  lcdPrintSDStat(s, sdCardType);
  // Pick a numbered filename, 00 to 99.
//  char filename[15] = "data_##.txt";
//
//  for (uint8_t i = 0; i < 100; i++) {
//    filename[5] = '0' + i / 10;
//    filename[6] = '0' + i % 10;
//    if (!SD.exists(filename)) {
//      // Use this one!
//      break;
//    }
//  }

  sdInitialized = true;
} // initSD

File sdOpenFile(char* filename) {
  if (!sdInitialized) {
    DEBUG_PORT.print( F("Couldn't create ") );
    DEBUG_PORT.print(filename);
    DEBUG_PORT.print( F(", SD not yet initialized.") );
    lcdPrintSDFail("FAIL");
    while (true) {}
  }
  
  File logfile = SD.open(filename, FILE_WRITE);
  if (!logfile) {
    DEBUG_PORT.print( F("Couldn't create ") );
    DEBUG_PORT.println(filename);
    lcdPrintSDFail("FAIL");
    while (true) {}
  }
  
  DEBUG_PORT.print( F("Writing to ") );
  DEBUG_PORT.println(filename);
  return logfile;
}
