#include "DeclaracionDeFunciones.h"

// Macros

int main()
{
    SetConsoleOutputCP(CP_UTF8);

    // UTILS
    Transferencias t;
    Vector transferencias;
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
        return 1;
    }

    Menu(&t, &m, vec, &transferencias, &rp, &contPaises, &pc, &inter);

    destruirVector(&transferencias);
    return 0;
}
