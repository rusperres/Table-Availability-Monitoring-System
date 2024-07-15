# Table-Finder-ESP32
IoT Table Availability Monitoring System using Arduino and Esp-32

This project was created for a Capstone Research project by 12-Generosity Group 3 in Senior High School with my groupmates: 
Aissha Monceda - Leader
Renz Gabriel Etcuban
Carlos James De Leon
Sol Bendanillo
Reeves Lei Nobe
Joshua Matillano
Diane Elaine Goerner
Jairus Jasper V. Colindres (Me)

I had the role of programmer while my groupmates were assigned there own tasks related to the research. I had also gained some suggestions and ideas from both my leader, Aissha, who originally suggested the main idea of a seat-monitoring system and other details, and my groupmate Renz, who suggested using a Captive Portal.

This project demonstrates the potential of using IoT devices in school settings in which the students can check the availability of tables in the cafeteria for them to assess whether it is worth to go to the cafeteria or to estimate whether a table will be empty by the time they get there or not.

The Flow of Data:
IR Sensor (E18-D80NK Proximity Sensor) --->  Arduino Uno ---> Computer (Node.js Server) ---> Esp-32

1. The IR Sensor is first connected to the Arduino Uno according to its specific wiring in the datasheet. In our case, the brown wire was connected to the 5V pin, blue to the ground, and black to the digital pin.
2. The Arduino is programmed to send the sensor's input into the Serial Monitor.
3. The Arduino Uno is then connected to a port on the computer where the Node.js Server is hosted via USB cable.
4. Install CH340 driver (for Arduino) and CP210x (for Esp-32). The driver for Esp-32 should be written on its chip, in our case it was CP1202. These drivers are for USB-to-Serial communication which will be necessary for the transferring of data to each device.
5. Using the Serialport package in Node.js, the input in the serial port(COM5 in our case) for the Arduino Uno is read and piped into the port in which the Esp-32 is connected (COM7 in our case).
6. The data received by the Esp-32 is then piped to a front-end that displays a table which uses HTML and CSS for design and JavaScript to change the color of the table according to the data. 
7. By implementing the WebSocket API on the Esp-32, real-time changes in the table's color are now possible. This means that the user would not nedd to reload thepage.
8. Last, we implemented a Captive Portal for the convenience of the user that when the user connects on the Esp-32's WiFi, it will immediately redirect to the table's page.

Note: The Captive Portal might not work on some devices for reasons we do not know for sure(it might be permissions and other security issues). 
