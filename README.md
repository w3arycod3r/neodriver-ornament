# neodriver-ornament

# Goal
Create a rechargable, battery-powered christmas ornament using Adafruit's "NeoPixels" (WS2812B RGB addressable LEDs). The ornament should contain a hole for a ribbon to hang on a tree. The ornament should be "small enough" as to not look out of place on a tree. It should be balanced and as symmetrical as possible so that it will hang straight. The weight should be comparable to other common ornaments. The battery should last for several days/weeks of continuous use.

Animations should be colorful, interesting, and eye-catching. It is acceptable to have a low duty cycle, i.e. the LEDs can have a short ON time playing an animation followed by a longer OFF time to save power. 
# Hardware
For the LEDs, I found various carrier boards of the WS2812B RGB LEDs on eBay. I found square matrices, rings, and circles of various sizes. Most of these boards simply had the LEDs soldered on one side with appropriate decoupling caps and traces connecting them all in a chain. The important connections (VDD, DIN, & GND) were exposed on the other side, to surface mount pads. I used small stranded wires from ribbon cables to fashion my own cables and connectors for each board. I soldered the wires to the pads, providing some strain relief with hot glue, then crimped a 3P JST-XH female connector to the other end. I would use a male 3P JST-XH on my driver board and connect the LED boards using this interface. This would make it easy to switch out and test different LED boards. Making these connectors was tedious and the solder connections are quite fragile.

<img src="img/assorted_neo.jpg" width=400> <img src="img/fab1.jpg" width=400>  

I chose the ATtiny85 for the microcontroller, as it is small, inexpensive, has various low-power modes, is available in a DIP-8 package, has sufficient I/O, and has some Arduino IDE support with extensions. I wanted all my components to be through-hole for easy assembly. I planned to have a potentiometer for brightness control, two pushbuttons for changing animations, and a power switch.

Later, I would discover that the NeoPixels drew several milliamps of current, even when they were switched OFF (commanded to show RGB = 0,0,0). So, I picked a nMOS power transistor to switch the GND of the NeoPixels using the microcontroller. I also switched the GND on the pot to save additional power. 

For the battery, I found a neat module which combines a LiFePO4 cell, holder, and USB charge circuit into a single, easy to solder PCB:

https://www.tindie.com/products/silicognition/lifepo4weredusb/  

I wanted to be able to reprogram the microcontroller after the device was assembled, so I included a AVR-ISP header on the board. Not quite as convenient as the USB connector on an Arduino, but good enough for my purposes. This means that I will need a AVR-ISP programmer. I used the official Atmel-ICE programmer. I looked into some of the third-party programmers, but I wasn't happy with the way they handled powering the board. The Atmel-ICE does not power the board but instead samples the VDD line and converts the 5V USB programming signals to the voltage level your board uses.


# Schematic
<img src="img/schem.png" width=900>  

## Protoboard
First, I soldered together a prototype on a piece of protoboard, based on my initial schematic. You can see these efforts below:
<img src="img/proto2.jpg" width=300> <img src="img/proto1.jpg" width=300>  
This wasn't very attractive, and it didn't hang very well on the tree, so I sought out to design a custom PCB. This was my first PCB design, so I learned a lot along the way. I decided to use KiCad, since it is FOSS and very beginner friendly. I watched and followed along with Digi-Key's YouTube tutorial on KiCad.

## v1
<img src="img/board8.jpg" width=300>
<img src="img/board6.jpg" width=300>
<img src="img/board1.jpg" width=300>

## v2

## v3



# Environment setup

# Compile & Flash
