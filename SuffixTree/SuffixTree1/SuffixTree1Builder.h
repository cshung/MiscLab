#pragma once

#include <string>
#include "SuffixTree1.h"
using namespace std;

class SuffixTree1Builder
{
public:
    SuffixTree1Builder(string& input);
    void BuildSuffixTree(SuffixTree1* suffixTree);
private:
    string& m_input;
};