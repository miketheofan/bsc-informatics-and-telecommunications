import java.util.Random; 

public class PassengerCompartment extends PlaneComponent{
	
	Random rand = new Random(); 
	
	private boolean security = false;
	
	private boolean cleaning = false;
	
	private int capacity;
	
	private PassengerCompartment pc = null;
	
	public PassengerCompartment(int capacity) {

		super();
		this.capacity=capacity;

		if( rand.nextInt(2) == 0 )
			pc = new PassengerCompartment(true,this.capacity/2);

		System.out.println("Passenger Compartment created.");
	}
	
	private PassengerCompartment(boolean b,int capacity) {

		super();
		this.capacity=capacity;

		System.out.println("Passenger Compartment created.");
	}
	
	public Random getRand() {

		return rand;
	}

	public void setRand(Random rand) {

		this.rand = rand;
	}

	public boolean isSecurity() {

		return security;
	}

	public void setSecurity(boolean security) {

		this.security = security;
	}

	public boolean isCleaning() {

		return cleaning;
	}

	public void setCleaning(boolean cleaning) {

		this.cleaning = cleaning;
	}

	public PassengerCompartment getPc() {

		return pc;
	}

	public void setPc(PassengerCompartment pc) {

		this.pc = pc;
	}
	
	public boolean ready_check() {

		if( this.security == true && this.cleaning == true) {

			if( this.pc != null && this.pc.ready_check() == true)
				return true;

			return true;
		}

		return false;
	}
	
	public void Process(Employee e) {

		e.workOn(this);

		if(pc!= null)
			e.workOn(this.pc);
	}

	public int getCapacity() {

		return capacity;
	}

	public void setCapacity(int capacity) {

		this.capacity = capacity;
	}
	
	@Override
	public String toString() {

		return "PassengerCompartment [rand=" + rand + ", security=" + security + ", cleaning=" + cleaning
				+ ", capacity=" + capacity + ", pc=" + pc + "] " + super.toString();
	}
 
	@Override
	public boolean equals(Object obj) {

		if (this == obj)
			return true;

		if (!super.equals(obj))
			return false;

		if (getClass() != obj.getClass())
			return false;

		PassengerCompartment other = (PassengerCompartment) obj;

		if (capacity != other.capacity)
			return false;

		if (cleaning != other.cleaning)
			return false;

		if (pc == null) {

			if (other.pc != null)
				return false;

		} else if (!pc.equals(other.pc))
			return false;

		if (rand == null) {

			if (other.rand != null)
				return false;

		} else if (!rand.equals(other.rand))
			return false;

		if (security != other.security)
			return false;
		
		return true;
	}

}
