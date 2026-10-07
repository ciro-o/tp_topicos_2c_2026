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
               t->p.pais_cod,
               t->p.pais_desc,
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
        Transferencias temp;

        // Este es para leer todo el documento
        puntero = malloc(sizeof(Transferencias) * cont);
        if(puntero == NULL){
            printf("No hay memoria");
            return;
        }

        FILE *file1 = fopen("transferencias_personales_clean.csv", "r");
            if(file1 == NULL){
                printf("Error al abrir archivo\n");
        return;
        }
        //Aca tengo el archivo en vector
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

               /* printf("Año: %d | Trimestre: %d | País: %s | Nombre Pais: %s \t\t| Operación: %s | Monto: %.2f\n",
               puntero[aux].anio,
               puntero[aux].trimestre,
               puntero[aux].p.pais_cod,
               puntero[aux].p.pais_desc,
               puntero[aux].operacion,
               puntero[aux].monto); */

            aux++;
        }

        int j=0;
        int k=0;
        for(k=0; k < aux -1; k++){
                for(j=1; j<aux; j++){
                    if (strcmp(puntero[j-1].p.pais_cod, puntero[j].p.pais_cod) > 0) {
                    temp = puntero[j];
                    puntero[j] = puntero[j-1];
                    puntero[j-1] = temp;
                }
        }

        }
    
        //printeo ordenado
        for(j=0; j<aux; j++){
            printf("Año: %d | Trimestre: %d | País: %s | Nombre Pais: %s \t\t| Operación: %s | Monto: %.2f\n",
               puntero[j].anio,
               puntero[j].trimestre,
               puntero[j].p.pais_cod,
               puntero[j].p.pais_desc,
               puntero[j].operacion,
               puntero[j].monto);
        }
   
       


        free(puntero);
        fclose(file1);

}

void crearVector(tdaVector *v, size_t tamElem, int cap)
{
    v->vec = (void*)malloc(tamElem * cap);
    if(!v){
        printf("Error en la memoria\n");
        exit(ERROR);
    }

    v->cantElem = 0;
    v->tamElem = tamElem;
    v->cap = cap;
}

void destruirVector(tdaVector *v)
{
    free(v->vec);
}

bool redimensionar(tdaVector *v, size_t nuevaCap)
{
    void *nVec = (void*)realloc(v->vec, nuevaCap * v->tamElem);
    if(!nVec)
        return false;

    v->vec = nVec;
    v->cap = nuevaCap;

    return true;
}

void insertarAlFinal(tdaVector *v, void *elem)
{
    if(v->cantElem == v->cap){
        if(!redimensionar(v, v->tamElem * 2)){
            printf("Error en la memoria\n");
            exit(ERROR);
        }
    }

    void *ult = v->vec + (v->cantElem*v->tamElem);
    memcpy(ult, elem, v->tamElem);
    v->cantElem++;
}

