import pygame

def main():
    # Initialize Pygame and the joystick module
    pygame.init()
    pygame.joystick.init()

    try:
        # Attempt to initialize the first joystick
        joystick = pygame.joystick.Joystick(0)
        joystick.init()
        print("Joystick detected:", joystick.get_name())
    except pygame.error:
        print("Joystick not detected.")
        return

    try:
        while True:
            # Handle Pygame events
            pygame.event.pump()

            axis_value = joystick.get_axis(1)
            print(f"Axis {1}: {axis_value}")

            # Delay to control the update rate
            pygame.time.delay(100)

    except KeyboardInterrupt:
        print("\nExiting...")
        pygame.quit()

if __name__ == "__main__":
    main()

