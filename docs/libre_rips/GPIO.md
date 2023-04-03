[Original Slideshow](https://docs.google.com/presentation/d/1u4Z7c7he3sVfFC5laRgGkQSrsFML22xEOw6-r-VxeQM/edit#slide=id.g4039ec07dc_0_0)

# Le Potato GPIO

## GPIO Info
- GPIO stands for General Purpose Input Output
- GPIO pins are digital pins (can be 0 or 1) capable of input or output
- GPIO input involves sensing the voltage level and using it to represent 0 or 1
- GPIO output involves setting the voltage level and using it to represent 0 or 1
- GPIO are not to be confused with analog input or output
- They cannot represent a number in a range (other than 0 or 1) or a decimal number (eg 0.5)
- For the 40-pin header found on Libre Computer Boards, all GPIOs are 3.3V logic level meaning that they use 3.3V or - 0V to represent digital 1 or 0 respectively. It is the same voltage as Raspberry Pi but different than some Arduino - boards. Some Arduinos use 5V logic level and connecting such systems to 3.3V logic level systems will damage the them due to overvoltage.

## GPIO Limitations
- GPIOs are generally used to signal or to light LEDs.
- GPIOs provide only a few milli-amps of current so they cannot drive motors.
- GPIOs have limited signaling/frequency throughput depending on what software interface is used.
    - Userspace software using file interface can operate on the n-KHz frequency
    - Userspace software using ioctl or registers can operate on the nn-KHz to nnn-KHz frequency
    - Kernel driver can usually can operate on the n-MHz frequency
    - Kernel driver with accelerated fixed function hardware on specific pins can operate on the nn-MHz frequency
- GPIO signaling/frequency will be further limited if current requirement is high.
    - Large LEDs and devices that require more current will slow voltage rise/fall times since more electrons need to be delivered to bring voltage levels up to 3.3V or down to 0V. Capacitance.

## GPIO: Controlling Direction and Level
[AML-S905x-CC-v1.0A Headers Reference](https://docs.google.com/spreadsheets/d/1U3z0Gb8HUEfCIMkvqzmhMpJfzRqjPXq7mFLC-hvbKlE/edit?usp=sharing)

All GPIO pins output either 3.3V or 0V with very limited current. Signaling and powering LEDs are the typical use-cases. Do not try to drive high current devices such as DC motors. Use transistors when working with devices that require more than 10mA!

There are two interfaces for controlling GPIO pins in Linux.
1) File operations on /sys/class/gpio
    - being deprecated in a few years
2) Ioctl operations on /dev/gpiochip
    - requires utilities like gpiod (gpioget/gpioset) unless your programming language supports ioctl

## GPIO: Controlling Direction and Level via gpiod (ioctl)
This is the preferred method of operating GPIO pins.
```
sudo apt-get install gpiod
```

```
gpioget $GPIOCHIP $GPIONUM
gpioset $GPIOCHIP $GPIONUM=0
```

Replace $GPIOCHIP and $GPIONUM with the GPIO chip and unquoted Linux GPIO # from the headers reference.

## GPIO: Controlling Direction and Level via file ops
This method is deprecated and will not be supported by Linux long term. Make sure you are root (sudo su)

1) Setting a GPIO
```
echo -n $GPIONUM > /sys/class/gpio/export
echo -n out > /sys/class/gpio/gpio$GPIONUM/direction
echo -n 0 > /sys/class/gpio/gpio$GPIONUM/value
echo -n $GPIONUM > /sys/class/gpio/unexport
```
2) Reading a GPIO
```
echo -n $GPIONUM > /sys/class/gpio/export
echo -n in > /sys/class/gpio/gpio$GPIONUM/direction
cat /sys/class/gpio/gpio$GPIONUM/value
echo -n $GPIONUM > /sys/class/gpio/unexport
```

Replace $GPIONUM with the quoted Linux GPIO # from the headers reference since many GPIOs are controlled by chip1 which is offset by 10.
