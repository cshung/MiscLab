#pragma once

#include "result.h"

class page_file_impl;

class page_file
{
public:
    page_file();
    ~page_file();
    result_t open(const char* file_name);
    result_t read_page(int page_number, void* buffer);
    result_t write_page(int page_number, void* buffer);
    result_t append_page(int* new_page_number);
    result_t close();
private:
    page_file_impl* m_impl;
};