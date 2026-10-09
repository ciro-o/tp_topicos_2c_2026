#include "DeclaracionDeFunciones.h"

//Macros


int main()
{
    SetConsoleOutputCP(CP_UTF8);

    //UTILS
    Transferencias t;
    Resumen_Pais rp;
    int cont = 0;
    int *Contador_Registros = &cont; //almacena la cantidad de registros del archivo de operaciones se usa en MostrarArchivo();

    //FIN UTILS

    int opcion;
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
        ProcesarContinentes();
        break;

    case 3:
        
        break;

    case 4:
        //printf("opcion %d", opcion);
        break;

    case 5:
        //printf("opcion %d", opcion);
        break;

    case 6:
        //printf("opcion %d", opcion);
        break;

    case 7:
        //printf("opcion %d", opcion);
        break;

    case 8:
        //printf("opcion %d", opcion);
        break;

    case 9:
        //printf("opcion %d", opcion);
        break;

    case 10:
        //printf("opcion %d", opcion);
        break;
    }



    return 0;
}
