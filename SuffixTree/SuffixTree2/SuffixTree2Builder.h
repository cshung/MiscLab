#pragma once

#include <string>
#include "SuffixTree2.h"
using namespace std;

class SuffixTree2Builder
{
public:
    SuffixTree2Builder(string& input);
    void BuildSuffixTree(SuffixTree2* suffixTree);
private:
    string& m_input;
};