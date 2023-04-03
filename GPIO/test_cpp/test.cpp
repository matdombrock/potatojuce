#include <gpiod.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char **argv)
{
  const char *chipname = "gpiochip1";
  struct gpiod_chip *chip;
  struct gpiod_line *lineLED;
  int i, val;

  // Open GPIO chip
  chip = gpiod_chip_open_by_name(chipname);

  // Open GPIO lines
  lineLED = gpiod_chip_get_line(chip, 98);

  // Open LED lines for output
  gpiod_line_request_output(lineLED, "example1", 0);

  // Blink LEDs in a binary pattern
  i = 0;
  while (true) {
    gpiod_line_set_value(lineLED, (i & 1) != 0);

    usleep(100000);
    i++;
  }

  // Release lines and chip
  gpiod_line_release(lineLED);
  gpiod_chip_close(chip);
  return 0;
}