#! /usr/bin/env python3
import csv
import sys
import gpxpy.gpx
from datetime import datetime, timedelta
from pathlib import Path

# constants
PATH_SPLIT_THRESHOLD = timedelta(seconds=60)

# methods
def to_datetime(row):
    return datetime.strptime(row["UTC Date/Time"], "%Y-%m-%d %H:%M:%S.%f")

def to_float(row, field, precision):
    str_val = row[field]
    val = float(str_val) / pow(10.0, precision) if str_val else 0
    return round(val, precision)

def knots_to_ms(knots):
    return knots * 0.514444444


def parse_point(row):
    gpx_point = gpxpy.gpx.GPXTrackPoint(
        latitude=to_float(row, "Lat", 7),
        longitude=to_float(row, "Lon", 7),
        elevation=to_float(row, "Alt", 2),
        speed=knots_to_ms(to_float(row, "Spd", 3)),
        time=to_datetime(row),
        horizontal_dilution=to_float(row, "HDOP", 3),
        vertical_dilution=to_float(row, "VDOP", 3),
        position_dilution=to_float(row, "PDOP", 3),
    )
    gpx_point.source = "gps"
    gpx_point.course = to_float(row, "Hdg", 2)
    gpx_point.satellites = row["Sats"]
    return gpx_point


if __name__ == '__main__':
    for input in sys.argv[1:]:
        print(input, end=" ")
        filename = Path(input)
        output = filename.with_suffix('.gpx')
        if output.exists():
            print("skipped")
            continue

        gpx = gpxpy.gpx.GPX()
        with open(str(filename), mode='r') as csv_file:
            csv_reader = csv.DictReader(csv_file)

            gps_previous_datetime = None
            gpx_track = gpxpy.gpx.GPXTrack()
            gpx.tracks.append(gpx_track)
            gpx_segment = None
            for row in csv_reader:
                # initialize time from first record
                gps_datetime = to_datetime(row)
                if not gpx.time:
                    gpx.time = gps_datetime

                # add new segment if there is enough time difference
                if not gpx_segment or (gps_previous_datetime and gps_datetime - gps_previous_datetime > PATH_SPLIT_THRESHOLD):
                    gpx_segment = gpxpy.gpx.GPXTrackSegment()
                    gpx_track.segments.append(gpx_segment)

                # parse to gpx_point
                gpx_point = parse_point(row)
                gpx_segment.points.append(gpx_point)

                # set current datetime as previous for next loop
                gps_previous_datetime = gps_datetime

        with open(str(output), mode='w') as gpx_file:
            gpx_file.write(gpx.to_xml(version='1.0'))

        print("OK")
