#include <fcntl.h>
#include <linux/uinput.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define msleep(ms) usleep((ms)*1000)

// miditest3.c
// midi to uinput button test mapping notes to buttons, and mod/pitch/volume to
// axes by Golden Child just maps the low register of the keyboard into button
// pressess using examples from
//
// https://forums.bannister.org/ubbthreads.php?ubb=showflat&Number=118786
// https://gist.github.com/matthewaveryusa/a721aad80ae89a5c69f7c964fa20fec1
// https://github.com/GrantEdwards/uinput-joystick-demo
// https://www.kernel.org/doc/html/v4.12/input/uinput.html
// https://blog.marekkraus.sk/c/linuxs-uinput-usage-tutorial-virtual-gamepad/

static void setup_abs(int fd, unsigned short chan, int min, int max);

int main(int argc, char *argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Usage: midi-to-joy </dev/midiX>");
    return EXIT_FAILURE;
  }

  int midi_fd = open(argv[1], O_RDONLY | O_NONBLOCK);

  if (midi_fd < 0) {
    perror("open /dev/midiX");
    return EXIT_FAILURE;
  }

  // sleep(1); // wait for a second
  int fd = open("/dev/uinput", O_WRONLY | O_NONBLOCK);

  if (fd < 0) {
    perror("open /dev/uinput");
    return EXIT_FAILURE;
  }

  ioctl(fd, UI_SET_EVBIT, EV_KEY); // enable button/key handling

  // event codes from linux/input-event-codes.h

  int translatetable[] = {
      0x29,
      BTN_0,
      0xd,
      BTN_1,
      0xe,
      BTN_2,
      0xf,
      BTN_3,
      0x10,
      BTN_4,
      0x11,
      BTN_5,
      0x12,
      BTN_6,
      0x13,
      BTN_7,
      0x14,
      BTN_8,
      0x15,
      BTN_9,
      0x16,
      BTN_TRIGGER_HAPPY1,
      0x17,
      BTN_TRIGGER_HAPPY2,
      0x18,
      BTN_DPAD_UP,
      0x19,
      BTN_DPAD_LEFT,
      0x1a,
      BTN_DPAD_DOWN,
      0x1b,
      BTN_DPAD_RIGHT,
      0x1c,
      0x2ff,
      0x1d,
      0x280, // 0x300 doesn't work
      0x1e,
      0x2F0, // 2F0 seems to be undefined but works
      0x1f,
      0x2F1,
      0x20,
      0x224, // 224 - 22f undefined
      0x21,
      0x22f,
      0x22,
      0x120,
      0x23,
      0x12f,
      0x24,
      0x130,
      0x25,
      0x13e}; // greater than 0x2ff key max doesn't show up as a button press

  int translatetablelen = sizeof(translatetable) / sizeof(int);

  // tell uinput which BTN events we will generate
  for (int i = 0; i < translatetablelen; i += 2)
    ioctl(fd, UI_SET_KEYBIT, translatetable[i + 1]);
  for (int i = 0; i < translatetablelen; i += 2)
    printf("%d => %d\n", translatetable[i], translatetable[i + 1]);

  ioctl(fd, UI_SET_EVBIT, EV_ABS); // enable analog absolute position handling

  //  setup_abs(fd, ABS_X,  -512, 512);
  //  setup_abs(fd, ABS_Y,  -512, 512);
  //  setup_abs(fd, ABS_Z,  0, 32767);

  //  setup_abs(fd, ABS_X, 0, 127);
  //  setup_abs(fd, ABS_Y, 0, 127);
  //  setup_abs(fd, ABS_Z, 0, 127);

  for (int i = 0; i <= ABS_MAX; i++) {
    if (i != ABS_RESERVED) {
      setup_abs(fd, i, 0, 127);
    }
  }

  struct uinput_setup setup = {.id =
                                   {
                                       .bustype = BUS_USB,
                                       .vendor = 0x3,
                                       .product = 0x3,
                                       .version = 2,
                                   },
                               .name = "LaunchControl XL joystick"};

  if (ioctl(fd, UI_DEV_SETUP, &setup)) {
    perror("UI_DEV_SETUP");
    return 1;
  }

  if (ioctl(fd, UI_DEV_CREATE)) {
    perror("UI_DEV_CREATE");
    return 1;
  }

  struct input_event button_event;

  unsigned char buffer[256];
  while (1) {
    int readval;

    if (readval = read(midi_fd, &buffer, 3), readval != -1) {
      printf("read: %x %x %x\n", buffer[0], buffer[1], buffer[2]);

      if (buffer[0] >= 0x80 && buffer[0] <= 0x9f) { // note on/off
        for (int i = 0; i < translatetablelen; i += 2)
          if (buffer[1] == translatetable[i]) {

            button_event.type = EV_KEY;
            button_event.code = translatetable[i + 1];
            button_event.value = (buffer[2] != 0) ? 1 : 0;
            printf("readval=%d  ev type=%d  key in=%d  => button out=%d "
                   "value=%d\n",
                   readval, button_event.type, buffer[1], button_event.code,
                   button_event.value);

            if (write(fd, &button_event, sizeof button_event) < 0) {
              perror("write");
              return 1;
            }

            // send SYNC event
            button_event.type = EV_SYN;
            button_event.code = 0;
            button_event.value = 0;

            if (write(fd, &button_event, sizeof button_event) < 0) {
              perror("write");
              return 1;
            }
          }
      } else if (buffer[0] >= 0b0 && buffer[0] <= 0xbf) { // CC
        button_event.type = EV_ABS;
        button_event.code =
            (buffer[1] == 1) ? ABS_X
            : (buffer[1] == 7)
                ? ABS_Y
                : 0; // my midi keyboard has 1 for volume, 7 for modulation
        button_event.value = (buffer[2]);

        printf("0xb0 readval=%d  ev type=%d  key in=%d  => button out=%d "
               "value=%d\n",
               readval, button_event.type, buffer[1], button_event.code,
               button_event.value);

        if (write(fd, &button_event, sizeof button_event) < 0) {
          perror("write");
          return 1;
        }

        // send SYNC event
        button_event.type = EV_SYN;
        button_event.code = 0;
        button_event.value = 0;

        if (write(fd, &button_event, sizeof button_event) < 0) {
          perror("write");
          return 1;
        }
      }
    }
    if (readval <= 0)
      msleep(50); // sleep for a bit so we don't use all the cpu
  }

  close(midi_fd);

  if (ioctl(fd, UI_DEV_DESTROY)) {
    printf("UI_DEV_DESTROY");
    return 1;
  }

  close(fd);

  return 0;
}

// enable and configure an absolute "position" analog channel

static void setup_abs(int fd, unsigned short chan, int min, int max) {
  if (ioctl(fd, UI_SET_ABSBIT, chan))
    perror("UI_SET_ABSBIT");

  struct uinput_abs_setup s = {
      .code = chan,
      .absinfo = {.minimum = min, .maximum = max},
  };

  if (ioctl(fd, UI_ABS_SETUP, &s))
    perror("UI_ABS_SETUP");
}
