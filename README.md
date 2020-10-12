# Greenhouse: Communication

## Requirements:

These requirements are defined according to these criteras:
 - Atomic:
	It is detailed enough so that it cannot be broken down into smaller requirements.

 - Testable:
	Testability is critical. Otherwise we can not be sure that the requirement is fulfilled

 - Description:
	The requirement contains enough information to implement and test it.
	Input, What's the input? A specific value or an event?
	Transformation, modification algorithm.
	Expected output.
	Timing.

 - Tag:
	It is important to tag the requirements in order to follow the chain from requirement to 
	implementation and to test.
	

### Customer Requirements:

13.
It shall be possible to connect the system to the internet via an ESP32

14.
It shall be possible to synchronize the date and time of the system using an NTP server.

15.
It shall be possible to monitor, control, configure and calibrate the system via the MQTT protocol and a broker.

16.
It shall be possible to log all the data/changes of the system on an SD card with a timestamp every second.

17.
It shall be possible to communicated to the system via a terminal in order to get the snapshot of the system,
list and read the logged files, control, configure and calibrate the system.

### Defined Requirements:



