import java.sql.Time;
import java.util.concurrent.BrokenBarrierException;
import java.util.concurrent.CyclicBarrier;


public class Worker extends Thread{
	int id;
	int q;
	int r;
	int[] s;
	int[] c;
	int x; 
	CyclicBarrier b;
	int NT;
	int g;
	int[] pos_vect;
	
	public Worker(int id, int[] s, int[] c, int x, CyclicBarrier b, int NT, int g, int[] pos ){
		this.id = id;
		this.s = s;
		this.c = c;
		this.x = x;
		this.NT = NT;
		this.b = b;		
		this.g = g;
		this.pos_vect = pos;
		
		this.q = 0;
		this.r = s.length-1;
	}
	
	public void run(){
		int pos;
		
		for (int i = 0; i < g; i++) {
//			System.out.println("Afisez r si q " +r +" " + q);
			pos = (int) (q + Math.ceil((float)((float)(r - q)/(float)(NT + 1)) * (id+1)));
//			System.out.println("pos este " + pos);
			if(s[pos] > x)
			{
				// Stanga
				c[id] = -1;
				pos_vect[id] = pos;
			}
			else if (s[pos] < x)
			{
				// Dreapta
				c[id] = 1;
				pos_vect[id] = pos;
			}
			else
			{
				// Am gasit
				c[id] = 0;
				pos_vect[id] = pos;
				System.out.println("Tread "+id+" am gasit x pe poz "+pos);
			}
			
			try {
				b.await();
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			} catch (BrokenBarrierException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			 
//			for (int j = 0; j < c.length; j++) {
//				System.out.print(c[j]+" ");
//			}
//			System.out.println(c[0]+" "+c[1]+" "+c[2]+" "+c[3]+" ");
//				
	
			
			int sum = 0;
			for (int j = 1; j < c.length; j++) {
				sum += c[j];
			}
			
			/* Tratam toate stanga */
			int ok = 0;
			if(sum == -NT){
				System.out.println("Toate stanga");
				r = pos_vect[0];
				ok = 1;
			}
			/* Tratam toate dreapta */
			if(sum == NT){
				System.out.println("Toate dreapta");
				r = pos_vect[NT-1];
				ok = 1;
			}
			
			if(ok == 0){
				int new_q = 0;
				int new_r = NT-1;
				for (int j = 1; j < c.length; j++) {
					if(c[j] == 0) return;
					
					if(c[j] != c[new_q]){
//						System.out.println("NU INTRA");
						new_r = j;
						break;
					}
						
					new_q = j;
	//				System.out.println("Noul q si r " + new_q + " " + new_r);
				}
//				System.out.println(new_r);
				
				q = pos_vect[new_q];
				r = pos_vect[new_r];
				System.out.println("Noul q si r " + q + " " + r);
				
				try {
					Thread.sleep(1000);
				} catch (InterruptedException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
			}
			
		}
		
	}

}
