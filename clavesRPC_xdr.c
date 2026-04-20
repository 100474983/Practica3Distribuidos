/*
 * Funciones de serialización/deserialización XDR generadas por rpcgen.
 * NO deben modificarse.
 * Permiten convertir estructuras C a formato de red y viceversa.
 */

#include "clavesRPC.h"
// Estas funciones se encargan de serializar y deserializar las estructuras RpcPaquete, RpcTupla, RpcClave y RpcGetResponse 
// para su transmisión a través de la red en las llamadas RPC. 
// Cada función utiliza las funciones de serialización/deserialización de XDR para cada campo de la estructura correspondiente.
bool_t
xdr_RpcPaquete (XDR *xdrs, RpcPaquete *objp)
{	
	// Variables locales para almacenar los datos de la estructura RpcPaquete durante la serialización/deserialización
	register int32_t *buf;
	// Serializa/deserializa cada campo de la estructura RpcPaquete utilizando las funciones de XDR correspondientes
	 if (!xdr_int (xdrs, &objp->x))
		 return FALSE;
	 if (!xdr_int (xdrs, &objp->y))
		 return FALSE;
	 if (!xdr_int (xdrs, &objp->z))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_RpcTupla (XDR *xdrs, RpcTupla *objp)
{	
	// Variables locales para almacenar los datos de la estructura RpcTupla durante la serialización/deserialización
	register int32_t *buf;
	int i;
	// Serializa/deserializa cada campo de la estructura RpcTupla utilizando las funciones de XDR correspondientes
	 if (!xdr_string (xdrs, &objp->key, 256))
		 return FALSE;
	 if (!xdr_string (xdrs, &objp->value1, 256))
		 return FALSE;
	 if (!xdr_int (xdrs, &objp->N_value2))
		 return FALSE;
	 if (!xdr_vector (xdrs, (char *)objp->V_value2, 32,
		sizeof (float), (xdrproc_t) xdr_float))
		 return FALSE;
	 if (!xdr_RpcPaquete (xdrs, &objp->value3))
		 return FALSE;
	return TRUE;
}


bool_t
xdr_RpcClave (XDR *xdrs, RpcClave *objp)
{	
	// Variables locales para almacenar los datos de la estructura RpcClave durante la serialización/deserialización
	register int32_t *buf;

	// Serializa/deserializa el campo key de la estructura RpcClave utilizando la función de XDR correspondiente
	 if (!xdr_string (xdrs, &objp->key, 256))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_RpcGetResponse (XDR *xdrs, RpcGetResponse *objp)
{	
	// Variables locales para almacenar los datos de la estructura RpcGetResponse durante la serialización/deserialización
	register int32_t *buf;

	int i;
	// Serializa/deserializa cada campo de la estructura RpcGetResponse utilizando las funciones de XDR correspondientes
	 if (!xdr_int (xdrs, &objp->status))
		 return FALSE;
	 if (!xdr_string (xdrs, &objp->value1, 256))
		 return FALSE;
	 if (!xdr_int (xdrs, &objp->N_value2))
		 return FALSE;
	 if (!xdr_vector (xdrs, (char *)objp->V_value2, 32,
		sizeof (float), (xdrproc_t) xdr_float))
		 return FALSE;
	 if (!xdr_RpcPaquete (xdrs, &objp->value3))
		 return FALSE;
	return TRUE;
}
