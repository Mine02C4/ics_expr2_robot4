import serial
ser = serial.Serial('/dev/ttyACM1', 9600)
#ser.write('fire=1x')
ser.write('turn=30x')
