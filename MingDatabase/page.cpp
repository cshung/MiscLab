#include "page.h"
#include <cstring>

using namespace std;

struct page_header
{
    uint32_t page_number;
    uint32_t num_keys;
    uint32_t num_values;
    uint16_t key_tail;
    uint16_t value_tail;
};

class page_impl
{
public:
    page_impl();
    ~page_impl();
    uint32_t get_page_number();
    void append_key(buffer key);
    page_iterator get_keys();
private:
    page_header* get_page_header();
private:
    uint8_t* bytes;
};

class page_iterator_impl
{
public:
    page_iterator_impl(void* begin, int num_items);
    ~page_iterator_impl();
    bool has_next();
    buffer next();
private:
    void* m_begin;
    int m_num_items;
};

page::page()
{
    this->m_impl = new page_impl();
}

uint32_t page::get_page_number()
{
    return this->m_impl->get_page_number();
}

void page::append_key(buffer key)
{
    this->m_impl->append_key(key);
}

page_iterator page::get_keys()
{
    return this->m_impl->get_keys();
}

page::~page()
{
    delete this->m_impl;
}

page_impl::page_impl()
{
    this->bytes = new uint8_t[PAGE_SIZE];
    this->get_page_header()->num_keys = 0;
    this->get_page_header()->num_values = 0;
    this->get_page_header()->page_number = 0;
    this->get_page_header()->key_tail = sizeof(page_header);
    this->get_page_header()->value_tail = PAGE_SIZE - 1;
}

page_impl::~page_impl()
{
    delete[] this->bytes;
}

uint32_t page_impl::get_page_number()
{
    return this->get_page_header()->page_number;
}

void page_impl::append_key(buffer key)
{
    memcpy(this->bytes + this->get_page_header()->key_tail, &key.size, sizeof(uint32_t));
    this->get_page_header()->key_tail += sizeof(uint32_t);
    memcpy(this->bytes + this->get_page_header()->key_tail, key.data, key.size);
    this->get_page_header()->key_tail += key.size;
    this->get_page_header()->num_keys += 1;
}

page_iterator page_impl::get_keys()
{
    page_iterator result(nullptr);
    return result;
}

page_header* page_impl::get_page_header()
{
    return (page_header*)this->bytes;
}

page_iterator::page_iterator(page_iterator_impl* impl)
{
    this->m_impl = impl;
}

bool page_iterator::has_next()
{
    return this->m_impl->has_next();
}

buffer page_iterator::next()
{
    return this->m_impl->next();
}