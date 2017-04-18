import RPi.GPIO as gpio
from time import sleep

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
    wait_time = 1
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
    gpio.output(pin1, 0)
    gpio.output(pin2, 0)
    gpio.output(pin3, 0)
    gpio.output(pin4, 0)
    sleep(1)
