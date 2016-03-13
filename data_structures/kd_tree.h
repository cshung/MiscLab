#pragma once
#include <vector>
#include <map>
using namespace std;

class kdtree
{
public:
    kdtree(const vector<pair<int, int>>& points);
    void print() const;
private:
    class kdtree_node
    {
    public:
        int m_dimension; // 0 represent x, 1 represent y
        pair<int, int> m_point;
        kdtree_node* m_left;
        kdtree_node* m_right;
        void print(int indent) const;
    };

    class kdtree_builder
    {
    public:
        kdtree_builder(const vector<pair<int, int>>& points);

        const vector<pair<int, int>>& m_points;
        vector<int> m_buffer_1;
        vector<int> m_buffer_2;
        vector<int> m_x_sorted_points;
        vector<int> m_y_sorted_points;

        kdtree_node* build(int begin, int end, int dimension);
    };

    kdtree_node* m_root;
};
