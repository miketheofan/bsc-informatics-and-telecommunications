import java.util.Arrays;

public class Plane extends Object{
	
	private String title;
	private int capacity;
	private PassengerCompartment[] pc = new PassengerCompartment[4];
	private EquipmentCompartment[] ec = new EquipmentCompartment[3];
	private CargoBay cb;
	
	public Plane(String title,int capacity) {
		
		super();
		this.title=title;
		this.capacity=capacity;
		for(int i=0;i<4;i++)
			this.pc[i] = new PassengerCompartment(this.capacity/4);
		for(int i=0;i<3;i++)
			this.ec[i] = new EquipmentCompartment();
		this.cb = new CargoBay();
			
		System.out.println("Plane Created.");
	}
	
	public String getTitle() {

		return title;
	}

	public void setTitle(String title) {

		this.title = title;
	}

	public int getCapacity() {

		return capacity;
	}

	public void setCapacity(int capacity) {

		this.capacity = capacity;
	}
	
	public PassengerCompartment[] getPc() {

		return pc;
	}

	public void setPc(PassengerCompartment[] pc) {

		this.pc = pc;
	}

	public EquipmentCompartment[] getEc() {

		return ec;
	}

	public void setEc(EquipmentCompartment[] ec) {

		this.ec = ec;
	}

	public CargoBay getCb() {

		return cb;
	}

	public void setCb(CargoBay cb) {

		this.cb = cb;
	}
	
	public boolean ready_check() {
		
		boolean temp_bool = true;

		for(int i=0;i<4;i++) 
			if( this.pc[i].ready_check() == false )
				temp_bool = false;

		for(int i=0;i<3;i++)
			if( this.ec[i].ready_check() == false )
				temp_bool = false;

		if( this.cb.ready_check() == false )
			temp_bool = false;

		return temp_bool;
	}
	
	public void Process(Employee e) {

		for(int i=0;i<4;i++)
			this.pc[i].Process(e);

		for(int i=0;i<3;i++)
			this.ec[i].Process(e);

		this.cb.Process(e);
	}
 
	@Override
	public String toString() {

		return "Plane [title=" + title + ", capacity=" + capacity + ", pc=" + Arrays.toString(pc) + ", ec="
				+ Arrays.toString(ec) + ", cb=" + cb + "] " + super.toString();
	}

	@Override
	public boolean equals(Object obj) {
		if (this == obj)
			return true;

		if (obj == null)
			return false;

		if (getClass() != obj.getClass())
			return false;

		Plane other = (Plane) obj;

		if (capacity != other.capacity)
			return false;

		if (cb == null) {
			if (other.cb != null)
				return false;
		} else if (!cb.equals(other.cb))
			return false;

		if (!Arrays.equals(ec, other.ec))
			return false;

		if (!Arrays.equals(pc, other.pc))
			return false;

		if (title == null) {
			if (other.title != null)
				return false;
		} else if (!title.equals(other.title))
			return false;
			
		return true;
	}

	
	
}
