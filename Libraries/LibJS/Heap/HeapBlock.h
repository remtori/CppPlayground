#pragma once

#include <ASL/NonnullOwnPtr.h>
#include <LibJS/Heap/Cell.h>

namespace JS {

class HeapBlock {
public:
    static const size_t default_block_size = 16 * KB;

    static NonnullOwnPtr<HeapBlock> create_with_cell_size(size_t cell_size, size_t block_Size = default_block_size);

    void operator delete(void*);

    size_t cell_size() const { return m_cell_size; }
    size_t cell_count() const { return (m_block_size - sizeof(HeapBlock)) / m_cell_size; }

    Cell* cell(size_t i) { return reinterpret_cast<Cell*>(&m_storage[i * m_cell_size]); }

    template<typename Callback>
    void for_each_cell(Callback callback)
    {
        for (size_t i = 0; i < cell_count(); ++i)
            callback(cell(i));
    }

    Cell* allocate();
    void dellocate(Cell*);

private:
    HeapBlock(size_t cell_size, size_t block_size);

    struct FreeCellList : public Cell {
        FreeCellList* next;
    };

    size_t m_block_size = 0;
    size_t m_cell_size = 0;
    FreeCellList* m_free_cell_list = nullptr;
    u8 m_storage[];
};

} // namespace JS
