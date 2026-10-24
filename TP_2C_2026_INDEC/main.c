#include "DeclaracionDeFunciones.h"

// Macros

int main()
{
    SetConsoleOutputCP(CP_UTF8);

    // UTILS
    Transferencias t;
    Vector transferencias;
    Vector contientes;
    Resumen_Pais *rp = NULL;
    Resumen_Continente vec[7];
    Intermedio_Arch inter;
    OrdenMonto m;
    P_Continentes pc;
    int contPaises;

    // FIN UTILS

    crearVector(&transferencias, sizeof(Transferencias), 256);
    if (!cargarTransferencias(&transferencias, "transferencias_personales_clean.csv"))
    {
        printf("Error al abrir transferencias_personales_clean.csv\n");
        destruirVector(&transferencias);
        return ERROR;
    }

    crearVector(&contientes, sizeof(P_Continentes), 10);
    if (!cargarContinentes(&contientes, "paises_continentes.csv"))
    {
        printf("Error al abrir paises_continentes.csv\n");
        destruirVector(&transferencias);
        destruirVector(&contientes);
        return ERROR;
    }

    Menu(&t, &m, vec, &transferencias, &contientes, &rp, &contPaises, &pc, &inter);

    destruirVector(&transferencias);
    destruirVector(&contientes);

    return 0;
}
