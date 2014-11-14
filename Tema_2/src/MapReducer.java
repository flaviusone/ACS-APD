import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.TimeUnit;


public class MapReducer {
	static int 	NT,	/* Numar threads */
					D,	/* Dimensiune fragmente */
					ND;	/* Numar documente analizate */
	static float 	X;	/* Prag de similaritate */
			
	static ArrayList<String> DOCS;
	private static BufferedReader in;
	
	/* 
	 * Hash de forma <Nume_fisier : Hash<Cuvant:Nr_ap>>
	 * Hash ce contine rezultatele operatiilor de Map
	 */
	static ConcurrentHashMap<String,ArrayList<HashMap<String, Integer>>> MapResults;
	
	
	public static void main(String[] args) throws NumberFormatException, IOException {

		/* Citire NT si nume fisier */
		NT = Integer.parseInt(args[0]);
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
		
		/* Solve */
		Map_Stage();
		System.out.println("Map Stage Done");
		Reduce_Stage();
		System.out.println("Reduce Stage Done");
//		System.out.println(MapResults.get("1mb-1").toString());
//		System.out.println(MapResults.get("1mb-2").size());
//		System.out.println(MapResults.get("1mb-3").size());
//		System.out.println(MapResults.get("1mb-4").size());
		Compare_Stage();
		
		/****************************************************/
		
		/* Scriere date in fisierul de output */
		
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
//		System.out.println(MapResults.get("1mb-2").size()*D);
		
	}
	public static void Reduce_Stage(){
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
		/* Crearea workpool-ului */
		ExecutorService compare_workpool = Executors.newFixedThreadPool(NT);
		
		
		compare_workpool.shutdown();
		
		/* De testat daca poate fi scos asta (e ca un barrier)*/
		try {
			  compare_workpool.awaitTermination(Long.MAX_VALUE, TimeUnit.NANOSECONDS);
		} catch (InterruptedException e) {
			System.out.println("Error " + e);
		}
	}
}
