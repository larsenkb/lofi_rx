#!/usr/bin/env python2
import RPi.GPIO as GPIO

GPIO.setmode(GPIO.BCM)

GPIO.setup(23, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)
print "Waiting for rising edge on port 23"
while(1):
    try:
        GPIO.wait_for_edge(23, GPIO.RISING)
        print "Rising edge detected."
    except KeyboardInterrupt:
        GPIO.cleanup()
