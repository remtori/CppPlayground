#pragma once

#include "Input.h"
#include <ASL/UtilityStruct.h>

namespace CrossPlatform {

class Event {
public:
    enum class Type {
        Unknown,
        KeyUp,
        KeyDown,
        MouseUp,
        MouseDown,
        MouseMove,
        MouseScroll,
    };

    enum Catergory {
        EventCatergoryUnknown = 0,
        EventCatergoryInput = 1 << 0,
        EventCatergoryKeyboard = 1 << 1,
        EventCatergoryMouse = 1 << 2,
        EventCatergoryWindow = 1 << 3,
    };

    Event() {}
    ~Event() {}

    bool is_in_catergory(Catergory c)
    {
        return catergory_flags() & c;
    }

    virtual int catergory_flags() const { return EventCatergoryUnknown; }
    virtual Type type() const { return Type::Unknown; }
};

class InputEvent : public Event {
public:
    bool shift() const { return m_mods & KeyModShift; }
    bool control() const { return m_mods & KeyModControl; }
    bool alt() const { return m_mods & KeyModAlt; }
    bool super() const { return m_mods & KeyModSuper; }
    bool caps_lock() const { return m_mods & KeyModCapsLock; }
    bool num_lock() const { return m_mods & KeyModNumLock; }

protected:
    InputEvent(int mods)
        : m_mods(mods)
    {
    }

    int m_mods;
};

class KeyEvent : public InputEvent {
public:
    KeyCode keycode() const { return m_keycode; }
    virtual int catergory_flags() const override { return EventCatergoryInput | EventCatergoryKeyboard; }

protected:
    KeyEvent(KeyCode keycode, int mods)
        : InputEvent(mods)
        , m_keycode(keycode)
    {
    }

    KeyCode m_keycode;
};

class KeyUpEvent : public KeyEvent {
public:
    KeyUpEvent(KeyCode c, int mods)
        : KeyEvent(c, mods)
    {
    }

    virtual Type type() const override { return Type::KeyUp; }
};

class KeyDownEvent : public KeyEvent {
public:
    KeyDownEvent(KeyCode c, int mods)
        : KeyEvent(c, mods)
    {
    }

    virtual Type type() const override { return Type::KeyDown; }
};

class MouseMoveEvent : public InputEvent {
public:
    MouseMoveEvent(float x, float y, int mods = 0)
        : InputEvent(mods)
    {
        m_pos.x = x;
        m_pos.y = y;
    }

    inline float x() const { return m_pos.x; }
    inline float y() const { return m_pos.y; }

    virtual Type type() const override { return Type::KeyDown; }
    virtual int catergory_flags() const override { return EventCatergoryInput | EventCatergoryMouse; }

private:
    FPosition m_pos;
};

class MouseUpEvent : public InputEvent {
public:
    MouseUpEvent(MouseButton btn, int mods = 0)
        : InputEvent(mods)
        , m_button(btn)
    {
    }

    inline MouseButton button() const { return m_button; }

    virtual Type type() const override { return Type::MouseUp; }
    virtual int catergory_flags() const override { return EventCatergoryInput | EventCatergoryMouse; }

private:
    MouseButton m_button;
};

class MouseDownEvent : public InputEvent {
public:
    MouseDownEvent(MouseButton btn, int mods = 0)
        : InputEvent(mods)
        , m_button(btn)
    {
    }

    inline MouseButton button() const { return m_button; }

    virtual Type type() const override { return Type::MouseDown; }
    virtual int catergory_flags() const override { return EventCatergoryInput | EventCatergoryMouse; }

private:
    MouseButton m_button;
};

class MouseScrollEvent : public InputEvent {
public:
    MouseScrollEvent(int offset_x, int offset_y, int mods = 0)
        : InputEvent(mods)
    {
        m_offset = { offset_x, offset_y };
    }

    inline Position offset() const { return m_offset; }

    virtual Type type() const override { return Type::MouseScroll; }
    virtual int catergory_flags() const override { return EventCatergoryInput | EventCatergoryMouse; }

private:
    Position m_offset;
};

} // namespace CrossPlatform