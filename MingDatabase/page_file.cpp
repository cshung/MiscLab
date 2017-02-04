#include "page_file.h"
#include "constant.h"
#include <cstdio>
#include <errno.h>
#include <cstdint>

class page_file_impl
{
public:
    page_file_impl(const char* file_name);
    ~page_file_impl();
    void read_page(int page_number, void* buffer);
    void write_page(int page_number, void* buffer);
    int append_page(); 
private:
    FILE* m_file;
};

#include "page_file.forwarders.inl"

page_file_impl::page_file_impl(const char* file_name)
{
    this->m_file = fopen(file_name, "rb+");
    if (this->m_file == nullptr)
    {
        this->m_file = fopen(file_name, "wb+");
    }
}

page_file_impl::~page_file_impl()
{
    fclose(this->m_file);
}

void page_file_impl::read_page(int page_number, void* buffer)
{
    fseek(this->m_file, page_number * PAGE_SIZE, SEEK_SET);
    fread(buffer, PAGE_SIZE, 1, this->m_file);
}

void page_file_impl::write_page(int page_number, void* buffer)
{
    fseek(this->m_file, page_number * PAGE_SIZE, SEEK_SET);
    fwrite(buffer, PAGE_SIZE, 1, this->m_file);
}

int page_file_impl::append_page()
{
    uint8_t blank_page[PAGE_SIZE];
    fseek(this->m_file, 0, SEEK_END);
    fwrite(blank_page, PAGE_SIZE, 1, this->m_file);
    long int size = ftell(this->m_file);
    return ((int)(size / PAGE_SIZE)) - 1;
}
