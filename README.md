# FlightComputer
All the modules running on the Raspberry Pi. 

## Transmitter-IDs
| Device | Id |
| --- | --- |
| Receiver-Wing | 13 |
| LoRa-Remote | 17 |
| Flight-Controller | 23 |
| Flight-Computer | 38 |
| FrSky-Remote | 56 |
| Ground-Station | 63 |
| Power Distribution Board | 74 |

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
