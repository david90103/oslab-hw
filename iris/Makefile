SUBDIRS = ga

all:
	for i in $(SUBDIRS); do\
		g++ -g -c $$i/*.cpp;\
	done
	g++ -g main.cpp ga.o -o main;

clean:
	rm -f *.exe
	rm -f *.o
	rm -f output_*.txt
	rm -f main