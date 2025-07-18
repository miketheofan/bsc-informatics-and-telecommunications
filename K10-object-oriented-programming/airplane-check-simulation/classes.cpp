#include <iostream>
#include <string>
#include "classes.h"
#include <time.h>
#include <stdlib.h>

using namespace std;

int int_for_object = 0;

void clone_encrypt_and_print(Object* object1){

	cout << "-->Entered clone_encrypt_and_print function....." << endl << endl;

	cout << "-->About to clone Object..." << endl << endl;
	Object* object2 = object1->Clone();

	String s1 = object1->toString();

	String s2 = object2->toString();

	cout << endl << "-->About to compare Object and its' clone." << endl;
	if( object1->equal(object2) ) cout << endl << "-->The two objects are equal!" << endl << endl;
	else cout << endl << "-->The two objects are not equal!" << endl << endl;
	if( object1->identical(object2) ) cout << "--The two objects are identical!" << endl << endl;
	else cout << "-->The two objects are not identical!" << endl << endl;

	cout << "-->String of Object is :" << endl << endl;
	s1.print();

	cout << endl << "-->Object's clone String is : " << endl << endl;
	s2.print();

	for(int i =0 ; i < s2.length()/10 ; i++ )
		
		s2.updateAt(rand()%s2.length(),'*');

	cout << endl << "-->Original String of Object is : " << endl << endl;
	s1.print();
	cout << endl;

	cout << endl << "-->String's clone after changing random characters is : " << endl << endl;
	s2.print();
	cout << endl;

	s1.concat(s2);

	cout << endl << "-->String after enhancing them both is : " << endl << endl;
	s1.print();

	cout << endl << "-->Length of String is : " << s1.length() << endl;

	if(s1.length()%2 == 0)
		cout << endl << "-->Middle characters of String are : " << s1.at(s1.length()/2) << s1.at(s1.length()/2+1) << endl;

	cout << endl << "-->String cleared." << endl;
	s1.clear();

	cout << endl << "-->Length of String after clearance is : " << s1.length() << endl << endl;

	delete object2;

}

/////// OBJECT ////////

Object::Object(int i = -1) : id(i){

	cout << "Object just created." << endl;
}

Object::~Object(){

	cout << "Object to be destroyed." << endl;
}

bool Object::equal(Object* o = NULL){

	if(this->id == o->getID()) return true;
	return false;
}

bool Object::identical(Object* o = NULL){

	if(this==o) return true;
	return false;
}

String Object::toString() const{

	String tempString(",ID is: ");
	tempString.setContent(to_string(id));
	tempString.setContent(" ");
	return tempString;
}

int Object::getID() const{
	return this->id;
}

void Object::setID(const int i){

	this->id = i;
}

//////// STRING ///////////

String::String(string x) : content(x) , Object(int_for_object++){

	cout << "String just created." << endl;
}

String::String(string s , int i) : content(s) , Object(i){

	cout << "String created." << endl;
}

String::~String(){

	cout << "String to be destroyed." << endl;
}

Object * String::Clone(){

	Object * s = new String(this->content,this->id);
	return s;
}

bool String::equal(Object* o = NULL){

	String* s = dynamic_cast<String*>(o);
	if(this->content == s->getContent() && this->id == s->getID()) return true;
	return false;
}

bool String::identical(Object* o = NULL){

	if(this==o) return true;
	return false;
}
	
int String::length() const{

	return this->content.length();
}

void String::clear(){

	this->content.clear();
	this->content = "";
}

void String::concat(const String s){

	this->content.append(s.content);
}

char String::at(const int i) const{

	return this->content[i];
}

void String::updateAt(const int i,const char c){

	this->content[i] = c;
}

void String::print() const{

	cout << "Content of string is : " << this->content << endl;
}

String String::toString() const{

	String tempString("String ");
	tempString.content.append(this->content);
	tempString.concat(Object::toString());
	return tempString;
}

string String::getContent() const{
	return this->content;
}

void String::setContent(const string s){

	this->content.append(s);
}

////////// PLANE /////////////

Plane::Plane(String title, int size = -1 ) : title(title) , size(size) , Object(int_for_object++){

	for(int i=0;i<3;i++){

		ec[i] = new EquipmentCompartment();

	}

	cb = new CargoBay();

	for(int i=0;i<4;i++){

		pc[i] = new PassengerCompartment(this->size/4);

	}

	cout << "Plane created." << endl;
}

