public class MapService implements Runnable {
	
	
	public MapService() {
		// TODO Auto-generated constructor stub
	}
	
	@Override
	public void run() {
		long threadId = Thread.currentThread().getId();
		System.out.println("Hello from thread " + threadId);
		
	}

}
