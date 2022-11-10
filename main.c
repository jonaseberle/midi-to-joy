#include <fcntl.h>
#include <linux/input.h>
#include <linux/uinput.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define msleep(ms) usleep((ms) * 1000)

//
// https://forums.bannister.org/ubbthreads.php?ubb=showflat&Number=118786
//
// https://gist.github.com/matthewaveryusa/a721aad80ae89a5c69f7c964fa20fec1
// https://github.com/GrantEdwards/uinput-joystick-demo
// https://www.kernel.org/doc/html/v4.12/input/uinput.html
// https://blog.marekkraus.sk/c/linuxs-uinput-usage-tutorial-virtual-gamepad/
//

static int fd, midi_fd;

static void setupAbs(int fd, unsigned short chan, int min, int max);
static void sendEvent(struct input_event input_event);

typedef struct {
    int in;
    int out;
} map;

const int btnCodes[] = {
//    BTN_DPAD_UP,
//    BTN_DPAD_DOWN,
//    BTN_DPAD_LEFT,
//    BTN_DPAD_RIGHT,
    BTN_0,
    BTN_1,
    BTN_2,
    BTN_3,
    BTN_4,
    BTN_5,
    BTN_6,
    BTN_7,
    BTN_8,
    BTN_9,
//    BTN_LEFT,
    BTN_DEAD,
    BTN_SOUTH,
    BTN_EAST,
    BTN_C,
    BTN_NORTH,
    BTN_WEST,
    BTN_Z,
    BTN_TL,
    BTN_TR,
    BTN_TL2,
    BTN_TR2,
    BTN_SELECT,
    BTN_START,
    BTN_MODE,
    BTN_THUMBL,
    BTN_THUMBR,
    BTN_RIGHT,
    BTN_MIDDLE,
    BTN_SIDE,
    BTN_EXTRA,
    BTN_FORWARD,
    BTN_BACK,
    BTN_TASK,
    BTN_TRIGGER,
    BTN_THUMB,
    BTN_THUMB2,
    BTN_TOP,
    BTN_TOP2,
    BTN_PINKIE,
    BTN_BASE,
    BTN_BASE2,
    BTN_BASE3,
    BTN_BASE4,
    BTN_BASE5,
    BTN_BASE6,
    BTN_GEAR_DOWN,
    BTN_GEAR_UP
    //    BTN_TOOL_PEN,
    //    BTN_TOOL_RUBBER,
    //    BTN_TOOL_BRUSH,
    //    BTN_TOOL_PENCIL,
    //    BTN_TOOL_AIRBRUSH,
    //    BTN_TOOL_FINGER,
    //    BTN_TOOL_MOUSE,
    //    BTN_TOOL_LENS,
    //    BTN_STYLUS3,
    //    BTN_TOUCH,
    //    BTN_STYLUS,
    //    BTN_STYLUS2,
    //    BTN_TOOL_DOUBLETAP,
    //    BTN_TOOL_TRIPLETAP,
};

