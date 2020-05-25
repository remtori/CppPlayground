#pragma once

namespace JS {

class Cell {
public:
    virtual ~Cell() {}

    virtual void visit()
    {
        set_visited(true);
    }

    bool is_alive() const { return m_alive; }
    bool is_visited() const { return m_visited; }

    void set_alive(bool alive) { m_alive = alive; }
    void set_visited(bool visited) { m_visited = visited; }

private:
    bool m_alive = true;
    bool m_visited = false;
};

} // namespace JS
