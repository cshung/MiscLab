page_file::page_file(const char* file_name)
{
    this->m_impl = new page_file_impl(file_name);
}

page_file::~page_file()
{
    delete this->m_impl;
}

void page_file::read_page(int page_number, void* buffer)
{
    this->m_impl->read_page(page_number, buffer);
}

void page_file::write_page(int page_number, void* buffer)
{
    this->m_impl->write_page(page_number, buffer);
}


int page_file::append_page()
{
    return this->m_impl->append_page();
}

