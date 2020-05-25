#pragma once

#include <ASL/Types.h>
#include <ASL/Vector.h>
#include <LibJS/Forward.h>
#include <LibJS/Heap/Cell.h>
#include <LibJS/Heap/HeapBlock.h>

namespace JS {

class Heap {
public:
    explicit Heap(Interpreter&);
    ~Heap();

    template<typename T, typename... Args>
    T* allocate(Args&&... args)
    {
        Cell* cell = allocate_cell(sizeof(T));
        new (cell) T(forward(args)...);
        return static_cast<T*>(cell);
    }

    void collect_garbage();

private:
    Cell* allocate_cell(size_t);

    Interpreter& m_interpreter;
    Vector<NonnullOwnPtr<HeapBlock>> m_blocks;
};

} // namespace JS