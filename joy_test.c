//
// Created by jsh on 24. 3. 19.
//
#include <stdio.h>
#include <stdlib.h>
#include <linux/joystick.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/poll.h>

int main() {
    int js;
    struct js_event jse;

    // Open the joystick device
    if ((js = open("/dev/input/js0", O_RDONLY | O_NONBLOCK)) == -1) {
        perror("Could not open joystick");
        exit(EXIT_FAILURE);
    }

    printf("Joystick opened successfully. Reading input...\n");

    // Main loop to read joystick input
    float motor_speed = 0;
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
                motor_speed = (float)jse.value / -32767.0 * 100.0;
            }
        }
        // Process motor speed or perform other tasks
        printf("Value: %f\n", motor_speed);
    }

    // Close the joystick device
    close(js);

    return 0;
}
