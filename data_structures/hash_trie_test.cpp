#include "hash_trie_test.h"
#include "hash_trie.h"
#include <iostream>
using namespace std;

void hash_trie_test()
{
	hash_trie map;
	cout << map.set("ab", 10086) << endl;
	cout << map.set("ba", 12580) << endl;
	int result;
	cout << map.get("ab", &result) << endl;
	cout << result << endl;
	cout << map.get("ba", &result) << endl;
	cout << result << endl;
}