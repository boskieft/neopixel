# SEQ3 and SEQ4 lookup tables

Lookup tables are used for a fast translation of the pixel colors to bit sequences to transmit over I2S.

## SEQ3 lookup table

The SEQ3 timing uses a 764 byte lookup table to translate each color byte into 3 output bytes over I2S.
The table is defined in neopixel_seq3.h and will only be linked (and use program memory) when required.
The provided Python file was used to create the table data.

## SEQ4 lookup table

The SEQ4 timing uses a 32 byte lookup table to translate 4-bit color nibbles into output bits.
The table is defined in neopixel_seq4.h and will only be linked (and use Program memory) when required.
