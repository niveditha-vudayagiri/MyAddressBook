#ifndef TRIENODE_H
#define TRIENODE_H
#define ALPHABET_SIZE (26)

#include<string>
using namespace std;
class TrieNode
{
    public:
        TrieNode();
        virtual ~TrieNode();
        void insert(TrieNode *root,string key);
        bool isWordEnd;
        int c_index;
        TrieNode *children[ALPHABET_SIZE];
};

#endif // TRIENODE_H
