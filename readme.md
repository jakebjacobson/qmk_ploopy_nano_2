# QMK Firmware - Ploopy Nano Fork

This is a fork of [qmk_firmware](https://github.com/qmk/qmk_firmware) that only supports the Ploopy Nano 2 trackball.

## Build Instructions

Run this command to compile the firmware:

```bash
make ploopyco/nano_2/rev2_003:default
```

When successful, this creates `ploopyco_nano_2_rev2_003_default.uf2` in the `.build/` directory. To flash the firmware:

1. Unplug the Ploopy Nano 2
2. Hold the button on the device
3. Plug it back in (the device will mount as storage)
4. Copy the `.uf2` file to the mounted drive

See the [official programming instructions](https://ploopyco.github.io/nano-2-trackball/appendices/programming/) for more details.

## DPI Configuration

The Ploopy Nano 2 only has one physical button, so there is no way to cycle DPI through the hardware. The trackball DPI is set to `1600` by default. To change it, edit the DPI options in code before flashing:

1. Open `keyboards/ploopyco/ploopyco.c`
2. Find the `PLOOPY_DPI_OPTIONS` define (line 39) and change the values
3. Update `PLOOPY_DPI_DEFAULT` to point to the correct index (0-based) in the options array
4. Build and flash the firmware as described above

For example:

```c
#define PLOOPY_DPI_OPTIONS { 800, 1600, 3200 }
#define PLOOPY_DPI_DEFAULT 1
```

This differs from the [main QMK firmware repo](https://github.com/qmk/qmk_firmware) where most devices with trackballs have a `DPI_CONFIG` keycode mapped to a physical button, allowing you to cycle DPI on the fly. In this fork, since the Nano 2 has only one button (which handles mouse clicks), there is no dedicated button for DPI cycling. If you wanted to add on-the-fly DPI cycling, you could map `DPI_CONFIG` to the left button combined with a long press or some other gesture, but it would require firmware changes.

## Custom Trackball Functionality

This fork adds specialized trackball controls for the Ploopy Nano 2. The device has a single physical button mapped at matrix position `(0, 0)` (defined in `keyboards/ploopyco/nano_2/info.json`). The button is mapped to `QK_MOUSE_BUTTON_1` (left mouse click) in `keyboards/ploopyco/nano_2/keymaps/default/keymap.c`.

**Left Button Click**: When the button is pressed, the key event flows through QMK's tapping system in `quantum/action_tapping.c`, which distinguishes between a quick tap and a hold based on `TAPPING_TERM`. On press, `process_action()` in `quantum/action.c` routes the keycode to `pointing_device_keycode_handler()` in `quantum/pointing_device/pointing_device.c:574`, which sets the appropriate button bit via `pointing_device_handle_buttons()` and sends the updated mouse report.

**Left Button Hold & Wiggle Toggle**: The hold detection is handled by QMK's action tapping subsystem (`quantum/action_tapping.c`), which tracks how long a key is held before another key is pressed. The wiggle toggle (between mouse movement and scroll modes) is implemented in the user keymap at `keyboards/ploopyco/nano_2/keymaps/default/keymap.c` in `pointing_device_task_user()`, which detects right/left wiggle gestures on the trackball and toggles `scrolling_mode` after 3+ wiggles.

**Changing the button to right-click**: The default button maps to `QK_MOUSE_BUTTON_1` (left-click). To change it to right-click instead, modify `keyboards/ploopyco/nano_2/keymaps/default/keymap.c` and change the keycode to `QK_MOUSE_BUTTON_2`. The mouse button keycodes are defined in `quantum/keycode.h` and `quantum/keycodes.h` as `QK_MOUSE_BUTTON_1` (bit `0x01`, left-click) through `QK_MOUSE_BUTTON_8` (bit `0x80`). The button behavior is routed through `pointing_device_keycode_handler()` in `quantum/pointing_device/pointing_device.c:574`, which calls `pointing_device_handle_buttons()` to set the correct button bit in the mouse report.

## Credits

Wiggle toggle implementation adapted from [pandrr/wiggleball](https://github.com/pandrr/wiggleball). Ploopy Nano 2 trackball information from [ploopyco/nano-2-trackball](https://github.com/ploopyco/nano-2-trackball).

## Documentation

* [See the official documentation on docs.qmk.fm](https://docs.qmk.fm)

## Supported Keyboards

* [Ploopy Nano 2](keyboards/ploopyco/nano_2/)

