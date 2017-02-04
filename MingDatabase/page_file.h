#pragma once

class page_file_impl;

class page_file
{
public:
    page_file(const char* file_name);
    ~page_file();
    void read_page(int page_number, void* buffer);
    void write_page(int page_number, void* buffer);
    int append_page();
private:
    page_file_impl* m_impl;
};