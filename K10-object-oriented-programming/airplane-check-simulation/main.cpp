#include <iostream>
#include <string>
#include "classes.h"

using namespace std;

int main(void){

	srand (time(NULL));

	String string_for_maintenance_employee("George");
	String string_for_security_employee("Andrew");
	String string_for_cleaning_employee("Albert");
	String string_for_plane_title("BOEING40");

	Employee *maintenance_employee = new MaintenanceEmployee(string_for_maintenance_employee);
	Employee *security_employee = new SecurityEmployee(string_for_security_employee);
	Employee *cleaning_employee = new CleaningEmployee(string_for_cleaning_employee);

	Plane p(string_for_plane_title,40);

	p.Process(maintenance_employee);
	maintenance_employee->Report();
	p.Process(security_employee);
	security_employee->Report();
	p.Process(cleaning_employee);
	cleaning_employee->Report();

	if( p.ready_check() == true )
		cout << endl << "Plane ready to take off!" << endl << endl;
	else
		cout << endl << "Plane not ready to take off!" << endl << endl;

	cout << "\n\n\n-------------------------------------------------------\n\n\n";

	Plane* plane_pointer = &p;
	clone_encrypt_and_print(plane_pointer);

	delete maintenance_employee;
	delete security_employee;
	delete cleaning_employee;

	return 0;
}