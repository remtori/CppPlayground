#include <LibJS/Heap/Heap.h>

#include <ASL/LogStream.h>

namespace JS {

Heap::Heap(Interpreter& interpreter)
    : m_interpreter(interpreter)
{
}

Heap::~Heap()
{
}

Cell* Heap::allocate_cell(size_t size)
{
    Cell* cell = nullptr;
    for (auto& block : m_blocks) {
        if (block->cell_size() < size)
            continue;

        cell = block->allocate();
        if (cell)
            break;
    }

    if (cell == nullptr) {
        auto block = HeapBlock::create_with_cell_size(size);
        cell = block->allocate();
        m_blocks.append(move(block));
    }

    dbg() << "Allocated a cell with size=" << size << " at=" << (size_t)cell;

    return cell;
}

void Heap::collect_garbage()
{
}

} // namespace JS
