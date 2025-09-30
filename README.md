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

This document provides reference information and usage instructions for the FT.S-GPIO tool intended for the Compulab Tensor line of add-on boards.

## Product Description

Based on the ARM Cortex M0 CPU, the device emulates a USB COM port and enables users to control connected GPIOs.

## Overview
## Prerequisites

Before you begin, ensure you have met the following requirements:
- Operating system: Windows 10, Linux, macOS
- Hardware: Compulab Tensor line of add-on boards

Compulab PN FT.S_GPIO - TEL-GPIO

## Highlights

- Implementation of USB-to-GPIO without the need for special drivers or deep system knowledge.
- Enables fast prototyping and a short time to market.
- Works with popular programming languages such as Python.
- Open software and hardware for even more flexibility.
- Built-in bidirectional isolation based on the ADM3260 chip.
- Built-in four isolated GPIOs expandable up to 20 with the FT.V-TERM4 adapter.
- Easily updatable software using the built-in USB bootloader.

# Software

The source code and binary are available in the [GitHub repository](https://github.com/Compulab-Tensor-PC/TEL-GPIO).

Bug reports and feature requests can be opened in the GitHub repository.

# Hardware



Each GPIO can be configured for output or input operation.

- Output - The GPIO can be set to a HIGH or LOW state.
- Input - The user can read whether the GPIO is in a HIGH or LOW state.

> Note: GPIOs default to output low (0.7 V, considered low).

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

![GPIO pictures notation](https://github.com/Compulab-Tensor-PC/TEL-GPIO/blob/master/IMG-6381.jpg "TEL_GPIO")

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

#### 1. Setting GPIO pin 10 to Output and then High
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

#### 4. Setting multiple GPIOs (pin 1 and pin 2) to High
```bash
&1  # Set GPIO pin 1 to Output
&2  # Set GPIO pin 2 to Output
^1  # Set GPIO pin 1 to High state
^2  # Set GPIO pin 2 to High state
```
#### 5. Disable serial echo
-D

#### 6. Enable serial echo
-E

#### 7. Enter the bootloader for a firmware update
-B

#### 8. Enable console output on GPIO level changes
-G

#### 9. Disable console output on GPIO level changes
-R

## Troubleshooting

Issue: The GPIO pins are not responsive.
Solution: Ensure the drivers are up to date and that the board is properly connected.

