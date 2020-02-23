#ifndef CONTACT_H
#define CONTACT_H
#include "SQLHelper.h"
#include<string>
#include<iostream>
using namespace std;

class Contact{

string FirstName;
string LastName;
string PhoneNumber;
string Address;
string Email;

public:
//Default  Constructor
Contact();

//Parameterised Constructors
Contact(string FirstName,string PhoneNumber,string LastName,string Address,string Email);

string GetPhoneNumber() { return PhoneNumber; }

string GetFirstName() { return FirstName; }

string GetLastName() { return LastName; }

string GetAddress() { return Address; }

string GetEmail() { return Email;}

void Clear() { PhoneNumber="", FirstName="", LastName="", Address="", Email="" ;}

void display();

virtual ~Contact();
};

#endif // CONTACT_H
