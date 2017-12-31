package main

import (
	"github.com/godbus/dbus"
	"fmt"
	"log"
	"github.com/tarm/serial"
	"github.com/ottojo/rclib"
	"encoding/json"
)

var conn *dbus.Conn

func main() {
	var err error
	conn, err = dbus.SessionBus()
	if err != nil {
		log.Fatal(err)
	}

	_, err = conn.RequestName("de.toolboxbodensee.plane", dbus.NameFlagDoNotQueue)
	if err != nil {
		log.Fatal(err)
	}

	c := &serial.Config{Name: "/dev/ttyACM0", Baud: 9600}
	s, err := serial.OpenPort(c)
	if err != nil {
		log.Fatal(err)
	}

	bytesChan := make(chan byte, 3)
	go func(c chan byte) {
		for {
			buf := make([]byte, 128)
			n, err := s.Read(buf)
			if err != nil {
				log.Fatal(err)
			}
			for _, v := range buf[:n] {
				c <- v
			}
		}
	}(bytesChan)
	packagesChan := make(chan rclib.Package, 16)
	go rclib.DecodePackages(bytesChan, packagesChan)

	for p := range packagesChan {
		sendOnPackageReceived(p)
	}
}

func sendOnPackageReceived(p rclib.Package) {
	jsonPackage, err := json.Marshal(p)
	if err != nil {
		fmt.Println(err)
	}
	err = conn.Emit("/rcdata", "de.toolboxbodensee.plane.onRcPackageReceived", string(jsonPackage))
	if err != nil {
		fmt.Println(err)
	}
}
