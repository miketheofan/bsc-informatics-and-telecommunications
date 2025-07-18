abstract public class PlaneComponent extends Object{
	
	public PlaneComponent() {

		super();
		System.out.println("Plane Component created.");
	}
	
	abstract public void Process(Employee e);
	abstract public boolean ready_check();
	
	@Override
	public String toString() {
		
		return "PlaneComponent [] " + super.toString();
	}
	
}
