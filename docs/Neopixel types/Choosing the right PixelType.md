# Choosing the right \<PixelType\>

When declaring a Neopixel driver, specify which type to use:

- GRB_SEQ3
- GRB_SEQ4
- GRBW_SEQ3
- GRBW_SEQ4

Example of a declaration:

NeopixelDriver\<PixelType::GRB_SEQ3\> npx;

## GRB or GRBW colors

This simply depends on whether the Neopixels have a separate white (W) color LED inside.

The usual order to send the colors to Neopixels is Green-Red-Blue, hence the GRB naming instead of RGB.

Currently, the driver does not support other color sequences.

## SEQ3 or SEQ4 timing

### What is the difference

Each R,G,B or W color consists of 8 color bits. These color bits are transmitted over I2S using pulse width modulation (PWM) with a sequence of 3 or 4 bits. The time for each color bit is always 1200ns, so for SEQ4 the transmitted bit speed must be higher (3.2 MHz) than for SEQ3 (2.4 MHz).

- SEQ3: 3 bits are transmitted for each color bit:
    100 = color bit 0 (1/3 duty cycle, 400ns)
    110 - color bit 1 (2/3 duty cycle, 800ns)
- SEQ4: 4  bits are transmitted for each color bit:
    1000 = color bit 0 (1/4 duty cycle, 300ns)
    1100 = color bit 1 (2/4 duty cycle, 600ns)

SEQ3 closely matches the original WS2812B timing, while SEQ4 better matches the specification of more modern Neopixels.

### RAM memory usage

The total number of bits that will be transmitted over I2S depends on the number of colors (GRB=3, GRBW=4) and the SEQ3/SEQ4 timing (3 or 4 bits per color bit). All these I2S bits are determined before the (DMA based) transmission and stored in RAM.

As a result, the required RAM memory per Neopixel is:

- **<GRB_SEQ3>:** 3 colors \* 1 byte per color \* 3 I2S bits per color bit = 9 bytes * 2 (DMA copy) = 18 bytes
- **<GRB_SEQ4>:** 3 colors \* 1 byte per color \* 4 I2S bits per color bit = 12 bytes * 2 (DMA copy) = 24 bytes
- **<GRBW_SEQ3>:** 4 colors \* 1 byte per color \* 3 I2S bits per color bit = 12 bytes * 2 (DMA copy) = 24 bytes
- **<GRBW_SEQ4>:** 4 colors \* 1 byte per color \* 4 I2S bits per color bit = 16 bytes * 2 (DMA copy) = 32 bytes

Note: the total RAM usage of this driver is higher, see @@@TODO

### Recommendation for choosing SEQ3 or SEQ4

In practice, SEQ3 often works fine anyway, while requiring less RAM memory than SEQ4 per Neopixel.

When facing glitches or strange colors, use SEQ4 instead of SEQ3.
