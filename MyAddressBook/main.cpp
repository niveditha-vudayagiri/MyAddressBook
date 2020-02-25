#include <iostream>
#include "Contact.h"
#include "MyContacts.h"
#include <windows.h>
#include<iomanip>
using namespace std;

int main()
{
    int ch,nex;
    string query,f_name,l_name,phone,addr,email;
    char confirm;
    set<int> C_indexes;
    MyContacts *Book;
    Contact *c;
    cout<<string(120, '-' )<<endl;
    cout<<string(53,' ')<<"My Contacts"<<string(53,' ')<<endl;
    cout<<string(120, '-' )<<endl;
    Book=new MyContacts;
    while(1)
    {
        cout<<string(120, '-' )<<endl;
        cout<<"1. Display all Contacts"<<endl;
        cout<<"2. Search Contact"<<endl;
        cout<<"3. Add to Contacts"<<endl;
        cout<<"4. Delete Existing Contact"<<endl;
        cout<<"5. Exit"<<endl;
        cout<<"\nMake your choice : ";
        cin>>ch;

        switch(ch)
        {
        case 1:
            Book=new MyContacts;
            Book->DisplayAll();
            break;

        case 2:
            cout<<"Enter the phone number or name to search...";
            cin>>query;
            Book->Search(query);
            break;

        case 3:
            cout<<"\nEnter the Phone Number: ";
            cin.ignore();
            getline(cin,phone);
            //Validation of phone number
            while(phone.length()!=10){
                cout<<"\nEnter Valid Phone Number (You entered "<<phone.length()<<" nos) :";
                getline(cin,phone);
            }
            C_indexes=Book->Search(phone);
            if(C_indexes.size()>0)
            {
             cout<<"\n The number you entered already exist! Do you wish to continue (Y/N) ?";
             cin>>confirm;
             if(confirm=='N'||confirm=='n')
                break;
            }
            cout<<"\n1. Create Contact ";
            cout<<"\n2. Update Existing ";
            cout<<"\nMake your choice : ";
            cin>>nex;
            cout<<"\nEnter the First Name: ";
            cin.ignore();
            getline(cin,f_name);
            while(f_name.length()>48)
            {
                cout<<"\n Enter Again( You exceeded 48 characters ) :";
                getline(cin,f_name);
            }
            cout<<"\nEnter the Last Name: ";
            getline(cin,l_name);
            while(l_name.length()>48)
            {
                cout<<"\nEnter Again( You exceeded 48 characters ) :";
                getline(cin,l_name);
            }
            if(nex==1)
            {
                cout<<"\nEnter the Address: ";
                getline(cin,addr);
                while(addr.length()>120){
                    cout<<"\nEnter again ( You exceeded 120 characters ) :";
                    getline(cin,addr);
                }
                cout<<"\nEnter the Email ID: ";
                getline(cin,email);
                while(email.length()>48){
                    cout<<"\nEnter again ( You exceeded 48 characters ) :";
                    getline(cin,email);
                }
                while(email.find(" ")!=string::npos || email.find("@")==string::npos){
                    cout<<"\nEnter again ( Incorrect Email format ) :";
                    getline(cin,email);
                }
                c=new Contact(f_name,phone,l_name,addr,email);
                if(Book->CreateNewContact(*c))
                cout<<"\nContact added Successfully!"<<endl;
            }
            else if(nex==2)
            {
                C_indexes=Book->Search(f_name+l_name);
                //Add number to existing contact
                if(C_indexes.size()==1)
                {
                    int index=*C_indexes.begin();
                    Contact existing=Book->GetContact(index);
                    c=new Contact(f_name,phone,l_name,existing.GetAddress(),existing.GetEmail());
                    if(Book->CreateNewContact(*c))
                        cout<<"\nNew Number added to Contact Successful!"<<endl;
                }

            }
            else
                cout<<"\n Incorrect Choice!";
            break;

        case 4:
            {
                cout<<"\nEnter the phone number or name to search..";
                cin>>query;
                set<int> C_indexes=Book->Search(query);
                if(C_indexes.size()>0)
                {
                   cout<<"\nDo you want to delete all(y/n)?";
                    cin>>confirm;
                    if(confirm=='y' || confirm =='Y')
                        Book->Remove(C_indexes);
                }
            }
            break;

        case 5:
        default:
            exit(0);
            break;

        }
    }
    cout<<"Bye!";
    return 0;
}
