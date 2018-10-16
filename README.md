# Fitness Tracker

**A Brief Description**

In this project, we have designed an embedded system which is used to keep track of the number of calories burnt and distance covered during jogging or walking. User can get these data and BPM by commanding GSM to send sms to user's mobile.




Required Equipments

    Arduino Uno

    SIM 900A GSM Module

    Piezo Sensor

    Two HC-05 Bluetooth Module

    Arduino Nano

    Pulse Sensor





How it works?

In this project , two arduino is used for interfacing to various hardware peripherals. First one is interfaced to a GSM Module , a Piezo sensor & a Bluetooth module and second one is interfaced to a Pulse sensor & a Bluetooth module.

Pulse sensor measures BPM (beats per minute) of user from his/her fingertip on every 2ms and sends to another arduino via bluetooth module. Piezo sensor is placed on the bottom of the user's foot and used to detect steps by sensing pressure on the top of the sensor. Calories and distance is calculated using this step count and height & weight of user.

User can start and stop the system by sending sms and also get his/her performance status via sms. When user request is sent to the number at the GSM, the GSM automatically sends a return reply to that mobile describing total distance covered, total calories burnt & BPM. It also sends a warning sms if BPM of user is in alarming condition. 
