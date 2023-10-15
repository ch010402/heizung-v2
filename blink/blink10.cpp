/* see license file
* compile with: g++ -Wall -o blink10 blink10.cpp -lgpiod
*/

#include <gpiod.h>
#include <unistd.h> //used for usleep

int main(int argc, char **argv)
{
  // set the chip name as const
  const char* chipname = "gpiochip0";
  // build gpiod lines
  struct gpiod_chip *chip;
  struct gpiod_line *red1;
  struct gpiod_line *red2;
  struct gpiod_line *red3;
  struct gpiod_line *yellow1;
  struct gpiod_line *yellow2;
  struct gpiod_line *yellow3;
  struct gpiod_line *green1;
  struct gpiod_line *green2;
  struct gpiod_line *green3;
  struct gpiod_line *blue;
  // vars
  int i;

  // open GPIO chip
  chip = gpiod_chip_open_by_name(chipname);
  // open GPIO line
  red1 = gpiod_chip_get_line(chip, 23);
  red2 = gpiod_chip_get_line(chip, 5);
  red3 = gpiod_chip_get_line(chip, 24);
  yellow1 = gpiod_chip_get_line(chip, 25);
  yellow2 = gpiod_chip_get_line(chip, 12);
  yellow3 = gpiod_chip_get_line(chip, 16);
  green1 = gpiod_chip_get_line(chip, 19);
  green2 = gpiod_chip_get_line(chip, 26);
  green3 = gpiod_chip_get_line(chip, 20);
  blue = gpiod_chip_get_line(chip, 22);
  // request line as output 
  gpiod_line_request_output(red1, "test", 0);
  gpiod_line_request_output(red2, "test", 0);
  gpiod_line_request_output(red3, "test", 0);
  gpiod_line_request_output(yellow1, "test", 0);
  gpiod_line_request_output(yellow2, "test", 0);
  gpiod_line_request_output(yellow3, "test", 0);
  gpiod_line_request_output(green1, "test", 0);
  gpiod_line_request_output(green2, "test", 0);
  gpiod_line_request_output(green3, "test", 0);
  gpiod_line_request_output(blue, "test", 0);

  // blink led
  i = 0;
  while (i < 1025) {
    // set the value of the gpiod line (name, ture/false)
    gpiod_line_set_value(red1, (i & 1) != 0);
    gpiod_line_set_value(red2, (i & 2) != 0);
    gpiod_line_set_value(red3, (i & 4) != 0);
    gpiod_line_set_value(yellow1, (i & 8) != 0);
    gpiod_line_set_value(yellow2, (i & 16) != 0);
    gpiod_line_set_value(yellow3, (i & 32) != 0);
    gpiod_line_set_value(green1, (i & 64) != 0);
    gpiod_line_set_value(green2, (i & 128) != 0);
    gpiod_line_set_value(green3, (i & 256) != 0);
    gpiod_line_set_value(blue, (i & 512) != 0);
    usleep(0.5 * 1000000); //in sec
    i++;
  }

  // release lines
  gpiod_line_release(red1);
  gpiod_line_release(red2);
  gpiod_line_release(red3);
  gpiod_line_release(yellow1);
  gpiod_line_release(yellow2);
  gpiod_line_release(yellow3);
  gpiod_line_release(green1);
  gpiod_line_release(green2);
  gpiod_line_release(green3);
  gpiod_line_release(blue);
  // close chip
  gpiod_chip_close(chip);

  return 0;
}