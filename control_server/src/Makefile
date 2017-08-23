CFLAGS = -I../inc/

all:
	gcc tmp_server.c mldr_send_recv.c mldr_connect_client.c -o server -lpthread $(CFLAGS)
clean:
	rm -rf server
        
