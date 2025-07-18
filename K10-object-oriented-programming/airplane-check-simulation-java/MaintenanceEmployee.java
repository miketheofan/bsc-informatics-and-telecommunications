public class MaintenanceEmployee extends Employee{
	
	public MaintenanceEmployee(String name) {
		super(name);
		System.out.println("Maintenance Employee created.");
	}
	
	public void Report() {
		System.out.println("Εκτελώ εργασίες συντήρησης σε χώρους εμπορευμάτων και σε χώρους εξοπλισμού.Ok!");
	}
	
	public void workOn(PassengerCompartment pc) {
		System.out.println("Maintenance Employee did not work on Passenger Compartment.");
	}
	
	public void workOn(EquipmentCompartment ec) {
		ec.setMaintenance(true);
		System.out.println("Maintenance Employee worked on Equipment Compartment.");
	}
	
	public void workOn(CargoBay cb) {
		cb.setMaintenance(true);
		System.out.println("Maintenance Employee worked on Cargo Bay.");
	}
	
	public String toString() {
		
		return "Maintenance Employee , " + super.toString();
	}
}
