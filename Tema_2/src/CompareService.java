import java.math.BigDecimal;
import java.math.RoundingMode;
import java.text.DecimalFormat;
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
		BigDecimal sum_precise = new BigDecimal("0");
		
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
		
		BigDecimal frec_a_pr = new BigDecimal("0");
		BigDecimal frec_b_pr = new BigDecimal("0");
		
		sum = (double) 0;
		for(String cuvant : sa){
			frec_a  = frecventa(cuvant, hash_a, nr_cuv_a);
			frec_b  = frecventa(cuvant, hash_b, nr_cuv_b);
			sum += frec_a * frec_b;
			
			frec_a_pr  = frecventa_precise(cuvant, hash_a, nr_cuv_a);
			frec_b_pr  = frecventa_precise(cuvant, hash_a, nr_cuv_a);
			sum_precise = sum_precise.add(frec_a_pr.multiply(frec_b_pr));
			
//			sum_precise.setScale(4, RoundingMode.FLOOR);
			System.out.print(frec_a_pr.multiply(frec_b_pr));
//			System.out.print(sum_precise.add(frec_a_pr.multiply(frec_b_pr)));
			System.out.println(" "+new DecimalFormat("0.0000").format(frec_a*frec_b).toString());
			
		}
//		System.out.println(sum+"--"+sum_precise);
		String files;
		if(fis_a.compareTo(fis_b) < 0){
			files = fis_a + ";" + fis_b + ";";
		}else{
			files = fis_b + ";" + fis_a + ";";
		}
		sum_precise = sum_precise.divide(new BigDecimal(100), 6, RoundingMode.FLOOR);
//		results_map.put(sum_precise.doubleValue(), files);
		results_map.put(sum/(double)100, files);
	}
	
	Double frecventa(String cuvant, HashMap<String, Integer> hash, int nr_cuv_doc){
		Double frecventa = new Double(0);
		Integer aparitii_cuvant = hash.get(cuvant);
		if (aparitii_cuvant == null) 
			aparitii_cuvant = 0;
		frecventa = ((double) aparitii_cuvant /(double) nr_cuv_doc) * 100;

//		BigDecimal demo = frecventa_precise(cuvant, hash, nr_cuv_doc);
		
		return frecventa;
	}
	BigDecimal frecventa_precise(String cuvant, HashMap<String, Integer> hash, int nr_cuv_doc){
		BigDecimal frecventa = null;
		Integer aparitii_cuvant = hash.get(cuvant);
		if (aparitii_cuvant == null) 
			aparitii_cuvant = 0;
		BigDecimal aparitii = new BigDecimal(aparitii_cuvant.toString());
		BigDecimal nr_cuv = new BigDecimal(String.valueOf(nr_cuv_doc));
		frecventa = aparitii.divide(nr_cuv , 10, RoundingMode.FLOOR);
		frecventa = frecventa.multiply(new BigDecimal(100));
		return frecventa;	
	}
}
