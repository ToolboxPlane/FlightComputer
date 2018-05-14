#! /bin/sh
### BEGIN INIT INFO
# Provides: FlightComputer
# Required-Start: $syslog
# Required-Stop: $syslog
# Default-Start: 2 3 4 5
# Default-Stop: 0 1 6
# Short-Description: gpsd and flightcomputer
# Description: does stuff
### END INIT INFO

case "(" in
    start)
        echo "Starting gpsd"
        gpsd -S 3000 /dev/ttyS0
        echo "Starting FlightComputer"
        /home/pi/FlightComputer/build/FlightComputer
        ;;
    stop)
        echo "Stopping FlightComputer"
        killall FlightComputer
        echo "Stopping gpsd"
        killall gpsd
        ;;
    *)
        echo "Usage: /etc/init.d/FlightComputer {start|stop}"
        exit 1
        ;;
esac

exit 0)

