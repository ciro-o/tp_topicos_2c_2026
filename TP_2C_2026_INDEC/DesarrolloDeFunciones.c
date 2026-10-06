#include "DeclaracionDeFunciones.h"

void MostrarArchivo(Transferencias *t, int* Contador_Registros){

    FILE *file1 = fopen("transferencias_personales_clean.csv", "r");
    if(file1 == NULL){
        printf("Error al abrir archivo\n");
        return;
    }

    char linea[256];
    fgets(linea, sizeof(linea), file1);
    while(fgets(linea, sizeof(linea), file1)) {
        // parsear CSV: anio, trimestre, pais_cod, pais_des, operacion, monto
        sscanf(linea, "%d;%d;%2s;%19[^;];%1s;%f",
               &t->anio,
               &t->trimestre,
               t->pais_cod,
               t->pais_desc,
               t->operacion,
               &t->monto);

        (*Contador_Registros)++;

        printf("Año: %d | Trimestre: %d | País: %s | Nombre Pais: %s \t\t| Operación: %s | Monto: %.2f\n",
               t->anio, t->trimestre, t->p.pais_cod, t->p.pais_desc, t->operacion, t->monto);
    }

    printf("\n Contador: %d \n", *(Contador_Registros));

    fclose(file1);
}

void MostrarArchivoPorPais(Transferencias *t, int cont){

        //malloc
        Transferencias *puntero;

        puntero = malloc(sizeof(Transferencias) * cont);

        FILE *file1 = fopen("transferencias_personales_clean.csv", "r");
            if(file1 == NULL){
                printf("Error al abrir archivo\n");
        return;
        }
        int aux = 0;
        char linea[256];
        fgets(linea, sizeof(linea), file1);
        while(fgets(linea, sizeof(linea), file1)) {
            // parsear CSV: anio, trimestre, pais_cod, pais_des, operacion, monto
            sscanf(linea, "%d;%d;%2s;%19[^;];%1s;%f",
               &puntero[aux].anio,
               &puntero[aux].trimestre,
               puntero[aux].p.pais_cod,
               puntero[aux].p.pais_desc,
               puntero[aux].operacion,
               &puntero[aux].monto);

               printf("Año: %d | Trimestre: %d | País: %s | Nombre Pais: %s \t\t| Operación: %s | Monto: %.2f\n",
               puntero[aux].anio,
               puntero[aux].trimestre,
               puntero[aux].p.pais_cod,
               puntero[aux].p.pais_desc,
               puntero[aux].operacion,
               puntero[aux].monto);

                               aux++;

        }



        free(puntero);
        fclose(file1);

}
