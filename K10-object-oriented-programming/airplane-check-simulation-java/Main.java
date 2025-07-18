public class Main {

	public static void main(String[] args){

		Employee me = new MaintenanceEmployee("Andrew");
		Employee se = new SecurityEmployee("Albert");
		Employee ce = new CleaningEmployee("Stelios");
		
		me.Report();
		se.Report();
		ce.Report();
		
		Plane p = new Plane("BOEING40",400);
		
		p.Process(me);
		p.Process(se);
		p.Process(ce);
						
		if(p.ready_check() == true)
			System.out.println("Plane ready to take off!");
		else
			System.out.println("Plane not ready to take off.");
		
	}
}
