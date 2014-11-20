import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.math.BigDecimal;
import java.math.RoundingMode;
import java.text.DecimalFormat;
import java.util.ArrayList;
import java.util.LinkedList;
import java.util.Collections;
import java.util.Comparator;
import java.util.HashMap;
import java.util.Map;
import java.util.TreeMap;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.TimeUnit;

/*
 * @author Flavius
 * 
 * Nume: Flavius-Costin Tirnacop 331CA
 * E-mail: flavius.tirnacop@cti.pub.ro
 * 
 */

public class MapReducer {
	static int 	NT,	/* Numar threads */
					D,	/* Dimensiune fragmente */
					ND;	/* Numar documente analizate */
	static Float 	X;	/* Prag de similaritate */
			
	static ArrayList<String> DOCS;
	private static BufferedReader in;
	private static BufferedWriter out;
	
	/* 
	 * Hash de forma <Nume_fisier : Hash<Cuvant:Nr_ap>>
	 * Hash ce contine rezultatele operatiilor de Map
	 */
	static HashMap<String,LinkedList<HashMap<String, Integer>>> MapResults;
	static Map<String, BigDecimal> results_map;
	
	public static void main(String[] args) throws NumberFormatException, IOException {

		/* Citire NT si nume fisier */
		NT = Integer.parseInt(args[0]);
//		NT = 1;
		in = new BufferedReader(new FileReader(args[1]));
		
		/* Citire date din fisier */
		D = Integer.parseInt(in.readLine());
		X = Float.parseFloat(in.readLine());
		ND = Integer.parseInt(in.readLine());
		DOCS = new ArrayList<String>();
		for(int i=0;i<ND;i++){
			DOCS.add(in.readLine());
		}
		in.close();	
		/****************************************************/
		
		long startTime = System.currentTimeMillis();

		Map_Stage();

		long endTime = System.currentTimeMillis();

		System.out.println("Map took " + (endTime - startTime) + " milliseconds");
		
		startTime = System.currentTimeMillis();

		Reduce_Stage();

		endTime = System.currentTimeMillis();

		System.out.println("Reduce took " + (endTime - startTime) + " milliseconds");
		
		startTime = System.currentTimeMillis();

		Compare_Stage();

		endTime = System.currentTimeMillis();

		System.out.println("Compare took " + (endTime - startTime) + " milliseconds");
		
		/* Solve */
//		Map_Stage();
//		System.out.println("Map Stage Done");
//		Reduce_Stage();
//		System.out.println("Reduce Stage Done");
//		Compare_Stage();
//		System.out.println("Compare Stage Done");
		
		/****************************************************/
		
		/* Scriere date in fisierul de output */
		
		ValueComparator bvc =  new ValueComparator(results_map);
		TreeMap<String,BigDecimal> sorted_map = new TreeMap<String,BigDecimal>(bvc);
		sorted_map.putAll(results_map);
		
		
		DecimalFormat df = new DecimalFormat("0.0000");
		df.setRoundingMode(RoundingMode.DOWN);
		
		out = new BufferedWriter (new FileWriter(args[2]));
		for(Map.Entry<String, BigDecimal> entry : sorted_map.entrySet()){
			if(entry.getValue().compareTo(new BigDecimal(X)) > 0){
				System.out.println(entry.getKey().toString()+df.format(entry.getValue()));
				out.write(entry.getKey().toString()+df.format(entry.getValue()));
				out.write("\n");
			}
		}
		out.close();
		
	}
	public static void Map_Stage(){
		File f;
		/* Initializare Hash de Rezultate */
		MapResults = new HashMap<String,LinkedList<HashMap<String, Integer>>>();
		
		/* Crearea workpool-ului */
		ExecutorService map_workpool = Executors.newFixedThreadPool(NT);
		
		/* Generarea de taskuri */
		for(String fis : DOCS){
			f = new File(fis);
			for (int i = 0; i < f.length() / D; i++) {
				map_workpool.submit(new MapService(fis, i*D, D, MapResults));
			}
		}

		map_workpool.shutdown();
		
		/* De testat daca poate fi scos asta (e ca un barrier)*/
		try {
			  map_workpool.awaitTermination(Long.MAX_VALUE, TimeUnit.NANOSECONDS);
		} catch (InterruptedException e) {
			System.out.println("Error " + e);
		}
	}
	public static void Reduce_Stage(){
		/* Creeam hash de rezultate */
		
		results_map = Collections.synchronizedMap(new HashMap<String,BigDecimal>());  
		
		/* Crearea workpool-ului */
		ExecutorService reduce_workpool = Executors.newFixedThreadPool(NT);
		
		/* Generam taskuri reduce */
		for(String fis : DOCS){
			LinkedList<HashMap<String, Integer>> list = MapResults.get(fis);
			reduce_workpool.submit(new ReduceService(fis, list));
		}
		reduce_workpool.shutdown();
		
		/* De testat daca poate fi scos asta (e ca un barrier)*/
		try {
			  reduce_workpool.awaitTermination(Long.MAX_VALUE, TimeUnit.NANOSECONDS);
		} catch (InterruptedException e) {
			System.out.println("Error " + e);
		}
		
	}
	public static void Compare_Stage(){
		HashMap<String, Integer> hash_a;
		HashMap<String, Integer> hash_b;
		
		/* Crearea workpool-ului */
		ExecutorService compare_workpool = Executors.newFixedThreadPool(NT);
		for (int i = 0; i < DOCS.size(); i++) {
			for (int j = i+1; j < DOCS.size(); j++){
				String fis_a = DOCS.get(i);
				String fis_b = DOCS.get(j);
				if(!fis_a.equals(fis_b)){
					hash_a = MapResults.get(fis_a).get(0);
					hash_b = MapResults.get(fis_b).get(0);
					compare_workpool.submit(new CompareService(fis_a, fis_b, hash_a, hash_b, results_map));
				}
			}
		}
		compare_workpool.shutdown();
		
		/* De testat daca poate fi scos asta (e ca un barrier)*/
		try {
			  compare_workpool.awaitTermination(Long.MAX_VALUE, TimeUnit.NANOSECONDS);
		} catch (InterruptedException e) {
			System.out.println("Error " + e);
		}
	}
		
}

class ValueComparator implements Comparator<String> {

    Map<String, BigDecimal> base;
    public ValueComparator(Map<String, BigDecimal> base) {
        this.base = base;
    }

    // Note: this comparator imposes orderings that are inconsistent with equals.    
    public int compare(String a, String b) {
        if (base.get(a).compareTo(base.get(b)) > 0) {
            return -1;
        } else if (base.get(a).compareTo(base.get(b)) < 0){
            return 1;
        }else{
        	return a.compareTo(b);
//        	return 1;
        }
        // returning 0 would merge keys
    }
}