Plane::Plane(String s,int size = -1,int i = -1,EquipmentCompartment** e=NULL,CargoBay* c=NULL,PassengerCompartment** p=NULL) : title(s) , size(size) , Object(i){

	for(int i=0;i<3;i++){

		this->ec[i] = new EquipmentCompartment(e[i]);
	}

	this->cb = new CargoBay(c);

	for(int i=0;i<4;i++){

		this->pc[i] = new PassengerCompartment(p[i]);
	}
	cout << "Plane created." << endl;
}

Plane::~Plane(){

	for(int i=0;i<3;i++)
		delete ec[i];
	
	for(int i=0;i<4;i++)
		delete pc[i];
	
	delete cb;
	
	cout << "Plane to be destroyed." << endl;
}

void Plane::Process(Employee* e) const{

	for(int i=0;i<3;i++){

		cout << endl << "Equipment compartment " << i << endl << endl;
		ec[i]->Process(e);
	
	}

	cout << endl << "Cargo bay" << endl << endl;
	cb->Process(e);

	for(int i=0;i<4;i++){

		cout << endl << "Passenger compartment " << i << endl << endl;
		pc[i]->Process(e);

	}

}

bool Plane::equal(Object* o){

	Plane* p = dynamic_cast<Plane*>(o);

	if(this->size==p->get_size() && this->id==p->getID() && this->title.equal(p->get_titlepointer()) && this->cb->equal(p->cb))
		
		if(this->pc[0]->equal(p->pc[0]) && this->pc[1]->equal(p->pc[1]) && this->pc[2]->equal(p->pc[2]) && this->pc[3]->equal(p->pc[3]))
			
			if(this->ec[0]->equal(p->ec[0]) && this->ec[1]->equal(p->ec[1]) && this->ec[2]->equal(p->ec[2]) )
				
				return true;

	return false;
}

bool Plane::identical(Object* o){

	if(this==o) return true;
	return false;
}

EquipmentCompartment** Plane::get_EquipmentCompartment(){

	return this->ec;
}

PassengerCompartment** Plane::get_PassengerCompartment(){

	return this->pc;
}

CargoBay* Plane::get_CargoBay() const{

	return this->cb;
}

void Plane::set_CargoBay(CargoBay* c){

	this->cb = c;
}

void Plane::set_EquipmentCompartment(EquipmentCompartment** e){

	for(int i=0;i<3;i++){

		this->ec[i] = e[i];

	}
}

void Plane::set_PassengerCompartment(PassengerCompartment** p){

	for(int i=0;i<4;i++){

		this->pc[i] = p[i];

	}
}

int Plane::get_size() const{

	return this->size;
}

String Plane::get_title() const{

	return this->title;
}

String* Plane::get_titlepointer(){

	String * sp = &title;
	return sp;
}

void Plane::set_size(const int i){

	this->size = i;
}

void Plane::set_title(const string s){

	this->title = s;
}

bool Plane::ready_check() const{

	bool ready = true;

	for(int i=0;i<3;i++){

		if(ec[i]->ready_check() == false) 
			
			ready = false;

	}

	if(cb->ready_check() == false)
		ready = false;

	for(int i=0;i<4;i++){

		if(pc[i]->ready_check() == false) 
			ready = false;
	
	}

	return ready;

}

String Plane::toString() const{

	String temp("Title is : ");
	temp.concat(title);
	temp.setContent(" , ");
	temp.setContent("max size is : ");
	temp.setContent(to_string(size));
	temp.concat(Object::toString());

	temp.setContent("\n\tInside Cargo Bay: ");
	temp.concat(cb->toString());

	temp.setContent("\n\tInside Equipment Compartments: ");
	temp.concat(ec[0]->toString());
	temp.setContent("\n\tInside Equipment Compartments: ");
	temp.concat(ec[1]->toString());
	temp.setContent("\n\tInside Equipment Compartments: ");
	temp.concat(ec[2]->toString());

	temp.setContent("\n\tInside Passenger Compartments: ");
	temp.concat(pc[0]->toString());
	temp.setContent("\n\tInside Passenger Compartments: ");
	temp.concat(pc[1]->toString());
	temp.setContent("\n\tInside Passenger Compartments: ");
	temp.concat(pc[2]->toString());
	temp.setContent("\n\tInside Passenger Compartments: ");
	temp.concat(pc[3]->toString());

	return temp;
}

