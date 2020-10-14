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

## MQTT

* [ReqID:01] It shall be possible to connect an ESP32 to internet via WiFi. 
* [ReqID:02] An MQTT server/broker should be created. 
* [ReqID:03] It shall possible then to connect to an MQTT broker. 
* [ReqID:04] It shall be possible to create variables for every signal/sensor in the MQTT broker.
* [ReqID:05] It shall be possible to create variables for actuators, configurating and calibrating in the system.
* [ReqID:06] It shall be possible to publish sensor values to their variables in the broker periodacally (1000ms)
* [ReqID:07] It shall be possible to subscribe values or control the systems actuators from the MQTT broker. 
* [ReqID:08] It shall be possible to configure and calibrate values from the MQTT broker.
* Overall it should be possible to monitor and control everything over the MQTT server.

## Terminal

* [ReqID:09] It shall possible to have a terminal menu.
* [ReqID:10] The menu shall have an option to view snapshots of the system. 
* [ReqID:11] The menu shall have an option to control the systems actuators. 
* [ReqID:12] The menu shall have an option to calibrate and configure the systems values. 
* [ReqID:13] It shall be possible to fetch log files from SD Card. 

## SD Card 

* [ReqID:14] The teensy should have an SD card.
* [ReqID:15] An SD driver should be implemented.
* [ReqID:16] There shall be a sd_init function.
* [ReqID:17] It shall be possible to store log files from the system in an SD Card. 
* [ReqID:18] It shall be possible to store every change and log in the system.
* [ReqID:19] It shall possible to synchronize date and time using an NTP server with the logs.



