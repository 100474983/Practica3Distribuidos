/*
 * Implementación del servidor RPC.
 * Este archivo conecta las llamadas remotas con la lógica local (claves.c).
 */

#include <string.h>
#include "clavesRPC.h"
#include "claves.h"

bool_t
destroy_1_svc(int *result, struct svc_req *rqstp)
{
	(void) rqstp;			// parametro no utilizado

	*result = destroy();	// Llama a la función local destroy (la de claves.h) y almacena el resultado en result
	return TRUE; // Indica que la llamada RPC se ha procesado correctamente
}

bool_t
set_value_1_svc(RpcTupla arg1, int *result, struct svc_req *rqstp)
{
	struct Paquete p;	// Estructura local para almacenar los valores del paquete

	(void) rqstp;
	
	// Copia los valores del paquete desde la estructura RpcTupla a la estructura local Paquete
	p.x = arg1.value3.x;
	p.y = arg1.value3.y;
	p.z = arg1.value3.z;

	// Llama a la función local set_value con los valores de la estructura RpcTupla y almacena el resultado en result
	*result = set_value(arg1.key, arg1.value1, arg1.N_value2, arg1.V_value2, p);
	return TRUE; // Indica que la llamada RPC se ha procesado correctamente
}

bool_t
get_value_1_svc(RpcClave arg1, RpcGetResponse *result, struct svc_req *rqstp)
{	// Variables locales para almacenar los resultados de la función get_value
	char value1[256];
	int n_value2;
	float v_value2[32];
	struct Paquete p;
	int i;
	int status;

	(void) rqstp;
	// Inicializa las variables locales para evitar valores basura en caso de error
	memset(value1, 0, sizeof(value1));
	memset(v_value2, 0, sizeof(v_value2));
	memset(&p, 0, sizeof(p));

	// Llama a la función local get_value con los valores de la estructura RpcClave y almacena el resultado en status
	status = get_value(arg1.key, value1, &n_value2, v_value2, &p);

	// Almacena el estado de la operación en el campo status de la estructura RpcGetResponse
	result->status = status;

	// Si la operación fue exitosa (status == 0), copia los valores obtenidos a la estructura RpcGetResponse
	if (status == 0) {
		// Si value1 ya tiene un valor asignado, libera la memoria antes de asignar el nuevo valor
		if (result->value1 != NULL) {
			free(result->value1);
			result->value1 = NULL;
		}
		// Asigna el nuevo valor a value1 utilizando strdup para duplicar la cadena y evitar problemas de memoria
		result->value1 = strdup(value1);
		if (result->value1 == NULL) {
			result->status = -1;
			return TRUE;
		}

		// Copia el número de elementos en N_value2 y los valores del array V_value2 a la estructura RpcGetResponse
		result->N_value2 = n_value2;

		for (i = 0; i < 32; i++) {
			result->V_value2[i] = 0.0f;
		}
		for (i = 0; i < n_value2 && i < 32; i++) {
			result->V_value2[i] = v_value2[i];
		}

		// Copia los valores del paquete a la estructura RpcGetResponse
		result->value3.x = p.x;
		result->value3.y = p.y;
		result->value3.z = p.z;
	} else {
		// Si la operación no fue exitosa, asegura que los campos de la estructura RpcGetResponse estén vacíos o en un estado seguro
		if (result->value1 != NULL) {
			free(result->value1);
			result->value1 = NULL;
		}

		result->N_value2 = 0;
		for (i = 0; i < 32; i++) {
			result->V_value2[i] = 0.0f;
		}
		result->value3.x = 0;
		result->value3.y = 0;
		result->value3.z = 0.0;
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
	return TRUE;
}

bool_t
delete_key_1_svc(RpcClave arg1, int *result, struct svc_req *rqstp)
{
	(void) rqstp;

	// Llama a la función local delete_key con el valor de la clave y almacena el resultado en result
	*result = delete_key(arg1.key);
	return TRUE;
}

bool_t
exist_1_svc(RpcClave arg1, int *result, struct svc_req *rqstp)
{
	(void) rqstp;

	// Llama a la función local exist con el valor de la clave y almacena el resultado en result
	*result = exist(arg1.key);
	return TRUE;
}

int
clavesrpc_prog_1_freeresult(SVCXPRT *transp, xdrproc_t xdr_result, caddr_t result)
{	
	// Esta función se encarga de liberar la memoria utilizada para almacenar los resultados de las llamadas RPC. 
	// Se llama automáticamente después de que se ha enviado la respuesta al cliente.
	(void) transp;
	xdr_free(xdr_result, result);
	return 1;
}