Object * Plane::Clone() {

	Object * p = new Plane(this->title,this->size,this->id,this->ec,this->cb,this->pc);
	return p;
}

////////// PLANE COMPONENT ////////////////

PlaneComponent::PlaneComponent() : Object(int_for_object++){

	cout << "Plane Component created." << endl;
}

PlaneComponent::PlaneComponent(int i) : Object(i){

	cout << "Plane Component created." << endl;
}

PlaneComponent::~PlaneComponent(){

	cout << "Plane Component to be destroyed." << endl;
}

String PlaneComponent::toString() const{

	String tempString("Plane Component");
	tempString.concat(Object::toString());
	return tempString;
}

///////////////// EMPLOYEE //////////////////

Employee::Employee(const Employee& e) : name(e.get_name()){}

Employee::Employee(String name) : name(name),Object(int_for_object++){

	cout << "Employee created." << endl;
}

Employee::Employee(String s,int i) : name(s) , Object(i){

	cout << "Employee created." << endl;
}

Employee::~Employee(){

	cout << "Employee to be destroyed." << endl;
}

String Employee::toString() const{

	String temp("Employee with name : ");
	temp.concat(this->name);
	temp.concat(Object::toString());
	return temp;
}

String Employee::get_name() const{

	return this->name;
}

String* Employee::get_namepointer(){

	String * sp = &name;
	return sp;
}

////////////////// SECURITY EMPLOYEE ////////////////

SecurityEmployee::SecurityEmployee(String name) : Employee(name){


	cout << "Security Employee created." << endl;
}

SecurityEmployee::SecurityEmployee(String s,int i) : Employee(s,i){

	cout << "Security Employee created." << endl;
} 

SecurityEmployee::~SecurityEmployee(){

	cout << "Security Employee to be destroyed." << endl;
}

bool SecurityEmployee::equal(Object* o){

	SecurityEmployee* s = dynamic_cast<SecurityEmployee*>(o);
	if(this->id == o->getID() && this->name.equal(s->get_namepointer())) return true;
	return false;
}

bool SecurityEmployee::identical(Object* o){

	if(this==o) return true;
	return false;
}

void SecurityEmployee::Report() const{

		cout << "Δουλεύω σε κάθε συνιστώσα αεροπλάνου κάνοντας ελέγχους.Ok!" << endl;
}

String SecurityEmployee::toString() const{

	String temp("Security Employee,");
	temp.concat(Employee::toString());
	return temp;
}

Object * SecurityEmployee::Clone() {

	Object * se = new SecurityEmployee(this->name,this->id);
	return se;
}

void SecurityEmployee::workOn(CargoBay* pc) {

	pc->set_security();
	cout << "Security Employee worked on cargo bay." << endl;
}

void SecurityEmployee::workOn(EquipmentCompartment* pc) {

	pc->set_security();
	cout << "Security Employee worked on equipment compartment." << endl;
}

void SecurityEmployee::workOn(PassengerCompartment* pc) {

	pc->set_security();
	cout << "Security Employee worked on passenger compartment." << endl;
}

//////////////// MAINTENANCE EMPLOYEE ///////////////////

MaintenanceEmployee::MaintenanceEmployee(String name) : Employee(name){

	cout << "Maintenance Employee created." << endl;
}

MaintenanceEmployee::MaintenanceEmployee(String s,int i) : Employee(s,i){

	cout << "Maintenance Employee created." << endl;
} 

MaintenanceEmployee::~MaintenanceEmployee(){

	cout << "Maintenance Employee to be destroyed." << endl;
}

bool MaintenanceEmployee::equal(Object* o){

	MaintenanceEmployee* m = dynamic_cast<MaintenanceEmployee*>(o);
	if(this->id == o->getID() && this->name.equal(m->get_namepointer())) return true;
	return false;
}

bool MaintenanceEmployee::identical(Object* o){

	if(this==o) return true;
	return false;
}

void MaintenanceEmployee::Report() const{

		cout << "Εκτελώ εργασίες συντήρησης σε χώρους εμπορευμάτων και σε χώρους εξοπλισμού.Ok!" << endl;
}

String MaintenanceEmployee::toString() const{

	String temp("Maintenance Employee,");
	temp.concat(Employee::toString());
	return temp;
}

