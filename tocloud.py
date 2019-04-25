import sys
import urllib.request
from time import sleep
import Adafruit_DHT as dht


import os
import glob
import time
os.system('modprobe w1-gpio')
os.system('modprobe w1-therm')
base_dir = '/sys/bus/w1/devices/'
device_folder = glob.glob(base_dir + '28*')[0]
device_file = device_folder + '/w1_slave'
def read_temp_raw():
    f = open(device_file, 'r')
    lines = f.readlines()
    f.close()
    return lines

def read_temp():
    lines = read_temp_raw()
    while lines[0].strip()[-3:] != 'YES':
        time.sleep(0.2)
        lines = read_temp_raw()
        
    equals_pos = lines[1].find('t=')
    if equals_pos != -1:
        temp_string = lines[1][equals_pos+2:]
        temp_c = float(temp_string) / 1000.0
        temp_f = temp_c * 9.0 / 5.0 + 32.0
        return temp_c, temp_f


def DHT11_data():
	# Reading from DHT22 and storing the temperature and humidity
	humi1, temp1 = dht.read_retry(dht.DHT11, 5)
	humi2, temp2 = dht.read_retry(dht.DHT11, 6)
	return humi1, temp1, humi2, temp2
while True:
	try:
		humi1, temp1, humi2, temp2 = DHT11_data()
		stempc,stempf=read_temp()
		
		# If Reading is valid
		if isinstance(humi1, float) and isinstance(temp1, float):
			# Formatting to two decimal places
			humi1 = '%.2f' % humi1					   
			temp1 = '%.2f' % temp1
			print(humi1)
			print(temp1)

			humi2 = '%.2f' % humi2					   
			temp2 = '%.2f' % temp2
			print(humi2)
			print(temp2)
			print("Soil Temp")
			
			print(stempc)

			
			
			f=urllib.request.urlopen("https://api.thingspeak.com/update?key=GXIZD2XSS09EQ38K&field1="+str(temp1)+"&field2="+str(humi1)+"&field3="+str(temp2)+"&field4="+str(humi2)+"&field5="+str(stempc))
			print(f)
			
                        
		else:
			print (Error)
		# DHT22 requires 2 seconds to give a reading, so make sure to add delay of above 2 seconds.
		sleep(20)
	except:
		continue


