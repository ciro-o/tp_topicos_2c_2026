#include "DeclaracionDeFunciones.h"

//Macros


int main()
{
    SetConsoleOutputCP(CP_UTF8);

    //UTILS
    Transferencias t;
    Resumen_Pais *rp = NULL;
    Resumen_Continente vec[7];
    OrdenMonto m;
    Intermedio_Arch inter;
    P_Continentes pc;
    int cont = 0;
    int contPaises;
    int *Contador_Registros = &cont; //almacena la cantidad de registros del archivo de operaciones se usa en MostrarArchivo();

    //FIN UTILS

    int opcion;

    crearVector(&t, sizeof(Transferencias), 256);
    if(!cargarTransferencias(&t, "transferencias_personales_clean.csv"))
    {
        printf("Error al abrir transferencias_personales_clean.csv\n");
        destruirVector(&t);
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
        "10. Salir.\n"
        "11. Importe debito por pais.\n"
        "12. Importe credito por pais.\n"
        "13. Orden por Monto.\n");

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
        OpcionSaldoTrimestralPorPais(&t);
        break;

    case 4:
        OpcionTotalesAnuales(&t);
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



    return 0;
}
