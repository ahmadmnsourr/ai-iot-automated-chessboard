import serial
import time

def close_serial_connection(port="/dev/ttyUSB0", baudrate=9600, timeout=1):
    try:
        # Open the serial connection
        ser = serial.Serial(port=port, baudrate=baudrate, timeout=timeout)
        if ser.is_open:
            print(f"Serial port {port} opened successfully.")

            # Optional: wait or send/receive data here
            time.sleep(1)

            # Close the serial connection
            ser.close()
            if not ser.is_open:
                print(f"Serial port {port} closed successfully.")
            else:
                print(f"Failed to close serial port {port}.")
        else:
            print(f"Failed to open serial port {port}.")
    except serial.SerialException as e:
        print(f"SerialException: {e}")
    except Exception as e:
        print(f"Unexpected error: {e}")

if __name__ == "__main__":
    close_serial_connection()
