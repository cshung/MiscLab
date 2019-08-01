#include "min_max_heap.h"
#include <algorithm>
using namespace std;

min_max_heap::min_max_heap(int capacity) : m_capacity(capacity), m_size(0), m_storage(new double[capacity])
{
}

min_max_heap::~min_max_heap()
{
    if (this->m_storage != nullptr)
    {
        delete[] this->m_storage;
    }
}

int min_max_heap::get_size() const
{
    return this->m_size;
}

bool min_max_heap::try_insert(double value)
{
    if (this->m_size == this->m_capacity)
    {
        return false;
    }

    // Step 1: Place the value in the last element
    this->m_storage[this->m_size] = value;
    this->m_size++;
    int node_number = this->m_size;

    // Step 2: Bubbling the value up
    if (this->is_min_node(node_number))
    {
        this->bubble_up_min_node(node_number);
    }
    else
    {
        this->bubble_up_max_node(node_number);
    }

    return true;
}

bool min_max_heap::try_get_min(double* min_value) const
{
    if (min_value == nullptr)
    {
        return false;
    }
    else if (this->m_size == 0)
    {
        return false;
    }
    else
    {
        *min_value = this->m_storage[0];
        return true;
    }
}

bool min_max_heap::try_get_max(double* max_value) const
{
    if (max_value == nullptr)
    {
        return false;
    }
    else if (this->m_size == 0)
    {
        return false;
    }
    else
    {
        if (this->m_size == 1)
        {
            *max_value = this->m_storage[0];
        }
        else if (this->m_size == 2)
        {
            *max_value = this->m_storage[1];
        }
        else
        {
            *max_value = max(this->m_storage[1], this->m_storage[2]);
        }

        return true;
    }
}

bool min_max_heap::has_parent_node(int node_index)
{
    return node_index != 1;
}

int min_max_heap::parent_node(int node_index)
{
    return node_index / 2;
}

bool min_max_heap::is_min_node(int node_index)
{
    if (node_index == 1)
    {
        return true;
    }
    else
    {
        return !(this->is_min_node(this->parent_node(node_index)));
    }
}

void min_max_heap::bubble_up_min_node(int node_number)
{
    if (this->has_parent_node(node_number))
    {
        int max_node_number = this->parent_node(node_number);
        if (this->m_storage[max_node_number - 1] < this->m_storage[node_number - 1])
        {
            swap(this->m_storage[max_node_number - 1], this->m_storage[node_number - 1]);
            this->bubble_up_max_node(max_node_number);
        }
        else if (this->has_parent_node(max_node_number))
        {
            int min_node_number = this->parent_node(max_node_number);
            if (this->m_storage[min_node_number - 1] > this->m_storage[node_number - 1])
            {
                swap(this->m_storage[min_node_number - 1], this->m_storage[node_number - 1]);
                this->bubble_up_min_node(min_node_number);
            }
        }
    }
}

void min_max_heap::bubble_up_max_node(int node_number)
{
    if (this->has_parent_node(node_number))
    {
        int min_node_number = this->parent_node(node_number);
        if (this->m_storage[min_node_number - 1] > this->m_storage[node_number - 1])
        {
            swap(this->m_storage[min_node_number - 1], this->m_storage[node_number - 1]);
            this->bubble_up_min_node(min_node_number);
        }
        else if (this->has_parent_node(min_node_number))
        {
            int max_node_number = this->parent_node(min_node_number);
            if (this->m_storage[max_node_number - 1] < this->m_storage[node_number - 1])
            {
                swap(this->m_storage[max_node_number - 1], this->m_storage[node_number - 1]);
                this->bubble_up_max_node(max_node_number);
            }
        }
    }
}

bool min_max_heap::try_delete_min(double* min_value)
{
    if (min_value == nullptr)
    {
        return false;
    }
    else if (this->m_size == 0)
    {
        return false;
    }
    else
    {
        *min_value = this->m_storage[0];
        this->m_size--;
        this->m_storage[0] = this->m_storage[this->m_size];
        this->bubble_down_min_node(1);
        return true;
    }
}