// @see input-event-codes.h
const int axisCodes[32] = {
    0x02,
    0x03,
    0x04,
    0x05,
    0x06,
    0x07,
    0x08,
    0x09,
    0x0a,
    0x0b,
    0x0c,
    0x0d,
    0x0e,
    0x0f,
    0x1d,
    0x1e,
    0x1f,
    0x21,
    0x22,
    0x23,
    0x24,
    0x25,
    0x26,
    0x27,
    0x28,
    0x29,
    0x2a,
    0x2b,
    0x2c,
    0x2d,
    0x3e,
    0x3f
};

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: midi-to-joy </dev/midiX>\n");
        return EXIT_FAILURE;
    }

    printf("opening %s\n", argv[1]);
    midi_fd = open(argv[1], O_RDWR | O_NONBLOCK);

    if (midi_fd < 0) {
        perror("open /dev/midiX");
        return EXIT_FAILURE;
    }

    // init LaunchControl XL lights as "dim"
    //unsigned char buf[3] = {0xb8, 0x00, 0x7d};
    // init LaunchControl XL lights off
    unsigned char buf[3] = { 0xb8, 0x00, 0x00 };
    if (write(midi_fd, &buf, 3) < 0) {
        perror("write");
        exit(EXIT_FAILURE);
    }

    // sleep(1); // wait for a second
    fd = open("/dev/uinput", O_WRONLY | O_NONBLOCK);

    if (fd < 0) {
        perror("open /dev/uinput");
        return EXIT_FAILURE;
    }

    // enable button/key handling
    ioctl(fd, UI_SET_EVBIT, EV_KEY);
    // @see input-event-codes.h
    // valid button codes are 0x100 - 0x151
    // max. 64
    printf("Setting up btns... ");
    for (int i = 0; i < 24; i++) {
        printf("%x ", btnCodes[i]);
        ioctl(fd, UI_SET_KEYBIT, btnCodes[i]);
    }
    printf("\n");

    // enable analog absolute position handling
    ioctl(fd, UI_SET_EVBIT, EV_ABS);
    // only 24 recognized by X-Plane?
    printf("Setting up axii... ");
    for (int i = 0; i < 32; i++) {
        printf("%x ", axisCodes[i]);
        setupAbs(fd, axisCodes[i], 0, 127);
    }
    printf("\n");

    struct uinput_setup setup = { .id =
                                  {
                                      .bustype = BUS_USB,
                                      .vendor = 0x3,
                                      .product = 0x3,
                                      .version = 2,
                                  },
                                  .name = "LaunchControl XL joystick" };

    if (ioctl(fd, UI_DEV_SETUP, &setup)) {
        perror("UI_DEV_SETUP");
        return 1;
    }

    if (ioctl(fd, UI_DEV_CREATE)) {
        perror("UI_DEV_CREATE");
        return 1;
    }

    struct input_event input_event;

    unsigned char buffer[256];
    while (1) {
        int readval;

        if (readval = read(midi_fd, &buffer, 3), readval != -1) {
            printf("read: %x %x(%d) %x\n", buffer[0], buffer[1], buffer[1], buffer[2]);

            int channel = buffer[0] & 0x0f;
            int bank = channel - 8;
            bool isNote = (buffer[0] & 0xf0) == 0x80 || (buffer[0] & 0xf0) == 0x90;
            bool isCC = (buffer[0] & 0xf0) == 0xb0;
            // @todo: 0xf0 would be SysEx and the message would contain 7 blocks

            if (isNote) {
                bool isNoteOn = buffer[2] != 0;
                int note = buffer[1];
                int btn = -1;
                if (note >= 41 && note <= 44) {
                    btn = note - 41 + bank * 24;
                } else if (note >= 57 && note <= 60) {
                    btn = note - 53 + bank * 24;
                } else if (note >= 73 && note <= 76) {
                    btn = note - 65 + bank * 24;
                } else if (note >= 89 && note <= 92) {
                    btn = note - 77 + bank * 24;
                } else if (note >= 105 && note <= 108) {
                    btn = note - 85 + bank * 24;
                }

                printf("note=%d channel=%d on=%d => btn=%d\n", note, channel, isNoteOn, btn);
                if (btn != -1) {
                    input_event.type = EV_KEY;
                    // @see input-event-codes.h
                    // valid button codes are 0x100 - 0x151
                    input_event.code = btnCodes[btn];
                    input_event.value = isNoteOn;

                    sendEvent(input_event);
                }
            } else if (isCC) {
                int cc = buffer[1];
                int velocity = buffer[2];

                int axis = -1;
                if (cc >= 13 && cc <= 20) { // axis 24-31
                    axis = cc + 11 + bank * 32;
                } else if (cc >= 29 && cc <= 36) { // axis 16-23
                    axis = cc - 13 + bank * 32;
                } else if (cc >= 49 && cc <= 56) { // axis 8-15
                    axis = cc - 41 + bank * 32;
                } else if (cc >= 77 && cc <= 84) { // axis 0-7
                    axis = cc - 77 + bank * 32;
                }

                printf("CC=%d channel=%d vel=%d => axis=%d\n", cc, channel, velocity, axis);
                if (axis != -1) {
                    input_event.type = EV_ABS;
                    // @see input-event-codes.h
                    input_event.code = axisCodes[axis];
                    input_event.value = velocity;

                    sendEvent(input_event);
                }
            }
        }
        if (readval <= 0) {
            msleep(50); // sleep for a bit so we don't use all the cpu
        }
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
static void setupAbs(int fd, unsigned short chan, int min, int max) {
    if (ioctl(fd, UI_SET_ABSBIT, chan)) {
        perror("UI_SET_ABSBIT");
    }

    struct uinput_abs_setup s = {
        .code = chan,
        .absinfo = { .minimum = min, .maximum = max },
    };

    if (ioctl(fd, UI_ABS_SETUP, &s)) {
        perror("UI_ABS_SETUP");
    }
}

static void sendEvent(struct input_event input_event) {
    // send value
    if (write(fd, &input_event, sizeof input_event) < 0) {
        perror("write");
        exit(EXIT_FAILURE);
    }

    // send SYN
    input_event.type = EV_SYN;
    input_event.code = 0;
    input_event.value = 0;

    if (write(fd, &input_event, sizeof input_event) < 0) {
        perror("write SYN");
        exit(EXIT_FAILURE);
    }
}
