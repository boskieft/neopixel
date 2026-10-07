# Power wiring

When using the USB-C connector of the ESP32xx module to power it, the 5V output of that module can be used to power a small amount of Neopixels as well. Add a 470 uF capacitor to buffer peak currents.

Using too much power will cause weird stability issues on the ESP32xx module, ultimately leading to reset.

Good alternative is to use a dedicated, strong 5V power supply for the Neopixels.
