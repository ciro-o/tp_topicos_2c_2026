#ifndef DECLARACIONDEFUNCIONES_H_INCLUDED
#define DECLARACIONDEFUNCIONES_H_INCLUDED

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
//----------- Libreria Externa -----------
#include <windows.h>

//---------------- Structs ---------------

//•	paises_continentes.csv
typedef struct
{
    char pais_cod[3];
    char pais_desc[20];
    char continente[20];
} P_Continentes;

//•	transferencias_personales_clean.csv
typedef struct
{
    int anio;
    int trimestre;

    //Aca entra la struct de P_Continentes
    //char pais_cod[3];
    //char pais_desc[20];
    //char continente[20];

    P_Continentes p;

    char operacion[2]; //OPERACION toma los valores “C” para crédito y “D” para débito.
    float monto; //MONTO usa punto como separador decimal.

} Transferencias;

//--------- Funciones ----------

//1) Leer archivos de operaciones (Transferencias personales)
void MostrarArchivo(Transferencias *t);
//2) Agrupar información por pais (por código de país capaz) (vector dinamico)
void MostrarArchivoPorPais(Transferencias *t);
//3) Contar cantidad de operaciones por país agrupados (por código de país capaz) (vector dinamico)
void MostrarCantOperacionesPorPais(Transferencias *t);
//4) Sumar el importe de todas las operaciones Debito (por código de país capaz) (vector dinamico)
void ImporteDebitoPorPais(Transferencias *t);
//5) Sumar el importe de todas las operaciones Credito (por código de país capaz) (vector dinamico)
void ImporteCreditoPorPais(Transferencias *t);
//6) Ordenar de mayor a menor según la columna monto
void OrdenarPorMonto(Transferencias *t);


#endif // DECLARACIONDEFUNCIONES_H_INCLUDED
