#include "keyboard.hpp"

#include <memory>
#include "usb/classdriver/keyboard.hpp"
#include "task.hpp"

namespace {

const char keycode_map[256] = {
  0,    0,    0,    0,    ']', '\t',  'm',  'u', // 0
  '1',  '[',  'i',  'p',  '5', '\n',  'l',  'j', // 8
  '`',  ' ',  '=',  '9',  'y',  '2',  'o',  '3', // 16
  '6',  '.',  't',  'n',  '4',  '/',  'd',  'f', // 24
  'h',  'g',  'z',  'x',  'c',  'v',  '9',  'b', // 32
  '8',  's',  'e',  'r',  ' ',  'q',  'w',  '7', // 40
  '-',  ',',  '#', '\'',  'k',  '`', '\b',  '.', // 48
  '/',  0,    0,    0,  'a',    0,    0,    0,   // 56
  0,    0,    0,    0,    0,    0,    0,    0,   // 64
  0,    0,    0,    0,    0,    0,    0,    0,   // 72
  0,    0,    0,    0,    '/',  '*',  '-',  '+', // 80
  '\n', '1',  '2',  '3',  '4',  '5',  '6',  '7', // 88
  '8',  '9',  '0',  '.', '\\',  0,    0,    '=', // 96
};

const char keycode_map_shifted[256] = {
  0,    0,    0,    0,    ']', '\t',  'M',  'U', // 0
  '!',  '[',  'I',  'P',  '%', '\n',  'L',  'J', // 8
  '`',  ' ',  '+',  '9',  'Y',  '@',  'O',  '#', // 16
  '^',  '.',  'T',  'N',  '$',  '/',  'D',  'F', // 24
  'H',  'G',  'Z',  'X',  'C',  'V',  '(',  'B', // 32
  '*',  'S',  'E',  'R',  ' ',  'Q',  'W',  '&', // 40
  '-',  ',',  '#', '\'',  'K',  '`', '\b',  '.', // 48
  '/',  0,    0,    0,  'A',    0,    0,    0,   // 56
  0,    0,    0,    0,    0,    0,    0,    0,   // 64
  0,    0,    0,    0,    0,    0,    0,    0,   // 72
  0,    0,    0,    0,    '/',  '*',  '-',  '+', // 80
  '\n', '1',  '2',  '3',  '4',  '5',  '6',  '7', // 88
  '8',  '9',  '0',  '.', '\\',  0,    0,    '=', // 96
};

} // namespace

void InitializeKeyboard() {
  usb::HIDKeyboardDriver::default_observer =
    [](uint8_t modifier, uint8_t keycode, bool press) {
      const bool shift = (modifier & (kLShiftBitMask | kRShiftBitMask)) != 0;
      char ascii = keycode_map[keycode];
      if (shift) {
        ascii = keycode_map_shifted[keycode];
      }
      Message msg{Message::kKeyPush};
      msg.arg.keyboard.modifier = modifier;
      msg.arg.keyboard.keycode = keycode;
      msg.arg.keyboard.ascii = ascii;
      msg.arg.keyboard.press = press;
      task_manager->SendMessage(1, msg);
    };
}
