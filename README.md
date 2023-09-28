# Introduction
## Table of Contents

1. [Introduction](#introduction)
    - [About This Document](#about-this-document)
    - [Product Description](#product-description)
    - [Overview](#overview)
    - [Highlights](#highlights)
2. [Prerequisites](#prerequisites)
3. [Installation and Setup](#installation-and-setup)
4. [Usage Examples](#usage-examples)
5. [Troubleshooting](#troubleshooting)
6. [FAQs](#faqs)
7. [Contributions](#contributions)
8. [License](#license)
9. [Contact](#contact)

## About This Document

This document is a reference document and usage instructions for FT.S-GPIO tell intended for Compulab Tensor line of add on boards.

## Product Description

Based on the ARM Cortex M0 CPU, Emulates COM port on USB and enables the user to control connected GPIO’s.

## Overview
## Prerequisites

Before you begin, ensure you have met the following requirements:
- Operating system: Windows 10, Linux, macOS
- Hardware: Compulab Tensor line of add-on boards

Compulab PN FT.S_GPIO - TELL-GPIO

## Highlights

- Implementation of USB to GPIO without the need for special drivers or deep system knowledge.
- Enables fast prototyping and fast time to market.
- Can be used with popular coding language’s like Python
- Open software and Hardware for even more flexibility
- Build-in bidirectional isolation based on the ADM3260 chip.
- Build-in 4 Isolated GPIO’s expandable up to 20 with FT.V-TERM4 adapter.
- Easily updatable SW using built-in USB bootloader.

# Software

The source code and Binary are available [github Repo](https://github.com/Compulab-Tensor-PC/TEL-GPIO)

Bugs and futures can be opened in the Github repository.

# Hardware

Schematics and Gerber files are available here:

_TODO add the location._

Each GPIO can be programmed for Output or input direction.

- Output - The GPIO Can be controlled for High or LOW output state.
- Input - The user can read if the GPIO is now in a HIGH state or LOW, state.

> Note: GPIO's initial state is Output low.

# Pinout

|Pin Name|Pin Number|Pin Type|Description|
|---|---|---|---|
|VDD|1|Power|Power supply voltage|
|GND|2|Power|Ground|
|GPIO_0|3|I/O GPIO|Extension I/O GPIO|
|GPIO_1|4|I/O GPIO|Extension I/O GPIO|
|GPIO_2|5|I/O GPIO|Extension I/O GPIO|
|GPIO_3|6|I/O GPIO|Extension I/O GPIO|
|GPIO_4|7|I/O GPIO|Extension I/O GPIO|
|GPIO_5|8|I/O GPIO|Extension I/O GPIO|
|GPIO_6|9|I/O GPIO|Extension I/O GPIO|
|GPIO_7|10|I/O GPIO|Extension I/O GPIO|
|GPIO_8|11|I/O GPIO|Extension I/O GPIO|
|GPIO_9|12|I/O GPIO|Extension I/O GPIO|
|GPIO_10|13|I/O GPIO|Extension I/O GPIO|
|GPIO_11|14|I/O GPIO|Extension I/O GPIO|
|GPIO_12|15|I/O GPIO|Extension I/O GPIO|
|GPIO_13|16|I/O GPIO|Extension I/O GPIO|
|GPIO_14|17|I/O GPIO|Extension I/O GPIO|
|GPIO_15|18|I/O GPIO|Extension I/O GPIO|
|GPIO_16|19|I/O GPIO|Extension I/O GPIO|
|GPIO_17|20|I/O GPIO|Extension I/O GPIO|
|GPIO_18|21|I/O GPIO|Extension I/O GPIO|
|GPIO_19|22|I/O GPIO|Extension I/O GPIO|
|GPIO_20|23|I/O GPIO|Extension I/O GPIO|
|LED1|24|LED (Green)|On-board LED|
|LED2|25|LED (Green)|On-board LED|

# Commands

|Command|Description|
|---|---|
|#n,x|Set GPIO "n" to "x" (1 - High, 0 - Low)|
|@n,x|Set GPIO "n" direction to "x" (1 - Output, 0 - Input)|
|$n|Read GPIO "n" state (High or Low, for Input direction)|

# Using FT.S-GPIO from Terminal

To use FT.S-GPIO from a terminal, follow these steps:

1. Connect the FT.S-GPIO board to your computer using a USB cable.
2. Open a terminal program on your computer.
3. Configure the terminal program to use the following settings:
    - Baud rate: 115200
    - Data bits: 8
    - Parity: None
    - Stop bits: 1
    - Flow control: None
4. Send commands to the FT.S-GPIO board using the command format described in the Commands table above.
5. Initially send the command that will set the GPIO as Output.
6. Send the command that will set GPIO to high state
## Usage Examples

To set GPIO pin 5 to high, use the following command:

```bash
@5,1
#5,1
```
## Troubleshooting

Issue: The GPIO pins are not responsive
Solution: Ensure the drivers are up-to-date and the board is properly connected.
## FAQs

Q: [Your Question Here]
A: [Your Answer Here]
## Contributions

If you want to contribute to this project, please ...
## License

This project is licensed under [Your License Here].
## Contact

For more information, please contact [Your Contact Information Here].