Object * MaintenanceEmployee::Clone() {

	Object * me = new MaintenanceEmployee(this->name,this->id);
	return me;
}

void MaintenanceEmployee::workOn(EquipmentCompartment* pc) {

	pc->set_maintenance();
	cout << "Maintenance employee worked on equipment compartment." << endl;
}

void MaintenanceEmployee::workOn(CargoBay* pc) {

	pc->set_maintenance();
	cout << "Maintenance employee worked on cargo bay." << endl;
}

void MaintenanceEmployee::workOn(PassengerCompartment* pc) {

	cout << "Maintenance employee didn't work on passenger compartment." << endl;
}

//////////////// CLEANING EMPLOYEE //////////////

CleaningEmployee::CleaningEmployee(String name) : Employee(name){

	cout << "Cleaning Employee created." << endl;
}

CleaningEmployee::CleaningEmployee(String s,int i) : Employee(s,i){

	cout << "Cleaning Employee created." << endl;
} 

CleaningEmployee::~CleaningEmployee(){

	cout << "Cleaning Employee to be destroyed." << endl;
}

bool CleaningEmployee::equal(Object* o){

	CleaningEmployee* c = dynamic_cast<CleaningEmployee*>(o);
	if(this->id == o->getID() && this->name.equal(c->get_namepointer())) return true;
	return false;
}

bool CleaningEmployee::identical(Object* o){

	if(this==o) return true;
	return false;
}

void CleaningEmployee::Report() const{

		cout << "Εργάζομαι σε χώρους επιβατών και σε χώρους εμπορευμάτων.Ok!" << endl;
}

String CleaningEmployee::toString() const{

	String temp("Cleaning Employee,");
	temp.concat(Employee::toString());
	return temp;
}

Object * CleaningEmployee::Clone() {

	Object * ce = new CleaningEmployee(this->name,this->id);
	return ce;
}

void CleaningEmployee::workOn(CargoBay* cb) {

	cb->set_cleaning();
	cout << "Cleaning Employee worked on cargo bay." << endl;
}

void CleaningEmployee::workOn(PassengerCompartment* pc) {

	pc->set_cleaning();
	cout << "Cleaning Employee worked on passenger compartment" << endl;
}

void CleaningEmployee::workOn(EquipmentCompartment* cb) {

	cout << "Cleaning Employee didn't work on equipment compartment." << endl;
}

///////////////// CARGO BAY ///////////////////

CargoBay::CargoBay(){

	ec = new EquipmentCompartment();
	cout << "Inside Equipment Compartment created." << endl;

	cout << "Cargo Bay created." << endl;
}

CargoBay::CargoBay(int i,EquipmentCompartment* e) : PrivateCompartment(i){

	this->ec = new EquipmentCompartment(e);
	cout << "Cargo Bay created." << endl;
}

CargoBay::CargoBay(const CargoBay* c) : PrivateCompartment(c->id) {

	this->ec = new EquipmentCompartment(c->ec);
	this->security=c->security;
	this->maintenance=c->maintenance;
	this->cleaning=c->cleaning;
	cout << "Cargo Bay created." << endl;
}

CargoBay::~CargoBay(){

	cout << "Cargo Bay to be destroyed." << endl;
	delete this->ec;
}

bool CargoBay::equal(Object* o){

	CargoBay* c = dynamic_cast<CargoBay*>(o);
	if(this->id == c->getID() && c->security == this->security && c->maintenance == this->maintenance && this->cleaning==c->cleaning)
		if(this->ec->equal(c->ec)) 
			return true;
	return false;
}

bool CargoBay::identical(Object* o){

	if(this==o) return true;
	return false;
}

bool CargoBay::get_security() const{

	return this->security;
}

bool CargoBay::get_maintenance() const{

	return this->maintenance;
}

bool CargoBay::get_cleaning() const{

	return this->cleaning;
}

EquipmentCompartment* CargoBay::get_EquipmentCompartment() const{

	return this->ec;
}

void CargoBay::set_EquipmentCompartment(EquipmentCompartment* e){

	this->ec = e;
}

