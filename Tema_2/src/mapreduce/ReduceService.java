package mapreduce;
import java.util.LinkedList;
import java.util.HashMap;
/*
 * @author Flavius
 * 
 * Nume: Flavius-Costin Tirnacop 341C1
 * E-mail: flavius.tirnacop@cti.pub.ro
 * 
 */
public class ReduceService implements Runnable{
	
	LinkedList<HashMap<String, Integer>> lista_map;
	String nume_fis;
	HashMap<String, Integer> a; 
	HashMap<String, Integer> b;

	public ReduceService(String nume_fis, LinkedList<HashMap<String, Integer>> list){
		this.lista_map = list;
		this.nume_fis = nume_fis;
		this.b = new HashMap<String, Integer>();
	}

	@Override
	public void run() {
		while(lista_map.size()>1){

			a = lista_map.remove(0);
			b = lista_map.remove(0);
			for(String cuv_a : a.keySet()){
				Integer value = b.get(cuv_a);
				if(value == null)
				{
					b.put(cuv_a, a.get(cuv_a));
				}
				else
				{
					b.put(cuv_a, a.get(cuv_a) + value);
				}
			}
			
			lista_map.add(b);
		}
	}

}
