#include "Contact.h"
#include<iomanip>
Contact::Contact(){
	FirstName="";
	LastName="";
	PhoneNumber="";
	Address="";
	Email="";
}

Contact::Contact(string FirstName,string PhoneNumber,string LastName="",string Address="India",string Email="example@gmail.com"){
	this->FirstName=FirstName;
	this->LastName=LastName;
	this->PhoneNumber=PhoneNumber;
	this->Address=Address;
	this->Email=Email;
}

void Contact::display(int index){
    cout<<left
        <<setw(5)<<index
        <<setw(15)<<GetFirstName()
        <<setw(15)<<GetLastName()
        <<setw(15)<<GetPhoneNumber()
        <<setw(40)<<GetAddress()
        <<setw(30)<<GetEmail()<<endl;
}

Contact::~Contact()
{
    //dtor
}
