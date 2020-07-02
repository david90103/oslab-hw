SUBDIRS = es hc

all:
	for i in $(SUBDIRS); do\
		g++ $$i/*.cpp -o $$i;\
	done

clean:
	rm -f *.exe