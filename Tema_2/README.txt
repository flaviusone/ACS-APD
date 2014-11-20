 Nume: Flavius-Costin Tirnacop
 Grupa: 341C1
 E-mail: flavius.tirnacop@cti.pub.ro
 Tema#2

 1. Detalii testare
 	Tema a fost testata pe ElementaryOS Luna si scrisa in Eclipse Juno
 2. Detalii implementare

 	2.1 MapReducer.java
 	Contine implementarea la nivelul threadului master.Astfel algoritmul va fi
 	urmatorul:

 	- se citesc datele de intrare
 	- se formeaza solutii partiale pentru etapa de MAP care se adauga in workpoolul pentru MAP. Workpool-ul este creeat cu ExecutorService. Fiecare task din workpool-ul de map va contine datele specificate in enunt
 	- fiecare worker va citi din fisier fragmentul specific va numara aparitiile cuvintelor si va calcula hash-ul de output pe care il va pune intr-un alt hashtable de rezutlate de forma HashMap<String, ArrayList<HashMap<String, Integer>>>

 	- se creeaza solutii partiale de tip REDUCE pentru workpoolul de reduce. fiecare solutie partiala va contine o lsita de hashtable pentru un fisier specific.
 	- workerii de tip REDUCE ce vor lua fiecare solutie partiala din workpoolul de REDUCE si vor combina toate hashtable-urile rezultate in urma etapei de MAP pentru a obtine un hashtable final

 	- se creeaza solutii partiale de tip COMPARE ce vor avea date de intrare cele din enunt. Acestea vor compara similaritatile a doua fisiere si vor pune rezultatele intr-un hashtable final de forma <String, BigDecimal>

 	- se sorteaza hashtable-ul final pentru a ptuea determina primele elemente mai mari decat X

 	- se afiseaza in fisier folosind 4 zecimale trunchiat

 	2.2 MapService
 	Contine implementarea etapei MAP. Se foloseste de interfata Callable pentru a putea folosit in Executorservice.
 	2.3 ReduceService
 	Contine implementarea etapei REDUCE. Se foloseste de interfata Callable pentru a putea folosit in Executorservice.
 	2.4 CompareService
 	Contine implementarea etapei COMPARE. Se foloseste de interfata Callable pentru a putea folosit in Executorservice.


 	OBSERVATII:
 		1. Sortarea se face dupa numerele cu mai multe zecimale (inainte de trunchiere)