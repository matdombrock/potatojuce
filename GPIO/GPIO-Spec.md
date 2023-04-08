# GPIO IPC Spec

---

## Read vs Write
Pins can only be in one of these two modes.

---

## Adding a Pin

### FIFO Path:
```
/tmp/pgpio-in
```

### Syntax:
```
add <pinName> <chipName> <lineNumber> <mode>
```

- `pinName` - A user set arbitraty name
- `chipName` - The name of the GPIO chip
- `lineNumber` - The Linux line number for the GPIO pin
- `mode` - Either `w` for write or `r` for read.

### Example:
```
add led1 gpiochip1 91 w
```

---

## Reading a Pin (Get)

### FIFO Path:
```
/tmp/pgpio-out
```

- One bit of output is written for each pin registerd as a read pin. 

- New lines are written any time the value of **any** read pin has changed. 
  - No output is written if the internal processing detects no change

### Example Output:
```
0010
0110
0100
0000
```
Here we registered 4 read pins. If we had 7 it might look like this:
```
0110111
1001100
0111101
```

---

## Writing a Pin (Set):

### FIFO Path:
```
/tmp/pgpio-in
```
