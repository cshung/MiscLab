#pragma once

#include <string>
#include "SuffixTree3.h"
using namespace std;

class SuffixTree3Builder
{
public:
    SuffixTree3Builder(string& input);
    void BuildSuffixTree(SuffixTree3* suffixTree);
private:
    string& m_input;
};