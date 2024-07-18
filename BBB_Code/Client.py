import socket
import cv2
import pickle
import struct
import serial
import socket

from struct import *

from face_detect import face_detect

HOST = "192.168.7.1"  # The server's hostname or IP address
PORT = 65433  # The port used by the server

if __name__=="__main__":
	# your code here:
	with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
		s.bind((HOST, PORT))
		s.listen()
		conn, addr = s.accept() # s.accept() returns a new socket object, which we'll use to communicate
					# with the Server
		
		packed_receivedFrame_data = b'' # this initializes an *empty* Byte-Object
		payload_size = struct.calcsize("L") # this is the size of the format in which 'Byte Object' is stored
		with conn:
			print(f"Connected by {addr}")
			while True: # Now that the Beaglebone is connected with the Host Computer, continually
				    	# receive frames from the Host computer

			#  First retrieve the size of the frame:
				while len(packed_receivedFrame_data) < payload_size:
					packed_receivedFrame_data += conn.recv(4096)

				packed_frame_size = packed_receivedFrame_data[:payload_size]
				packed_receivedFrame_data = packed_receivedFrame_data[payload_size:]
				unpacked_frame_size = struct.unpack("L", packed_frame_size)[0]

				while len(packed_receivedFrame_data) < unpacked_frame_size:
					packed_receivedFrame_data += conn.recv(4096)
				
				unpacked_receivedFrame_data = packed_receivedFrame_data[:unpacked_frame_size]
				packed_receivedFrame_data = packed_receivedFrame_data[unpacked_frame_size:]

				# Extract frame:
				received_frame = pickle.loads(unpacked_receivedFrame_data)

				# Finally, perform image-processing on the video frame (will be completed later)