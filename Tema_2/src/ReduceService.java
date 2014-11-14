import java.util.ArrayList;
import java.util.HashMap;

public class ReduceService implements Runnable{
	
	ArrayList<HashMap<String, Integer>> lista_map;
	String nume_fis;
	HashMap<String, Integer> a; 
	HashMap<String, Integer> b;

	public ReduceService(String nume_fis, ArrayList<HashMap<String, Integer>> list){
		this.lista_map = list;
		this.nume_fis = nume_fis;
	}

	@Override
	public void run() {
		
		while(lista_map.size()>1){
			a = lista_map.remove(lista_map.size()-1);
			b = lista_map.remove(lista_map.size()-1);
			
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
