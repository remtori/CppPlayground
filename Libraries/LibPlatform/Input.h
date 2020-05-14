#pragma once

namespace CrossPlatform {

#define _FOR_EVERY_KEY_CODE \
    __KEY(Space)            \
    __KEY(Apostrophe)       \
    __KEY(Comma)            \
    __KEY(Minus)            \
    __KEY(Period)           \
    __KEY(Slash)            \
    __KEY(D0)               \
    __KEY(D1)               \
    __KEY(D2)               \
    __KEY(D3)               \
    __KEY(D4)               \
    __KEY(D5)               \
    __KEY(D6)               \
    __KEY(D7)               \
    __KEY(D8)               \
    __KEY(D9)               \
    __KEY(Semicolon)        \
    __KEY(Equal)            \
    __KEY(A)                \
    __KEY(B)                \
    __KEY(C)                \
    __KEY(D)                \
    __KEY(E)                \
    __KEY(F)                \
    __KEY(G)                \
    __KEY(H)                \
    __KEY(I)                \
    __KEY(J)                \
    __KEY(K)                \
    __KEY(L)                \
    __KEY(M)                \
    __KEY(N)                \
    __KEY(O)                \
    __KEY(P)                \
    __KEY(Q)                \
    __KEY(R)                \
    __KEY(S)                \
    __KEY(T)                \
    __KEY(U)                \
    __KEY(V)                \
    __KEY(W)                \
    __KEY(X)                \
    __KEY(Y)                \
    __KEY(Z)                \
    __KEY(LeftBracket)      \
    __KEY(Backslash)        \
    __KEY(RightBracket)     \
    __KEY(GraveAccent)      \
    __KEY(Escape)           \
    __KEY(Enter)            \
    __KEY(Tab)              \
    __KEY(Backspace)        \
    __KEY(Insert)           \
    __KEY(Delete)           \
    __KEY(Right)            \
    __KEY(Left)             \
    __KEY(Down)             \
    __KEY(Up)               \
    __KEY(PageUp)           \
    __KEY(PageDown)         \
    __KEY(Home)             \
    __KEY(End)              \
    __KEY(CapsLock)         \
    __KEY(ScrollLock)       \
    __KEY(NumLock)          \
    __KEY(PrintScreen)      \
    __KEY(Pause)            \
    __KEY(F1)               \
    __KEY(F2)               \
    __KEY(F3)               \
    __KEY(F4)               \
    __KEY(F5)               \
    __KEY(F6)               \
    __KEY(F7)               \
    __KEY(F8)               \
    __KEY(F9)               \
    __KEY(F10)              \
    __KEY(F11)              \
    __KEY(F12)              \
    __KEY(F13)              \
    __KEY(F14)              \
    __KEY(F15)              \
    __KEY(F16)              \
    __KEY(F17)              \
    __KEY(F18)              \
    __KEY(F19)              \
    __KEY(F20)              \
    __KEY(F21)              \
    __KEY(F22)              \
    __KEY(F23)              \
    __KEY(F24)              \
    __KEY(F25)              \
    __KEY(KP0)              \
    __KEY(KP1)              \
    __KEY(KP2)              \
    __KEY(KP3)              \
    __KEY(KP4)              \
    __KEY(KP5)              \
    __KEY(KP6)              \
    __KEY(KP7)              \
    __KEY(KP8)              \
    __KEY(KP9)              \
    __KEY(KPDecimal)        \
    __KEY(KPDivide)         \
    __KEY(KPMultiply)       \
    __KEY(KPSubtract)       \
    __KEY(KPAdd)            \
    __KEY(KPEnter)          \
    __KEY(KPEqual)          \
    __KEY(LeftShift)        \
    __KEY(LeftControl)      \
    __KEY(LeftAlt)          \
    __KEY(LeftSuper)        \
    __KEY(RightShift)       \
    __KEY(RightControl)     \
    __KEY(RightAlt)         \
    __KEY(RightSuper)       \
    __KEY(Menu)             \
    __KEY(Unknown)

enum class KeyCode {
#define __KEY(x) x,
    _FOR_EVERY_KEY_CODE
#undef __KEY
};

enum KeyMod {
    KeyModShift = 1 << 0,
    KeyModControl = 1 << 1,
    KeyModAlt = 1 << 2,
    KeyModSuper = 1 << 3,
    KeyModCapsLock = 1 << 4,
    KeyModNumLock = 1 << 5,
};

enum class MouseButton {
    Left,
    Right,
    Middle,
};

const char* to_string(KeyCode code)
{
    switch (code) {
#define __KEY(x)     \
    case KeyCode::x: \
        return #x;
        _FOR_EVERY_KEY_CODE
#undef __KEY
    default:
        return "Unknown KeyCode";
    }
}

const char* to_string(KeyMod mods)
{
    switch (mods) {
    case KeyModShift:
        return "Shift";
    case KeyModControl:
        return "Control";
    case KeyModAlt:
        return "Alt";
    case KeyModSuper:
        return "Super";
    case KeyModCapsLock:
        return "CapsLock";
    case KeyModNumLock:
        return "NumLock";
    default:
        return "Unknown Modifer Key";
    }
}

const char* to_string(MouseButton btn)
{
    switch (btn) {
    case MouseButton::Left:
        return "LeftMouseButton";
    case MouseButton::Right:
        return "RightMouseButton";
    case MouseButton::Middle:
        return "MiddleMouseButton";
    default:
        return "Unknown Mouse Button";
    }
}

} // namespace CrossPlatform

using CrossPlatform::to_string;
