#include "MyContacts.h"
#include "Contact.h"
#include <set>
#include<vector>
#include <iostream>
#include <iomanip>
#include<string>
#include <algorithm>
#include<cctype>
#include <sstream>
#define CHAR_TO_INDEX(c) ((int)tolower(c) - (int)'a')
#define INT_TO_INDEX(c) c-'0'
using namespace std;



TrieNode *getNode(void)
{
    TrieNode *pNode = new TrieNode;
    pNode->isWordEnd = false;

    for (int i = 0; i < ALPHABET_SIZE; i++)
        pNode->children[i] = NULL;

    return pNode;
}

MyContacts::MyContacts()
{
    TrieName=getNode();
    TriePhone=getNode();
    //Get m_Contacts
    MYSQL_ROW row;
    MYSQL_RES *res;
    res=sqlhelper.ReadResultFromQuery("SELECT * from Contacts;");

    while(row=mysql_fetch_row(res)){
        Contact newContact(row[0],row[2],row[1],row[3],row[4]);
        Add(newContact);
    }
}

MyContacts::~MyContacts()
{

}


void AddToTrie(TrieNode *root,string key,int C_index)
{
    try
    {

    TrieNode *curr=root;
    for(int c=0;c<key.length();c++)
    {
        int index=(isalpha(key[c]))?CHAR_TO_INDEX(key[c]):INT_TO_INDEX(key[c]);
        if(!curr->children[index])
            curr->children[index]=getNode();
        curr=curr->children[index];
    }
    curr->isWordEnd=true;
    curr->c_index=C_index;
    }catch(exception &e)
    {
        cout<<"Caught";
    }
    cout<<"Adding to Trie "<<key<<"at index"<<C_index<<endl;
}

void UpdateTrie(TrieNode *root,string key)
{
    TrieNode *curr=root;
    for(int c=0;c<key.length();c++)
    {
        int index=(isalpha(key[c]))?CHAR_TO_INDEX(key[c]):INT_TO_INDEX(key[c]);
        curr=curr->children[index];
    }
    curr->c_index=curr->c_index+1;
    cout<<"Updating Trie for"<<key<<curr->c_index<<endl;
}

bool isChrono(string c1,string c2)
{
    for(int i=0;i<c1.length();i++)
        c1[i]=tolower(c1[i]);
    for(int i=0;i<c2.length();i++)
        c2[i]=tolower(c2[i]);

    if(c1 > c2)
        return true;
    else
        return false;
}

void MyContacts::CreateNewContact(Contact C)
{
    Add(C);
    stringstream ss;
    ss<<"INSERT INTO Contacts (FirstName,LastName,PhoneNumber,Address,Email) values ('"<<C.GetFirstName()
    <<"','"<<C.GetLastName()<<"','"<<C.GetPhoneNumber()<<"','"<<C.GetAddress()<<"','"<<C.GetEmail()<<"')";

    sqlhelper.ExecuteQuery(ss.str());
}

void MyContacts::Add(Contact C)
{
    try{
    //Add to m_Contacts vector using Insertion Sort

    int inserter=m_Contacts.size()-1;
    if(m_Contacts.size()==0)
        m_Contacts.push_back(C);
    else
    {
        while(inserter>=0)
        {
        string s1=m_Contacts[inserter].GetFirstName()+m_Contacts[inserter].GetLastName();
        string s2=C.GetFirstName()+C.GetLastName();
        string p=m_Contacts[inserter].GetPhoneNumber();
        if(isChrono(s1,s2))
            {
                UpdateTrie(TrieName,s1);
                UpdateTrie(TriePhone,p);
                inserter--;
            }
        else
            break;
        }
    m_Contacts.insert(m_Contacts.begin()+inserter+1,1,C);
    }

    //Insert key using name into Trie
    int C_index=inserter+1;
    string key=C.GetFirstName()+C.GetLastName();
    //while(key!="")
    //{
         AddToTrie(TrieName,key,C_index);
         //key=key.substr(1,key.length()-1);
    //}

    key=C.GetPhoneNumber();
    //while(key!="")
    //{
        AddToTrie(TriePhone,key,C_index);
        //key=key.substr(1,key.length()-1);
    //}

    }catch(const std::exception &exc)
    {
        cout<<"Exception occured while adding Contact"<< exc.what();
    }
}

