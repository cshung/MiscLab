#include "hash_trie.h"
#include <cstring>

using namespace std;

hash_trie::hash_trie()
{
	this->m_root = new trie_node();	
}

hash_trie::~hash_trie()
{
	delete this->m_root;
}

bool hash_trie::set(const char* key, int value)
{
	int hash = this->hash(key);	
	trie_node* new_root;
	bool succeed = this->insert_trie(key, value, hash, 32, this->m_root, &new_root);
	if (succeed)
	{
		// optionally, we can keep the old root for persistence
		this->m_root = (hash_trie::trie_node*)new_root;
	}
}

int hash_trie::hash(const char* key)
{
	int hash = 0;
	const char* key_cursor = key;
	while (*key_cursor != '\0')
	{
		// TODO: Improve this hash 
		hash = hash ^ (*key_cursor);
		key_cursor++;
	}

	return hash;
}

bool hash_trie::insert_trie(const char* key, int value, int hash, int num_bits, trie_node* current, trie_node** ppResult)
{
	int bit = hash & 1;
	bool succeed = false;
	if (num_bits > 1)
	{
		trie_node* result;
		if (bit == 0)
		{
			succeed = insert_trie(key, value, hash >> 1, num_bits - 1, (trie_node*)current->m_left, &result);
			if (!succeed)
			{
				return false;
			}
			*ppResult = new trie_node();
			(*ppResult)->m_left = result;
			(*ppResult)->m_right= current->m_right;
		}
		else
		{
			succeed = insert_trie(key, value, hash >> 1, num_bits - 1, (trie_node*)current->m_right, &result);
			if (!succeed)
			{
				return false;
			}
			*ppResult = new trie_node();
			(*ppResult)->m_left = result;
			(*ppResult)->m_right = current->m_right;
		}
		return true;
	}
	else
	{
		// TODO: insert bucket node here
	}
}

hash_trie::node::~node()
{

}

hash_trie::trie_node::trie_node() : m_left(nullptr), m_right(nullptr)
{

}

hash_trie::trie_node::~trie_node()
{
	if (this->m_left != nullptr)
	{
		delete this->m_left;
	}
	if (this->m_right != nullptr)
	{
		delete this->m_right;
	}
}

hash_trie::bucket_node::bucket_node(const char* key, int value)
{
	this->m_value = value;
	int key_length = strlen(key);
	this->m_key = new char[key_length + 1];
	strcpy(this->m_key, key);
	this->m_next = nullptr;
}

hash_trie::bucket_node::bucket_node()
{
	delete[] this->m_key;
}