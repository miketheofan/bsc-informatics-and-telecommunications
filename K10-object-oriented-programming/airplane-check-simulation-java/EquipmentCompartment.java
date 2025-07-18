public class EquipmentCompartment extends PrivateCompartment{

	private boolean security = false;
	
	private boolean maintenance = false;
	
	public EquipmentCompartment() {

		super();

		System.out.println("Equipment Compartment created.");
	}

	public boolean isSecurity() {

		return security;
	}

	public void setSecurity(boolean security) {

		this.security = security;
	}

	public boolean isMaintenance() {

		return maintenance;
	}

	public void setMaintenance(boolean maintenance) {

		this.maintenance = maintenance;
	}
	
	public boolean ready_check() {

		if( this.security == true && this.maintenance == true )
			return true;

		return false;
	}
	
	public void Process(Employee e) {

		e.workOn(this);
	}
	
	@Override
	public String toString() {

		return "EquipmentCompartment [security=" + security + ", maintenance=" + maintenance + "] " + super.toString();
	}
 
	@Override
	public boolean equals(Object obj) {

		if (this == obj)
			return true;

		if (obj == null)
			return false;

		if (getClass() != obj.getClass())
			return false;

		EquipmentCompartment other = (EquipmentCompartment) obj;

		if (maintenance != other.maintenance)
			return false;

		if (security != other.security)
			return false;
		
		return true;
	}
}
