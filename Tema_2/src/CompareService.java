import java.util.HashMap;
import java.util.HashSet;
import java.util.Map;
import java.util.Set;


public class CompareService implements Runnable {
	
	String fis_a;
	String fis_b;
	HashMap<String, Integer> hash_a;
	HashMap<String, Integer> hash_b;
	Map<Double, String> results_map;
	Double sum = (double) 0;
	public CompareService(String fis_a, String fis_b,HashMap<String, Integer> hash_a, HashMap<String, Integer> hash_b, Map<Double, String> results_map2){
		this.fis_a = fis_a;
		this.fis_b = fis_b;
		this.hash_a = hash_a;
		this.hash_b = hash_b;
		this.results_map = results_map2;
	}

	@Override
	public void run() {
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
		sa.addAll(sb);
		
		/* Calculam similaritate */
		Double frec_a = new Double(0);
		Double frec_b = new Double(0);
		
		for(String cuvant : sa){
			
			frec_a  = frecventa(cuvant, hash_a, nr_cuv_a);
			frec_a = frec_a * 1000 / 1000;
			frec_b  = frecventa(cuvant, hash_b, nr_cuv_b);
			frec_b = frec_b * 1000 / 1000;
			sum += frec_a * frec_b;
		}
		String files;
		if(fis_a.compareTo(fis_b) < 0){
			files = fis_a + ";" + fis_b + ";";
		}else{
			files = fis_b + ";" + fis_a + ";";
		}
//		System.out.println(files);
		results_map.put(sum/(double)100, files);
	}
	
	Double frecventa(String cuvant, HashMap<String, Integer> hash, int nr_cuv_doc){
		Double frecventa = new Double(0);
		Integer aparitii_cuvant = hash.get(cuvant);
		if (aparitii_cuvant == null) 
			aparitii_cuvant = 0;
		frecventa = ((double) aparitii_cuvant /(double) nr_cuv_doc) * 100;
		return frecventa;
	}
}
