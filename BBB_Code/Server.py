import socket
import pickle
import cv2 
import struct

HOST = "192.168.7.1" # ip address of host pc
PORT = 65433  # Port to listen on (non-privileged ports are > 1023)

if __name__=="__main__":
    # your code here:

    video_frames_capture = cv2.VideoCapture(0) # 0 specifies the first webcam source from the host-computer
    # Set the frame-rate of the VideoCapture object:
    video_frames_capture.set(cv2.CAP_PROP_FPS, 12) #set the 
    while video_frames_capture == False:
        print("No video camera source to capture frames from!")


    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.connect((HOST, PORT)) #connects with the client using the Port Number:
        
        while video_frames_capture.isOpened(): # when connection is established between the server and the
                    # client, beging sending frames from the server's (AKA host
                    # computer) webcam to the Beagleboard. Basically only do it
                    # as long as the video-camera is open

            ifFrame, frame = video_frames_capture.read()
            # ret: is a booleanregarding whether or not there was a return at all
            # frame: is each frame being returned. If no frame, there won't be an error, will simply not get any frames

            if ifFrame: # if we actually got a frame and not just a blank-image
                # Convert-image to gray-scale
                greyscaled_frame = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
                    
                # Down-sample the grey-scaled image:
                greyscaled_downSampled_frame = cv2.pyrDown(greyscaled_frame)
                    # 2 common options for downsampling: pyrDown() or resize():
                    # pyrDown() convolves the image with a low-pass filter (aka
                    # FIR-moving average filter), and on top of that, samples the
                    # actual image. However, resize() just uses interpolation
                    # to produce a lower-dimension form of the image, without
                    # any convolutional-smoothing

            # Finally, send this frame over to the client:
                # 1). Since the image is a matrix, we need to serialize it. We can
                # accomplish this using the pickle.dumps() function:
                serialized_frame = pickle.dumps(greyscaled_downSampled_frame)
                    # a key thing to note: pickle.dumps() returns a byte object
                    # Bye Object: One of the core built-in types in Python for manipulating
                    # binrary data. A bytes object is an immutable (non-modifiable) sequence
                    # of single byte values, each one ranging from 0-255.

                # 2). In addition, use struct.pack() to format serialized-data
                # to a format serialized-frame data to a low-level binary packing
                # that is more efficient to send.

                # 2a). Send the serialized frame-length first:
                message_size = struct.pact("L", len(serialized_frame))
                    # "L": format frame-values into unsigned long values - do this
                    # b/c my computer assumably has larger/high-resolution frames.
                    # Note: This function also returns the byte-object version of the 
                    # frame-length
                
                # 2b). Then send the actual serialized frame-data:
                s.sendall(message_size + serialized_frame)
                    # Here, we add together two 'Byte Object' data types, which
                    # is like concatenating two arrays together in python, using the 
                    # '+' operation