TARGET=stunserver

all :
	gcc -Iinclude/ -Ilibxml/include -Llibxml/lib src/server.c src/utils.c src/stun.c -lxml -lexpat -o $(TARGET)

clean : 
	rm -rf $(TARGET)