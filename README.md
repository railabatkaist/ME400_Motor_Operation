# ME400_Motor_Operation

## Description

This project is designed to control motors using a Raspberry Pi 4 and the BTS7960 motor driver. It provides the motor operation code in two programming languages: C and Python. The aim is to showcase how joystick input can be utilized to control the speed and direction of a motor, offering a practical example of interfacing hardware components with software in embedded systems.

## Prerequisites

Before you can compile and run the software, you need to install some prerequisites. This guide assumes you are using Ubuntu 22.04.

### Installing Dependencies

1. **Python**: Ensure you have Python installed. Your project uses Python scripts, so Python is essential. To install Python, use:
    ```bash
    sudo apt update
    sudo apt install python3 python3-pip
    ```
    ```bash
    pip3 install pyserial RPi.GPIO pygame
    ```

2. **GCC for C compilation**: Your project contains C files, so you need GCC to compile them.
    ```bash
    sudo apt update
    sudo apt install build-essential
    ```

3. **WiringPi (for GPIO access on Raspberry Pi)**: This is required for `motor_operation.c` to interface with GPIO pins.
    ```bash
    sudo apt install -y git
    git clone https://github.com/WiringPi/WiringPi.git
    cd WiringPi
    git pull origin
    ./build
    # Check the installation statement
    gpio -v
    ```

4. **Joystick support**: To interact with the joystick, ensure you have joystick support installed.
    ```bash
    sudo apt-get install joystick
    ```

## Hardware Setup

- **Raspberry Pi 4**: This project is designed to run on a Raspberry Pi 4. Ensure that it is set up with Raspberry Pi OS and connected to your motor driver and joystick.
  
- **BTS7960 Motor Driver**: Connect the motor driver to the Raspberry Pi according to the driver's datasheet. Ensure that the input pins from the Pi are correctly connected to the driver's control inputs.


## About the Code

### joy_test

The `joy_test` code is designed to test the joystick inputs without controlling the motor. It opens the joystick device, reads its inputs, and prints them to the console. This is useful for verifying that the joystick is working correctly and for debugging input issues.

### motor_operation

The `motor_operation` code integrates joystick inputs with motor control logic. It reads joystick movements and translates them into motor speed and direction commands, controlling the motor via the GPIO pins on the Raspberry Pi. This demonstrates a practical application of using joystick inputs for real-world physical control.


## Compiling C Programs

To compile the C programs (`joy_test.c` and `motor_operation.c`), use the following GCC commands:

For `joy_test.c`:
```bash
gcc -o joy_test joy_test.c
```

For `motor_operation.c`:
```bash
gcc -o motor_operation motor_operation.c -lwiringPi
```

## Running the Programs

- **For Python scripts**
```bash
python3 <script_name>.py
```

- **For compiled C programs**
```bash
./joy_test
```
or
```bash
./motor_operation
```