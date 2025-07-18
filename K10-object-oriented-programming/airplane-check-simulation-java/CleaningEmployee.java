public class CleaningEmployee extends Employee{

	public CleaningEmployee(String name) {

		super(name);

		System.out.println("Cleaning Employee created.");
	}
	
	public void Report() {

		System.out.println("Εργάζομαι σε χώρους επιβατών και σε χώρους εμπορευμάτων.Ok!");
	}
	
	public void workOn(PassengerCompartment pc) {

		pc.setCleaning(true);

		System.out.println("Cleaning Employee worked on Passenger Compartment.");
	}
	
	public void workOn(EquipmentCompartment ec) {

		System.out.println("Cleaning Employee did not work on Equipment Compartment.");
	}
	
	public void workOn(CargoBay cb) {

		cb.setCleaning(true);

		System.out.println("Cleaning Employee worked on Cargo Bay.");
	}
	
	@Override
	public String toString() {

		return "CleaningEmployee [] " + super.toString();
	}

	@Override
	public boolean equals(Object obj) {

		if (this == obj)
			return true;

		if (!super.equals(obj))
			return false;

		if (getClass() != obj.getClass())
			return false;
		
		return true;
	}
}
