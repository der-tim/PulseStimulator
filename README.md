#   Pulsed Stimulation Script

  This script creates a simple TTL pulse pattern that can be used to drive, for example, a stimulus isolator for electrical stimulation.

The script creates a burst of 
* N pulses of 
* length L  
* with a frequency F 
* for a time B
* followed by an inter-burst interval I

The script needs arbitrary serial communication as on/off switch. Open the Arduino IDE, start the serial monitor, and connect your Arduino via USB to your computer. When starting, the script should print out status messages on the serial console and wait for _any_ input to start the stimulation. After the stimulation is started, the script will quietly generate the pulses as specified.

Note: the script will stop when receiving _any_ serial input, so make sure to not accidentally send/press Enter in the serial monitor.

