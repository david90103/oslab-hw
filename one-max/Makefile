SUBDIRS = hc es sa ts

all:
	for i in $(SUBDIRS); do\
		g++ -c $$i/*.cpp;\
	done
	g++ -g main.cpp es.o hc.o sa.o ts.o -o main;

clean:
	rm -f *.exe
	rm -f *.o
	rm -f output_*.txt