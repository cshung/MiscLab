#pragma once

class btree_impl;

class btree
{
public:
    btree();
    ~btree();
private:
    btree_impl* m_impl;
};
