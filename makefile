programa: funtg_s.c funtg_p.c taldegen_s.c
	gcc -O2 -o programa_serie taldegen_s.c funtg_s.c -lm
	gcc -O2 -fopenmp -o programa_paralelo taldegen_s.c funtg_p.c -lm
