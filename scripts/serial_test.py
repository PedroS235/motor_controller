import serial
import time

# This script was mainly generated by ChatGPT!


def main():
    # Change the first parameter to your port name.
    # In Windows, it will be something like "COM1"
    # In Linux, it will be something like "/dev/ttyUSB0"
    with serial.Serial("/dev/ttyUSB0", 9600, timeout=1) as ser:
        time.sleep(2)

        while True:
            print("\nOptions:")
            print("1. Send close-loop mode command")
            print("2. Send open-loop mode command")
            print("3. Request robot's odometry/pose")
            print("4. Request motor status")
            print("5. Exit")

            choice = input("Enter your choice: ")

            if choice == "1":
                x = input("Enter linear velocity x: ")
                w = input("Enter angular velocity w: ")
                command = f"c {x} {w}\n"
                ser.write(command.encode())
            elif choice == "2":
                left_pwm = input("Enter PWM for left motor (0-255): ")
                right_pwm = input("Enter PWM for right motor (0-255): ")
                command = f"o {left_pwm} {right_pwm}\n"
                ser.write(command.encode())
            elif choice == "3":
                ser.write(b"q\n")
            elif choice == "4":
                ser.write(b"m\n")
            elif choice == "5":
                print("Exiting...")
                break
            else:
                print("Invalid choice. Try again.")
                continue

            # Read the response from the Arduino and print it
            response = ser.readline().decode().strip()
            print(f"Response: {response}")


if __name__ == "__main__":
    main()