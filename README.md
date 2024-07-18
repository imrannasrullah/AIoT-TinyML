# AIoT-TinyML
Interfacing a remote camera feed for AI/AIot-based image processing using a custom Real Time Operating System

Steps to run the project:

Setup the BeagleBone
1. Power it on with the cable that comes with it. After a couple of minutes, you should be able to SSH into it.
2. The remote address is "debian@192.168.7.2", and the password is "temppwd"
3. Connect the BeagleBone to an Ethernet wire to establish Internet connection. Do "ping 8.8.8.8" and see if that works to confirm.
4. So "sudo apt-get update" and "sudo apt-get upgrade"
5. Use Pip3 to install the following commands:
6. "pip3 install -upgrade pip", "pip3 install PySerial", and "sudo apt install python3-opencv"
7. In the SSH terminal, run: python3, import cv2, import serial (on new lines) to see if install properly
7. SSH into the BeagleBone, and transfer the Client.py file and "faarcascade_frontalface_default.xml" via WinSCP to it.
   
To start the entire setup
1. Power on the BeagleBone, SSH into it, and run the 2 config-pin commands (run "config-pin p9.11 uart" and "config-pin p9.13 uart")
2. Plug in the Tiva and wait until it powers on
3. FIRST start the RTOS on the TIVA using CCS (Code Composier Studio), and THEN run the Server.py on your PC, and lastly Client.py on the BeagleBone
4. The camera on your computer should turn on automatically, and face detection should start.
