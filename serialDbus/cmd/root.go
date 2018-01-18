package cmd

import (
	"fmt"
	"os"

	"encoding/json"
	"github.com/godbus/dbus"
	"github.com/ottojo/rclib"
	"github.com/spf13/cobra"
	"github.com/tarm/serial"
	"log"
)

var serialPort string
var baudRate int
var dbusInterface string
var dbusSignalName string
var dbusPath string

// rootCmd represents the base command when called without any subcommands
var rootCmd = &cobra.Command{
	Use:   "serialDbus",
	Short: "Decodes RadioControlProtocol packages from a serial interfaces and broadcasts them to dbus.",
	Long: `SerialDbus connects to a serial port, decodes the incoming bytes to packages using rcLib.
The decoded Packages will be sent json encoded as a dbus signal.`,
	Run: RunRoot,
}

// Execute adds all child commands to the root command and sets flags appropriately.
// This is called by main.main(). It only needs to happen once to the rootCmd.
func Execute() {
	if err := rootCmd.Execute(); err != nil {
		fmt.Println(err)
		os.Exit(1)
	}
}

func init() {
	rootCmd.Flags().StringVarP(&serialPort, "serialPort", "D", "/dev/ttyACM0", "Path to serial port")
	rootCmd.Flags().IntVarP(&baudRate, "baud", "b", 9600, "Serial port baud rate")
	rootCmd.Flags().StringVarP(&dbusInterface, "interface", "i", "de.toolboxbodensee.plane", "Dbus interface name")
	rootCmd.Flags().StringVarP(&dbusSignalName, "signalName", "n", "onRcPackageReceived", "Dbus signal name")
	rootCmd.Flags().StringVarP(&dbusPath, "path", "p", "/rcdata", "Dbus object path")
}

var conn *dbus.Conn

func RunRoot(cmd *cobra.Command, args []string) {
	var err error
	conn, err = dbus.SystemBus()
	if err != nil {
		log.Fatal(err)
	}
	fmt.Println("Successfully connected to SystemBus")

	_, err = conn.RequestName(dbusInterface, dbus.NameFlagDoNotQueue)
	if err != nil {
		log.Fatal(err)
	}
	fmt.Println("Successfully requested name \"" + dbusInterface + "\"")

	c := &serial.Config{Name: serialPort, Baud: baudRate}
	s, err := serial.OpenPort(c)
	if err != nil {
		log.Fatal(err)
	}
	fmt.Println("Successfully connected to serial port \"" + serialPort + " \"")

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
	err = conn.Emit(dbus.ObjectPath(dbusPath), dbusInterface+"."+dbusSignalName, string(jsonPackage))
	if err != nil {
		fmt.Println(err)
	}
}