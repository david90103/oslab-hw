SUBDIRS = pso de
all:
	for i in $(SUBDIRS); do\
		g++ -c $$i/*.cpp;\
	done
	g++ -g main.cpp pso.o de.o -o main;
clean:
	rm -f *.exe
	rm -f *.o
	rm -f output_*.txt
	rm -f main