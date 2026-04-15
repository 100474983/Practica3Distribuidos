#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <rpc/rpc.h>

#include "claves.h"
#include "clavesRPC.h"

static CLIENT *crear_cliente(void) {
    char *server = getenv("IP_TUPLAS");

    if (server == NULL) {
        fprintf(stderr, "Error: IP_TUPLAS no definida\n");
        return NULL;
    }

    CLIENT *clnt = clnt_create(server, CLAVESRPC_PROG, CLAVESRPC_VERS, "tcp");

    if (clnt == NULL) {
        clnt_pcreateerror(server);
        return NULL;
    }

    return clnt;
}

int destroy(void) {
    CLIENT *clnt = crear_cliente();
    if (!clnt) return -1;

    int res;
    enum clnt_stat stat;

    stat = destroy_1(&res, clnt);

    if (stat != RPC_SUCCESS) {
        clnt_perror(clnt, "RPC destroy falló");
        clnt_destroy(clnt);
        return -1;
    }

    clnt_destroy(clnt);
    return res;
}

int set_value(char *key, char *value1, int N_value2, float *V_value2, struct Paquete value3) {

    if (!key || !value1 || !V_value2)
        return -1;

    CLIENT *clnt = crear_cliente();
    if (!clnt) return -1;

    RpcTupla arg;
    int res;
    enum clnt_stat stat;

    memset(&arg, 0, sizeof(arg));

    arg.key = key;
    arg.value1 = value1;
    arg.N_value2 = N_value2;

    for (int i = 0; i < N_value2 && i < 32; i++)
        arg.V_value2[i] = V_value2[i];

    arg.value3.x = value3.x;
    arg.value3.y = value3.y;
    arg.value3.z = value3.z;

    stat = set_value_1(arg, &res, clnt);

    if (stat != RPC_SUCCESS) {
        clnt_perror(clnt, "RPC set_value falló");
        clnt_destroy(clnt);
        return -1;
    }

    clnt_destroy(clnt);
    return res;
}

int get_value(char *key, char *value1, int *N_value2, float *V_value2, struct Paquete *value3) {

    if (!key || !value1 || !N_value2 || !V_value2 || !value3)
        return -1;

    CLIENT *clnt = crear_cliente();
    if (!clnt) return -1;

    RpcClave arg;
    RpcGetResponse res;
    enum clnt_stat stat;

    memset(&res, 0, sizeof(res));

    arg.key = key;

    stat = get_value_1(arg, &res, clnt);

    if (stat != RPC_SUCCESS) {
        clnt_perror(clnt, "RPC get_value falló");
        clnt_destroy(clnt);
        return -1;
    }

    if (res.status != 0) {
        clnt_destroy(clnt);
        return -1;
    }

    if (res.value1 != NULL) {
        strncpy(value1, res.value1, 255);
        value1[255] = '\0';
    } else {
        value1[0] = '\0';
    }

    *N_value2 = res.N_value2;

    for (int i = 0; i < *N_value2 && i < 32; i++)
        V_value2[i] = res.V_value2[i];

    value3->x = res.value3.x;
    value3->y = res.value3.y;
    value3->z = res.value3.z;

    clnt_destroy(clnt);
    return 0;
}

int modify_value(char *key, char *value1, int N_value2, float *V_value2, struct Paquete value3) {

    if (!key || !value1 || !V_value2)
        return -1;

    CLIENT *clnt = crear_cliente();
    if (!clnt) return -1;

    RpcTupla arg;
    int res;
    enum clnt_stat stat;

    memset(&arg, 0, sizeof(arg));

    arg.key = key;
    arg.value1 = value1;
    arg.N_value2 = N_value2;

    for (int i = 0; i < N_value2 && i < 32; i++)
        arg.V_value2[i] = V_value2[i];

    arg.value3.x = value3.x;
    arg.value3.y = value3.y;
    arg.value3.z = value3.z;

    stat = modify_value_1(arg, &res, clnt);

    if (stat != RPC_SUCCESS) {
        clnt_perror(clnt, "RPC modify_value falló");
        clnt_destroy(clnt);
        return -1;
    }

    clnt_destroy(clnt);
    return res;
}

int delete_key(char *key) {

    if (!key)
        return -1;

    CLIENT *clnt = crear_cliente();
    if (!clnt) return -1;

    RpcClave arg;
    int res;
    enum clnt_stat stat;

    arg.key = key;

    stat = delete_key_1(arg, &res, clnt);

    if (stat != RPC_SUCCESS) {
        clnt_perror(clnt, "RPC delete_key falló");
        clnt_destroy(clnt);
        return -1;
    }

    clnt_destroy(clnt);
    return res;
}

int exist(char *key) {

    if (!key)
        return -1;

    CLIENT *clnt = crear_cliente();
    if (!clnt) return -1;

    RpcClave arg;
    int res;
    enum clnt_stat stat;

    arg.key = key;

    stat = exist_1(arg, &res, clnt);

    if (stat != RPC_SUCCESS) {
        clnt_perror(clnt, "RPC exist falló");
        clnt_destroy(clnt);
        return -1;
    }

    clnt_destroy(clnt);
    return res;
}