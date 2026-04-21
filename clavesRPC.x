/* Estructura auxiliar con 3 enteros */
struct RpcPaquete {
    int x;
    int y;
    int z;
};

/* Tupla completa almacenada en el sistema */
struct RpcTupla {
    string key<256>;
    string value1<256>;
    int N_value2;
    float V_value2[32];
    RpcPaquete value3;
};

/* Solo clave (para busquedas) */
struct RpcClave {
    string key<256>;
};

/* Respuesta del GET: status 0=OK, -1=error */
struct RpcGetResponse {
    int status;
    string value1<256>;
    int N_value2;
    float V_value2[32];
    RpcPaquete value3;
};

/* Definicion del programa RPC */
program CLAVESRPC_PROG {

    version CLAVESRPC_VERS {

        int DESTROY(void) = 1;
        int SET_VALUE(RpcTupla) = 2;
        RpcGetResponse GET_VALUE(RpcClave) = 3;
        int MODIFY_VALUE(RpcTupla) = 4;
        int DELETE_KEY(RpcClave) = 5;
        int EXIST(RpcClave) = 6;

    } = 1;

} = 0x20000001;
