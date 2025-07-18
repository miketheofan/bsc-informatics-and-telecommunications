public class CargoBay extends PrivateCompartment{

	private boolean security = false;
	
	private boolean maintenance = false;
	
	private boolean cleaning = false;
	
	private EquipmentCompartment ec = null;
	
	public CargoBay() {

		super();
		ec = new EquipmentCompartment();
		System.out.println("Cargo Bay created.");
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

	public boolean isCleaning() {

		return cleaning;
	}

	public void setCleaning(boolean cleaning) {

		this.cleaning = cleaning;
	}

	public EquipmentCompartment getEc() {

		return ec;
	}

	public void setEc(EquipmentCompartment ec) {

		this.ec = ec;
	}
	
	public boolean ready_check() {

		if( this.security == true && this.maintenance == true && this.cleaning == true) {
			
			if( this.ec.ready_check() == true )
				return true;

			return true;
		}
		return false;
	}
	
	public void Process(Employee e) {

		e.workOn(this);

		if(ec!= null)
			e.workOn(this.ec);
	}
	
	@Override
	public String toString() {

		return "CargoBay [security=" + security + ", maintenance=" + maintenance + ", cleaning=" + cleaning + ", ec="
				+ ec + "] " + super.toString();
	} 

	@Override
	public boolean equals(Object obj) {

		if (this == obj)
			return true;

		if (obj == null)
			return false;

		if (getClass() != obj.getClass())
			return false;

		CargoBay other = (CargoBay) obj;

		if (cleaning != other.cleaning)
			return false;

		if (ec == null) {

			if (other.ec != null)
				return false;

		} else if (!ec.equals(other.ec))
			return false;

		if (maintenance != other.maintenance)
			return false;

		if (security != other.security)
			return false;
		
		return true;
	}
}
