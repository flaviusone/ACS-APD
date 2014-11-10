import java.io.BufferedReader;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;


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
	static ConcurrentHashMap<String	, HashMap<String, Integer>> MapResults;
	
	
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
		String separator = "\t\n\r.,;:!?<>[]{}()- ";	
		/****************************************************/
		
		/* Solve */
		Map_Stage();
		Reduce_Stage();
		Compare_Stage();
		
		/****************************************************/
		
		/* Scriere date in fisierul de output */
		
	}
	public static void Map_Stage(){
		File f;
		/* Initializare Hash de Rezultate */
		MapResults = new ConcurrentHashMap<String, HashMap<String,Integer>>();
		
		/* Crearea workpool-ului */
		ExecutorService map_workpool = Executors.newFixedThreadPool(NT);
		
		/* Generarea de taskuri */
		for(String fis : DOCS){
			System.out.println(fis);
			f = new File(fis);
			for (int i = 0; i < f.length() / D; i++) {
				map_workpool.submit(new MapService(fis, i*D, D, MapResults));
			}
		}

		map_workpool.shutdown();
		while(true){
		if(map_workpool.isTerminated())
			break;
		}
		System.out.println(MapResults.toString());
		
	}
	public static void Reduce_Stage(){
		
	}
	public static void Compare_Stage(){
		
	}
}
