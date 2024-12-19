PROGRAM = webserver
OBJECTS = main.o clients_common.o server_fork.o clients_statemachine.o thread_pool.o ll_double.o

webserver-clean: clean webserver

webserver: $(OBJECTS)
	clang -g -o webserver $(OBJECTS) -I. -Inet -Lnet -lWildcatNetworking

%.o: %.c
	clang -g -c -o $@ -I. -Inet $<

clean:
	rm -f *.o webserver
