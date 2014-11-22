package mapreduce;
import java.io.IOException;
import java.io.RandomAccessFile;
import java.util.LinkedList;
import java.util.HashMap;
import java.util.StringTokenizer;

/*
 * @author Flavius
 * 
 * Nume: Flavius-Costin Tirnacop 341C1
 * E-mail: flavius.tirnacop@cti.pub.ro
 * 
 */
public class MapService implements Runnable {
	HashMap<String,LinkedList<HashMap<String, Integer>>> MapResults;
	HashMap<String, Integer> result;
	String nume_fis;
	long offset;
	final private int D;
	String separator = "_;:/?~\\.,><~`[]{}()!@#$%^&-+'=*\"| \t\n";

	public MapService(String nume_fis, long offset, int D, 
		HashMap<String,LinkedList<HashMap<String, Integer>>> MapResults) {
		this.nume_fis = nume_fis;
		this.offset = offset;
		this.D = D;
		this.MapResults = MapResults;
		result = new HashMap<String, Integer>();
	}
	
	@Override
	public void run() {
		RandomAccessFile file = null ;
		byte[] bytes = null;
		int val = 0;
		/* Citire fragment din fisier */
		try {
			/* Descidem fisier si seek la offset */
			try {
				file = new RandomAccessFile(nume_fis, "r");
			} catch (Exception e) {
				System.out.println(e);
			}			
			if(offset != 0)
				file.seek(offset-1);
			/* Citire din fisier fragment */
			bytes = new byte[D+100];
			val = file.read(bytes);
			file.close();
		} catch (IOException e) {
			e.printStackTrace();
		}
		
		/* Modificare fragment */
		
		/* Sarim peste primul cuvant */
		int counter_start = 0;
		while(true){
			if(!Character.isLetterOrDigit(bytes[counter_start])){
				break;
			}
			counter_start++;
		}
		int counter_end;
		/* Luam si ultimul cuvant */
		if (val != D + 100){
			counter_end = val;
		}else{
			counter_end = D; 
			while(true){
				if(!Character.isLetterOrDigit(bytes[counter_end])){
					break;
				}
				counter_end++;
			}
		}
		
		/* Recream fragment final */
		String fragment_final = new String(bytes, counter_start, counter_end-counter_start);
		
		/* Numarare aparitii */

		StringTokenizer st = new StringTokenizer(fragment_final, separator);
		String cuvant;
		Integer value;
		while(st.hasMoreTokens()){
			cuvant = new String(st.nextToken().toLowerCase());
			/* Adaugare in hash */
			value = result.get(cuvant); 
			if(value == null){
				result.put(cuvant, 1);
			}else{
				result.put(cuvant, value+1);
			}
		}
				
		/* Adaugare rezultat la hash */
		synchronized (MapResults) {
			LinkedList<HashMap<String, Integer>> list = MapResults.get(nume_fis);
			if(list == null)
			{
				list = new LinkedList<HashMap<String,Integer>>();
				list.add(result);
			}
			else
			{
				list.add(result);
			}
			MapResults.put(nume_fis, list);
		}
	}
	
}
