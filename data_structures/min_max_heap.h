#pragma once

class min_max_heap
{
public:
    min_max_heap(int capacity);
    ~min_max_heap();
    int get_size() const;
    bool try_insert(double value);
    bool try_get_min(double* min_value) const;
    bool try_get_max(double* max_value) const;
    bool try_delete_min(double* min_value);
    bool try_delete_max(double* max_value);
private:
    int m_capacity;
    int m_size;
    double* m_storage;

    bool is_min_node(int node_number);
    bool has_parent_node(int node_number);
    int parent_node(int node_number);
    void bubble_up_min_node(int node_number);
    void bubble_up_max_node(int node_number);
    void bubble_down_min_node(int node_number);
    void bubble_down_max_node(int node_number);
};
