[![Build Status](https://travis-ci.org/ToolboxPlane/FlightComputer.svg?branch=master)](https://travis-ci.org/ToolboxPlane/FlightComputer)
# FlightComputer
All the modules running on the Raspberry Pi. 

## Output Package
Both output packages are 4-Channel, 10 bit Packages.

| Channel | Content |
| --- | --- |
| 0 | Power (between 0 and 1023) |
| 1 | Pitch-Target + 180 |
| 2 | Roll-Target + 180 |
| 3 | Empty |

## Waypoint file Format
The waypoint file is a csv file.
Each line consists of a latitude, a longitude, an altitude, the maximum distance to the waypoint (all doubles),
and a integer wheter the plane is allowed to land (1 or 0).

### Example
```
Timestamp; Lat; Lon; Altitude; MaxDelta; LandingAllowed
0; 0; 0; 17; 10; 0
0; 10; 10; 0; 5; 1
```
