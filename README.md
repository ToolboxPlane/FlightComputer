[![Build Status](https://travis-ci.org/ToolboxPlane/FlightComputer.svg?branch=master)](https://travis-ci.org/ToolboxPlane/FlightComputer)
# FlightComputer
All the modules running on the Raspberry Pi. 

## Output Package
Both output packages are 4-Channel, 10 bit Packages.

### Flightcontroller
| Channel | Content |
| --- | --- |
| 0 | Power (between 0 and 1023) |
| 1 | Pitch-Target + 180 |
| 2 | Roll-Target + 180 |
| 3 | Empty |

### LoRa
| Channel | Content |
| --- | --- |
| 0 | -RSSI |
| 1 | Altitude above ground |
| 2 | Latitude * 10 |
| 3 | Longitude * 10 |

## Recording
Recordings are saved as pseudo-csv files. The first line
is the recording start time as a unix timestamp
(seconds since 1970). The second line is the textual
representation of the column names this is used for compatibility
and human readability. As a convention the first column is 
always the timestamp. All following lines are data.

## Waypoint file Format
The waypoint file is a special type of recording.
Each line consists of a latitude, a longitude, an altitude, the maximum distance to the waypoint (all doubles),
and a integer wheter the plane is allowed to land (1 or 0).

### Example
```
0
Timestamp; Lat; Lon; Altitude; MaxDelta; LandingAllowed
0; 0; 0; 17; 10; 0
0; 10; 10; 0; 5; 1
```
