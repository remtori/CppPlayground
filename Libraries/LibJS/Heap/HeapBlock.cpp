#include <LibJS/Heap/HeapBlock.h>

#include <ASL/Assertions.h>
#include <ASL/StdLibExtras.h>
#include <new>
#include <stdlib.h>

namespace JS {

NonnullOwnPtr<HeapBlock> HeapBlock::create_with_cell_size(size_t cell_size, size_t block_size)
{
    HeapBlock* block = static_cast<HeapBlock*>(aligned_alloc(block_size, block_size));
    ASSERT(block);

    new (block) HeapBlock(cell_size, block_size);
    return adopt_own(*block);
}

HeapBlock::HeapBlock(size_t cell_size, size_t block_size)
    : m_block_size(block_size)
    , m_cell_size(cell_size)
{
    for (size_t i = 0; i < cell_count(); ++i) {
        auto* free_cell = static_cast<FreeCellList*>(cell(i));
        free_cell->set_alive(false);
        if (i == cell_count() - 1)
            free_cell->next = nullptr;
        else
            free_cell->next = static_cast<FreeCellList*>(cell(i + 1));
    }

    m_free_cell_list = static_cast<FreeCellList*>(cell(0));
}

void HeapBlock::operator delete(void* ptr)
{
    free(ptr);
}

Cell* HeapBlock::allocate()
{
    if (!m_free_cell_list)
        return nullptr;

    return exchange(m_free_cell_list, m_free_cell_list->next);
}

void HeapBlock::dellocate(Cell* cell)
{
    ASSERT(cell->is_alive());
    ASSERT(!cell->is_visited());
    cell->~Cell();
    auto* free_cell = static_cast<FreeCellList*>(cell);
    free_cell->set_alive(false);
    free_cell->next = m_free_cell_list;
    m_free_cell_list = free_cell;
}

} // namespace JS