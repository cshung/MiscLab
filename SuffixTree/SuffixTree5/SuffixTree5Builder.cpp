//#include "SuffixTree5Builder.h"
//#include "SuffixTree5.h"
//#include <iostream>
//using namespace std;
//
//SuffixTree5Builder::SuffixTree5Builder() : m_lastInternalNode(nullptr)
//#ifdef _DEBUG
//    , m_extensionCount(0)
//    , m_lastInternalNodeBirthday(-1)
//#endif
//{
//
//}
//
//void SuffixTree5Builder::BuildSuffixTree(string s, SuffixTree5* suffixTree)
//{
//    for (unsigned int end = 1; end <= s.length(); end++)
//    {
//        for (unsigned int start = 0; start < end; start++)
//        {
//            suffixTree->Add(s.substr(start, end - start), this);
//        }
//    }
//}