#include "Citizens.hpp"
#include "countriesList.hpp"
#include "virusesList.hpp"

citizenID Citizen::getID(){
	return this->id;
}

string Citizen::getFirstName(){
	return this->fn->getContent();
}

string Citizen::getLastName(){
	return this->ln->getContent();
}

string Citizen::getCountry(){
	return this->c->getCountry();
}

int Citizen::getAge(){
	return this->a->getContent();
}

string Citizen::getVirusName(){
	return this->vn->getName();
}

string Citizen::getDate(){
	return this->d->getContent();
}

void Citizen::print(){

	cout << id << " " << fn->getContent() << " " << ln->getContent() << " " << c->getCountry() << " " << a->getContent() << " " << vn->getName() << " ";

	if(this->d != emptyDateNode) cout << d->getContent() << endl;
	else cout << endl;
}

Citizen::Citizen(citizenID id,stringNode* fn,stringNode* ln,countriesList* c,ageNode* a,virusesList* vn,stringNode* d)
	: id(id) , fn(fn) , ln(ln) , c(c) , a(a) , vn(vn) , d(d){}

Citizen::~Citizen(){}