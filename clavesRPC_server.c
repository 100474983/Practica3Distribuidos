/*
 * Implementación del servidor RPC.
 * Este archivo conecta las llamadas remotas con la lógica local (claves.c).
 */

#include <stdio.h>
#include <string.h>
#include "clavesRPC.h"
#include "claves.h"

/* Buffers estáticos para get_value: evitan double-free con xdr_free */
static char get_value1_buf[256];
static RpcGetResponse get_response_buf;

bool_t
destroy_1_svc(int *result, struct svc_req *rqstp)
{
    (void) rqstp;     // parametro no utilizado
    *result = destroy();   // Llama a la función local destroy (la de claves.h) y almacena el resultado en result
    printf("[SERVIDOR] destroy() -> %d\n", *result);
    return TRUE;  // Indica que la llamada RPC se ha procesado correctamente
}

bool_t
set_value_1_svc(RpcTupla arg1, int *result, struct svc_req *rqstp)
{
    struct Paquete p;  // Estructura local para almacenar los valores del paquete
    (void) rqstp;

    // Copia los valores del paquete desde la estructura RpcTupla a la estructura local Paquete
    p.x = arg1.value3.x;
    p.y = arg1.value3.y;
    p.z = arg1.value3.z;

    // Llama a la función local set_value con los valores de la estructura RpcTupla y almacena el resultado en result
    *result = set_value(arg1.key, arg1.value1, arg1.N_value2, arg1.V_value2, p);
    printf("[SERVIDOR] set_value(key=%s) -> %d\n", arg1.key, *result);
    return TRUE; // Indica que la llamada RPC se ha procesado correctamente
}

bool_t
get_value_1_svc(RpcClave arg1, RpcGetResponse *result, struct svc_req *rqstp)
{   // Variables locales para almacenar los resultados de la función get_value
    char value1[256];
    int n_value2;
    float v_value2[32];
    struct Paquete p;
    int i, status;

    (void) rqstp;
	// Inicializa las variables locales para evitar valores basura en caso de error
    memset(value1, 0, sizeof(value1));
    memset(v_value2, 0, sizeof(v_value2));
    memset(&p, 0, sizeof(p));
    memset(get_value1_buf, 0, sizeof(get_value1_buf));

	// Llama a la función local get_value con los valores de la estructura RpcClave y almacena el resultado en status
    status = get_value(arg1.key, value1, &n_value2, v_value2, &p);
    printf("[SERVIDOR] get_value(key=%s) -> %d\n", arg1.key, status);

    // Almacena el estado de la operación en el campo status de la estructura RpcGetResponse
    result->status = status;

	// Si la operación fue exitosa (status == 0), copia los valores obtenidos a la estructura RpcGetResponse
    if (status == 0) {
        strncpy(get_value1_buf, value1, 255);
        get_value1_buf[255] = '\0';
        result->value1  = get_value1_buf;   /* puntero a buffer estático, xdr_free no lo liberará */
        result->N_value2 = n_value2;
        for (i = 0; i < 32; i++) result->V_value2[i] = 0.0f;
        for (i = 0; i < n_value2 && i < 32; i++) result->V_value2[i] = v_value2[i];
        result->value3.x = p.x;
        result->value3.y = p.y;
        result->value3.z = p.z;
    } else {
        get_value1_buf[0] = '\0';
        result->value1   = get_value1_buf;
        result->N_value2 = 0;
        for (i = 0; i < 32; i++) result->V_value2[i] = 0.0f;
        result->value3.x = result->value3.y = result->value3.z = 0;
    }

    return TRUE;
}

bool_t
modify_value_1_svc(RpcTupla arg1, int *result, struct svc_req *rqstp)
{
    // Variables locales para almacenar los valores del paquete
    struct Paquete p;
    (void) rqstp;

	// Copia los valores del paquete desde la estructura RpcTupla a la estructura local Paquete
    p.x = arg1.value3.x;
    p.y = arg1.value3.y;
    p.z = arg1.value3.z;

	// Llama a la función local modify_value con los valores de la estructura RpcTupla y almacena el resultado en result
    *result = modify_value(arg1.key, arg1.value1, arg1.N_value2, arg1.V_value2, p);
    printf("[SERVIDOR] modify_value(key=%s) -> %d\n", arg1.key, *result);
    return TRUE;
}

bool_t
delete_key_1_svc(RpcClave arg1, int *result, struct svc_req *rqstp)
{
    (void) rqstp;

	// Llama a la función local delete_key con el valor de la clave y almacena el resultado en result
    *result = delete_key(arg1.key);
    printf("[SERVIDOR] delete_key(key=%s) -> %d\n", arg1.key, *result);
    return TRUE;
}

bool_t
exist_1_svc(RpcClave arg1, int *result, struct svc_req *rqstp)
{
    (void) rqstp;

	// Llama a la función local exist con el valor de la clave y almacena el resultado en result
    *result = exist(arg1.key);
    printf("[SERVIDOR] exist(key=%s) -> %d\n", arg1.key, *result);
    return TRUE;
}

int
clavesrpc_prog_1_freeresult(SVCXPRT *transp, xdrproc_t xdr_result, caddr_t result)
{
    (void) transp;
    /*
     * NO llamamos a xdr_free aquí porque result->value1 apunta
     * a un buffer estático, no a memoria del heap.
     * Llamar a xdr_free causaría double-free o free() sobre stack/BSS.
     */
    (void) xdr_result;
    (void) result;
    return 1;
}