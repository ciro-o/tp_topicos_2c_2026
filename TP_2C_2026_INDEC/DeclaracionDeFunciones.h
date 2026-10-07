#ifndef DECLARACIONDEFUNCIONES_H_INCLUDED
#define DECLARACIONDEFUNCIONES_H_INCLUDED

#define TAMLINEA 100
#define ERROR -1

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
    P_Continentes p;
    //char pais_cod[3];
    //char pais_desc[20];
    char operacion[2]; //OPERACION toma los valores “C” para crédito y “D” para débito.
    float monto; //MONTO usa punto como separador decimal.

} Transferencias;

typedef struct
{
    char continente[20];
    long registros;
    double total_credito;
    double total_debito;
} Resumen_Continente;

typedef struct
{
    void *vec;
    size_t tamElem;
    int cantElem;
    int cap;
} tdaVector;

//--------- Funciones ----------

//1) Leer archivos de operaciones (Transferencias personales)
void MostrarArchivo(Transferencias *t, int *);
//2) Agrupar información por pais (por código de país capaz) (vector dinamico)
void MostrarArchivoPorPais(Transferencias *t, int );
//3) Contar cantidad de operaciones por país agrupados (por código de país capaz) (vector dinamico)
void MostrarCantOperacionesPorPais(Transferencias *t);
//4) Sumar el importe de todas las operaciones Debito (por código de país capaz) (vector dinamico)
void ImporteDebitoPorPais(Transferencias *t);
//5) Sumar el importe de todas las operaciones Credito (por código de país capaz) (vector dinamico)
void ImporteCreditoPorPais(Transferencias *t);
//6) Ordenar de mayor a menor según la columna monto
void OrdenarPorMonto(Transferencias *t);

<<<<<<< HEAD
// Punto 2
// Punto 3
// Punto 4
// Punto 5
// Punto 6
// Punto 7
// Punto 8
// Punto 9
// Punto 10
=======
//TDA Vector
void crearVector(tdaVector *v, size_t tamElem, int cap);
void destruirVector(tdaVector *v);
bool redimensionar(tdaVector *v, size_t nuevaCap);
void insertarAlFinal(tdaVector *v, void *elem);

void ProcesarContinentes();
void MostrarContinentes(Resumen_Continente *v);
void cargarVec(tdaVector *v, char *nombArch);
>>>>>>> 5e7437ab0d31de5a55e0ed4cc1e50b961443e46e

#endif // DECLARACIONDEFUNCIONES_H_INCLUDED
