SUBDIRS = ga kmeans

all:
	for i in $(SUBDIRS); do\
		g++ -g -c $$i/*.cpp;\
	done
	g++ -g main.cpp ga.o kmeans.o kmeans_pr.o -o main;

clean:
	rm -f *.exe
	rm -f *.o
	rm -f output_*.txt
	rm -f main