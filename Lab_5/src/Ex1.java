import java.util.Vector;


public class Ex1 {
	public static void main(String[] args){
		int cores = Runtime.getRuntime().availableProcessors();
		Vector <Ex1_Thread> x = new Vector <Ex1_Thread>();
		for (int i = 0; i < cores; i++) {
			Ex1_Thread aux = new Ex1_Thread(i);
			x.add(aux);
		}
		
		for (int i = 0; i < x.size(); i++) {
			x.get(i).start();
		}
		
	}

}
