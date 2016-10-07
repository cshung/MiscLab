#pragma once

class hash_trie_snapshot;

class hash_trie
{
public:
    hash_trie();
    ~hash_trie();
    bool get(const char* key, int* pValue) const;
    bool set(const char* key, int value);
    hash_trie_snapshot snapshot();
    void restore(hash_trie_snapshot& snapshot);
private:
    struct node
    {
        node();
        virtual ~node();
        void add_ref();
        void release();
        int m_ref;
    };
    struct trie_node : node
    {
        trie_node(node* left, node* right);
        virtual ~trie_node();
        node* m_left;
        node* m_right;
    };
    struct bucket_node : node
    {
        bucket_node(const char* key, int value, bucket_node* next);
        ~bucket_node();
        char* m_key;
        int m_value;
        bucket_node* m_next;
    };
    trie_node* m_root;

    int hash(const char* key) const;
    bool insert_trie(const char* key, int value, int hash, int num_bits, trie_node* current, trie_node** ppResult);
    bool insert_bucket(const char* key, int value, bucket_node* current, bucket_node** ppResult);
    friend class hash_trie_snapshot;
};

class hash_trie_snapshot
{
public:
    hash_trie_snapshot(hash_trie::trie_node* root);
    ~hash_trie_snapshot();
private:
    hash_trie::trie_node* m_root;
    friend class hash_trie;
};