bool CargoBay::ready_check() const{

	if(this->security==true && this->maintenance==true && this->cleaning==true){

		if(ec!=NULL){

			if(ec->ready_check() == true){

				cout << endl << "Inside Equipment Compartment ready." << endl;
				cout << endl << "Cargo Bay ready." << endl;
				return true;
			}else{

				cout << endl << "Inside Equipment Compartment not ready." << endl;
				return false;
			}

		}else{

			cout << endl << "Cargo bay ready." << endl;
			return true;
		}

	}else{

		cout << endl << "Cargo bay not ready." << endl;
		return false;
	}

}

void CargoBay::Process(Employee* e) { 

	e->workOn(this); 
	if(this->ec != NULL) {
		cout << "\tEntering equipment compartment inside cargo..." << endl;
		e->workOn(this->ec); 
	}

}

String CargoBay::toString() const{

	String temp("Cargo Bay ");
	
	if(this->cleaning==true)
		temp.setContent("bool cleaning = true ");
	else
		temp.setContent("bool cleaning = false ");

	if(this->maintenance==true)
		temp.setContent("bool maintenance = true ");
	else
		temp.setContent("bool maintenance = true ");

	if(this->security==true)
		temp.setContent("bool security = true,");
	else
		temp.setContent("bool security = false,");

	temp.concat(PrivateCompartment::toString());

	if(ec!=NULL){
		temp.setContent("\n\tExtra Equipment Compartment : ");
		temp.concat(ec->toString());
	}
	return temp;
}

Object * CargoBay::Clone() {

	Object * cb = new CargoBay(this->id,this->ec);
	return cb;
}

void CargoBay::set_security(){

	this->security = true;
}

void CargoBay::set_maintenance(){

	this->maintenance = true;
}

void CargoBay::set_cleaning(){

	this->cleaning = true;
}

//////////// EQUIPMENT COMPARTMENT /////////////////

EquipmentCompartment::EquipmentCompartment(){

	cout << "Equipment Compartment created." << endl;
}

EquipmentCompartment::EquipmentCompartment(int i) : PrivateCompartment(i){

	cout << "Equipment Compartment created." << endl;
}

EquipmentCompartment::EquipmentCompartment(const EquipmentCompartment* e) : PrivateCompartment(e->id){

	this->security=e->security;
	this->maintenance=e->maintenance;
	cout << "Equipment Compartment created." << endl;
}

EquipmentCompartment::~EquipmentCompartment(){

	cout << "Equipment Compartment to be destroyed." << endl;
}

bool EquipmentCompartment::equal(Object* o){

	EquipmentCompartment* e = dynamic_cast<EquipmentCompartment*>(o);
	if(this->id == o->getID() && e->security == this->security && e->maintenance == this->maintenance) 
		return true;
	return false;
}

bool EquipmentCompartment::identical(Object* o){

	if(this==o) return true;
	return false;
}

bool EquipmentCompartment::get_security() const{

	return this->security;
}

bool EquipmentCompartment::get_maintenance() const{

	return this->maintenance;
}

bool EquipmentCompartment::ready_check() const{

	cout << endl;

	if(this->security==true && this->maintenance==true) {
		cout << "Equipment compartment ready." << endl;
		return true;
	}
	cout << "Equipment compartment not ready." << endl;
	return false;
}

void EquipmentCompartment::Process(Employee* e) { 
	e->workOn(this);
}

String EquipmentCompartment::toString() const{

	String temp("Equipment Compartment ");

	if(security==false)
		temp.setContent("bool security = false ");
	else
		temp.setContent("bool security = true ");

	if(maintenance==false)
		temp.setContent("bool maintenance = false,");
	else
		temp.setContent("bool maintenance = true,");

	temp.concat(PrivateCompartment::toString());
	return temp;
}

Object * EquipmentCompartment::Clone() {

	Object * cb = new EquipmentCompartment(this->id);
	return cb;
}

void EquipmentCompartment::set_security(){

	this->security = true;
}

void EquipmentCompartment::set_maintenance(){

	this->maintenance = true;
}

///////////////// PASSENGER COMPARTMENT //////////////

PassengerCompartment::PassengerCompartment(int i) : size(i){

	if( rand()%2 ){
		pc = new PassengerCompartment(i/2,true);
		cout << "Inside Passenger Compartment created." << endl;
	}

	cout << "Passenger Compartment created." << endl;
}

PassengerCompartment::PassengerCompartment(int i,bool stop) : size(i){

	cout << "Passenger Compartment created." << endl;
}

