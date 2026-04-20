/* Estructura auxiliar con 3 enteros */
struct RpcPaquete {
    int x;
    int y;
    int z;
};

/* Tupla completa almacenada en el sistema */
struct RpcTupla {
    string key<256>;       // clave identificadora
    string value1<256>;    // primer valor (string)
    int N_value2;          // número de floats válidos
    float V_value2[32];    // array de floats
    RpcPaquete value3;     // estructura adicional
};

/* Solo clave (para búsquedas) */
struct RpcClave {
    string key<256>;
};

/* Respuesta del GET */
struct RpcGetResponse {
    int status;            // 0 = OK, -1 = error
    string value1<256>;
    int N_value2;
    float V_value2[32];
    RpcPaquete value3;
};

/* Definición del programa RPC */
program CLAVESRPC_PROG {

    version CLAVESRPC_VERS {

        int DESTROY(void) = 1;              // borrar todo
        int SET_VALUE(RpcTupla) = 2;        // insertar
        RpcGetResponse GET_VALUE(RpcClave) = 3; // obtener
        int MODIFY_VALUE(RpcTupla) = 4;     // modificar
        int DELETE_KEY(RpcClave) = 5;       // borrar clave
        int EXIST(RpcClave) = 6;            // comprobar existencia

    } = 1;  // numero de version

} = 0x20000001; // ID del programa