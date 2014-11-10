import java.util.ArrayList;

/**
 * Clasa ce reprezinta o solutie partiala pentru problema de rezolvat. Aceste
 * solutii partiale constituie task-uri care sunt introduse in workpool.
 */
class PartialSolution {
	
	int[][] matrix = new int[7][7];
	ArrayList<Integer> pasi = new ArrayList<Integer>();
	
	String cuvant = null;
	int flag = 0;
	
	PartialSolution(ArrayList<Integer> x){
		pasi = x;		
		
		//initializam matrice adiacenta
		for(int i = 0;i<7;i++){
			for(int j = 0;j<7;j++){
				matrix[i][j] = 0;
			}
		}
		matrix[0][1] = 1;	matrix[2][1] = 1;	matrix[4][2] = 1;
		matrix[0][3] = 1;	matrix[2][4] = 1;	matrix[4][3] = 1;
		matrix[0][5] = 1;	matrix[2][5] = 1;	matrix[4][6] = 1;
		matrix[1][0] = 1;	matrix[3][1] = 1;	matrix[5][0] = 1;
		matrix[1][2] = 1;	matrix[3][4] = 1;	matrix[5][1] = 1;
		matrix[1][5] = 1;	matrix[3][6] = 1;	matrix[5][2] = 1;
		matrix[5][6] = 1;	matrix[6][4] = 1;	
		matrix[6][3] = 1;	matrix[6][5] = 1;	
		
	}
	
	public String toString() {
		return pasi.toString();
	}
}

/**
 * Clasa ce reprezinta un thread worker.
 */
class Worker extends Thread {
	WorkPool wp;

	public Worker(WorkPool workpool) {
		this.wp = workpool;
	}

	/**
	 * Procesarea unei solutii partiale. Aceasta poate implica generarea unor
	 * noi solutii partiale care se adauga in workpool folosind putWork().
	 * Daca s-a ajuns la o solutie finala, aceasta va fi afisata.
	 */
	void processPartialSolution(PartialSolution ps) {
 		
		int pas;
		
		if(ps.pasi.size() == 7){
			pas = ps.pasi.get(ps.pasi.size()-1);
			
			if(ps.matrix[pas][0] == 1){
				System.out.println(ps.toString());
				return; //am terminat
			}else{
				return;
			}
		}
		
		
		pas = ps.pasi.get(ps.pasi.size()-1);
		
//		System.out.println("Debug");
		
		for(int j = 0;j<7;j++){
				if(ps.matrix[pas][j] == 1 && !ps.pasi.contains(j)){
					ArrayList<Integer> temp = new ArrayList<Integer>(ps.pasi);
					temp.add(j);
					PartialSolution ps1 = new PartialSolution(temp) ;
					wp.putWork(ps1);
				}
		}
		
	}
	
	public void run() {
		System.out.println("Thread-ul worker " + this.getName() + " a pornit...");
		while (true) {
			PartialSolution ps = wp.getWork();
			if (ps == null)
				break;
			
			processPartialSolution(ps);
		}
		System.out.println("Thread-ul worker " + this.getName() + " s-a terminat...");
	}

	
}


public class ReplicatedWorkers {

	public static void main(String args[]) {
		ArrayList<Integer> inceput = new ArrayList<Integer>();
		inceput.add(0);
		
		WorkPool workpool = new WorkPool(4);
		PartialSolution temp = new PartialSolution(inceput);
		workpool.putWork(temp);
		
		Worker worker1 = new Worker(workpool);
		Worker worker2 = new Worker(workpool);
		Worker worker3 = new Worker(workpool);
		Worker worker4 = new Worker(workpool);
		
		worker1.run();
		worker2.run();
		worker3.run();
		worker4.run();
	
		System.out.println("Muf");
	}
	
}


