#include <iostream>
#include <vector>
using namespace std;

int main()
{
    // Assuming ascii alphabet
    //           0123456
    char s[7] = "ababaa";
    size_t length = _countof(s);

    vector<int> buffer1;
    vector<int> buffer2;
    vector<int> buffer3;
    vector<int> buffer4;
    vector<int> buffer5;
    vector<int> buffer6;

    buffer1.resize(length);
    buffer2.resize(length);
    buffer3.resize(length);
    buffer4.resize(length);
    buffer5.resize(256);
    buffer6.resize(length);

    vector<int>* order = &buffer1;
    vector<int>* new_order = &buffer2;

    vector<int>* classes = &buffer3;
    vector<int>* new_classes = &buffer4;

    vector<int>& alphabet_count = buffer5;
    vector<int>& class_count = buffer6;

    // In the first initialization round, we sort all the circular shifted string of length 1, which is simply the alphabets, easy

    // Initialization
    for (size_t i = 0; i < 256; i++)
    {
        alphabet_count[i] = 0;
    }
    // Counting
    for (size_t i = 0; i < length; i++)
    {
        alphabet_count[s[i]]++;
    }
    // Running sum
    for (size_t i = 1; i < 256; i++)
    {
        alphabet_count[i] += alphabet_count[i - 1];
    }
    // Placement
    for (size_t i = length; i > 0; i--)
    {
        size_t j = i - 1;
        char c = s[j];
        alphabet_count[c]--;
        (*order)[alphabet_count[c]] = j;
    }
    // Class computation
    (*classes)[(*order)[0]] = 0;
    for (size_t i = 1; i < length; i++)
    {
        if (s[(*order)[i]] == s[(*order)[i - 1]])
        {
            (*classes)[(*order)[i]] = (*classes)[(*order)[i - 1]];
        }
        else
        {
            (*classes)[(*order)[i]] = (*classes)[(*order)[i - 1]] + 1;
        }
    }

    size_t run_length = 1;
    // In this while loop, we will sort them with increasing long cyclic shifts 
    while (run_length < length)
    {
        cout << "Beginning sorting round" << endl;
        // Initialize the counts
        for (size_t i = 0; i < length; i++)
        {
            class_count[i] = 0;
        }

        // Counting
        for (size_t i = 0; i < length; i++)
        {
            class_count[(*classes)[i]]++;
        }

        // Running sum
        for (size_t i = 1; i < length; i++)
        {
            class_count[i] += class_count[i - 1];
        }

        // Placement
        for (size_t i = length; i > 0; i--)
        {
            int j = i - 1;
            int start = ((*order)[j] + length - run_length) % length;
            int start_class = (*classes)[start];
            class_count[start_class]--;
            (*new_order)[class_count[start_class]] = start;
        }

        // Class computation
        (*new_classes)[(*new_order)[0]] = 0;
        for (size_t i = 1; i < length; i++)
        {
            int p_msb_index = (*new_order)[i - 1];
            int p_lsb_index = (p_msb_index + length + run_length) % length;
            int c_msb_index = (*new_order)[i];
            int c_lsb_index = (c_msb_index + length + run_length) % length;
            int p_msb_class = (*classes)[p_msb_index];
            int p_lsb_class = (*classes)[p_lsb_index];
            int c_msb_class = (*classes)[c_msb_index];
            int c_lsb_class = (*classes)[c_lsb_index];
            if (p_msb_class != c_msb_class)
            {
                (*new_classes)[c_msb_index] = (*new_classes)[p_msb_index] + 1;
            }
            else if (p_lsb_class != c_lsb_class)
            {
                (*new_classes)[c_msb_index] = (*new_classes)[p_msb_index] + 1;
            }
            else
            {
                (*new_classes)[c_msb_index] = (*new_classes)[p_msb_index];
            }
        }

        vector<int>* temp = new_classes;
        new_classes = classes;
        classes = temp;

        vector<int>* temp2 = new_order;
        new_order = order;
        order = temp2;

        for (size_t i = 0; i < length; i++)
        {
            cout << (*order)[i] << "\t";
            cout << "'";
            for (size_t j = 0; j < run_length * 2; j++)
            {
                int k = ((*order)[i] + j) % length;
                cout << s[k];
            }
            cout << "'\t" << (*classes)[(*order)[i]] << endl;
        }

        run_length = run_length * 2;
    }

    cout << "Final result:" << endl;
    for (size_t i = 0; i < length; i++)
    {
        cout << "'";
        for (size_t j = (*order)[i]; j < length; j++)
        {
            cout << s[j];
        }
        cout << "'" << endl;
    }

    return 0;
}