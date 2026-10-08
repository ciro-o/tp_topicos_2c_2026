#ifndef DECLARACIONDEFUNCIONES_H_INCLUDED
#define DECLARACIONDEFUNCIONES_H_INCLUDED

#define TAMLINEA 100
#define ERROR -1

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
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

//Para el punto 2
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
} Vector;

//para el punto 1.3 que cuenta registros y montos
typedef struct
{
    char pais_cod[3];
    char pais_desc[20];
    long registros;
    double total_credito;
    double total_debito;
} Resumen_Pais;

//--------- Funciones ----------
typedef void (*Accion)(void*);

//TDA Vector.
void crearVector(Vector *v, size_t tamElem, int cap);
void destruirVector(Vector *v);
bool redimensionar(Vector *v, size_t nuevaCap);
void insertarAlFinal(Vector *v, void *elem);
void vectorRecorrer( const Vector * v, Accion accion);

//1) Leer archivos de operaciones (Transferencias personales)
void MostrarArchivo(Transferencias *t, int *);
//2) Agrupar información por pais (por código de país capaz) (vector dinamico)
void MostrarArchivoPorPais(Transferencias *t, int );
//3) Contar cantidad de operaciones por país agrupados (por código de país capaz) (vector dinamico)
void MostrarCantOperacionesPorPais(Transferencias *t, Resumen_Pais *rp);
//4) Sumar el importe de todas las operaciones Debito (por código de país capaz) (vector dinamico)
void ImporteDebitoPorPais(Transferencias *t);
//5) Sumar el importe de todas las operaciones Credito (por código de país capaz) (vector dinamico)
void ImporteCreditoPorPais(Transferencias *t);
//6) Ordenar de mayor a menor según la columna monto
void OrdenarPorMonto(Transferencias *t);

// Punto 2
void ProcesarContinentes();
P_Continentes *buscarPorPaisDesc(const Vector *v, const char *pais_desc);
Resumen_Continente *buscarEnResumen(Resumen_Continente *inicio, Resumen_Continente *fin, const char *continente);
void cargarVec(Vector *v, char *nombArch);
void MostrarContinentes(void *elem);
// Punto 3
// Punto 4
// Punto 5
// Punto 6
// Punto 7
// Punto 8
// Punto 9
// Punto 10

#endif // DECLARACIONDEFUNCIONES_H_INCLUDED
