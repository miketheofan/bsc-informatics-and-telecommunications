using namespace std;

class Employee;
class MaintenanceEmployee;
class SecurityEmployee;
class CleaningEmployee;
class String;
class PlaneComponent;
class PassengerCompartment;
class PrivateCompartment;
class CargoBay;
class EquipmentCompartment;
class Plane;
class Object;

void clone_encrypt_and_print(Object*);

class Object{

protected:
	
	int id;

public:

	Object(int);

	virtual ~Object();

	virtual Object * Clone()  = 0;

	virtual String toString() const = 0;

	virtual bool equal(Object*) = 0;

	virtual bool identical(Object*) = 0;

	int getID() const;

	void setID(const int);
};

class String : public Object{

private:

	string content;

public:

	String(string);

	String(string,int);

	virtual ~String();
	
	int length() const;

	void clear();

	void concat(const String);

	char at(const int) const;

	void updateAt(const int,const char);

	void print() const;

	String toString() const;

	string getContent() const;

	void setContent(const string);

	Object * Clone() ;

	bool equal(Object*);

	bool identical(Object*);

};

class PlaneComponent : public Object{ 

public:
	
	PlaneComponent();

	PlaneComponent(int);

	virtual ~PlaneComponent();

	String toString() const;

	virtual void Process(Employee*)  = 0;

	virtual bool ready_check() const = 0;

};

class PassengerCompartment : public PlaneComponent{ 

private:

	PassengerCompartment *pc = NULL;

	int size;

	bool security = false;

	bool cleaning = false;

public:

	PassengerCompartment(int);

	PassengerCompartment(int,int,PassengerCompartment*);

	PassengerCompartment(int,bool);

	PassengerCompartment(const PassengerCompartment*);

	virtual ~PassengerCompartment();

	String toString() const;

	Object * Clone() ;

	void Process(Employee*) ;

	bool ready_check() const;

	void set_security();

	void set_cleaning();

	void set_size(const int);

	bool equal(Object*);

	bool identical(Object*);

	bool get_security() const;

	bool get_cleaning() const;

	int get_size() const;

	PassengerCompartment* get_PassengerCompartment() const;

	void set_PassengerCompartment(PassengerCompartment*);
	  	
};

class PrivateCompartment : public PlaneComponent{

public:

	PrivateCompartment();

	PrivateCompartment(int);

	virtual ~PrivateCompartment();

	String toString() const;

	Object * Clone() ;

	void Process(Employee*) ;

	bool ready_check() const;

	bool equal(Object*);

	bool identical(Object*);

};

class EquipmentCompartment : public PrivateCompartment{ 

private:

	bool security = false;

	bool maintenance = false;

public:

	EquipmentCompartment();

	EquipmentCompartment(int);

	EquipmentCompartment(const EquipmentCompartment*);

	virtual ~EquipmentCompartment();

	String toString() const;

	Object * Clone() ;

	void Process(Employee*) ;

	bool ready_check() const;

	void set_security();

	void set_maintenance();

	bool equal(Object*);

	bool identical(Object*);

	bool get_security() const;

	bool get_maintenance() const;

};

class CargoBay : public PrivateCompartment{ 

private:

	EquipmentCompartment *ec = NULL;

	bool security = false;

	bool maintenance = false;

	bool cleaning = false;

public:

	CargoBay();

	CargoBay(int,EquipmentCompartment*);

	CargoBay(const CargoBay*);

	virtual ~CargoBay();

	String toString() const;

	Object * Clone() ;

	void Process(Employee*) ;

	bool ready_check() const;

	void set_security();

	void set_maintenance();

	void set_cleaning();

	bool equal(Object*);

	bool identical(Object*);

	bool get_security() const;

	bool get_maintenance() const;

	bool get_cleaning() const;

	EquipmentCompartment* get_EquipmentCompartment() const;

	void set_EquipmentCompartment(EquipmentCompartment*);

};

class Plane : public Object{ 

private:

	CargoBay* cb;

	EquipmentCompartment* ec[3];

	PassengerCompartment* pc[4];

	String title;

	int size;

public:

	Plane(String,int);

	Plane(String,int,int,EquipmentCompartment**,CargoBay*,PassengerCompartment**);

	virtual ~Plane();

	Object * Clone() ;
	
	String toString() const;

	void Process(Employee*) const;

	bool ready_check() const;

	int get_size() const;

	String get_title() const;

	String* get_titlepointer();

	void set_size(const int);

	void set_title(const string);

	EquipmentCompartment** get_EquipmentCompartment();

	PassengerCompartment** get_PassengerCompartment();

	CargoBay* get_CargoBay() const;

	void set_CargoBay(CargoBay*);

	void set_EquipmentCompartment(EquipmentCompartment**);

	void set_PassengerCompartment(PassengerCompartment**);

	bool equal(Object*);

	bool identical(Object*);

};

class Employee : public Object{ 

protected:

	String name;

public:
  
	Employee(const Employee&);

	Employee(String);

	Employee(String,int);

	virtual ~Employee();

	String toString() const;

	virtual void workOn(CargoBay*)  = 0;

	virtual void workOn(PassengerCompartment*)  = 0;

	virtual void workOn(EquipmentCompartment*)  = 0;

	virtual void Report() const = 0;

	String get_name() const;

	String* get_namepointer();

};

class SecurityEmployee : public Employee{

public:

	SecurityEmployee(String);

	SecurityEmployee(String,int);

	virtual ~SecurityEmployee();

	Object * Clone() ;

	void Report() const;

	void workOn(CargoBay*) ;

	void workOn(PassengerCompartment*) ;

	void workOn(EquipmentCompartment*) ;

	String toString() const;

	bool equal(Object*);

	bool identical(Object*);

};

class MaintenanceEmployee : public Employee{ 

public:

	MaintenanceEmployee(String);

	MaintenanceEmployee(String,int);

	virtual ~MaintenanceEmployee();

	void Report() const;

	Object * Clone() ;

	void workOn(CargoBay*) ;

	void workOn(PassengerCompartment*) ;

	void workOn(EquipmentCompartment*) ;

	String toString() const;

	bool equal(Object*);

	bool identical(Object*);

};

class CleaningEmployee : public Employee{ 

public:

	CleaningEmployee(String);

	CleaningEmployee(String,int);

	virtual ~CleaningEmployee();

	void Report() const;

	Object * Clone() ;

	void workOn(CargoBay*) ;

	void workOn(PassengerCompartment*) ;

	void workOn(EquipmentCompartment*) ;

	String toString() const;

	bool equal(Object*);

	bool identical(Object*);

};

