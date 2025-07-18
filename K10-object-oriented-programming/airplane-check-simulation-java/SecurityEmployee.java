public class SecurityEmployee extends Employee{
	
	public SecurityEmployee(String name) {

		super(name);
		System.out.println("Security Employee created.");
	}
	
	public void Report() {

		System.out.println("Δουλεύω σε κάθε συνιστώσα του αεροπλάνου κάνοντας ελέγχους.Ok!");
	}
	
	public void workOn(PassengerCompartment pc) {

		pc.setSecurity(true);
		System.out.println("Security Employee worked on Passenger Compartment.");
	}
	
	public void workOn(EquipmentCompartment ec) {

		ec.setSecurity(true);
		System.out.println("Security Employee worked on Equipment Compartment.");
	}
	
	public void workOn(CargoBay cb) {

		cb.setSecurity(true);
		System.out.println("Security Employee worked on Cargo Bay.");
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

	@Override
	public String toString() {
		
		return "SecurityEmployee [] " + super.toString();
	}
	
}
