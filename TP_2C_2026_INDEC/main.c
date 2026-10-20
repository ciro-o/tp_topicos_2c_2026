#include "DeclaracionDeFunciones.h"

//Macros


int main()
{
    SetConsoleOutputCP(CP_UTF8);

    //UTILS
    Transferencias t;
    Vector transferencias;
    Resumen_Pais *rp = NULL;
    Resumen_Continente vec[7];
    Intermedio_Arch inter;
    P_Continentes pc;
    int contPaises;


    //FIN UTILS

    int opcion;

    crearVector(&transferencias, sizeof(Transferencias), 256);
    if(!cargarTransferencias(&transferencias, "transferencias_personales_clean.csv"))
    {
        printf("Error al abrir transferencias_personales_clean.csv\n");
        destruirVector(&transferencias);
        return 1;
    }

    printf
    (   "1. Totales por país.\n"
        "2. Totales por continente.\n"
        "3. Saldo trimestral por país.\n"
        "4. Totales anuales y variación interanual.\n"
        "5. Ranking de países.\n"
        "6. Participación porcentual por continente.\n"
        "7. Generación del archivo intermedio transferencias_base_larga.csv.\n"
        "8. Promedio trimestral por país.\n"
        "9. Promedio trimestral por continente, operación y año.\n"
        "10. Salir.\n");

    printf("Elija una opcíon: ");
    scanf("%d", &opcion);

    switch (opcion)
    {
    case 1:
        MostrarArchivoPorPais(&t, 5411);
        MostrarCantOperacionesPorPais(&t, &rp);
        break;

    case 2:
        procesarContinentes(vec);
        mostrarVec(vec);
        break;

    case 3:
        OpcionSaldoTrimestralPorPais(&transferencias);
        break;

    case 4:
        OpcionTotalesAnuales(&transferencias);
        break;

    case 5:
        Ranking_pais(&t, &rp, &contPaises);
        break;

    case 6:
        procesarContinentes(vec);
        GenerarArchivoParticipacion(vec);
        MostrarParticipacionDesdeArchivo();
        break;

    case 7:
        GenerarArchivoIntermedio(&t, &pc, &inter);
        break;

    case 8:
        OpcionPromedioTrimestralPorPais();
        break;

    case 9:

        break;

    case 10:

        break;
    }


    destruirVector(&transferencias);
    return 0;
}
