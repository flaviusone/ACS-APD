import java.util.ArrayList;
import java.util.concurrent.CyclicBarrier;


public class MainClass {
	
	public static void main(String[] args) throws InterruptedException {
		int[] s = {1, 2, 4, 7 , 15, 21 ,23, 28, 32, 34, 37 , 39, 40, 42, 45, 66, 103};
		int q = 0;
		int r = s.length-1;
		int NT = 4;
		
		int x = 21;
		
		int[] c = new int[NT];
		int[] pos = new int[NT];
		ArrayList<Worker> workers = new ArrayList<Worker>();
		int g = (int) Math.ceil(Math.log(s.length-1+1)/Math.log(3+1));
		CyclicBarrier b = new CyclicBarrier(NT);
		
		for (int i = 0; i < NT; i++) {
			workers.add(new Worker(i, s, c, x, b, NT, g, pos));
		}
		
		for (Worker e : workers) {
			e.start();
		}
		for (Worker e : workers) {
			e.join();
		}
		
		System.out.println("END");
		
	}

}
