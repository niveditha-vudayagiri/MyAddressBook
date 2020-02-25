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
#define INT_TO_INDEX(c) c-'0'+27
using namespace std;



TrieNode *getNode(void)
{
    TrieNode *pNode = new TrieNode;
    return pNode;
}

MyContacts::MyContacts()
{
    PreSufTree=getNode();
    //Get m_Contacts
    MYSQL_ROW row;
    MYSQL_RES *res;
    string query="SELECT FirstName,PhoneNumber,LastName,Address,Email ";
    query+="from Contacts c JOIN phonenumbers p where c.CNo=p.CNo ";
    query+="order by FirstName,LastName ;";
    res=sqlhelper.ReadResultFromQuery(query);

    while(row=mysql_fetch_row(res)){
        Contact newContact(row[0],row[1],row[2],row[3],row[4]);
        Add(newContact);
    }
}

MyContacts::~MyContacts()
{

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

void MyContacts::AddToTrie(TrieNode *root,string key,int C_index)
{
    try
    {

    TrieNode *curr=root;
    for(int c=0;c<key.length();c++)
    {
        //Traverse through Hash-Map children
        int index = (isalpha(key[c]))?CHAR_TO_INDEX(key[c]):INT_TO_INDEX(key[c]);
        if(!curr->children[index])
            curr->children[index]=getNode();
        curr=curr->children[index];
        //Add to indices
        set<int> c_index=curr->c_index;
        c_index.insert(C_index);
        curr->c_index=c_index;
        //curr->c_index=C_index;
    }

    }catch(exception &e)
    {
        cout<<"Caught";
    }
}

bool MyContacts::CreateNewContact(Contact C)
{
    Add(C);
    stringstream ss;
    char * CNo;
    MYSQL_ROW row;
    //Check if contact exist
    ss<<"SELECT * from Contacts where FirstName='"<<C.GetFirstName()<<"' and LastName='"<<C.GetLastName()<<"'";
    MYSQL_RES *res=sqlhelper.ReadResultFromQuery(ss.str());
    ss.str("");
    if(mysql_num_rows(res)==0)
    {
        ss<<"INSERT INTO Contacts (FirstName,LastName,Address,Email) values ('"<<C.GetFirstName()
        <<"','"<<C.GetLastName()<<"','"<<C.GetAddress()<<"','"<<C.GetEmail()<<"')";
        sqlhelper.ExecuteQuery(ss.str());
        ss.str("");
    }
    ss<<"SELECT Cno from Contacts where FirstName='"<<C.GetFirstName()<<"' and LastName='"<<C.GetLastName()<<"'";
    res=sqlhelper.ReadResultFromQuery(ss.str());
    ss.str("");
    if(mysql_num_rows(res)==NULL)
    {
        cout<<"\nContact was not inserted properly"<<endl;
        return false;
    }
    while(row=mysql_fetch_row(res))
    {
        CNo=row[0];
    }
    //Insert into Phone numbers
    ss<<"INSERT INTO phonenumbers (CNo,PhoneNumber) values ("<<CNo<<",'"<<C.GetPhoneNumber()<<"')";
    sqlhelper.ExecuteQuery(ss.str());
}

void MyContacts::Add(Contact C)
{
    try{
    //Add to m_Contacts and sort alphabetically
    m_Contacts.push_back(C);

    //Insert key using name into Trie
    int C_index=m_Contacts.size()-1;
    string key=C.GetFirstName()+C.GetLastName();
    while(key!="")
    {
         AddToTrie(PreSufTree,key,C_index);
         key=key.substr(1,key.length()-1);
    }

    key=C.GetPhoneNumber();
    while(key!="")
    {
        AddToTrie(PreSufTree,key,C_index);
        key=key.substr(1,key.length()-1);
    }

    }catch(const std::exception &exc)
    {
        cout<<"Exception occured while adding Contact"<< exc.what();
    }
}

void MyContacts::DisplayAll()
{
    std::vector<Contact>::iterator i;
    int it=1;
    cout<<endl<<left
    <<setw(5)<<"S.No"
    <<setw(15)<<"First Name"
    <<setw(15)<<"Last Name"
    <<setw(15)<<"Phone Number"
    <<setw(40)<<"Address"
    <<setw(30)<<"Email";
    cout<<string(120, '-' )<<endl;
    for(i= m_Contacts.begin();i != m_Contacts.end();i++)
    {
        if((*i).GetPhoneNumber()!="")
            (*i).display(it++);
    }
}

bool isLastNode(struct TrieNode* root)
{
    for (int i = 0; i < ALPHABET_SIZE; i++)
        if (root->children[i])
            return 0;
    return 1;
}

// print suggestions for given query prefix.
set<int> printAutoSuggestions(TrieNode* root, const string query)
{
    TrieNode* pCrawl = root;
    set<int> C_indexes;
    int level,n = query.length();
    for (level = 0; level < n; level++)
    {
        int index = (isalpha(query[level]))?CHAR_TO_INDEX(query[level]):INT_TO_INDEX(query[level]);
        if(pCrawl->children[index])
            pCrawl = pCrawl->children[index];
        else
            return C_indexes;
    }
    C_indexes=pCrawl->c_index;
    return C_indexes;
}

void RemoveIndexFromTrie(TrieNode *root,const string query,int index_to_del){
    TrieNode* pCrawl=root;
    int level,n=query.length();
    for(level=0;level<n;level++)
    {
        int index = (isalpha(query[level]))?CHAR_TO_INDEX(query[level]):INT_TO_INDEX(query[level]);
        if(pCrawl->children[index])
            pCrawl = pCrawl->children[index];

        set<int> currIndexList=pCrawl->c_index;
        std::set<int>::iterator position = std::find(currIndexList.begin(), currIndexList.end(), index_to_del);
        if (position != currIndexList.end())
            currIndexList.erase(position);

        pCrawl->c_index=currIndexList;
    }
}

bool isNumber(string query)
{
    for(int i=0;i<query.length();i++)
        if(isalpha(query[0]))
            return false;

    return true;
}

set<int> MyContacts::Search(string query)
{
    set<int> C_indexes;
    //Search the TrieName and TriePhone to get indexes

    C_indexes=printAutoSuggestions(PreSufTree,query);
    cout<<"We have found "<<C_indexes.size()<<" results...."<<endl<<endl;
    set<int>::iterator it;
    int in=1;
    for ( it = C_indexes.begin(); it != C_indexes.end(); it++)
        if(m_Contacts[(*it)].GetPhoneNumber()!="")
                m_Contacts[(*it)].display(in++);


    cout<<endl;
    return C_indexes;
}

void MyContacts::Remove(set<int> C_indices)
{
    set<int>::iterator i;
    stringstream ss;
    MYSQL_RES *res;
    MYSQL_ROW row;
    char * CNo;
    for(i=C_indices.begin();i!=C_indices.end();i++)
    {
    int index_to_remove=*i;

    string FirstName=m_Contacts[index_to_remove].GetFirstName();
    string LastName=m_Contacts[index_to_remove].GetLastName();
    string name_to_remove=FirstName+LastName;
    string num_to_remove=m_Contacts[index_to_remove].GetPhoneNumber();
    //Remove from Trie
    RemoveIndexFromTrie(PreSufTree,name_to_remove,index_to_remove);
    //Remove Contact from m_Contacts
    m_Contacts[index_to_remove].Clear();

    //Update DB

    ss<<"SELECT Cno from Contacts where FirstName='"<<FirstName;
    ss<<"' and LastName='"<<LastName<<"'";
    res=sqlhelper.ReadResultFromQuery(ss.str());
    ss.str("");
    while(row=mysql_fetch_row(res))
    {
        CNo=row[0];
    }
    ss<<"DELETE from contacts where CNo = "<<CNo<<";";
    sqlhelper.ExecuteQuery(ss.str());
    ss.str("");
    ss<<"DELETE from phonenumbers where CNo ="<<CNo<<";";
    sqlhelper.ExecuteQuery(ss.str());
    }
}





