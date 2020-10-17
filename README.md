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

13. It shall be possible to connect the system to the internet via an ESP32
14. It shall be possible to synchronize the date and time of the system using an NTP server.
15. It shall be possible to monitor, control, configure and calibrate the system via the MQTT protocol and a broker.
16. It shall be possible to log all the data/changes of the system on an SD card with a timestamp every second.
17. It shall be possible to communicated to the system via a terminal in order to get the snapshot of the system, list and read the logged files, control, configure and calibrate the system.

### Defined Requirements:

## MQTT

* [ReqID:01] It shall be possible to connect an ESP32 to internet via WiFi.
* [ReqID:02] An MQTT cloud server/broker should be created. 
* [ReqID:03] It shall possible then to connect to an MQTT broker.
* [ReqID:04] It shall be possible to create a list topics for every signal/sensor in the MQTT client automatically with help of python.
* [ReqID:05] It shall be possible to get all the data from Teensy by the system.
* [ReqID:06] It shall be possible to publish all the signals, acuators, everything to their variables in the cloud server/broker periodacally (1000ms).
* [ReqID:07] It shall be possible commucate between ESP32 and Teensy over I2C.
* [ReqID:08] It shall be possible to initialise the module.
* [ReqID:09] It shall be possible to send the status of the module over canbus. <esp32_status>

## NTP

* [ReqID:10] It shall be possbile to read date and time from ESP32 via NTP and send it over I2C to Teensy and then forward it to the CAN-bus.
* [ReqID:11] It shall be possible to initialise the RTC of teensy and then forward it to the CAN-bus. <ntp_status>
* [ReqID:12] It shall be possible to initialise the module.

## Terminal

* [ReqID:13] It shall possible to have a terminal application and a menu system.
* [ReqID:14] The menu shall have an option to view snapshots of the system.
* [ReqID:15] The menu shall have an option to overwrite the states of actuactors and all the calibration values. 
* [ReqID:16] It shall be possible to list and handle log files from SD card using a SD driver. 
* [ReqID:17] It shall be possible to initialise the module.
* [ReqID:18] It shall be possible to send the status of the terminal <terminal_status> to the CAN-BUS every (1000ms).

## Log Manager

* [ReqID:19] It shall be possible to have an application log manager. 
* [ReqID:20] It shall be possible to have a date-depent log rotation routine.
* [ReqID:21] It shall be possible to log only changes in a log file.
* [ReqID:22] It shall be possible to initialise the module.
* [ReqID:23] It shall be possible to store the status of the module SD card <sdcard_status> to the CAN-BUS every (1000ms).
* [ReqID:24] It shall be possible to send the available memory of the SD card <free_mem> to the CAN-BUS every (1000ms).
* [ReqID:25] It shall be possible to write internal errors of the log manager to a specific file <error.log>
* [ReqID:26] It shall be possible to turn the buildin led on when there is a critical error.

## SD Card

* [ReqID:27] The teensy should have an 2GB SD card.
* [ReqID:28] It shall be possible to initialize the SD card and status.
* [ReqID:29] It should be possible to calculate free memory of the sd card. 
* [ReqID:30] It should be possible to create files function. 
* [ReqID:31] It should be possible to write file function.
* [ReqID:32] It should be possible to edit(append) file function.
* [ReqID:33] It should be possible to sorted list files function. 
* [ReqID:34] It should be possible to delete files function.
* [ReqID:35] It should be possible to set the date and time of the log files.
