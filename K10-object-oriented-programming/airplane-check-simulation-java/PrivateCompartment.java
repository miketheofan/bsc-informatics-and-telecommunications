public class PrivateCompartment extends PlaneComponent{

	public PrivateCompartment() {

		super();

		System.out.println("Private Compartment created.");
	}
	
	@Override
	public String toString() {
		
		return "PrivateCompartment [] " + super.toString();
	}

	public boolean ready_check(){return true;};
	public void Process(Employee e){};

}

