SUBDIRS = aco
all:
	for i in $(SUBDIRS); do\
		g++ -c $$i/*.cpp;\
	done
	g++ -g main.cpp aco.o -o main;
	g++ -g draw.cpp -o draw;
clean:
	rm -f *.exe
	rm -f *.o
	rm -f output_*.txt
	rm -f main