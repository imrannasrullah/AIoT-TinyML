# AIoT-TinyML
Interfacing a remote camera feed for AI/AIot-based image processing using a custom Real Time Operating System

Steps to run the project:
1.	Setup the BeagleBone
•	Power it on with the cable that comes with it. After a couple of minutes, you should be able to SSH into it.
•	The remote address is "debian@192.168.7.2", and the password is "temppwd"
•	Connect the BeagleBone to an Ethernet wire to establish Internet connection. Do "ping 8.8.8.8" and see if that works to confirm.
•	So "sudo apt-get update" and "sudo apt-get upgrade"
•	Use Pip3 to install the following commands:
o	"pip3 install -upgrade pip", "pip3 install PySerial", and "sudo apt install python3-opencv"
o	In the SSH terminal, run: python3, import cv2, import serial (on new lines) to see if install properly
2.	SSH into the BeagleBone, and transfer the Client.py file and "faarcascade_frontalface_default.xml" via WinSCP to it.
o	I used the website, https://realpython.com/python-sockets/ to learn how to program the sockets from the client and server side.
3.	To start the entire setup
o	First, power on the BeagleBone, SSH into it, and run the 2 config-pin commands (run "config-pin p9.11 uart" and "config-pin p9.13 uart")
o	Second, Plug in the Tiva and wait until it powers on
o	Third, FIRST start the RTOS on the TIVA using CCS (Code Composier Studio), and THEN run the Server.py on your PC, and lastly Client.py on the BeagleBone
o	The camera on your computer should turn on automatically, and face detection should start.
