#include "X11Window.h"

#include "Input.h"
#include <ASL/Types.h>

#include <X11/keysym.h>

namespace CrossPlatform {

namespace internal {

static bool x11_initialized = false;
static int* x11_keycodes;
static int* x11_scancodes;

int translate_state(int state);
KeyCode translate_keysyms(KeySym* keysym, int width);

void x11_init(::Display* display)
{
    int scancode_min, scancode_max;
    XDisplayKeycodes(display, &scancode_min, &scancode_max);

    x11_keycodes = new int[scancode_max];
    x11_scancodes = new int[scancode_max];

    int width;
    KeySym* keysyms = XGetKeyboardMapping(display, scancode_min, scancode_max - scancode_min + 1, &width);

    for (int scancode = scancode_min; scancode < -scancode_max; ++scancode) {
        size_t base = (scancode - scancode_min) * width;
        x11_keycodes[scancode] = (int)translate_keysyms(&keysyms[base], width);
        x11_scancodes[x11_keycodes[scancode]] = scancode;
    }

    free(keysyms);
}

X11Window::X11Window(int x, int y, int width, int height, const char* title)
{
    m_x11_display = XOpenDisplay(NULL);
    ASSERT(m_x11_display);
    if (!x11_initialized)
        x11_init(m_x11_display);

    m_x11_screen = DefaultScreen(m_x11_display);

    m_x11_window = XCreateSimpleWindow(
        m_x11_display,
        RootWindow(m_x11_display, m_x11_screen),
        x, y, width, height,
        1,
        BlackPixel(m_x11_display, m_x11_screen),
        WhitePixel(m_x11_display, m_x11_screen));

    set_title(title);

    XSelectInput(
        m_x11_display, m_x11_window,
        ExposureMask | KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask
            | PointerMotionMask);
    XMapWindow(m_x11_display, m_x11_window);
}

X11Window::~X11Window()
{
    if (m_x11_display)
        XCloseDisplay(m_x11_display);
}

void X11Window::set_title(const char* title)
{
    XStoreName(m_x11_display, m_x11_window, title);
    XSetIconName(m_x11_display, m_x11_window, title);
    m_title = title;
}

void X11Window::set_window_rect(Rect r)
{
    if (r.x == -1)
        r.x = m_rect.x;
    if (r.y == -1)
        r.y = m_rect.y;
    if (r.width == -1)
        r.width = m_rect.width;
    if (r.height == -1)
        r.height = m_rect.height;

    XMoveResizeWindow(m_x11_display, m_x11_window, r.x, r.y, r.width, r.height);
}

void X11Window::process_event(XEvent& event)
{
    switch (event.type) {
    case Expose: {
        auto gc = DefaultGC(m_x11_display, m_x11_screen);
        XDrawString(m_x11_display, m_x11_window, gc, 200, 200, "Hello", 6);
    }
    case KeyPress: {
        int key = x11_keycodes[event.xkey.keycode];
        int mods = translate_state(event.xkey.state);
        KeyDownEvent key_event((KeyCode)key, mods);
    }

    case KeyRelease: {
        // Handle Repeated key
        if (XEventsQueued(m_x11_display, QueuedAfterReading)) {
            XEvent next_event;
            XPeekEvent(m_x11_display, &next_event);

            if (next_event.type == KeyPress
                && next_event.xkey.time == event.xkey.time
                && next_event.xkey.keycode == event.xkey.keycode) {
                // Ignore repeated key release
                return;
            }
        }
    }
    }
}

void X11Window::poll_events()
{
    XPending(m_x11_display);

    while (QLength(m_x11_display)) {
        XEvent xevent;
        XNextEvent(m_x11_display, &xevent);
        process_event(xevent);
    }

    XFlush(m_x11_display);
}

int translate_state(int state)
{
    int mods = 0;
    if (state & ShiftMask)
        mods |= KeyModShift;
    if (state & ControlMask)
        mods |= KeyModControl;
    if (state & Mod1Mask)
        mods |= KeyModAlt;
    if (state & Mod4Mask)
        mods |= KeyModSuper;
    if (state & LockMask)
        mods |= KeyModCapsLock;
    if (state & Mod2Mask)
        mods |= KeyModNumLock;

    return mods;
}

KeyCode translate_keysyms(KeySym* keysyms, int width)
{
    if (width > 1) {
        switch (keysyms[1]) {
        case XK_KP_0:
            return KeyCode::KP0;
        case XK_KP_1:
            return KeyCode::KP1;
        case XK_KP_2:
            return KeyCode::KP2;
        case XK_KP_3:
            return KeyCode::KP3;
        case XK_KP_4:
            return KeyCode::KP4;
        case XK_KP_5:
            return KeyCode::KP5;
        case XK_KP_6:
            return KeyCode::KP6;
        case XK_KP_7:
            return KeyCode::KP7;
        case XK_KP_8:
            return KeyCode::KP8;
        case XK_KP_9:
            return KeyCode::KP9;
        case XK_KP_Separator:
        case XK_KP_Decimal:
            return KeyCode::KPDecimal;
        case XK_KP_Equal:
            return KeyCode::KPEqual;
        case XK_KP_Enter:
            return KeyCode::KPEnter;
        default:
            break;
        }
    }

    switch (keysyms[0]) {
    case XK_Escape:
        return KeyCode::Escape;
    case XK_Tab:
        return KeyCode::Tab;
    case XK_Shift_L:
        return KeyCode::LeftShift;
    case XK_Shift_R:
        return KeyCode::RightShift;
    case XK_Control_L:
        return KeyCode::LeftControl;
    case XK_Control_R:
        return KeyCode::RightControl;
    case XK_Meta_L:
    case XK_Alt_L:
        return KeyCode::LeftAlt;
    case XK_Mode_switch: // Mapped to Alt_R on many keyboards
    case XK_Meta_R:
    case XK_Alt_R:
        return KeyCode::RightAlt;
    case XK_Super_L:
        return KeyCode::LeftSuper;
    case XK_Super_R:
        return KeyCode::RightSuper;
    case XK_Menu:
        return KeyCode::Menu;
    case XK_Num_Lock:
        return KeyCode::NumLock;
    case XK_Caps_Lock:
        return KeyCode::CapsLock;
    case XK_Print:
        return KeyCode::PrintScreen;
    case XK_Scroll_Lock:
        return KeyCode::ScrollLock;
    case XK_Pause:
        return KeyCode::Pause;
    case XK_Delete:
        return KeyCode::Delete;
    case XK_BackSpace:
        return KeyCode::Backspace;
    case XK_Return:
        return KeyCode::Enter;
    case XK_Home:
        return KeyCode::Home;
    case XK_End:
        return KeyCode::End;
    case XK_Page_Up:
        return KeyCode::PageUp;
    case XK_Page_Down:
        return KeyCode::PageDown;
    case XK_Insert:
        return KeyCode::Insert;
    case XK_Left:
        return KeyCode::Left;
    case XK_Right:
        return KeyCode::Right;
    case XK_Down:
        return KeyCode::Down;
    case XK_Up:
        return KeyCode::Up;
    case XK_F1:
        return KeyCode::F1;
    case XK_F2:
        return KeyCode::F2;
    case XK_F3:
        return KeyCode::F3;
    case XK_F4:
        return KeyCode::F4;
    case XK_F5:
        return KeyCode::F5;
    case XK_F6:
        return KeyCode::F6;
    case XK_F7:
        return KeyCode::F7;
    case XK_F8:
        return KeyCode::F8;
    case XK_F9:
        return KeyCode::F9;
    case XK_F10:
        return KeyCode::F10;
    case XK_F11:
        return KeyCode::F11;
    case XK_F12:
        return KeyCode::F12;
    case XK_F13:
        return KeyCode::F13;
    case XK_F14:
        return KeyCode::F14;
    case XK_F15:
        return KeyCode::F15;
    case XK_F16:
        return KeyCode::F16;
    case XK_F17:
        return KeyCode::F17;
    case XK_F18:
        return KeyCode::F18;
    case XK_F19:
        return KeyCode::F19;
    case XK_F20:
        return KeyCode::F20;
    case XK_F21:
        return KeyCode::F21;
    case XK_F22:
        return KeyCode::F22;
    case XK_F23:
        return KeyCode::F23;
    case XK_F24:
        return KeyCode::F24;
    case XK_F25:
        return KeyCode::F25;

    // Numeric keypad
    case XK_KP_Divide:
        return KeyCode::KPDivide;
    case XK_KP_Multiply:
        return KeyCode::KPMultiply;
    case XK_KP_Subtract:
        return KeyCode::KPSubtract;
    case XK_KP_Add:
        return KeyCode::KPAdd;

    // These should have been detected in secondary keysym test above!
    case XK_KP_Insert:
        return KeyCode::KP0;
    case XK_KP_End:
        return KeyCode::KP1;
    case XK_KP_Down:
        return KeyCode::KP2;
    case XK_KP_Page_Down:
        return KeyCode::KP3;
    case XK_KP_Left:
        return KeyCode::KP4;
    case XK_KP_Right:
        return KeyCode::KP6;
    case XK_KP_Home:
        return KeyCode::KP7;
    case XK_KP_Up:
        return KeyCode::KP8;
    case XK_KP_Page_Up:
        return KeyCode::KP9;
    case XK_KP_Delete:
        return KeyCode::KPDecimal;
    case XK_KP_Equal:
        return KeyCode::KPEqual;
    case XK_KP_Enter:
        return KeyCode::KPEnter;

    // Last resort: Check for printable keys (should not happen if the XKB
    // extension is available). This will give a layout dependent mapping
    // (which is wrong, and we may miss some keys, especially on non-US
    // keyboards), but it's better than nothing...
    case XK_a:
        return KeyCode::A;
    case XK_b:
        return KeyCode::B;
    case XK_c:
        return KeyCode::C;
    case XK_d:
        return KeyCode::D;
    case XK_e:
        return KeyCode::E;
    case XK_f:
        return KeyCode::F;
    case XK_g:
        return KeyCode::G;
    case XK_h:
        return KeyCode::H;
    case XK_i:
        return KeyCode::I;
    case XK_j:
        return KeyCode::J;
    case XK_k:
        return KeyCode::K;
    case XK_l:
        return KeyCode::L;
    case XK_m:
        return KeyCode::M;
    case XK_n:
        return KeyCode::N;
    case XK_o:
        return KeyCode::O;
    case XK_p:
        return KeyCode::P;
    case XK_q:
        return KeyCode::Q;
    case XK_r:
        return KeyCode::R;
    case XK_s:
        return KeyCode::S;
    case XK_t:
        return KeyCode::T;
    case XK_u:
        return KeyCode::U;
    case XK_v:
        return KeyCode::V;
    case XK_w:
        return KeyCode::W;
    case XK_x:
        return KeyCode::X;
    case XK_y:
        return KeyCode::Y;
    case XK_z:
        return KeyCode::Z;
    case XK_1:
        return KeyCode::D1;
    case XK_2:
        return KeyCode::D2;
    case XK_3:
        return KeyCode::D3;
    case XK_4:
        return KeyCode::D4;
    case XK_5:
        return KeyCode::D5;
    case XK_6:
        return KeyCode::D6;
    case XK_7:
        return KeyCode::D7;
    case XK_8:
        return KeyCode::D8;
    case XK_9:
        return KeyCode::D9;
    case XK_0:
        return KeyCode::D0;
    case XK_space:
        return KeyCode::Space;
    case XK_minus:
        return KeyCode::Minus;
    case XK_equal:
        return KeyCode::Equal;
    case XK_bracketleft:
        return KeyCode::LeftBracket;
    case XK_bracketright:
        return KeyCode::RightBracket;
    case XK_backslash:
        return KeyCode::Backslash;
    case XK_semicolon:
        return KeyCode::Semicolon;
    case XK_apostrophe:
        return KeyCode::Apostrophe;
    case XK_grave:
        return KeyCode::GraveAccent;
    case XK_comma:
        return KeyCode::Comma;
    case XK_period:
        return KeyCode::Period;
    case XK_slash:
        return KeyCode::Slash;
    default:
        break;
    }

    // No matching translation was found
    return KeyCode::Unknown;
}

} // namespace internal

} // namespace CrossPlatform
