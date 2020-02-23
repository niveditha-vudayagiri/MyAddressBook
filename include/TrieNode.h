#ifndef TRIENODE_H
#define TRIENODE_H
#define ALPHABET_SIZE (36)
#include<set>
#include<string>
using namespace std;
class TrieNode
{
    public:
        TrieNode();
        virtual ~TrieNode();
        void insert(TrieNode *root,string key);

        TrieNode *children[ALPHABET_SIZE];
        set<int> c_index;
    private:

};

#endif // TRIENODE_H
