SUBDIRS = hc es

all:
	for i in $(SUBDIRS); do\
		g++ $$i/*.cpp -o $$i;\
	done

clean:
	rm -f *.exe
	rm -f output_*.txt