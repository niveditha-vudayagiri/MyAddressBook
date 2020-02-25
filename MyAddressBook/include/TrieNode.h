#ifndef TRIENODE_H
#define TRIENODE_H
#define ALPHABET_SIZE (36)
#include<set>
#include<string>
#include<tr1/unordered_map>
using namespace std;
using namespace tr1;
class TrieNode
{
    public:
        TrieNode();
        virtual ~TrieNode();
        void insert(TrieNode *root,string key);

        unordered_map<int,TrieNode*> children;
        set<int> c_index;
    private:

};

#endif // TRIENODE_H
