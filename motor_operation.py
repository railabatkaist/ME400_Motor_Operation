import RPi.GPIO as GPIO
import time
import pygame


class MotorController:
    def __init__(self, rpwm_pin=12, lpwm_pin=13, l_en_pin=16, r_en_pin=26, pwm_freq=100):
        # Initialize motor controller with default pins and PWM frequency
        self.RPWM = rpwm_pin
        self.LPWM = lpwm_pin
        self.L_EN = l_en_pin
        self.R_EN = r_en_pin
        self.PWM_FREQ = pwm_freq
        self.rpwm = None
        self.lpwm = None
	
    def setup_motor(self):
    	# Setup GPIO pins for motor control
        GPIO.setmode(GPIO.BCM)
        GPIO.setwarnings(False)
        GPIO.setup(self.RPWM, GPIO.OUT)
        GPIO.setup(self.LPWM, GPIO.OUT)
        GPIO.setup(self.L_EN, GPIO.OUT)
        GPIO.setup(self.R_EN, GPIO.OUT)
	
    def set_motor_mode(self):
    	# Enable motor control
        GPIO.output(self.R_EN, True)
        GPIO.output(self.L_EN, True)
        # Start PWM
        self.rpwm = GPIO.PWM(self.RPWM, self.PWM_FREQ)
        self.lpwm = GPIO.PWM(self.LPWM, self.PWM_FREQ)
        self.rpwm.start(0)
        self.lpwm.start(0)

    def operate(self, speed, threshold=1e-2):
    	# Operate the motors based on speed
        print("motor speed : " + str(speed))
        if (speed > 0 and abs(speed) > threshold):
            self.forward(speed)
        elif (speed < 0 and abs(speed) > threshold):
            self.backward(-speed)
        else:
            self.stop()

    def forward(self, speed):
    	# Move motors forward
        print("Forward speed : " + str(speed))
        speed *= self.PWM_FREQ
        self.rpwm.ChangeDutyCycle(speed)
        self.lpwm.ChangeDutyCycle(0)
        time.sleep(0.1)

    def backward(self, speed):
    	# Move motors backward
        print("Backward speed : " + str(speed))
        speed *= self.PWM_FREQ
        self.rpwm.ChangeDutyCycle(0)
        self.lpwm.ChangeDutyCycle(speed)
        time.sleep(0.1)

    def stop(self):
    	# Stop motors
        print("STOP !!")
        self.rpwm.ChangeDutyCycle(0)
        self.lpwm.ChangeDutyCycle(0)
        time.sleep(0.1)


if __name__ == "__main__":
    # Joypad Initialization
    pygame.init()
    pygame.joystick.init()
    try:
        # Attempt to initalize the joystick
        joystick = pygame.joystick.Joystick(0)
        joystick.init()
        print("Joystick Detected: ", joystick.get_name())
    except pygame.error:
        print("Joystick not detected.")

    # Motor Initialization
    motor = MotorController()
    motor.setup_motor()
    motor.set_motor_mode()

    try:
        while True:
            # Handle Pygame event
            pygame.event.pump()
            # Read the state of the joystick's 1 axis, value (-1.0 ~ 1.0)
            axis_value = joystick.get_axis(1)
            # Forward direction is considered as negative value
            # In this code, we restrict the motor power (Max : 0.2)
            motor.operate(-axis_value * 0.2)
            time.sleep(0.1)

    except KeyboardInterrupt:
        print("\nExiting..")
        pygame.quit()
        GPIO.cleanup()
