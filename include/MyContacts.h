#ifndef MYCONTACTS_H
#define MYCONTACTS_H
#include "SQLHelper.h"
#include<vector>
#include "Contact.h"
#include "TrieNode.h"

class MyContacts
{
    public:
        MyContacts();
        virtual ~MyContacts();
        void Add(Contact c);
        void AddToTrie(TrieNode *root,string key,int C_index);
        void CreateNewContact(Contact c);
        set<int> Search(string query);
        void Remove(set<int> C_indexes);
        void DisplayAll();
        vector<Contact> GetAllContacts(){ return m_Contacts; }

    private:
        vector<Contact> m_Contacts;
        TrieNode *PreSufTree;
        //TrieNode *TriePhone;
        SQLHelper sqlhelper;
};

#endif // MYCONTACTS_H
