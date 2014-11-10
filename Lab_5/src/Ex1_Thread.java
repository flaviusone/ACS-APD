
public class Ex1_Thread extends Thread{
	int id;
	public Ex1_Thread(int id){
		this.id = id;
	}
	
	public void run(){
		System.out.println("Running from thread " + this.id);
	}

}
