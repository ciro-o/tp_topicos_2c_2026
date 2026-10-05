#ifndef DECLARACIONDEFUNCIONES_H_INCLUDED
#define DECLARACIONDEFUNCIONES_H_INCLUDED

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
//----------- Libreria Externa -----------
#include <windows.h>

//---------------- Structs ---------------

//•	transferencias_personales_clean.csv
typedef struct {

	int anio;
	int trimestre;

	//Aca entra la struct de P_Continentes
	//char pais_cod[3];
	//char pais_desc[20];
	//char continente[20];

	P_Continentes p;	

	char operacion[2]; //OPERACION toma los valores “C” para crédito y “D” para débito.
	float monto; //MONTO usa punto como separador decimal.

}Transferencias;

//•	paises_continentes.csv
typedef struct{

	char pais_cod[3];
	char pais_desc[20];
	char continente[20];
}P_Continentes;

#endif // DECLARACIONDEFUNCIONES_H_INCLUDED
