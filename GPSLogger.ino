#include <Arduino.h>

#define DEBUG_PORT Serial

// ------------------ GPS
#include <NMEAGPS.h>
#include <GPSport.h>
#include <Streamers.h>

#ifndef NMEAGPS_INTERRUPT_PROCESSING
#error You must define NMEAGPS_INTERRUPT_PROCESSING in NMEAGPS_cfg.h!
#endif

static const int LED = 2;

static NMEAGPS gps;

// ------------------- LCD
#include "lcd.h"

// ------------------ SD
#include "sd.h"

// ------------------- how often flush data
#define LCD_INTERVAL 2500
#define LOG_INTERVAL 10000
static unsigned long lastLcdTime = millis();
static unsigned long lastLogTime = millis();

void filenameWithDate(char * fname, NeoGPS::time_t dateTime) {
  sprintf_P(fname, (PGM_P)F("log_%04d%02d%02d_%02d%02d%02d.txt"),
              dateTime.full_year(),
              dateTime.month,
              dateTime.date,
              dateTime.hours,
              dateTime.minutes,
              dateTime.seconds
   );
}

bool startedWriting = false;
File logfile;
char fname[30];

static void GPSloop()
{
  if (gps.available()) {
    gps_fix fix = gps.read();
//    trace_header(DEBUG_PORT);
//    trace_all(DEBUG_PORT, gps, fix);
    // Log the fix information if we have a location and time
    if (fix.valid.location && fix.valid.time && fix.valid.date) {
      DEBUG_PORT.println("valid fix");
      if (!startedWriting) {
        filenameWithDate(fname, fix.dateTime);
        logfile = sdOpenFile(fname);
        gps.overrun(false);
        trace_header(logfile);
        startedWriting = true;
      }
      if (millis() - lastLogTime > LOG_INTERVAL) {
        DEBUG_PORT.println("sd write");
        trace_all(logfile, gps, fix);
        logfile.flush();
        gps.overrun(false);
        lastLogTime = millis();
        return;
      }
    }
    if (fix.valid.status) {
//        DEBUG_PORT.println("valid status");
        if (millis() - lastLcdTime > LCD_INTERVAL) {
          lcdPrintFix(fix.status);
          if (fix.valid.location) {
            lcdPrintLatLon(fix.latitude(), fix.longitude());
          } else {
            lcdPrintLatLon(0, 0);
          }
          if (fix.valid.satellites) {
            lcdPrintSatellites(fix.satellites);
          }
          if (fix.valid.time) {
            lcdPrintTime(fix.dateTime, LOG_INTERVAL - (millis() - lastLogTime));
          }
          lastLcdTime = millis();
        }
    }
  }

} // GPSloop

//----------------------------------------------------------------

void GPSisr( uint8_t c )
{
  gps.handle( c );

} // GPSisr

void gpsInit() {
  gps.overrun(false);
}

//----------------------------------------------------------------

void setup()
{
  DEBUG_PORT.begin(9600);
  while (!DEBUG_PORT)
    ; // wait for serial port to connect.

  gpsPort.attachInterrupt( GPSisr );
  gpsPort.begin( 4800 );

  // Enable the LED for blinking feedback
  pinMode( LED, OUTPUT );

  lcdInit();
  sdInit();
  gpsInit();
} // setup

//----------------------------------------------------------------

void loop()
{
  GPSloop();

  if (gps.overrun()) {
    gps.overrun( false );
    DEBUG_PORT.println( F("DATA OVERRUN: fix data lost!") );
  }
}

//----------------------------------------------------------------
