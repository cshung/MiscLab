#pragma once

#include "buffer.h"

class page_impl;
class page_iterator_impl;

// A small page size for easy debugging
const int PAGE_SIZE = 100;

class page_iterator
{
public:
    page_iterator(page_iterator_impl* impl);
    ~page_iterator();
    bool has_next();
    buffer next();
private:
    page_iterator_impl* m_impl;
};

class page
{
public:
    page();
    ~page();    
    uint32_t get_page_number();
    void append_key(buffer key);
    page_iterator get_keys();
private:
    page_impl* m_impl;
};