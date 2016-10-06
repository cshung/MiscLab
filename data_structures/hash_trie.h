#pragma once

class hash_trie
{
public:
	hash_trie();
	~hash_trie();
	bool get(const char* key, int* pValue);
	bool set(const char* key, int value);
private:
	struct node
	{
		virtual ~node();
	};
	struct trie_node : node
	{
		trie_node();
		virtual ~trie_node();
		node* m_left;
		node* m_right;
	};
	struct bucket_node : node
	{
		bucket_node(const char* key, int value);
		bucket_node();
		char* m_key;
		int m_value;
		bucket_node* m_next;
	};
	trie_node* m_root;

	int hash(const char* key);
	bool insert_trie(const char* key, int value, int hash, int num_bits, trie_node* current, trie_node** ppResult);
	bool insert_bucket(const char* key, int value, bucket_node* current, bucket_node** ppResult);
};
