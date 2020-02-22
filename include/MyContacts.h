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
        void CreateNewContact(Contact c);
        void Search(string query);
        void Remove(Contact c);
        void DisplayAll();

    private:
        vector<Contact> m_Contacts;
        TrieNode *TrieName;
        TrieNode *TriePhone;
        SQLHelper sqlhelper;
};

#endif // MYCONTACTS_H
