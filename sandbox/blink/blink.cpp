/* see license file
* compile with: g++ -Wall -o blink blink.cpp -lgpiod
*/

#include <gpiod.h>
#include <unistd.h> //used for usleep

int main(int argc, char **argv)
{
  // set the chip name as const
  const char *chipname = "gpiochip0";
  // build gpiod lines
  struct gpiod_chip *chip;
  struct gpiod_line *led;
  // vars
  int i;

  // open GPIO chip
  chip = gpiod_chip_open_by_name(chipname);
  // open GPIO line
  led = gpiod_chip_get_line(chip, 22);
  // request line as output 
  gpiod_line_request_output(led, "test", 0);

  // blink led
  i = 0;
  while (i < 51) {
    // set the value of the gpiod line (name, ture/false)
    gpiod_line_set_value(led, (i & 1) != 0);
    usleep(1*1000000); //in sec
    i++;
  }

  // release lines
  gpiod_line_release(led);
  // close chip
  gpiod_chip_close(chip);

  return 0;
}