#include <stdio.h>
#include <stdlib.h>
#include <linux/joystick.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/poll.h>
#include <wiringPi.h>

#define RPWM_PIN 12
#define LPWM_PIN 13
#define L_EN_PIN 16
#define R_EN_PIN 26
#define PWM_FREQ 100

int rpwm, lpwm, len, ren;

void setup_motor() {
    wiringPiSetupGpio();
    pinMode(RPWM_PIN, PWM_OUTPUT);
    pinMode(LPWM_PIN, PWM_OUTPUT);
    pinMode(L_EN_PIN, OUTPUT);
    pinMode(R_EN_PIN, OUTPUT);
    digitalWrite(R_EN_PIN, HIGH);
    digitalWrite(L_EN_PIN, HIGH);
    pwmSetMode(PWM_MODE_MS);
    pwmSetClock(PWM_FREQ);
    pwmSetRange(100);
    pwmWrite(RPWM_PIN, 0);
    pwmWrite(LPWM_PIN, 0);
}

void forward(float speed) {
    printf("Forward speed: %.3f\n", speed);
    pwmWrite(RPWM_PIN, (int)(speed * 100));
    pwmWrite(LPWM_PIN, 0);
    // delay(100);
}

void backward(float speed) {
    printf("Backward speed: %.3f\n", speed);
    pwmWrite(RPWM_PIN, 0);
    pwmWrite(LPWM_PIN, (int)(speed * 100));
    // delay(100);
}

void stop() {
    printf("STOP !!\n");
    pwmWrite(RPWM_PIN, 0);
    pwmWrite(LPWM_PIN, 0);
    // delay(100);
}

void operate(float speed) {
	if (speed < 0) {
        forward(-speed * 0.2); // Restrict motor power (Max: 0.2)
    } else if (speed > 0) {
        backward(speed * 0.2); // Restrict motor power (Max: 0.2)
    } else {
        stop();
    }
}

int main() {
    int js;
    struct js_event jse;

    // Open the joystick device
    if ((js = open("/dev/input/js0", O_RDONLY | O_NONBLOCK)) == -1) {
        perror("Could not open joystick");
        exit(EXIT_FAILURE);
    }

    printf("Joystick opened successfully. Reading input...\n");

    // Setup motor
    setup_motor();
    
    float threshold=0.01;

    // Main loop to read joystick input
    while (true) {
        // Use poll to check if data is available for reading
        struct pollfd fds;
        fds.fd = js;
        fds.events = POLLIN;
        int ret = poll(&fds, 1, 0); // Poll with timeout of 0 milliseconds
        if (ret > 0 && (fds.revents & POLLIN)) {
            // Joystick event available, read it
            ssize_t bytes = read(js, &jse, sizeof(jse));
            if (bytes == sizeof(jse) && jse.type == JS_EVENT_AXIS && jse.number == 1) {
                // Extract joystick event data and update motor speed
                float motor_speed = (float)jse.value / -32767.0;
                printf("axis value: %.3f\n", motor_speed);
                operate(motor_speed);
            }
        }
        // Add delay to control the loop rate
        // delay(100);
    }

    // Close the joystick device
    close(js);

    return 0;
}