PassengerCompartment::PassengerCompartment(int size,int i,PassengerCompartment* p) : size(size) , PlaneComponent(i){

	if(p!=NULL)
		this->pc = new PassengerCompartment(p);
	cout << "Passenger Compartment created." << endl;
}

PassengerCompartment::PassengerCompartment(const PassengerCompartment* p) : PlaneComponent(p->id) {

	this->size = p->size;
	this->security=p->security;
	this->cleaning=p->cleaning;
	cout << "Passenger Compartment created." << endl;
}

PassengerCompartment::~PassengerCompartment(){

	cout << "Passenger Compartment to be destroyed." << endl;
	delete this->pc;
}

bool PassengerCompartment::equal(Object* o){

	PassengerCompartment* p = dynamic_cast<PassengerCompartment*>(o);
	
	if(this->id == o->getID() && p->security == this->security && p->cleaning == this->cleaning ){
		
		if(p->pc != NULL){
			
			if(this->pc->equal( p->pc )) {
				return true;
			}
			return false;
		
		}
		return true;
	
	}
	
	return false;
}

bool PassengerCompartment::identical(Object* o){

	if(this==o) return true;
	return false;
}

bool PassengerCompartment::get_security() const{

	return this->security;
}

bool PassengerCompartment::get_cleaning() const{

	return this->cleaning;
}

PassengerCompartment* PassengerCompartment::get_PassengerCompartment() const{

	return this->pc;
}

bool PassengerCompartment::ready_check() const{

	if(this->security==true && this->cleaning==true){

		if(pc!=NULL){

			if(pc->ready_check() == true){

				cout << endl << "Inside Passenger Compartment ready." << endl;
				return true;
			}else{

				cout << endl << "Inside Passenger Compartment not ready." << endl;
				return false;
			}
		}else{

			cout << endl << "Passenger Compartment ready." << endl;
			return true;
		}
	}else{

		cout << endl << "Passenger Compartment not ready." << endl;
		return false;
	}
	
}

void PassengerCompartment::Process(Employee* e) {

	e->workOn(this);
	if(this->pc != NULL) e->workOn(this->pc);
}

String PassengerCompartment::toString() const{

	String temp("Passenger Compartment ");

	if(security==false)
		temp.setContent("bool security = false ");
	else
		temp.setContent("bool security = true ");

	if(cleaning==false)
		temp.setContent("bool cleaning = false ");
	else
		temp.setContent("bool cleaning = true ");

	temp.setContent("size is ");
	temp.setContent(to_string(size));
	temp.setContent(",");
	temp.concat(PlaneComponent::toString());
	if(pc!=NULL){
		temp.setContent("\n\tExtra Passenger Compartment : ");
		temp.concat(pc->toString());
	}
	return temp;
}

Object * PassengerCompartment::Clone() {

	Object * pc = new PassengerCompartment(this->size,this->id,this->pc);
	return pc;
}

void PassengerCompartment::set_security(){

	this->security = true;
}

void PassengerCompartment::set_cleaning(){

	this->cleaning = true;
}

int PassengerCompartment::get_size() const{

	return this->size;
}

void PassengerCompartment::set_size(const int i){

	this->size = i;
}

void PassengerCompartment::set_PassengerCompartment(PassengerCompartment* p){

	this->pc = p;
}

//////////// PRIVATE COMPARTMENT //////////////////

PrivateCompartment::PrivateCompartment(){

	cout << "Private Compartment created." << endl;
}

PrivateCompartment::PrivateCompartment(int i) : PlaneComponent(i){

	cout << "Private Compartment created." << endl;
} 

PrivateCompartment::~PrivateCompartment(){

	cout << "Private Compartment to be destroyed." << endl;
}

bool PrivateCompartment::equal(Object* o){

	PrivateCompartment* p = dynamic_cast<PrivateCompartment*>(o);
	if(this->id == o->getID()) return true;
	return false;
}

bool PrivateCompartment::identical(Object* o){

	if(this==o) return true;
	return false;
}

bool PrivateCompartment::ready_check() const{}

void PrivateCompartment::Process(Employee* e) {}

String PrivateCompartment::toString() const{

	String temp("Private Compartment,");
	temp.concat(PlaneComponent::toString());
	return temp;
}

Object * PrivateCompartment::Clone() {

	Object * pc = new PrivateCompartment(this->id);
	return pc;
}