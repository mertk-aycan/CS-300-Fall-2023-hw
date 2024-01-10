#ifndef _STRUTILS_H
#define _STRUTILS_H

#include <iostream>
#include <string>
using namespace std;


void StripPunc(string& s);  
void StripWhite(string& s); 

string LowerString(const string& s); 
string UpperString(const string& s);

void ToLower(string& s); 
void ToUpper(string& s); 

int atoi(const string& s);           
double atof(const string& s);        
string itoa(int n);                  
string tostring(int n);              
string tostring(double d);           

#endif 
