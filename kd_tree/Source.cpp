#include <algorithm>
#include <iostream>
#include <vector>
#include <map>
#include <cassert>

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

kdtree::kdtree(const vector<pair<int, int>>& points)
{
    this->m_root = kdtree_builder(points).build(0, points.size(), 0);    
}

kdtree::kdtree_builder::kdtree_builder(const vector<pair<int, int>>& points) : m_points(points)
{
    this->m_buffer_1.resize(points.size());
    this->m_buffer_2.resize(points.size());
    this->m_x_sorted_points.resize(points.size());
    this->m_y_sorted_points.resize(points.size());
    for (size_t i = 0; i < points.size(); i++)
    {
        this->m_x_sorted_points[i] = i;
        this->m_y_sorted_points[i] = i;
    }
    sort(this->m_x_sorted_points.begin(), this->m_x_sorted_points.end(), [&](int p1, int p2) { 
        return points[p1].first < points[p2].first; });
    sort(this->m_y_sorted_points.begin(), this->m_y_sorted_points.end(), [&](int p1, int p2) { return points[p1].second < points[p2].second; });
}

kdtree::kdtree_node* kdtree::kdtree_builder::build(int begin, int end, int dimension)
{
    // If there is no points to process, then just return null on the end of the list
    if (begin == end)
    {
        return nullptr;
    }
    int mid = begin + (end - begin) / 2;

    vector<int>& partitioning_dimension = dimension == 0 ? this->m_x_sorted_points : this->m_y_sorted_points;
    vector<int>& rearranging_dimension = dimension == 0 ? this->m_y_sorted_points : this->m_x_sorted_points;

    // Step 1: this->m_buffer_1 can be used to quickly tell if a point is on the left (1) mid (0) or right (-1)
    for (int i = begin; i < mid; i++) { this->m_buffer_1[partitioning_dimension[i]] = 1; }
    this->m_buffer_1[partitioning_dimension[mid]] = 0;
    for (int i = mid + 1; i < end; i++) { this->m_buffer_1[partitioning_dimension[i]] = -1; }

    // Step 2: fill this->m_buffer_2[begin, mid) with the points on the left
    int j = begin;
    for (int i = begin; i < end; i++)
    {
        if (this->m_buffer_1[rearranging_dimension[i]] == 1)
        {
            this->m_buffer_2[j++] = rearranging_dimension[i];
        }
    }

    // Step 3: fill this->m_buffer_2[mid] with the points on the mid
    this->m_buffer_2[j++] = partitioning_dimension[mid];

    // Step 4: fill this->m_buffer_2[mid + 1, end) with the points on the right
    for (int i = begin; i < end; i++)
    {
        if (this->m_buffer_1[rearranging_dimension[i]] == -1)
        {
            this->m_buffer_2[j++] = rearranging_dimension[i];
        }
    }

    // Step 5: Copy this->m_buffer_2[begin, end) to rearranging_dimension[begin, end)
    for (int i = begin; i < end; i++)
    {
        rearranging_dimension[i] = this->m_buffer_2[i];
    }

    kdtree_node* result = new kdtree_node();
    result->m_dimension = dimension;
    result->m_point = this->m_points[this->m_x_sorted_points[mid]];
    result->m_left = this->build(begin, mid, 1 - dimension);
    result->m_right = this->build(mid + 1, end, 1 - dimension);
    return result;
}

void kdtree::print() const
{
    if (this->m_root != nullptr)
    {
        this->m_root->print(0);
    }
}

void kdtree::kdtree_node::print(int indent) const
{
    for (int i = 0; i < indent; i++)
    {
        cout << " ";
    }
    if (this->m_dimension == 0)
    {
        cout << "| ";
    }
    else
    {
        cout << "- ";
    }
    cout << this->m_point.first << ", " << this->m_point.second << endl;
    if (this->m_left != nullptr) { this->m_left->print(indent + 2); }
    if (this->m_right != nullptr) { this->m_right->print(indent + 2); }
}

int main(int argc, char** argv)
{
    vector<pair<int, int>> points;
    points.push_back(pair<int, int>(1, 7));
    points.push_back(pair<int, int>(2, 6));
    points.push_back(pair<int, int>(3, 5));
    points.push_back(pair<int, int>(4, 4));
    points.push_back(pair<int, int>(5, 3));
    points.push_back(pair<int, int>(6, 2));
    points.push_back(pair<int, int>(7, 1));
    kdtree tree(points);
    tree.print();
    return 0;
}