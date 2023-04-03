# GPIO
[Source](https://developer.technexion.com/docs/using-gpio-from-a-linux-shell)
## Overview
![Board Image](libre_rips/board_img.png)

- Green - GPIO (General)
- Red - 5v
- Orange - 3.3v
- Blue - ADC
- Dark - Ground

## CLI Usage

### Install `gpiod`
```
sudo apt install gpiod
```

## Detect GPIO Chips
```
gpiodetect
```

Example:
```
$ gpiodetect
gpiochip0 [aobus-banks] (11 lines)
gpiochip1 [periphs-banks] (100 lines)
```

## List Lines
```
gpioinfo
```

Example:
```
$ gpioinfo
gpiochip0 - 11 lines:
	line   0:    "UART TX"       unused   input  active-high 
	line   1:    "UART RX"       unused   input  active-high 
	line   2:   "Blue LED" "librecomputer:blue" output active-high [used]
	line   3: "SDCard Voltage Switch" "VCC_CARD" output active-high [used]
	line   4: "7J1 Header Pin5" unused input active-high 
	line   5: "7J1 Header Pin3" unused input active-high 
	line   6: "7J1 Header Pin12" unused input active-high 
	line   7:      "IR In"       unused   input  active-high 
	line   8: "9J3 Switch HDMI CEC/7J1 Header " unused input active-high 
	line   9: "7J1 Header Pin13" unused input active-high 
	line  10: "7J1 Header Pin15" unused output active-high 
gpiochip1 - 100 lines:
	line   0:      unnamed       unused   input  active-high 
	line   1:      unnamed       unused   input  active-high 
	line   2:      unnamed       unused   input  active-high 
	line   3:      unnamed       unused   input  active-high 
	line   4:      unnamed       unused   input  active-high
    ...
```

You can reference the [official doc](https://docs.google.com/spreadsheets/d/1U3z0Gb8HUEfCIMkvqzmhMpJfzRqjPXq7mFLC-hvbKlE/edit#gid=0) to find the pin you want to use and then use a `grep` to find the line you want. 

For example, if we want to find **Pin8** from that doc we could do this:
```
$ gpioinfo | grep Pin8
	line  91: "7J1 Header Pin8" unused output active-high 
```

That lets us know that the line number for that pin is **91**. 

### Get Pins
```
gpioget <chipNumber> <lineNumber>
```

Example:
```
gpioget gpiochip1 91
```

You can also omit the `gpiochip` part:
```
gpioget 1 91
```

### Set Pins
```
gpioset <chipNumber> <lineNumber>=<bool>
```

Example:
```
## Set the pin off
gpioset gpiochip1 91=0

## Set the pin on
gpioset gpiochip1 91=1
```

*Note: The `gpiochip` prfix can be omitted here as well. 