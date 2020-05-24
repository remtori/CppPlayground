#pragma once

#include <LibGUI/Input.h>
#include <X11/Xlib.h>

namespace GUI {

namespace internal {

struct X11Data {
    ::Display* display = nullptr;
    int screen = 0;
};

void init(void*& data_ptr);
int translate_state(int state);
KeyCode translate_keycode(int keycode);

} // namespace internal

} // namespace GUI
