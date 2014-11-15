import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.text.DecimalFormat;
import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.Map;
import java.util.TreeMap;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.TimeUnit;


public class MapReducer {
	static int 	NT,	/* Numar threads */
					D,	/* Dimensiune fragmente */
					ND;	/* Numar documente analizate */
	static Double 	X;	/* Prag de similaritate */
			
	static ArrayList<String> DOCS;
	private static BufferedReader in;
	private static BufferedWriter out;
	
	/* 
	 * Hash de forma <Nume_fisier : Hash<Cuvant:Nr_ap>>
	 * Hash ce contine rezultatele operatiilor de Map
	 */
	static ConcurrentHashMap<String,ArrayList<HashMap<String, Integer>>> MapResults;
	static Map<Double, String> results_map;
	
	public static void main(String[] args) throws NumberFormatException, IOException {

		/* Citire NT si nume fisier */
		NT = Integer.parseInt(args[0]);
		in = new BufferedReader(new FileReader(args[1]));
		
		/* Citire date din fisier */
		D = Integer.parseInt(in.readLine());
		X = Double.parseDouble(in.readLine());
		ND = Integer.parseInt(in.readLine());
		DOCS = new ArrayList<String>();
		for(int i=0;i<ND;i++){
			DOCS.add(in.readLine());
		}
		in.close();	
		/****************************************************/
		
		/* Solve */
		Map_Stage();
		System.out.println("Map Stage Done");
		Reduce_Stage();
		System.out.println("Reduce Stage Done");
		Compare_Stage();
		System.out.println("Compare Stage Done");
		ArrayList<String> outputs = new ArrayList<String>();
		for(Map.Entry<Double,String> entry : results_map.entrySet()){
			if(entry.getKey() > X)
				outputs.add(entry.getValue()+new DecimalFormat("0.0000").format(entry.getKey()).toString());
		}
		
		/****************************************************/
		
		/* Scriere date in fisierul de output */
		
		out = new BufferedWriter (new FileWriter(args[2]));
		Collections.reverse(outputs);
		for(String x : outputs){
			out.write(x);
			out.write("\n");
		}
		out.close();
		
	}
	public static void Map_Stage(){
		File f;
		/* Initializare Hash de Rezultate */
		MapResults = new ConcurrentHashMap<String,ArrayList<HashMap<String, Integer>>>();
		
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
		results_map = Collections.synchronizedMap(new TreeMap<Double, String>());  
		
		/* Crearea workpool-ului */
		ExecutorService reduce_workpool = Executors.newFixedThreadPool(NT);
		
		/* Generam taskuri reduce */
		for(String fis : DOCS){
			ArrayList<HashMap<String, Integer>> list = MapResults.get(fis);
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
		
		for(String fis_a : DOCS){
			for(String fis_b : DOCS){
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
