#include "Citizen.hpp"

citizenID Citizen::getID(){
	return this->id;
}

string Citizen::getFirstName(){
	return this->fn;
}

string Citizen::getLastName(){
	return this->ln;
}

string Citizen::getCountry(){
	return this->c;
}

int Citizen::getAge(){
	return this->a;
}

string Citizen::getVirusName(){
	return this->vn;
}

string Citizen::getDate(){
	return this->d;
}

void Citizen::print(string info){

	cout << id << " " << fn << " " << ln << " " << c << " " << a << " " << vn << " " << info << " ";

	if(this->d != emptyDate) cout << d << endl;
	else cout << endl;
}

Citizen::Citizen(citizenID id,firstName fn,lastName ln,country c,age a,virus vn,date d)
	: id(id) , fn(fn) , ln(ln) , c(c) , a(a) , vn(vn) , d(d){}

Citizen::~Citizen(){}