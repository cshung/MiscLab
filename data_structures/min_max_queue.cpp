#include "min_max_queue.h"

#include <deque>
#include <algorithm>
using namespace std;

struct min_max_queue_element
{
    min_max_queue_element(int min, int max, int val);
    int m_min;
    int m_max;
    int m_val;
};

class min_max_queue_impl
{
public:
    min_max_queue_impl();
    void enqueue(int val);
    bool dequeue(int* pVal);
    bool min(int* pMin) const;
    bool max(int* pMax) const;
private:
    deque<min_max_queue_element> m_deque;
    int m_stack_1_size;
    int m_stack_2_size;
};

min_max_queue::min_max_queue()
{
    this->m_impl = new min_max_queue_impl();
}

min_max_queue::~min_max_queue()
{
    delete this->m_impl;
}

void min_max_queue::enqueue(int val)
{
    this->m_impl->enqueue(val);
}

bool min_max_queue::dequeue(int* pVal)
{
    return this->m_impl->dequeue(pVal);
}

bool min_max_queue::min(int* pMin) const
{
    return this->m_impl->min(pMin);
}

bool min_max_queue::max(int* pMax) const
{
    return this->m_impl->max(pMax);
}

min_max_queue_element::min_max_queue_element(int min, int max, int val) : m_min(min), m_max(max), m_val(val)
{
}

min_max_queue_impl::min_max_queue_impl() : m_stack_1_size(0), m_stack_2_size(0)
{
}

void min_max_queue_impl::enqueue(int val)
{
    int min = val;
    int max = val;
    if (this->m_stack_1_size > 0)
    {
        min = ::min(min, this->m_deque.back().m_min);
        max = ::max(max, this->m_deque.back().m_max);
    }
    this->m_deque.push_back(min_max_queue_element(min, max, val));
    this->m_stack_1_size++;
}

bool min_max_queue_impl::dequeue(int* pVal)
{
    if (this->m_stack_1_size == 0 && this->m_stack_2_size == 0)
    {
        return false;
    }
    else
    {
        if (this->m_stack_2_size == 0)
        {
            while (this->m_stack_1_size > 0)
            {
                int val = this->m_deque.back().m_val;
                int min = val;
                int max = val;
                if (this->m_stack_2_size > 0)
                {
                    min = ::min(min, this->m_deque.front().m_min);
                    max = ::max(max, this->m_deque.front().m_max);
                }

                this->m_deque.push_front(min_max_queue_element(min, max, val));
                this->m_stack_2_size++;

                this->m_deque.pop_back();
                this->m_stack_1_size--;
            }
        }

        *pVal = this->m_deque.front().m_val;
        this->m_deque.pop_front();
        this->m_stack_2_size--;
        return true;
    }
}

bool min_max_queue_impl::min(int* pMin) const
{
    bool hasMin = false;

    if (this->m_stack_1_size > 0)
    {
        *pMin = this->m_deque.back().m_min;
        hasMin = true;
    }

    if (this->m_stack_2_size > 0)
    {
        if (hasMin)
        {
            *pMin = ::min(*pMin, this->m_deque.front().m_min);
        }
        else
        {
            *pMin = this->m_deque.front().m_min;
        }
        hasMin = true;
    }

    return hasMin;
}

bool min_max_queue_impl::max(int* pMax) const
{
    bool hasMax = false;

    if (this->m_stack_1_size > 0)
    {
        *pMax = this->m_deque.back().m_max;
        hasMax = true;
    }

    if (this->m_stack_2_size > 0)
    {
        if (hasMax)
        {
            *pMax = ::max(*pMax, this->m_deque.front().m_max);
        }
        else
        {
            *pMax = this->m_deque.front().m_max;
        }
        hasMax = true;
    }

    return hasMax;
}
