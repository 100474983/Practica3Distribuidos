/*
 * Stub de cliente generado automáticamente por rpcgen.
 * NO debe modificarse.
 * Se encarga de convertir llamadas locales en llamadas remotas.
 */

#include <memory.h> /* for memset */
#include "clavesRPC.h"

/* Default timeout can be changed using clnt_control() */
static struct timeval TIMEOUT = { 25, 0 };

// función destroy 
enum clnt_stat 
destroy_1(int *clnt_res, CLIENT *clnt)
{	
	// Llamada RPC para destruir el servidor, no se envía ningún argumento (NULL) y se espera un int como respuesta
	 return (clnt_call (clnt, DESTROY, (xdrproc_t) xdr_void, (caddr_t) NULL,
		(xdrproc_t) xdr_int, (caddr_t) clnt_res,
		TIMEOUT));

}

// función set_value
enum clnt_stat 
set_value_1(RpcTupla arg1, int *clnt_res,  CLIENT *clnt)
{	
	// Llamada RPC para establecer un valor, se envía una estructura RpcTupla y se espera un int como respuesta
	return (clnt_call(clnt, SET_VALUE,
		(xdrproc_t) xdr_RpcTupla, (caddr_t) &arg1,
		(xdrproc_t) xdr_int, (caddr_t) clnt_res,
		TIMEOUT));
}

// función get_value
enum clnt_stat 
get_value_1(RpcClave arg1, RpcGetResponse *clnt_res,  CLIENT *clnt)
{	
	// Llamada RPC para obtener un valor, se envía una estructura RpcClave y se espera una estructura RpcGetResponse como respuesta
	return (clnt_call(clnt, GET_VALUE,
		(xdrproc_t) xdr_RpcClave, (caddr_t) &arg1,
		(xdrproc_t) xdr_RpcGetResponse, (caddr_t) clnt_res,
		TIMEOUT));
}

// función modify_value
enum clnt_stat 
modify_value_1(RpcTupla arg1, int *clnt_res,  CLIENT *clnt)
{	
	// Llamada RPC para modificar un valor, se envía una estructura RpcTupla y se espera un int como respuesta
	return (clnt_call(clnt, MODIFY_VALUE,
		(xdrproc_t) xdr_RpcTupla, (caddr_t) &arg1,
		(xdrproc_t) xdr_int, (caddr_t) clnt_res,
		TIMEOUT));
}

// función delete_key
enum clnt_stat 
delete_key_1(RpcClave arg1, int *clnt_res,  CLIENT *clnt)
{	
	// Llamada RPC para eliminar una clave, se envía una estructura RpcClave y se espera un int como respuesta
	return (clnt_call(clnt, DELETE_KEY,
		(xdrproc_t) xdr_RpcClave, (caddr_t) &arg1,
		(xdrproc_t) xdr_int, (caddr_t) clnt_res,
		TIMEOUT));
}

// función exist
enum clnt_stat 
exist_1(RpcClave arg1, int *clnt_res,  CLIENT *clnt)
{
	// Llamada RPC para verificar la existencia de una clave, se envía una estructura RpcClave y se espera un int como respuesta
	return (clnt_call(clnt, EXIST,
		(xdrproc_t) xdr_RpcClave, (caddr_t) &arg1,
		(xdrproc_t) xdr_int, (caddr_t) clnt_res,
		TIMEOUT));
}
