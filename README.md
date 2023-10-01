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



Each GPIO can be programmed for Output or input direction.

- Output - The GPIO Can be controlled for a High or LOW output state.
- Input - The user can read if the GPIO is now in a HIGH state or LOW, state.

> Note: GPIO's initial state is Output low. (0.7v - considered low ) 

# Pinout

|Pin Number Console|Location|Description|MCU_GPIO|MCU_PORT|
|------------------|--------|-----------|--------|--------|
|1                 |I/O GPIO|On board GPIO|GPIO_0  |GPIOA   |
|2                 |I/O GPIO|On board GPIO|GPIO_1  |GPIOA   |
|3                 |I/O GPIO|On board GPIO|GPIO_2  |GPIOA   |
|4                 |I/O GPIO|On board GPIO|GPIO_3  |GPIOA   |
|5                 |I/O GPIO|Extension I/O - 1|GPIO_4  |GPIOA   |
|6                 |I/O GPIO|Extension I/O - 1|GPIO_5  |GPIOA   |
|7                 |I/O GPIO|Extension I/O - 1|GPIO_6  |GPIOA   |
|8                 |I/O GPIO|Extension I/O - 1|GPIO_7  |GPIOA   |
|9                 |I/O GPIO|Extension I/O - 2|GPIO_8  |GPIOA   |
|10                |I/O GPIO|Extension I/O - 2|GPIO_9  |GPIOA   |
|11                |I/O GPIO|Extension I/O - 2|GPIO_10 |GPIOA   |
|12                |I/O GPIO|Extension I/O - 2|GPIO_15 |GPIOA   |
|13                |I/O GPIO|Extension I/O - 3|GPIO_0  |GPIOB   |
|14                |I/O GPIO|Extension I/O - 3|GPIO_1  |GPIOB   |
|15                |I/O GPIO|Extension I/O - 3|GPIO_2  |GPIOB   |
|16                |I/O GPIO|Extension I/O - 3|GPIO_3  |GPIOB   |
|17                |I/O GPIO|Extension I/O - 4|GPIO_4  |GPIOB   |
|18                |I/O GPIO|Extension I/O - 4|GPIO_5  |GPIOB   |
|19                |I/O GPIO|Extension I/O - 4|GPIO_6  |GPIOB   |
|20                |I/O GPIO|Extension I/O - 4|GPIO_7  |GPIOB   |
|--                |I/O GPIO|Extension I/O GPIO, On board LED|GPIO_14 |GPIOB   |
|--                |I/O GPIO|Extension I/O GPIO, On board LED|GPIO_15 |GPIOB   |



# Commands

|Command|Description|
|---|---|
|#n|Set GPIO "n" to high or low (1 - High, 0 - Low)|
|@n|Set GPIO "n" direction (1 - Output, 0 - Input)|
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

## Usage Examples

#### 1. Setting GPIO pin 10 to Output and then to High state
```bash
&10  # Set GPIO pin 10 to Output
^10  # Set GPIO pin 10 to High state
```

#### 2. Reading the state of GPIO pin 3
```bash
%  # Set GPIO pin 3 to Input
@  # Read the state of GPIO pin 3
```

#### 3. Toggling GPIO pin 7 from High to Low
```bash
&7  # Set GPIO pin 7 to Output
^7  # Set GPIO pin 7 to High state
_7  # Set GPIO pin 7 to Low state
```

#### 4. Setting Multiple GPIOs (pin 1 and pin 2) to High
```bash
&1  # Set GPIO pin 1 to Output
&2  # Set GPIO pin 2 to Output
^1  # Set GPIO pin 1 to High state
^2  # Set GPIO pin 2 to High state
```
#### 5. Disable Serial Echo
-D

#### 6. Enable Serial Echo
-E

#### 7. Enter boot loader for Firmware update
-B

#### 8. Enable output to console on GPIO level change
-G

#### 9. Disable output to console on GPIO level change
-R

## Troubleshooting

Issue: The GPIO pins are not responsive
Solution: Make sure the drivers are up-to-date and that the board is properly connected.

