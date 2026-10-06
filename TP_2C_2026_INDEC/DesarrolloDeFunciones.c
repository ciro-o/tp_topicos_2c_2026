#include "DeclaracionDeFunciones.h"

void MostrarArchivo(Transferencias *t){

    FILE *file1 = fopen("transferencias_personales_clean.csv", "r");
    if(file1 == NULL){
        printf("Error al abrir archivo\n");
        return;
    }

    char linea[256];

    while(fgets(linea, sizeof(linea), file1)) {
        // parsear CSV: anio, trimestre, pais_cod, pais_des, operacion, monto
        sscanf(linea, "%d;%d;%2s;%19[^;];%1s;%f",
               &t->anio,
               &t->trimestre,
               t->p.pais_cod,
               t->p.pais_desc,
               t->operacion,
               &t->monto);

        printf("Año: %d | Trimestre: %d | País: %s | Nombre Pais: %s \t\t| Operación: %s | Monto: %.2f\n",
               t->anio, t->trimestre, t->p.pais_cod, t->p.pais_desc, t->operacion, t->monto);
    }

    fclose(file1);
}

void MostrarArchivoPorPais(Transferencias *t){


}
