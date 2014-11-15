import java.util.HashMap;
import java.util.HashSet;
import java.util.Map;
import java.util.Set;


public class CompareService implements Runnable {
	
	String fis_a;
	String fis_b;
	HashMap<String, Integer> hash_a;
	HashMap<String, Integer> hash_b;
	Map<Float, String> results_map;
	
	public CompareService(String fis_a, String fis_b,HashMap<String, Integer> hash_a, HashMap<String, Integer> hash_b, Map<Float, String> results_map2){
		this.fis_a = fis_a;
		this.fis_b = fis_b;
		this.hash_a = hash_a;
		this.hash_b = hash_b;
		this.results_map = results_map2;
	}

	@Override
	public void run() {
//		System.out.printf("Compar %s cu %s\n", fis_a, fis_b);
//		results_map.put((float) Math.random(), fis_a+fis_b);
		int nr_cuv_a = 0;
		int nr_cuv_b = 0;
		for(Integer x : hash_a.values()){
			nr_cuv_a += x;
		}
		for(Integer x : hash_b.values()){
			nr_cuv_b += x;
		}
		
		/* Intersectam cuvintele din a si b */
		Set<String> sa =  new HashSet<String>(hash_a.keySet());
		Set<String> sb = new HashSet<String>(hash_b.keySet());
		sb.addAll(sa);
	}
}
