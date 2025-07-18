abstract public class Employee extends Object{
	
	private String name;
	
	public Employee(String name) {
		
		super();
		this.name = name;
		System.out.println("Employee created.");
	}
	
	public void setName(String name) {

		this.name=name;
	}
	
	public String getName() {

		return this.name;
	}
	
	abstract public void Report();
	abstract public void workOn(PassengerCompartment pc);	
	abstract public void workOn(EquipmentCompartment ec);	
	abstract public void workOn(CargoBay cb);

	@Override
	public String toString() {
		
		return "Employee [name=" + name + "] " + super.toString();
	}
 
}
