#include "DeclaracionDeFunciones.h"


void MostrarArchivo(Transferencias *t, int* Contador_Registros)
{

    FILE *file1 = fopen("transferencias_personales_clean.csv", "r");
    if(file1 == NULL)
    {
        printf("Error al abrir archivo\n");
        return;
    }

    char linea[256];
    fgets(linea, sizeof(linea), file1);
    while(fgets(linea, sizeof(linea), file1))
    {
        // parsear CSV: anio, trimestre, pais_cod, pais_des, operacion, monto
        sscanf(linea, "%d;%d;%2s;%64[^;];%1s;%f",
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

//arreglar ya que el cont esta hardcodeado
void MostrarArchivoPorPais(Transferencias *t, int cont){

    //malloc
    //Transferencias *t;
    Transferencias temp;
    Transferencias *reg;

    // Este es para leer todo el documento
    t = malloc(sizeof(Transferencias) * cont);
    if(t == NULL)
    {
        printf("No hay memoria");
        return;
    }

    FILE *file1 = fopen("transferencias_personales_clean.csv", "r");
    if(file1 == NULL)
    {
        printf("Error al abrir archivo\n");
        return;
    }
    //Aca tengo el archivo en vector
    int aux = 0;
    char linea[256];
    fgets(linea, sizeof(linea), file1);
    while(fgets(linea, sizeof(linea), file1))
    {
        // parsear CSV: anio, trimestre, pais_cod, pais_des, operacion, monto
        sscanf(linea, "%d;%d;%2s;%64[^;];%1s;%f",
               &t[aux].anio,
               &t[aux].trimestre,
               t[aux].p.pais_cod,
               t[aux].p.pais_desc,
               t[aux].operacion,
               &t[aux].monto);

        aux++;
    }

    int k;
    //ordenamiento
    for (k = 0; k < aux - 1; k++)
    {
        for (reg = t + 1; reg < t + aux; reg++)
        {
            if (strcmp((reg-1)->p.pais_cod, reg->p.pais_cod) > 0)
            {
                temp = *reg;
                *reg = *(reg-1);
                *(reg-1) = temp;
            }
        }
    }

    FILE *salida = fopen("paises_ordenado.txt", "w");
    if (salida == NULL)
    {
        printf("No se pudo crear el archivo\n");
        return;
    }

    //Escritura de archivo

    for (reg = t; reg < t + aux; reg++)
    {
        fprintf(salida, "%d;%d;%s;%s;%s;%.2f\n",
                reg->anio, reg->trimestre, reg->p.pais_cod, reg->p.pais_desc, reg->operacion, reg->monto);
    }

    fclose(salida);

    //printeo ordenado
    /* for (reg = puntero; reg < puntero + aux; reg++)
    {
        printf("Año: %d | Trimestre: %d | País: %s | Nombre Pais: %s \t\t| Operación: %s | Monto: %.2f\n",
               reg->anio, reg->trimestre, reg->p.pais_cod, reg->p.pais_desc, reg->operacion, reg->monto);
    } */
    free(t);
    fclose(file1);

}

void MostrarCantOperacionesPorPais(Transferencias *t, Resumen_Pais **rp)
{

    FILE * file1;
    file1 = fopen("paises_ordenado.txt", "r");
    if(file1 == NULL)
    {
        printf("Archivo corrupto");
        return;
    }
    t = malloc(sizeof(Transferencias));
    if(t == NULL)
    {
        printf("No hay memoria");
        return;
    }

    //Aca tengo el archivo en vector
    int aux = 0;
    int capacidad = 1;
    char linea[256];
    fgets(linea, sizeof(linea), file1);
    while(fgets(linea, sizeof(linea), file1))
    {
        //realloc si esta lleno
        if(aux == capacidad)
        {
            capacidad++;
            t = realloc(t, sizeof(Transferencias) * capacidad);
            if(t == NULL)
            {
                printf("No hay memoria");
                return;
            }
        }

        // parsear CSV: anio, trimestre, pais_cod, pais_des, operacion, monto
        sscanf(linea, "%d;%d;%2s;%64[^;];%1s;%f",
               &t[aux].anio,
               &t[aux].trimestre,
               t[aux].p.pais_cod,
               t[aux].p.pais_desc,
               t[aux].operacion,
               &t[aux].monto);

        aux++;
    }

    //creo el arch de todos los paises y sus montos

    // ahora vuelvo a rp
    *rp = malloc(sizeof(Resumen_Pais));
    if(*rp == NULL)
    {
        printf("No hay memoria");
        return;
    }

    int cap = 1;
    int i=0;
    int estatico =0;
    int k=0;
    //---- primer registro todo cero -----
    strcpy((*rp)[0].pais_cod, t[0].p.pais_cod);
    strcpy((*rp)[0].pais_desc, t[0].p.pais_desc);
    (*rp)[0].registros = 0;
    (*rp)[0].total_credito = 0;
    (*rp)[0].total_debito = 0;
    //----- recorro los registros -----
    for(k=0; k<aux; k++)
    {
        if(strcmp(t[estatico].p.pais_cod, t[k].p.pais_cod) == 0)
        {
            //Vacio, sumo afuera
        }
        else
        {

            i++;
            estatico = k;

            if(i == cap)
            {
                cap++;
                *rp = realloc(*rp, sizeof(Resumen_Pais) * cap);
                if(*rp == NULL)
                {
                    printf("No hay memoria");
                    return;
                }
            }

            strcpy((*rp)[i].pais_cod, t[estatico].p.pais_cod);
            strcpy((*rp)[i].pais_desc, t[estatico].p.pais_desc);
            (*rp)[i].registros = 0;
            (*rp)[i].total_credito = 0;
            (*rp)[i].total_debito = 0;

        }

        (*rp)[i].registros = (*rp)[i].registros + 1;
        if(t[k].operacion[0] == 'C')
        {
            (*rp)[i].total_credito += t[k].monto;
        };
        if(t[k].operacion[0] == 'D')
        {
            (*rp)[i].total_debito += t[k].monto;
        };

    }

    fclose(file1);
    free(t);



    //imprimo el vector
    int j;
    for (j = 0; j <= i; j++)
    {
        printf("País: %s | Nombre Pais: %-34s | Cant. Operaciones: %4ld | Total Credito: %10.2f | Total Debito: %10.2f\n",
               (*rp)[j].pais_cod,
               (*rp)[j].pais_desc,
               (*rp)[j].registros,
               (*rp)[j].total_credito,
               (*rp)[j].total_debito);
    }

    free(*rp);
}

void ImporteDebitoPorPais(Transferencias *t, Resumen_Pais **rp){

    FILE * file1;
    file1 = fopen("paises_ordenado.txt", "r");
    if(file1 == NULL)
    {
        printf("Archivo corrupto");
        return;
    }
    t = malloc(sizeof(Transferencias));
    if(t == NULL)
    {
        printf("No hay memoria");
        return;
    }

    //Aca tengo el archivo en vector
    int aux = 0;
    int capacidad = 1;
    char linea[256];
    fgets(linea, sizeof(linea), file1);
    while(fgets(linea, sizeof(linea), file1))
    {
        //realloc si esta lleno
        if(aux == capacidad)
        {
            capacidad++;
            t = realloc(t, sizeof(Transferencias) * capacidad);
            if(t == NULL)
            {
                printf("No hay memoria");
                return;
            }
        }

        // parsear CSV: anio, trimestre, pais_cod, pais_des, operacion, monto
        sscanf(linea, "%d;%d;%2s;%64[^;];%1s;%f",
               &t[aux].anio,
               &t[aux].trimestre,
               t[aux].p.pais_cod,
               t[aux].p.pais_desc,
               t[aux].operacion,
               &t[aux].monto);

        aux++;
    }
    // ahora vuelvo a rp
    *rp = malloc(sizeof(Resumen_Pais));
    if(*rp == NULL)
    {
        printf("No hay memoria");
        return;
    }

    int cap = 1;
    int i=0;
    int estatico =0;
    int k=0;
    //---- primer registro todo cero -----
    strcpy((*rp)[0].pais_cod, t[0].p.pais_cod);
    strcpy((*rp)[0].pais_desc, t[0].p.pais_desc);
    (*rp)[0].registros = 0;
    (*rp)[0].total_credito = 0;
    (*rp)[0].total_debito = 0;
    //----- recorro los registros -----
    for(k=0; k<aux; k++)
    {
        if(strcmp(t[estatico].p.pais_cod, t[k].p.pais_cod) == 0)
        {
            //Vacio, sumo afuera
        }
        else
        {

            i++;
            estatico = k;

            if(i == cap)
            {
                cap++;
                *rp = realloc(*rp, sizeof(Resumen_Pais) * cap);
                if(*rp == NULL)
                {
                    printf("No hay memoria");
                    return;
                }
            }

            strcpy((*rp)[i].pais_cod, t[estatico].p.pais_cod);
            strcpy((*rp)[i].pais_desc, t[estatico].p.pais_desc);
            (*rp)[i].registros = 0;
            (*rp)[i].total_credito = 0;
            (*rp)[i].total_debito = 0;

        }

        (*rp)[i].registros = (*rp)[i].registros + 1;
        if(t[k].operacion[0] == 'C')
        {
            (*rp)[i].total_credito += t[k].monto;
        };
        if(t[k].operacion[0] == 'D')
        {
            (*rp)[i].total_debito += t[k].monto;
        };

    }

    fclose(file1);
    free(t);

    //imprimo el vector
    int j;
    for (j = 0; j <= i; j++)
    {
        printf("País: %s | Nombre Pais: %-34s | Total Debito: %10.2f\n",
               (*rp)[j].pais_cod,
               (*rp)[j].pais_desc,
               (*rp)[j].total_debito);
    }

    free(*rp);
}

void ImporteCreditoPorPais(Transferencias *t, Resumen_Pais **rp){

    FILE * file1;
    file1 = fopen("paises_ordenado.txt", "r");
    if(file1 == NULL)
    {
        printf("Archivo corrupto");
        return;
    }
    t = malloc(sizeof(Transferencias));
    if(t == NULL)
    {
        printf("No hay memoria");
        return;
    }

    //Aca tengo el archivo en vector
    int aux = 0;
    int capacidad = 1;
    char linea[256];
    fgets(linea, sizeof(linea), file1);
    while(fgets(linea, sizeof(linea), file1))
    {
        //realloc si esta lleno
        if(aux == capacidad)
        {
            capacidad++;
            t = realloc(t, sizeof(Transferencias) * capacidad);
            if(t == NULL)
            {
                printf("No hay memoria");
                return;
            }
        }

        // parsear CSV: anio, trimestre, pais_cod, pais_des, operacion, monto
        sscanf(linea, "%d;%d;%2s;%64[^;];%1s;%f",
               &t[aux].anio,
               &t[aux].trimestre,
               t[aux].p.pais_cod,
               t[aux].p.pais_desc,
               t[aux].operacion,
               &t[aux].monto);

        aux++;
    }
    // ahora vuelvo a rp
    *rp = malloc(sizeof(Resumen_Pais));
    if(*rp == NULL)
    {
        printf("No hay memoria");
        return;
    }

    int cap = 1;
    int i=0;
    int estatico =0;
    int k=0;
    //---- primer registro todo cero -----
    strcpy((*rp)[0].pais_cod, t[0].p.pais_cod);
    strcpy((*rp)[0].pais_desc, t[0].p.pais_desc);
    (*rp)[0].registros = 0;
    (*rp)[0].total_credito = 0;
    (*rp)[0].total_debito = 0;
    //----- recorro los registros -----
    for(k=0; k<aux; k++)
    {
        if(strcmp(t[estatico].p.pais_cod, t[k].p.pais_cod) == 0)
        {
            //Vacio, sumo afuera
        }
        else
        {

            i++;
            estatico = k;

            if(i == cap)
            {
                cap++;
                *rp = realloc(*rp, sizeof(Resumen_Pais) * cap);
                if(*rp == NULL)
                {
                    printf("No hay memoria");
                    return;
                }
            }

            strcpy((*rp)[i].pais_cod, t[estatico].p.pais_cod);
            strcpy((*rp)[i].pais_desc, t[estatico].p.pais_desc);
            (*rp)[i].registros = 0;
            (*rp)[i].total_credito = 0;
            (*rp)[i].total_debito = 0;

        }

        (*rp)[i].registros = (*rp)[i].registros + 1;
        if(t[k].operacion[0] == 'C')
        {
            (*rp)[i].total_credito += t[k].monto;
        };
        if(t[k].operacion[0] == 'D')
        {
            (*rp)[i].total_debito += t[k].monto;
        };

    }

    fclose(file1);
    free(t);

    //imprimo el vector
    int j;
    for (j = 0; j <= i; j++)
    {
        printf("País: %s | Nombre Pais: %-34s | Total Credito: %10.2f\n",
               (*rp)[j].pais_cod,
               (*rp)[j].pais_desc,
               (*rp)[j].total_credito);
    }

    free(*rp);
}

void OrdenarPorMonto(Transferencias *t, OrdenMonto *m){

    //aca tengo el archivo en el vector
    FILE * file1;
    file1 = fopen("paises_ordenado.txt", "r");
    if(file1 == NULL)
    {
        printf("Archivo corrupto");
        return;
    }
    t = malloc(sizeof(Transferencias));
    if(t == NULL)
    {
        printf("No hay memoria");
        return;
    }

    //Aca tengo el archivo en vector
    int aux = 0;
    int capacidad = 1;
    char linea[256];
    fgets(linea, sizeof(linea), file1);
    while(fgets(linea, sizeof(linea), file1))
    {
        //realloc si esta lleno
        if(aux == capacidad)
        {
            capacidad++;
            t = realloc(t, sizeof(Transferencias) * capacidad);
            if(t == NULL)
            {
                printf("No hay memoria");
                return;
            }
        }

        // parsear CSV: anio, trimestre, pais_cod, pais_des, operacion, monto
        sscanf(linea, "%d;%d;%2s;%64[^;];%1s;%f",
               &t[aux].anio,
               &t[aux].trimestre,
               t[aux].p.pais_cod,
               t[aux].p.pais_desc,
               t[aux].operacion,
               &t[aux].monto);

        aux++;
    }

    //ahora tengo que contar los montos por vector de cada pais, comparando el code pais

    m = malloc(sizeof(OrdenMonto));
    capacidad = 1;

    int i=0;
    int estatico =0;
    int k=0;
    //---- primer registro todo cero -----
    strcpy(m[0].pais_cod, t[0].p.pais_cod);
    strcpy(m[0].pais_desc, t[0].p.pais_desc);
    m[0].monto = 0;
    //----- recorro los registros -----
    for(k=0; k<aux; k++)
    {
        if(strcmp(t[estatico].p.pais_cod, t[k].p.pais_cod) == 0)
        {
            //Vacio, sumo afuera
        }
        else
        {
            i++;
            estatico = k;
            if(i == capacidad)
            {
                capacidad++;
                m = realloc(m, sizeof(OrdenMonto) * capacidad);
                if(m == NULL)
                {
                    printf("No hay memoria");
                    return;
                }
            }
            strcpy(m[i].pais_cod, t[estatico].p.pais_cod);
            strcpy(m[i].pais_desc, t[estatico].p.pais_desc);
            m[i].monto = 0;

        }
        if(t[k].operacion[0] == 'C')
        {
            m[i].monto += t[k].monto;
        };
        if(t[k].operacion[0] == 'D')
        {
            m[i].monto += t[k].monto;
        };

    }


    //despues ordeno el vector
    OrdenMonto maux;
    for(k=0; k < capacidad; k++){
       for(i=0; i < capacidad -1 ; i++){
        if(m[i].monto < m[i+1].monto){
            maux = m[i+1];
            m[i+1] = m[i];
            m[i] = maux;
            }
        }
    }

    //por ultimo imprimo el vector ordenado

    //imprimo el vector
    int j;
    for (j = 0; j <= i; j++)
    {
        printf("País: %s | Nombre Pais: %-34s | Total Monto: %10.2f\n",
               m[j].pais_cod,
               m[j].pais_desc,
               m[j].monto);
    }




}

void parsear(char *linea, Transferencias *reg)
{
    char *act;

    act = strchr(linea, '\n');
    if (act)
        *act = '\0';
    act = strchr(linea, '\r');
    if (act)
        *act = '\0';

    act = strrchr(linea, ';');
    sscanf(act + 1, "%f", &reg->monto);
    *act = '\0';

    act = strrchr(linea, ';');
    *reg->operacion = *(act + 1);
    *(reg->operacion + 1) = '\0';
    *act = '\0';

    act = strrchr(linea, ';');
    strncpy(reg->p.pais_desc, act + 1, sizeof(reg->p.pais_desc) - 1);
    reg->p.pais_desc[sizeof(reg->p.pais_desc) - 1] = '\0';
    *act = '\0';

    act = strrchr(linea, ';');
    strncpy(reg->p.pais_cod, act + 1, sizeof(reg->p.pais_cod) - 1);
    reg->p.pais_cod[sizeof(reg->p.pais_cod) - 1] = '\0';
    *act = '\0';

    act = strrchr(linea, ';');
    sscanf(act + 1, "%d", &reg->trimestre);
    *act = '\0';

    sscanf(linea, "%d", &reg->anio);
}

void parsearCont(char *linea, P_Continentes *reg)
{
    char *act;

    act = strchr(linea, '\n');
    if (act)
        *act = '\0';
    act = strchr(linea, '\r');
    if (act)
        *act = '\0';

    act = strrchr(linea, ';');
    strncpy(reg->continente, act + 1, sizeof(reg->continente) - 1);
    *(reg->continente + sizeof(reg->continente) - 1) = '\0';
    *act = '\0';

    act = strrchr(linea, ';');
    strncpy(reg->pais_desc, act + 1, sizeof(reg->pais_desc) - 1);
    *(reg->pais_desc + sizeof(reg->pais_desc) - 1) = '\0';
    *act = '\0';

    strncpy(reg->pais_cod, linea, sizeof(reg->pais_cod) - 1);
    *(reg->pais_cod + sizeof(reg->pais_cod) - 1) = '\0';
}

void crearVector(Vector *v, size_t tamElem, int cap)
{
    v->vec = (void*)malloc(tamElem * cap);
    if(!v->vec)
    {
        printf("Error en la memoria\n");
        exit(ERROR);
    }

    v->cantElem = 0;
    v->tamElem = tamElem;
    v->cap = cap;
}

void inicializarResumen(Resumen_Continente *vec)
{
    Resumen_Continente *r;
    const char *nombres[7] = {"Americas", "Europe", "Oceania", "Asia", "Africa", "Antarctica", "Indeterminado"};
    int i;
    for (i = 0; i < 7; i++)
    {
        r = vec + i;
        strcpy(r->continente, *(nombres + i));
        r->registros = 0;
        r->total_credito = 0.0;
        r->total_debito = 0.0;
    }
}

void destruirVector(Vector *v)
{
    free(v->vec);
}

bool redimensionar(Vector *v, size_t nuevaCap)
{
    void *nVec = (void*)realloc(v->vec, nuevaCap * v->tamElem);
    if(!nVec)
        return false;

    v->vec = nVec;
    v->cap = nuevaCap;

    return true;
}

void insertarAlFinal(Vector *v, void *elem)
{
    if(v->cantElem == v->cap)
    {
        if(!redimensionar(v, v->cap * 2))
        {
            printf("Error en la memoria\n");
            exit(ERROR);
        }
    }

    void *ult = v->vec + (v->cantElem*v->tamElem);
    memcpy(ult, elem, v->tamElem);
    v->cantElem++;
}

void vectorRecorrer(const Vector * v, Accion accion)
{
    void *ult = v->vec + (v->cantElem-1)*v->tamElem;
    for (void *i = v->vec; i<=ult; i+=v->tamElem)
        accion(i);
}

void cargarVec(Vector *v, char *nombArch)
{
    FILE *fp = fopen(nombArch, "rt");
    if(!fp)
    {
        printf("Error al abrir el archivo\n");
        exit(ERROR);
    }

    char linea[TAMLINEA];
    P_Continentes reg;

    fgets(linea, TAMLINEA, fp); //Salteamos encabezados
    while(fgets(linea, TAMLINEA, fp))
    {
        parsearCont(linea, &reg);
        insertarAlFinal(v,(void*)&reg);
    }

    fclose(fp);
}

P_Continentes *buscarPorPaisDesc(const Vector *v, const char *pais_desc)
{
    char *actual = (char *)v->vec;
    char *fin = actual + (v->cantElem * v->tamElem);
    P_Continentes *p;

    while (actual < fin)
    {
        p = (P_Continentes *)actual;
        if (strcmp(p->pais_desc, pais_desc) == 0)
        {
            return p;
        }
        actual += v->tamElem;
    }

    return NULL;
}

Resumen_Continente *buscarEnResumen(Resumen_Continente *inicio, Resumen_Continente *fin, const char *continente)
{
    Resumen_Continente *i;
    for (i = inicio; i < fin; i++)
    {
        if (strcmp(i->continente, continente) == 0)
        {
            return i;
        }
    }
    return NULL;
}

void mostrarVec(Resumen_Continente *vec)
{
    Resumen_Continente *inicio;
    Resumen_Continente *fin = vec + 7;

    printf("%-15s %12s %15s %15s\n", "CONTINENTE", "registros", "total_credito", "total_debito");
    printf("-------------------------------------------------------------\n");
    for (inicio = vec; inicio < fin; inicio++)
    {
        printf("%-15s %12ld %15.2f %15.2f\n",
               inicio->continente,
               inicio->registros,
               inicio->total_credito,
               inicio->total_debito);
    }
}

void procesarContinentes(Resumen_Continente *vec)
{
    Vector v;
    FILE *pf = fopen("transferencias_personales_clean.csv", "r");
    if(pf == NULL)
    {
        printf("Error al abrir archivo\n");
        return;
    }

    crearVector(&v,sizeof(P_Continentes),10);
    cargarVec(&v, "paises_continentes.csv");
    inicializarResumen(vec);

    char linea[TAMLINEA];
    Transferencias reg;
    P_Continentes *pais_hallado;
    Resumen_Continente *resumen_hallado;

    fgets(linea, sizeof(linea), pf);
    while(fgets(linea, sizeof(linea), pf))
    {
        parsear(linea, &reg);

        pais_hallado = buscarPorPaisDesc(&v, reg.p.pais_desc);

        resumen_hallado = buscarEnResumen(vec, vec + 7, pais_hallado->continente);

        if (resumen_hallado != NULL)
        {
            resumen_hallado->registros++;
            if (*(reg.operacion) == 'C')
                resumen_hallado->total_credito += reg.monto;
            else
                resumen_hallado->total_debito += reg.monto;
        }
    }

    destruirVector(&v);
    fclose(pf);
}

void Ranking_pais(Transferencias *t, Resumen_Pais **rp, int *contPaises){

    //tengo todo el archivo leido en un vector
    FILE * file1;
    file1 = fopen("transferencias_personales_clean.csv", "r");
    if(file1 == NULL)
    {
        printf("Archivo corrupto");
        return;
    }
    t = malloc(sizeof(Transferencias));
    if(t == NULL)
    {
        printf("No hay memoria");
        return;
    }

    //Aca tengo el archivo en vector
    int aux = 0;
    int capacidad = 1;
    char linea[256];
    fgets(linea, sizeof(linea), file1);
    while(fgets(linea, sizeof(linea), file1))
    {
        //realloc si esta lleno
        if(aux == capacidad)
        {
            capacidad++;
            t = realloc(t, sizeof(Transferencias) * capacidad);
            if(t == NULL)
            {
                printf("No hay memoria");
                return;
            }
        }

        // parsear CSV: anio, trimestre, pais_cod, pais_des, operacion, monto
        sscanf(linea, "%d;%d;%2s;%64[^;];%1s;%f",
               &t[aux].anio,
               &t[aux].trimestre,
               t[aux].p.pais_cod,
               t[aux].p.pais_desc,
               t[aux].operacion,
               &t[aux].monto);

        aux++;
    }
    //TERMINA: tengo todo el archivo leido en un vector

    //ordeno todo el archivo por pais, registro, credito y debito
    Transferencias *reg, temp;
    for (int k = 0; k < aux - 1; k++)
    {
        for (reg = t + 1; reg < t + aux; reg++)
        {
            if (strcmp((reg-1)->p.pais_cod, reg->p.pais_cod) > 0)
            {
                temp = *reg;
                *reg = *(reg-1);
                *(reg-1) = temp;
            }
        }
    }

    //indexo todo el vec por paises
    *rp = malloc(sizeof(Resumen_Pais));
    if(*rp == NULL)
    {
        printf("No hay memoria");
        return;
    }

    int cap = 1;
    int i=0;
    int estatico =0;
    int k=0;
    //---- primer registro todo cero -----
    strcpy((*rp)[0].pais_cod, t[0].p.pais_cod);
    strcpy((*rp)[0].pais_desc, t[0].p.pais_desc);
    (*rp)[0].registros = 0;
    (*rp)[0].total_credito = 0;
    (*rp)[0].total_debito = 0;
    //----- recorro los registros -----
    for(k=0; k<aux; k++)
    {
        if(strcmp(t[estatico].p.pais_cod, t[k].p.pais_cod) == 0)
        {
            //Vacio, sumo afuera
        }
        else
        {

            i++;
            estatico = k;

            if(i == cap)
            {
                cap++;
                *rp = realloc(*rp, sizeof(Resumen_Pais) * cap);
                if(*rp == NULL)
                {
                    printf("No hay memoria");
                    return;
                }
            }

            strcpy((*rp)[i].pais_cod, t[estatico].p.pais_cod);
            strcpy((*rp)[i].pais_desc, t[estatico].p.pais_desc);
            (*rp)[i].registros = 0;
            (*rp)[i].total_credito = 0;
            (*rp)[i].total_debito = 0;

        }

        (*rp)[i].registros = (*rp)[i].registros + 1;
        if(t[k].operacion[0] == 'C')
        {
            (*rp)[i].total_credito += t[k].monto;
        };
        if(t[k].operacion[0] == 'D')
        {
            (*rp)[i].total_debito += t[k].monto;
        };

    }
    *contPaises = i +1;
    //TERMINA

    //Pregunto por cantidad de paises y tiro un for validando
    int cantPaises;
    char tipoOperacion;
    printf("Elija la cantidad de paises: ");
    scanf("%d", &cantPaises);
    printf("Elija el tipo de Operacion: ");
    scanf(" %c", &tipoOperacion);
    if(cantPaises < 1 || cantPaises > *contPaises){
        printf("Reingrese la cantidad de paises: ");
        scanf("%d", &cantPaises);
    }
    if(tipoOperacion != 'c' && tipoOperacion != 'C' && tipoOperacion != 'd' && tipoOperacion != 'D' ){
        printf("Reingrese el tipo de Operacion: ");
        scanf(" %c", &tipoOperacion);
    }

    if(tipoOperacion == 'c' || tipoOperacion == 'C'){
      //ordeno por monto credito
        Resumen_Pais Raux;
    for(k=0; k < *contPaises; k++){
       for(i=0; i < *contPaises -1 ; i++){
        if((*rp)[i].total_credito < (*rp)[i+1].total_credito){
            Raux = (*rp)[i+1];
            (*rp)[i+1] = (*rp)[i];
            (*rp)[i] = Raux;
            }
        }
    }
        //imprimo el vector
    int j;
    for (j = 0; j <= cantPaises; j++)
    {
        printf("Top %d | País: %s | Nombre Pais: %-34s | Cant. Operaciones: %4ld | Total Credito: %10.2f\n",
               j+1,
                (*rp)[j].pais_cod,
               (*rp)[j].pais_desc,
               (*rp)[j].registros,
               (*rp)[j].total_credito);
    }
    }

    if(tipoOperacion =='d' || tipoOperacion == 'D'){
        //ordeno por monto credito
        Resumen_Pais Raux;
    for(k=0; k < *contPaises; k++){
       for(i=0; i < *contPaises -1 ; i++){
        if((*rp)[i].total_debito < (*rp)[i+1].total_debito){
            Raux = (*rp)[i+1];
            (*rp)[i+1] = (*rp)[i];
            (*rp)[i] = Raux;
            }
        }
    }
        //imprimo el vector
    int j;
    for (j = 0; j < cantPaises; j++)
    {
        printf("Top %d | País: %s | Nombre Pais: %-34s | Cant. Operaciones: %4ld | Total Debito: %10.2f\n",
                j+1,
                (*rp)[j].pais_cod,
               (*rp)[j].pais_desc,
               (*rp)[j].registros,
               (*rp)[j].total_debito);
    }
    }

    fclose(file1);
    free(*rp);
    free(t);


}

void GenerarArchivoParticipacion(Resumen_Continente *vec)
{
    FILE *pf;
    Resumen_Continente *ini;
    Resumen_Continente *fin = vec + 7;
    float total_c = 0;
    float total_d = 0;
    Participacion_Continente reg;

    pf = fopen("participacion_continentes.dat", "wb");
    if (!pf)
    {
        printf("Error al crear el archivo binario.\n");
        return;
    }

    for(ini = vec; ini < fin; ini++)
    {
        total_c += ini->total_credito;
        total_d += ini->total_debito;
    }

    for(ini = vec; ini < fin; ini++)
    {
        strcpy(reg.continente, ini->continente);

        if (ini->total_credito > 0)
            reg.porc_credito = ((ini->total_credito / total_c) * 100);
        else
            reg.porc_credito = 0;

        if (ini->total_debito > 0)
            reg.porc_debito = ((ini->total_debito / total_d) * 100);
        else
            reg.porc_debito = 0;

        fwrite(&reg, sizeof(Participacion_Continente), 1, pf);
    }

    fclose(pf);
    printf("Archivo binario '%s' generado exitosamente.\n", "participacion_continentes.dat");
}

int compararPorContinente(const void *a, const void *b)
{
    const Participacion_Continente *p1 = (const Participacion_Continente *)a;
    const Participacion_Continente *p2 = (const Participacion_Continente *)b;

    return strcmp(p1->continente, p2->continente);
}

void MostrarParticipacionDesdeArchivo()
{
    FILE *pf = fopen("participacion_continentes.dat", "rb");
    if (!pf)
    {
        printf("Error al abrir el archivo binario.\n");
        return;
    }

    Participacion_Continente reg[7];
    Participacion_Continente *pr=reg;
    int i;

    fread(&reg, sizeof(Participacion_Continente), 7, pf);

    qsort(reg, 7, sizeof(Participacion_Continente), compararPorContinente);

    printf("%-18s %8s %8s\n", "CONTINENTE", "C", "D");
    printf("------------------------------------\n");
    for(i=0; i<7; i++)
    {
        printf("%-18s %8.2f %8.2f\n", pr->continente, pr->porc_credito, pr->porc_debito);
        pr++;
    }
    printf("\n");

    fclose(pf);
}

void GenerarArchivoIntermedio(Transferencias *t, P_Continentes *pc, Intermedio_Arch *inter){

    int capacidad1 = 1;
    int capacidad2 = 1;
    int capacidad3 = 1;
    int aux = 0;
    int k = 0;
    int j = 0;
    int i = 0;


    FILE * file1;
    file1 = fopen("transferencias_personales_clean.csv", "r");
    if(file1 == NULL){
        printf("Archivo corrupto");
        return;
    }
    FILE * file2;
    file2 = fopen("paises_continentes.csv", "r");
    if(file2 == NULL){
        printf("Archivo corrupto");
        return;
    }

    // Este es para leer todo el documento | TRANSFERENCIAS
    t = malloc(sizeof(Transferencias));
    if(t == NULL)
    {
        printf("No hay memoria");
        return;
    }

    // Este es para los continentes | PC
    pc = malloc(sizeof(P_Continentes));
    if(pc == NULL)
    {
        printf("No hay memoria");
        return;
    }

    //leo el archivo y lo vuelvo a vector
    char linea[256];
    fgets(linea, sizeof(linea), file1);
    while(fgets(linea, sizeof(linea), file1))
    {
        //realloc si esta lleno
        if(aux == capacidad1)
        {
            capacidad1++;
            t = realloc(t, sizeof(Transferencias) * capacidad1);
            if(t == NULL)
            {
                printf("No hay memoria");
                return;
            }
        }

        // parsear CSV: anio, trimestre, pais_cod, pais_des, operacion, monto
        sscanf(linea, "%d;%d;%2s;%64[^;];%1s;%f",
               &t[aux].anio,
               &t[aux].trimestre,
               t[aux].p.pais_cod,
               t[aux].p.pais_desc,
               t[aux].operacion,
               &t[aux].monto);

        aux++;
    }

    //ahora tengo que leer registro por registro, buscarle el el continentes y volcarlo en inter
    fgets(linea, sizeof(linea), file2);
    while(fgets(linea, sizeof(linea), file2)){
        if(k == capacidad2)
        {
            capacidad2++;
            pc = realloc(pc, sizeof(P_Continentes) * capacidad2);
            if(pc == NULL)
            {
                printf("No hay memoria");
                return;
            }
        }
        sscanf(linea, "%2s;%32[^;];%16[^;]", pc[k].pais_cod, pc[k].pais_desc, pc[k].continente);
        k++;
    }

        // Este es para volcar lo nuevo ya leido | INTER
    inter = malloc(sizeof(Intermedio_Arch) * aux);
    if(inter == NULL)
    {
        printf("No hay memoria");
        return;
    }

    for(i = 0; i < aux; i++)
    {
        for(j = 0; j < k; j++)
        {
            if(strcmp(pc[j].pais_cod, t[i].p.pais_cod) == 0)
            {

                // acá copiás los campos de t[i] + pc[j].continente hacia inter[i]
                //anio, trimestre, pais code, pais desc, continene, operacion, monto
                inter[i].anio = t[i].anio;
                inter[i].trimestre = t[i].trimestre;
                strcpy(inter[i].pais_cod, t[i].p.pais_cod);
                strcpy(inter[i].pais_desc, t[i].p.pais_desc);
                strcpy(inter[i].continente, pc[j].continente);
                strcpy(inter[i].operacion, t[i].operacion);
                inter[i].monto = t[i].monto;
            }
        }
    }

    for(j = 0; j < aux; j++){
        printf("%d %d %s %-20.20s %s %s %.2f\n", inter[j].anio, inter[j].trimestre, inter[j].pais_cod, inter[j].pais_desc, inter[j].continente, inter[j].operacion, inter[j].monto);

    }

    FILE *salida = fopen("transferencias_base_larga.csv", "w");
    if (salida == NULL)
    {
        printf("No se pudo crear el archivo\n");
        return;
    }

    //Escritura de archivo
    Intermedio_Arch *reg;

    for (reg = inter; reg < inter + aux; reg++)
    {
        fprintf(salida, "%d;%d;%s;%s;%s;%s;%.2f\n",
                reg->anio, reg->trimestre, reg->pais_cod, reg->pais_desc, reg->continente, reg->operacion, reg->monto);
    }

    fclose(salida);
    fclose(file1);
    fclose(file2);
    free(t);
    free(inter);
    free(pc);

}
