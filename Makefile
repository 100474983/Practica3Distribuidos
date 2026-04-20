# Compilador y flags generales
CC = gcc
CFLAGS = -Wall -Wextra -Wno-unused-variable -Wno-unused-parameter -Wno-cast-function-type -g -fPIC -pthread

# RPC flags
RPC_CFLAGS := $(shell pkg-config --cflags libtirpc 2>/dev/null)
RPC_LIBS := $(shell pkg-config --libs libtirpc 2>/dev/null)

# Verificar si pkg-config encontró las librerías RPC
ifeq ($(strip $(RPC_LIBS)),)
	RPC_LIBS = -lnsl
endif

# Añadir flags de RPC a CFLAGS
CFLAGS += $(RPC_CFLAGS)

# Targets principales y librerías
TARGET_SERVER = clavesRPC_server
TARGET_CLIENT = app-cliente
LIBCLAVES = libclaves.so
LIBPROXY = libproxyclaves.so

# Generar archivos RPC a partir del archivo .x
RPC_GEN_FILES = clavesRPC.h clavesRPC_clnt.c clavesRPC_svc.c clavesRPC_xdr.c clavesRPC_server.c

# Compilación todo en uno
all: rpcgen $(LIBCLAVES) $(LIBPROXY) $(TARGET_SERVER) $(TARGET_CLIENT)

# Comando para generar los archivos RPC, menos server y client (esos se generaron solo la primera compilación)
rpcgen: clavesRPC.x
	rm -f clavesRPC_clnt.c clavesRPC_svc.c clavesRPC_xdr.c clavesRPC.h
	rpcgen -NM clavesRPC.x

# Librería de claves
$(LIBCLAVES): claves.o
	$(CC) -shared -o $(LIBCLAVES) claves.o -pthread

# Librería proxy
$(LIBPROXY): proxy-rpc.o clavesRPC_clnt.o clavesRPC_xdr.o
	$(CC) -shared -o $(LIBPROXY) proxy-rpc.o clavesRPC_clnt.o clavesRPC_xdr.o $(RPC_LIBS)

# Targets para server y client, asegurando que se compilen los archivos RPC necesarios
$(TARGET_SERVER): clavesRPC_svc.o clavesRPC_server.o clavesRPC_xdr.o claves.o
	$(CC) -o $(TARGET_SERVER) clavesRPC_svc.o clavesRPC_server.o clavesRPC_xdr.o claves.o -pthread $(RPC_LIBS)

$(TARGET_CLIENT): app-cliente.o $(LIBPROXY)
	$(CC) -o $(TARGET_CLIENT) app-cliente.o -L. -lproxyclaves $(RPC_LIBS)

# Reglas de compilación para los archivos fuente
claves.o: claves.c claves.h
	$(CC) $(CFLAGS) -c claves.c

proxy-rpc.o: proxy-rpc.c claves.h clavesRPC.h
	$(CC) $(CFLAGS) -c proxy-rpc.c

app-cliente.o: app-cliente.c claves.h
	$(CC) $(CFLAGS) -c app-cliente.c

clavesRPC_clnt.o: clavesRPC_clnt.c clavesRPC.h
	$(CC) $(CFLAGS) -c clavesRPC_clnt.c

clavesRPC_svc.o: clavesRPC_svc.c clavesRPC.h
	$(CC) $(CFLAGS) -c clavesRPC_svc.c

clavesRPC_xdr.o: clavesRPC_xdr.c clavesRPC.h
	$(CC) $(CFLAGS) -c clavesRPC_xdr.c

clavesRPC_server.o: clavesRPC_server.c clavesRPC.h claves.h
	$(CC) $(CFLAGS) -c clavesRPC_server.c

run-server: $(TARGET_SERVER)
	./$(TARGET_SERVER)

run-client: $(TARGET_CLIENT)
	LD_LIBRARY_PATH=. ./$(TARGET_CLIENT)

# Limpieza de archivos generados
clean:
	rm -f *.o $(TARGET_SERVER) $(TARGET_CLIENT) $(LIBCLAVES) $(LIBPROXY)
	rm -f clavesRPC_clnt.c clavesRPC_svc.c clavesRPC_xdr.c clavesRPC.h Makefile.clavesRPC

# Limpieza completa, incluyendo archivos generados por rpcgen
distclean: clean
	rm -f $(RPC_GEN_FILES)