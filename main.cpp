#include <iostream>
#include "Contact.h"
#include "MyContacts.h"
#include <windows.h>
#include<iomanip>

using namespace std;

int main()
{
    int ch;
    string query,f_name,l_name,phone,addr,email;
    char confirm;
    MyContacts *Book;
    Contact *c;
    cout<<string(120, '-' )<<endl;
    cout<<setw(120)<<"My Contacts"<<endl;
    cout<<string(120, '-' )<<endl;
    Book=new MyContacts;
    while(1)
    {
        cout<<string(120, '-' )<<endl;
        cout<<"1. Display all Contacts"<<endl;
        cout<<"2. Search Contact"<<endl;
        cout<<"3. Add New Contact"<<endl;
        cout<<"4. Delete Existing Contact"<<endl;
        cout<<"5. Refresh All"<<endl;
        cout<<"6. Exit"<<endl;
        cout<<"\n Make your choice : ";
        cin>>ch;

        switch(ch)
        {
        case 1:
            Book->DisplayAll();
            break;

        case 2:
            cout<<"Enter the phone number or name to search...";
            cin>>query;
            Book->Search(query);
            break;

        case 3:
            cout<<"\n Enter the First Name: ";
            cin>>f_name;
            cout<<"\n Enter the Last Name: ";
            cin>>l_name;
            cout<<"\n Enter the Phone Number: ";
            cin>>phone;
            cout<<"\n Enter the Address: ";
            cin.ignore();
            getline(cin,addr);
            cout<<"\n Enter the Email ID: ";
            cin>>email;
            c=new Contact(f_name,phone,l_name,addr,email);
            Book->CreateNewContact(*c);
            break;

        case 4:
            {
            cout<<"\n Enter the phone number or name to search..";
            cin>>query;
            set<int> C_indexes=Book->Search(query);
            cout<<"\n Do you want to delete all(y/n)?";
            cin>>confirm;
            if(confirm=='y')
                Book->Remove(C_indexes);
            }
            break;

        case 5:
            cout<<endl<<"Refreshing your Contacts"<<endl;
            Book=new MyContacts;
            break;

        case 6:
        default:
            exit(0);
            break;

        }
    }
    cout<<"Bye!";
    return 0;
}
