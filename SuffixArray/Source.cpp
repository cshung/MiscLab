#include <iostream>
#include <vector>
using namespace std;

int main()
{
    // Assuming ascii alphabet
    string s = "ababaa$";
    
    vector<int> buffer1;
    vector<int> buffer2;
    vector<int> buffer3;
    vector<int> buffer4;
    vector<int> buffer5;
    vector<int> buffer6;

    buffer1.resize(s.length());
    buffer2.resize(s.length());
    buffer3.resize(s.length());
    buffer4.resize(s.length());
    buffer5.resize(256);
    buffer6.resize(s.length());

    vector<int>& order = buffer1;
    vector<int>& new_order = buffer2;

    vector<int>& classes = buffer3;
    vector<int>& new_classes = buffer2;

    vector<int>& alphabet_count = buffer5;
    vector<int>& class_count = buffer6;

    // Initialization
    for (size_t i = 0; i < 256; i++)
    {
        alphabet_count[i] = 0;
    }
    for (size_t i = 0; i < s.length(); i++)
    {
        alphabet_count[s[i]]++;
    }

    return 0;
}