void MyContacts::DisplayAll()
{
    std::vector<Contact>::iterator i;
    cout<<left
    <<setw(15)<<"First Name"
    <<setw(15)<<"Last Name"
    <<setw(20)<<"Phone Number"
    <<setw(40)<<"Address"
    <<setw(30)<<"Email";
    cout<<string(120, '-' )<<endl;
    for(i= m_Contacts.begin();i != m_Contacts.end();i++)
    {
        (*i).display();
    }
}

bool isLastNode(struct TrieNode* root)
{
    for (int i = 0; i < ALPHABET_SIZE; i++)
        if (root->children[i])
            return 0;
    return 1;
}

// Recursive function to print auto-suggestions for given
// node.
set<int> suggestionsRec(struct TrieNode* root, string currPrefix)
{
    // found a string in Trie with the given prefix
    set<int> result;
    if (root->isWordEnd)
    {
        //cout<<currPrefix<<endl;
        result.insert(root->c_index);
    }

    if (isLastNode(root))
        return result;

    for (int i = 0; i < ALPHABET_SIZE; i++)
    {
        if (root->children[i])
        {
            //To Match Prefix String
            // append current character to currPrefix string
            currPrefix.push_back('a' + i);
            // recur over the rest
            set<int> rec=suggestionsRec(root->children[i], currPrefix);
            result.insert(rec.begin(),rec.end());
            // remove last character
            currPrefix=currPrefix.substr(0,currPrefix.length()-1);

            //Match Suffix String
            // append current character to currPrefix string
            currPrefix.insert(0,1,'a' + i);
            // recur over the rest
            rec=suggestionsRec(root->children[i], currPrefix);
            result.insert(rec.begin(),rec.end());
            // remove last character
            currPrefix=currPrefix.substr(1,currPrefix.length());
        }
    }

    return result;
}

// print suggestions for given query prefix.
set<int> printAutoSuggestions(TrieNode* root, const string query)
{
    TrieNode* pCrawl = root;
    set<int> C_indexes;
    // Check if prefix is present and find the
    // the node (of last level) with last character
    // of given string.
    int level;
    int n = query.length();
    for (level = 0; level < n; level++)
    {
        int index = (isalpha(query[level]))?CHAR_TO_INDEX(query[level]):INT_TO_INDEX(query[level]);
        if(pCrawl->children[index])
            pCrawl = pCrawl->children[index];
        //else
            //return C_indexes;
    }

    // If prefix is present as a word.
    bool isWord = (pCrawl->isWordEnd == true);

    // If prefix is last node of tree (has no
    // children)
    bool isLast = isLastNode(pCrawl);

    // If prefix is present as a word, but
    // there is no subtree below the last
    // matching node.
    if (isWord && isLast)
    {
        cout << query << endl;
    }

    // If there are are nodes below last
    // matching character.
    if (!isLast)
    {
        string prefix = query;
        C_indexes=suggestionsRec(pCrawl, prefix);
    }

    return C_indexes;
}

void MyContacts::Remove(Contact c)
{

}

bool isNumber(string query)
{
    for(int i=0;i<query.length();i++)
        if(isalpha(query[0]))
            return false;

    return true;
}

void MyContacts::Search(string query)
{
    //Search the TrieName and TriePhone to get indexes
    if(!isNumber(query))
        {
        set<int> C_indexes=printAutoSuggestions(TrieName,query);
        cout<<"We have found "<<C_indexes.size()<<" results...."<<endl<<endl;
        set<int>::iterator it;
        for ( it = C_indexes.begin(); it != C_indexes.end(); it++)
            m_Contacts[(*it)].display();
        }
    else
    {
        set<int> C_indexes=printAutoSuggestions(TriePhone,query);
        cout<<"We have found "<<C_indexes.size()<<" results...."<<endl<<endl;
        set<int>::iterator it;
        for ( it = C_indexes.begin(); it != C_indexes.end(); it++)
            m_Contacts[(*it)].display();
    }
    cout<<endl;
}

