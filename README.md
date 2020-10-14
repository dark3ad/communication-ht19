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
* [ReqID:04] It shall be possible to create a list topics for every signal/sensor in the MQTT broker automatically with help of python.
* [ReqID:05] It shall be possible to create values for actuators, configurating and calibrating in the system.
* [ReqID:06] It shall be possible to publish all the signals, acuators, everything to their variables in the broker periodacally (1000ms)
* [ReqID:07] It shall be possible to subscribe and overwrite states and calibration values from the MQTT broker. 
* [ReqID:08] It shall be possible to initialize the commucation to teensy by I2C.
* [ReqID:09] It shall be possbile to read date and time from NTP and distribute the datetime to the Teensy.
* Overall it should be possible to monitor and control everything over the MQTT server.

## Terminal

* [ReqID:10] It shall possible to have a terminal application and a menu system.
* [ReqID:11] The menu shall have an option to view snapshots of the system. 
* [ReqID:12] The menu shall have an option to control, overwrite the states and calibration values. 
* [ReqID:13] It shall be possible to list and handle log files from SD card using a sd driver. 

## Log Manager

* [ReqID:14] Create an application log manager. 
* [ReqID:15] There shall be a log rotation (to remove old files).
* [ReqID:16] It shall be possible to log only changes in a log file.

## SD Card 

* [ReqID:16] The teensy should have an SD card.
* [ReqID:17] It shall be possible to initialize the SD card and status function.
* [ReqID:18] There shall be a init function.
* [ReqID:19] Should be able to check free memory in the sd card. 
* [ReqID:20] There shall be a create files function. 
* [ReqID:21] There shall be a write file function.
* [ReqID:22] There shall be an edit file function.
* [ReqID:23] There shall be an list files function. 
* [ReqID:24] There shall be a delete files function.
* [ReqID:25] It shall be possible to set the date and time of the log files.
* [ReqID:26] It shall be possible to store <amount_of_files>.