void min_max_heap::bubble_down_min_node(int node_number)
{
    double min_value = this->m_storage[node_number - 1];
    bool is_next_node_type_max_node = true;
    int next_node_number = node_number;

    for (int i = 0; i < 2; i++)
    {
        int candidate_node_number = node_number * 2 + i;
        if (candidate_node_number > this->m_size)
        {
            break;
        }
        else
        {
            double candidate_value = this->m_storage[candidate_node_number - 1];
            if (candidate_value < min_value)
            {
                min_value = candidate_value;
                next_node_number = candidate_node_number;
            }
        }
    }

    for (int i = 0; i < 4; i++)
    {
        int candidate_node_number = node_number * 4 + i;
        if (candidate_node_number > this->m_size)
        {
            break;
        }
        else
        {
            double candidate_value = this->m_storage[candidate_node_number - 1];
            if (candidate_value < min_value)
            {
                min_value = candidate_value;
                next_node_number = candidate_node_number;
                is_next_node_type_max_node = false;
            }
        }
    }
    if (next_node_number != node_number)
    {
        swap(this->m_storage[node_number - 1], this->m_storage[next_node_number - 1]);
        if (is_next_node_type_max_node)
        {
            this->bubble_down_max_node(next_node_number);
        }
        else
        {
            this->bubble_down_min_node(next_node_number);
        }
    }
}

bool min_max_heap::try_delete_max(double* max_value)
{
    if (max_value == nullptr)
    {
        return false;
    }
    else if (this->m_size == 0)
    {
        return false;
    }
    else if (this->m_size == 1)
    {
        *max_value = this->m_storage[0];
        this->m_size--;
        return true;
    }
    else if (this->m_size == 2)
    {
        *max_value = this->m_storage[1];
        this->m_size--;
        return true;
    }
    else
    {
        int max_node_number = (this->m_storage[1] > this->m_storage[2]) ? 2 : 3;
        this->m_size--;
        *max_value = this->m_storage[max_node_number - 1];
        this->m_storage[max_node_number - 1] = this->m_storage[this->m_size];
        this->bubble_down_max_node(max_node_number);
        return true;
    }
}

void min_max_heap::bubble_down_max_node(int node_number)
{
    double max_value = this->m_storage[node_number - 1];
    bool is_next_node_type_min_node = true;
    int next_node_number = node_number;
    for (int i = 0; i < 2; i++)
    {
        int candidate_node_number = node_number * 2 + i;
        if (candidate_node_number > this->m_size)
        {
            break;
        }
        else
        {
            double candidate_value = this->m_storage[candidate_node_number - 1];
            if (candidate_value > max_value)
            {
                max_value = candidate_value;
                next_node_number = candidate_node_number;
            }
        }
    }

    for (int i = 0; i < 4; i++)
    {
        int candidate_node_number = node_number * 4 + i;
        if (candidate_node_number > this->m_size)
        {
            break;
        }
        else
        {
            double candidate_value = this->m_storage[candidate_node_number - 1];
            if (candidate_value > max_value)
            {
                max_value = candidate_value;
                next_node_number = candidate_node_number;
                is_next_node_type_min_node = false;
            }
        }
    }
    if (next_node_number != node_number)
    {
        swap(this->m_storage[node_number - 1], this->m_storage[next_node_number - 1]);
        if (is_next_node_type_min_node)
        {
            this->bubble_down_min_node(next_node_number);
        }
        else
        {
            this->bubble_down_max_node(next_node_number);
        }
    }
}

bool min_max_heap::verify_consistency()
{
    if (this->m_size == 0)
    {
        return true;
    }

    return this->verify_consistency(1, (1 << 31), ~(1 << 31));
}

bool min_max_heap::verify_consistency(int node_number, double minimum, double maximum)
{
    double this_value = this->m_storage[node_number - 1];
    if (this_value < minimum)
    {
        return false;
    }
    if (this_value > maximum)
    {
        return false;
    }

    if (this->is_min_node(node_number))
    {
        minimum = this_value;
    }
    else
    {
        maximum = this_value;
    }

    int left_child = node_number * 2;
    int right_child = left_child + 1;
    bool result = true;
    if (left_child <= this->m_size)
    {
        result = result && verify_consistency(left_child, minimum, maximum);
    }
    if (result && right_child <= this->m_size)
    {
        result = result && verify_consistency(right_child, minimum, maximum);
    }

    return result;
}
