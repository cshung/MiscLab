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
	if (!succeed)
	{
		return false;
	}
	// optionally, we can keep the old root for persistence
	this->m_root = (hash_trie::trie_node*)new_root;
	return true;
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
	node* result;
	trie_node* trie_result;
	bucket_node* bucket_result;
	trie_node* left_trie_node = (trie_node*)((current == nullptr) ? nullptr : current->m_left);
	trie_node* right_trie_node = (trie_node*)((current == nullptr) ? nullptr : current->m_right);
	bucket_node* left_bucket_node = (bucket_node*)((current == nullptr) ? nullptr : current->m_left);
	bucket_node* right_bucket_node = (bucket_node*)((current == nullptr) ? nullptr : current->m_right);
	if (bit == 0)
	{
		if (num_bits > 1)
		{
			succeed = insert_trie(key, value, hash >> 1, num_bits - 1, left_trie_node, &trie_result);
			result = trie_result;
		}
		else
		{
			succeed = insert_bucket(key, value, left_bucket_node, &bucket_result);
			result = bucket_result;
		}
		if (!succeed)
		{
			return false;
		}
		*ppResult = new trie_node();
		(*ppResult)->m_left = result;
		(*ppResult)->m_right= right_trie_node;
	}
	else
	{
		if (num_bits > 1)
		{
			succeed = insert_trie(key, value, hash >> 1, num_bits - 1, right_trie_node, &trie_result);
			result = trie_result;
		}
		else
		{
			succeed = insert_bucket(key, value, right_bucket_node, &bucket_result);
			result = bucket_result;
		}
		if (!succeed)
		{
			return false;
		}
		*ppResult = new trie_node();
		(*ppResult)->m_left = left_trie_node;
		(*ppResult)->m_right = result;
	}

	return true;
}

bool hash_trie::insert_bucket(const char* key, int value, bucket_node* current, bucket_node** ppResult)
{
	if (current == nullptr)
	{
		*ppResult = new bucket_node(key, value);
		return true;
	}
	else if (strcmp(current->m_key, key) == 0)
	{
		return false;
	}
	else
	{
		bucket_node* result;
		bool succeed = this->insert_bucket(key, value, current->m_next, &result);
		if (!succeed)
		{
			return false;
		}
		*ppResult = new bucket_node(current->m_key, value);
		(*ppResult)->m_next = result;
	}
	return true;
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