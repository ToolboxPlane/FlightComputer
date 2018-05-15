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

## JSON-Library by nlohmann (https://github.com/nlohmann/json)

## Waypoint file Format
The waypoint file is a json file, the main structure is an array of all waypoints.
Each waypoint consists of a latitude, a longitude, an altitude, the maximum distance to the waypoint (all numbers),
and a boolean wheter the plane is allowed to land.

### Example
```
[
  {
    "lat": 0,
    "lon": 0,
    "altitude": 17,
    "maxDelta": 5,
    "landingAllowed": false
  },
  {
    "lat": 0,
    "lon": 0,
    "altitude": 17,
    "maxDelta": 5,
    "landingAllowed": false
  }
]
```
