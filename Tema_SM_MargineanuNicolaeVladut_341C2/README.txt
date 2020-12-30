Margineanu Nicolae-Vladut 341 C2
Tema 1 si 2 SM

		Structuri multiprocesor – Temele 1 şi 2

 Tema aleasa:Cholesky Decomposition
 
 Factorizarea Cholesky este un tip de factorizare ce consta in obtinerea unei
 matrici inferior triunghiulare care inmultita cu simetrica sa va rezulta in
 matricea originala. Pentru ca o matrice sa poata fi supusa acestei descompuneri,
 ea trebuie sa fie Hermitica si pozitiv definita.

 Pe baza rezultatelor, MPI este cea mai rapida varianta pentru descompunerea
 Cholesky, urmata de pthreads.
 Varianta Hybrid : MPI – OpenMP este cea mai lenta dintre 5 variante analizate.
 De asemenea, se observa ca atat varianta OpenMP, cat si varianta Hybrid:
 MPI – OpenMp sunt mai lente decat implementarea seriala.

 Results Time:
	1. mpi_results_4		3. pthreads_results_4
	Inputs : Time			Inputs : Time
	100 : 0.000000			100 : 0.000000
	200 : 0.000000			200 : 0.062500
	300 : 0.031250			300 : 0.062500		
	400 : 0.031250			400 : 0.093750
	500 : 0.062500			500 : 0.203125
	600 : 0.156250			600 : 0.281250
	700 : 0.140625			700 : 0.671875
	800 : 0.312500			800 : 0.843750
	900 : 0.343750			900 : 1.140625
	1000 : 0.515625			1000 : 1.421875

	2. openmp_results_4		4. serial_results
	Inputs : Time			Inputs : Time
	100 : 0.000000			100 : 0.015625
	200 : 0.062500			200 : 0.015625
	300 : 0.250000			300 : 0.046875
	400 : 0.312500			400 : 0.109375
	500 : 0.765625			500 : 0.234375
	600 : 1.421875			600 : 0.390625
	700 : 2.125000			700 : 0.656250
	800 : 2.921875			800 : 0.953125
	900 : 5.125000			900 : 1.390625
	1000 : 5.640625			1000 : 1.828125
