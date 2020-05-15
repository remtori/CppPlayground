#include "Input.h"

namespace GUI {

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

} // namespace GUI
