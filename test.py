import serial
from time import sleep

# ser = serial.Serial('COM8', 4800, bytesize=8,     parity=serial.PARITY_EVEN, stopbits=1, timeout=0, rtscts=1)
# sleep(1)
# print("ready ..")

REVISION = '0.1'

# ports = ['COM%s' % (i + 1) for i in range(10000)]

ports = ['COM8']
# ports.remove("COM3")
# ports.remove("COM1")
# ports.remove("COM4")
# ports.remove("COM20")
# ports.remove("COM20")
#print(ports)


def send_to_serial(serialport, string_command):
    string_command = string_command + '\n'                                # Add New Line at end of String
    string_command = string_command.encode()                          # Encode to bites
    ser = serial.Serial(serialport ,115200, bytesize=8,     parity=serial.PARITY_EVEN, stopbits=1, timeout=0, rtscts=1)  # open serial port
    print(ser.name)  # Print which port actually used
    print(string_command)
    ser.write(string_command)
    ser.close()

for port in ports:

    try:

        send_to_serial(port, "&1")
        sleep(1)
        send_to_serial(port, "_1")
        sleep(0.1)

        send_to_serial(port, "&2")
        sleep(1)
        send_to_serial(port, "_2")
        sleep(0.1)

        send_to_serial(port, "&3")
        sleep(1)
        send_to_serial(port, "_3")
        sleep(0.1)

        send_to_serial(port, "&4")
        sleep(1)
        send_to_serial(port, "_4")
        sleep(0.1)

        # send_to_serial(port, "&2")
        # time.sleep(1)
        # send_to_serial(port, "_2")
        # time.sleep(0.1)

        # send_to_serial(port, "&3")
        # time.sleep(1)
        # send_to_serial(port, "_3")
        # time.sleep(0.1)

        # send_to_serial(port, "&4")
        # time.sleep(1)
        # send_to_serial(port, "_4")
        
        # # serialport = serial.Serial(port)  # open serial port
        # send_to_serial(port, "#FF0000")
        # time.sleep(1)
        # send_to_serial(port, "#00FF00")
        # time.sleep(1)
        # send_to_serial(port, "#0000FF")
        # time.sleep(1)
        # send_to_serial(port, "#FFFFFF")
        # print("################SUCESS############################")
        # print(port)
    except:
        # pass
        print("################FAIL############################")
        print(port)
        # print()
        print("Something went wrong")
        break
    else:
        print("Nothing went wrong")

# if ser.writable():
#     print(ser.write(b'&3\n'))
#     print(ser.readline())
#     # ser.write(b"&3\n")
#     sleep(0.1)
#     # ser.write(b"B_3\n")
#     ser.close()
#     print("done")

# while True:
#   line = ser.read()
#   if len(line) > 0:
#     print(line)
# ser.close()
# # ser.close()