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
//----------- Macros -----------
//Punto 3 y 4: anchos de columnas y tamanos de los textos de las tablas
#define CANT_TRIMESTRES 4
#define ANCHO_COL_PAIS 42
#define ANCHO_COL_SALDO 12
#define TAM_NOMBRE_ARCHIVO 40
#define TAM_ETIQUETA 8
#define ANCHO_COL_ANIO 6
#define ANCHO_COL_MONTO 10
#define ANCHO_COL_VARIACION 14

//Punto 8
#define ANCHO_COL_COD 6
#define ANCHO_COL_OP 11
#define ANCHO_COL_TRIM 12
#define ANCHO_COL_PROM 10
//---------------- Structs ---------------

//•	paises_continentes.csv
typedef struct
{
    char pais_cod[3];
    char pais_desc[64];
    char continente[20];
} P_Continentes;

//•	transferencias_personales_clean.csv
typedef struct
{
    int anio;
    int trimestre;
    P_Continentes p;
    //char pais_cod[3];
    //char pais_desc[40];
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
    char pais_desc[64];
    long registros;
    double total_credito;
    double total_debito;
} Resumen_Pais;

typedef struct {
    char pais_cod[3];
    char pais_desc[64];
    float monto;
} OrdenMonto;

//Para el punto 7
typedef struct
{
    char continente[20];
    float porc_credito;
    float porc_debito;
} Participacion_Continente;

//punto 7
typedef struct{
    int anio;
    int trimestre;
    char pais_cod[3];
    char pais_desc[64];
    char continente[20];
    char operacion[2];
    double monto;
}Intermedio_Arch;

//Para el punto 3: acumulador de un pais para el anio pedido (un monto por trimestre)
typedef struct
{
    P_Continentes p; //reutiliza la estructura del pais (codigo, nombre y continente)
    double creditos[CANT_TRIMESTRES];
    double debitos[CANT_TRIMESTRES];
} SaldoPais;

//Para el punto 4: acumulador de un anio (total de creditos y de debitos)
typedef struct
{
    int anio;
    double creditos;
    double debitos;
} TotalAnual;

//Para el punto 8: promedio trimestral de un pais por tipo de operacion ([0] = credito, [1] = debito)
typedef struct
{
    char pais_cod[3];
    char pais_desc[64];
    bool hay[2];       //si el pais tiene registros de esa operacion en el archivo
    double suma[2];    //monto acumulado de los trimestres con monto mayor que cero
    int trimestres[2]; //cantidad de trimestres con monto mayor que cero
} PromedioPais;

//--------- Funciones ----------
typedef void (*Accion)(void*);
void parsearCont(char *linea, P_Continentes *reg);
bool parsear(char *linea, Transferencias *reg);

//TDA
void *obtenerOInsertarEnOrden(Vector *v, const void *elem, int (*cmp)(const void*, const void*));
bool ordenarInsercion(void *base, size_t cant, size_t tam, int (*cmp)(const void*, const void*));

//Utilidades
int contarCaracteres(const char *texto);
bool mostrarArchivoTexto(const char *nombArch);

//TDA Vector.
void crearVector(Vector *v, size_t tamElem, int cap);
void destruirVector(Vector *v);
bool redimensionar(Vector *v, size_t nuevaCap);
void insertarAlFinal(Vector *v, void *elem);
void vectorRecorrer( const Vector * v, Accion accion);

//1) Leer archivos de operaciones (Transferencias personales)
void MostrarArchivo(Transferencias *t);
//2) Agrupar información por pais (por código de país capaz) (vector dinamico)
void MostrarArchivoPorPais(Transferencias *t, int );
//3) Contar cantidad de operaciones por país agrupados (por código de país capaz) (vector dinamico)
void MostrarCantOperacionesPorPais(Transferencias *t, Resumen_Pais **rp);
//4) Sumar el importe de todas las operaciones Debito (por código de país capaz) (vector dinamico)
void ImporteDebitoPorPais(Transferencias *t, Resumen_Pais **rp);
//5) Sumar el importe de todas las operaciones Credito (por código de país capaz) (vector dinamico)
void ImporteCreditoPorPais(Transferencias *t, Resumen_Pais **rp);
//6) Ordenar de mayor a menor según la columna monto
void OrdenarPorMonto(Transferencias *t, OrdenMonto *m);

// Punto 2
void procesarContinentes(Resumen_Continente *vec);
P_Continentes *buscarPorPaisDesc(const Vector *v, const char *pais_desc);
Resumen_Continente *buscarEnResumen(Resumen_Continente *inicio, Resumen_Continente *fin, const char *continente);
void inicializarResumen(Resumen_Continente *vec);
void cargarVec(Vector *v, char *nombArch);
void mostrarVec(Resumen_Continente *vec);
// Punto 3
bool cargarTransferencias(Vector *v, const char *nombArch); //carga general del CSV principal: la usa main una sola vez
int cmpSaldoPais(const void *a, const void *b);
bool acumularSaldosPorPais(const Vector *transf, int anio, Vector *saldos);
void imprimirSaldos(FILE *destino, const Vector *saldos, int anio);
bool pedirAnio(int *anio);
void OpcionSaldoTrimestralPorPais(const Vector *transf);

// Punto 4
int cmpTotalAnual(const void *a, const void *b);
bool acumularTotalesPorAnio(const Vector *transf, Vector *totales);
void imprimirTotalesAnuales(const Vector *totales);
void OpcionTotalesAnuales(const Vector *transf);
// Punto 5
void Ranking_pais(Transferencias *t, Resumen_Pais **rp, int *cantPaises);
// Punto 6
void GenerarArchivoParticipacion(Resumen_Continente *vec);
int compararPorContinente(const void *a, const void *b);
void MostrarParticipacionDesdeArchivo();
// Punto 7
void GenerarArchivoIntermedio(Transferencias *t, P_Continentes *pc, Intermedio_Arch *inter);
// Punto 8
bool parsearIntermedio(char *linea, Intermedio_Arch *reg);
int cmpPaisPromedio(const void *a, const void *b);
int cmpPromedioCredito(const void *a, const void *b);
double promedioTrimestral(const PromedioPais *p, int op);
bool cargarPromedios(const char *nombArch, Vector *promedios);
void imprimirPromedios(const Vector *promedios);
void OpcionPromedioTrimestralPorPais();
// Punto 9

// Punto 10
void Menu();

#endif // DECLARACIONDEFUNCIONES_H_INCLUDED
