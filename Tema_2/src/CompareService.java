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
	Map<BigDecimal, String> results_map;
	Float sum = (float) 0;
	
	public CompareService(String fis_a, String fis_b,HashMap<String, Integer> hash_a, HashMap<String, Integer> hash_b, Map<BigDecimal, String> results_map2){
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
		Float frec_a = new Float(0);
		Float frec_b = new Float(0);
		
		BigDecimal frec_a_pr = new BigDecimal("0");
		BigDecimal frec_b_pr = new BigDecimal("0");
		
		sum = (float) 0;
		long temp_frec_a = 0;
		long temp_frec_b = 0;
//		System.out.println(sum_precise);
		for(String cuvant : sa){
			frec_a  = frecventa(cuvant, hash_a, nr_cuv_a);
//			temp_frec_a =(long) (frec_a * 10000);
//			frec_a = (Float) temp_frec / 10000;
			frec_b  = frecventa(cuvant, hash_b, nr_cuv_b);
//			temp_frec_b =(int) (frec_b * 10000);
//			frec_b = (Float) temp_frec / 10000;
//			sum += (float)temp_frec_a * temp_frec_b/(float)100000000;
			sum += frec_a * frec_b;
			
			try {
//				frec_a  = frecventa(cuvant, hash_a, nr_cuv_a);
				frec_a_pr  = frecventa_precise(cuvant, hash_a, nr_cuv_a);
//				frec_a_pr = frec_a_pr.setScale(5, RoundingMode.FLOOR);

				frec_b_pr  = frecventa_precise(cuvant, hash_b, nr_cuv_b);
//				frec_b_pr = frec_b_pr.setScale(5, RoundingMode.FLOOR);
				
				BigDecimal partial_sum = frec_a_pr.multiply(frec_b_pr);
//				System.out.println(partial_sum);
//				sum += partial_sum.floatValue();
//				if(partial_sum.floatValue() > 15){
//					System.out.println(frec_b_pr);
//					Thread.sleep(10000);
//					return;
//				}
				sum_precise = sum_precise.add(partial_sum);
			} catch (Exception e) {
				System.out.println(e);
			}
//			System.out.println(sum + "--" +sum_precise);
			
			
//			sum_precise.setScale(4, RoundingMode.FLOOR);
//			System.out.print(frec_a_pr.multiply(frec_b_pr));
//			System.out.print(sum_precise.add(frec_a_pr.multiply(frec_b_pr)));
//			System.out.println(" "+frec_a*frec_b);
			
		}
//		System.out.println(sum+"--"+sum_precise);
		String files;
		if(fis_a.compareTo(fis_b) < 0){
			files = fis_a + ";" + fis_b + ";";
		}else{
			files = fis_b + ";" + fis_a + ";";
		}

		sum_precise = sum_precise.divide(new BigDecimal("100"), 4, RoundingMode.FLOOR);
//		System.out.println(sum_precise+" "+sum_precise.floatValue());
//		System.out.println();
//		System.out.println(sum_precise);
		System.out.println(fis_a+";"+fis_b+" "+sum_precise);
		try {
			results_map.put(sum_precise, files);
		} catch (Exception e) {
			System.out.println(e);
		}
		
//		results_map.put(sum_precise.floatValue(), files);
//		results_map.put(sum/(float)100, files);
	}
	
	Float frecventa(String cuvant, HashMap<String, Integer> hash, int nr_cuv_doc){
		Float frecventa = new Float(0);
		Integer aparitii_cuvant = hash.get(cuvant);
		if (aparitii_cuvant == null) 
			aparitii_cuvant = 0;
//		System.out.println(aparitii_cuvant);
		frecventa = ((float) aparitii_cuvant /(float) nr_cuv_doc) * 100;
		return frecventa;
	}
	BigDecimal frecventa_precise(String cuvant, HashMap<String, Integer> hash, int nr_cuv_doc){
		BigDecimal frecventa = null;
		Integer aparitii_cuvant = hash.get(cuvant);
//		System.out.println();
		if (aparitii_cuvant == null) 
			aparitii_cuvant = 0;
		BigDecimal aparitii = new BigDecimal(aparitii_cuvant.toString());
		BigDecimal nr_cuv = new BigDecimal(String.valueOf(nr_cuv_doc));
		frecventa = aparitii.divide(nr_cuv , 8, RoundingMode.FLOOR).multiply(new BigDecimal("100"));

		return frecventa;	
	}
}
