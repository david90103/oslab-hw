SUBDIRS = aco lib de pso
all:
	for i in $(SUBDIRS); do\
		g++ -c -O3 $$i/*.cpp;\
	done
	g++ -g -O3 main.cpp *.o -o main;
	g++ -g -O3 draw.cpp -o draw;
clean:
	rm -f *.exe
	rm -f *.o
	rm -f output_*.txt
	rm -f main
	rm -f draw
