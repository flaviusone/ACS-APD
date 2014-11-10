import java.util.HashMap;
import java.util.concurrent.ConcurrentHashMap;

public class MapService implements Runnable {
	ConcurrentHashMap<String, HashMap<String, Integer>> MapResults;
	HashMap<String, Integer> result;
	String nume_fis;
	long offset;
	int D;
	
	public MapService(String nume_fis, long offset, int D, 
		ConcurrentHashMap<String,HashMap<String, Integer>> MapResults) {
		this.nume_fis = nume_fis;
		this.offset = offset;
		this.D = D;
		this.MapResults = MapResults;
		result = new HashMap<String, Integer>();
	}
	
	@Override
	public void run() {
		
		
		MapResults.put(nume_fis, result);
	}

}
