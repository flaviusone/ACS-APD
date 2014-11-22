package mapreduce;
import java.io.IOException;
import java.io.RandomAccessFile;
import java.util.LinkedList;
import java.util.HashMap;
import java.util.StringTokenizer;
/*
 * @author Flavius
 * 
 * Nume: Flavius-Costin Tirnacop 331CA
 * E-mail: flavius.tirnacop@cti.pub.ro
 * 
 */
public class MapService implements Runnable {
	HashMap<String,LinkedList<HashMap<String, Integer>>> MapResults;
	HashMap<String, Integer> result;
	String nume_fis;
	long offset;
	int D;
	String separator = ",'`;:/?~.><~`[]{}()!@#$%^&-+=*| \t\n\r\"\\";
//	String separator = ";:/?~\\.,><~`[]{}()!@#$%^&-+'=*\"| \t\n";

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
		/* Citire fragment din fisier */
		try {
			/* Descidem fisier si seek la offset */
			try {
				file = new RandomAccessFile(nume_fis, "r");
			} catch (Exception e) {
				System.out.println(e);
			}			
			if(offset == 0)
				file.seek(offset);
			else
				file.seek(offset-1);
			/* Citire din fisier fragment */
			bytes = new byte[D+100];
			file.read(bytes);
			file.close();
		} catch (IOException e) {
			e.printStackTrace();
		}

		/* Modificare fragment */
		
		/* Sarim peste primul cuvant */
		int counter_start = 0;
		while(true){
			if(separator.contains(""+(char)bytes[counter_start])){
				break;
			}
			counter_start ++;
		}
		
		/* Luam si ultimul cuvant */
		int counter_end = D;
		while(true){
			if(separator.contains(""+(char)bytes[counter_end])){
				break;
			}
			counter_end++;
		}
		counter_end -= counter_start;
		
		/* Recream fragment final */
		String fragment_final = new String(bytes, counter_start, counter_end);
		
		/* Numarare aparitii */

		StringTokenizer st = new StringTokenizer(fragment_final, separator);
		while(st.hasMoreTokens()){
			String cuvant = new String(st.nextToken().toLowerCase());
			/* Adaugare in hash */
			Integer value = result.get(cuvant); 
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
