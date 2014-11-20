import java.math.BigDecimal;
import java.math.RoundingMode;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Map;
import java.util.Set;

/*
 * @author Flavius
 * 
 * Nume: Flavius-Costin Tirnacop 331CA
 * E-mail: flavius.tirnacop@cti.pub.ro
 * 
 */

public class CompareService implements Runnable {
	
	String fis_a;
	String fis_b;
	HashMap<String, Integer> hash_a;
	HashMap<String, Integer> hash_b;
	Map<String, BigDecimal> results_map;
	Float sum = (float) 0;
	
	public CompareService(String fis_a, String fis_b,HashMap<String, Integer> hash_a, HashMap<String, Integer> hash_b, Map<String, BigDecimal> results_map2){
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
//		BigDecimal sum_precise = new BigDecimal("0");
		
		for(Integer x : hash_a.values()){
			nr_cuv_a += x;
		}
		for(Integer x : hash_b.values()){
			nr_cuv_b += x;
		}
		
		/* Intersectam cuvintele din a si b */
		Set<String> sa =  new HashSet<String>(hash_a.keySet());
		Set<String> sb = new HashSet<String>(hash_b.keySet());
		sa.retainAll(sb);
		
		/* Calculam similaritate */
		Float frec_a = new Float(0);
		Float frec_b = new Float(0);
		
//		BigDecimal frec_a_pr = new BigDecimal("0");
//		BigDecimal frec_b_pr = new BigDecimal("0");
		sum = (float) 0;
		for(String cuvant : sa){
			frec_a  = frecventa(cuvant, hash_a, nr_cuv_a);
			frec_b  = frecventa(cuvant, hash_b, nr_cuv_b);
			sum += frec_a * frec_b;

//			frec_a_pr  = frecventa_precise(cuvant, hash_a, nr_cuv_a);
//			frec_b_pr  = frecventa_precise(cuvant, hash_b, nr_cuv_b);
//			BigDecimal partial_sum = frec_a_pr.multiply(frec_b_pr);
//			sum_precise = sum_precise.add(partial_sum);					
		}
		String files = fis_a + ";" + fis_b + ";";

//		sum_precise = sum_precise.divide(new BigDecimal("100"), 4, RoundingMode.FLOOR);
//		results_map.put(files, sum_precise);
		
		sum /= (float) 100;
		results_map.put(files, new BigDecimal(sum).setScale(5, RoundingMode.FLOOR));
	}
	
	Float frecventa(String cuvant, HashMap<String, Integer> hash, int nr_cuv_doc){
		Float frecventa = new Float(0);
		Integer aparitii_cuvant = hash.get(cuvant);
		if (aparitii_cuvant == null) 
			aparitii_cuvant = 0;
		frecventa = ((float) aparitii_cuvant /(float) nr_cuv_doc) * 100;
		return frecventa;
	}
	BigDecimal frecventa_precise(String cuvant, HashMap<String, Integer> hash, int nr_cuv_doc){
		BigDecimal frecventa = null;
		Integer aparitii_cuvant = hash.get(cuvant);
		if (aparitii_cuvant == null) 
			aparitii_cuvant = 0;
		BigDecimal aparitii = new BigDecimal(aparitii_cuvant.toString());
		BigDecimal nr_cuv = new BigDecimal(String.valueOf(nr_cuv_doc));
		frecventa = aparitii.divide(nr_cuv , 10, RoundingMode.FLOOR).multiply(new BigDecimal("100"));

		return frecventa;	
	}
}
