from gpiozero import LED
from time import sleep

 #2, 3,     # SDA, SCL (used by I2C, be cautious)
  #  4, 5, 6,
   # 7, 8, 9, 10, 11,
    #12, 13, 14, 15, 16,
    #17, 18, 19, 20, 21,
    #22, 23, 24, 25, 26, 27


test_pin = 17  # change this to test another GPIO pin

led = LED(test_pin)

print(f"Blinking GPIO {test_pin} using gpiozero")
for _ in range(5):
    led.on()
    print("HIGH")
    sleep(0.5)
    led.off()
    print("LOW")
    sleep(0.5)
