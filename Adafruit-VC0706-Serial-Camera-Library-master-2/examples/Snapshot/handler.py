import serial
import time

# Open serial port (update COM port and baud rate as necessary)
ser = serial.Serial('/dev/tty.usbmodem54780037021', 9600)

# Wait for the Arduino to initialize
time.sleep(2)

# Open the output file (make sure the directory exists)
with open('output.jpg', 'wb') as outfile:
    # Continue reading from the serial port until the camera is done transmitting data
    while True:
        line = ser.readline()  # read a '\n' terminated line
        if line.strip() == b'done!':  # the camera indicates it's done with 'done!'
            break
        outfile.write(line)

# Close the serial port
ser.close()
