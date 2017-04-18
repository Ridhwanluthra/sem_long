import RPi.GPIO as gpio
import os
from time import sleep, time

#Defining GPIO pin numbers 
pin1 = 6 # This is GPIO6, hence, 5th pin in inner row from ports 41
pin2 = 7 # 8th pin in outer row from ports 42
pin3 = 8 # 9th pin in outer row from ports 39
pin4 = 9 # 10th pin in inner row from ports 40

# Setting up basic GPIO settings
gpio.setmode(gpio.BCM)
gpio.setwarnings(False)
gpio.setup(pin1, gpio.OUT)
gpio.setup(pin2, gpio.OUT)
gpio.setup(pin3, gpio.OUT)
gpio.setup(pin4, gpio.OUT)

def move_to_cell(index):
    print ('GOT IN')
    global pin1
    global pin2
    global pin3
    global pin4
    wait_time = 4
    if (index == 1):
        gpio.output(pin1, 1)
    elif (index == 2):
        gpio.output(pin2, 1)
    elif (index == 3):
        gpio.output(pin1, 1)
        gpio.output(pin2, 1)
    elif (index == 4):
        gpio.output(pin3, 1)
    elif (index == 6):
        gpio.output(pin1, 1)
        gpio.output(pin3, 1)
    sleep(wait_time)
        
def reset():
    global pin1
    global pin2
    global pin3
    global pin4
    gpio.output(pin1, 0)
    gpio.output(pin2, 0)
    gpio.output(pin3, 0)
    gpio.output(pin4, 0)
    sleep(1)
    

# Putting under try block as need to clean up pins on keyboard interrupt
try:
    # Turned off by default
    reset()
    while (1):
        if (next == '1'):
            move_to_cell(1)
        elif (next == '2'):
            move_to_cell(2)
        elif (next == '3'):
            move_to_cell(3)
        elif (next == '4'):
            move_to_cell(4)
        elif (next == '6'):
            move_to_cell(6)
        elif (next == '0'):
            reset()

except KeyboardInterrupt:
    gpio.cleanup()
finally:
    gpio.cleanup()
