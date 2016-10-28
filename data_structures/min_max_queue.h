#pragma once

class min_max_queue_impl;

class min_max_queue
{
public:
    min_max_queue();
    ~min_max_queue();
    void enqueue(int val);
    bool dequeue(int* pVal);
    bool min(int* pMin) const;
    bool max(int* pMax) const;
private:
    min_max_queue_impl* m_impl;
};
