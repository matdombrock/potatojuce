##
# Copy of https://www.libre.computer/blogs/gpio-headers-reference-for-aml-s905x-cc/
# Libre Computer docs 404 a lot!
##

GPIO Headers Reference for AML-S905X-CC

GPIOs (General Purpose Input/Output) are single-bit pins capable of digital input or output typically used for controlling LEDs or signaling. There is a total of 35 GPIO pins operating at 3.3V logic level on Le Potato which is 7 more than what is found on the Raspberry Pi series of boards. GPIOs are half duplex which means they can only be in either input or output mode but not both simultaneously.

When setup for input, applying 3.3V or 0V to the pin will correspond to 1 or 0 respectively when read in software. When setup for output, the pins will be either 3.3V or 0V depending on whether it is set as 1 or 0 in software. GPIOs typically provide only a few milli-amps of current so they should only directly drive low power things like LEDs. They cannot drive power hungry things like DC motors, which need power transistors to deliver adequate current.

Some of the GPIO pins have secondary functionality for signaling SPDIF, I2S, I2C, SPI, SDIO, UART, PCM, clock-generation, and more. They still operate at the same 3.3V logic level but these pins are usually connected to internal specialized hardware that can generate precisely timed signals without using the CPU to bit bang. This frees up the CPU to do other things like running your operating system and software.

There are two interfaces in the Linux kernel for controlling and reading from GPIO pins. Since the interfaces are standardized, you do not need to use libraries to read from and write to GPIOs. We have put together a simple guide for utilizing these interfaces. As an addition, we also mapped out the functionality and software pin numbers on Le Potato for easy access.

Screenshot from 2018-05-21 11-48-00

On the picture at the top of the page, GPIOs on the Le Potato are highlighted in green. 5V pins are red, 3.3V pins are orange, ADC pins are blue. Other un-highlighted pins are ground with the exception of the pins on the top right header.
