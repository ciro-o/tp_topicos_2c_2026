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

void MostrarArchivoPorPais(Transferencias *t, int cont)
{

    //malloc
    Transferencias *puntero;
    Transferencias temp;

    // Este es para leer todo el documento
    puntero = malloc(sizeof(Transferencias) * cont);
    if(puntero == NULL)
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
        sscanf(linea, "%d;%d;%2s;%30[^;];%1s;%f",
               &puntero[aux].anio,
               &puntero[aux].trimestre,
               puntero[aux].p.pais_cod,
               puntero[aux].p.pais_desc,
               puntero[aux].operacion,
               &puntero[aux].monto);

        aux++;
    }
    Transferencias *reg;
    int k;
    //ordenamiento
    for (k = 0; k < aux - 1; k++)
    {
        for (reg = puntero + 1; reg < puntero + aux; reg++)
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

    for (reg = puntero; reg < puntero + aux; reg++)
    {
        fprintf(salida, "%d;%d;%s;%s;%s;%.2f\n",
                reg->anio, reg->trimestre, reg->p.pais_cod, reg->p.pais_desc, reg->operacion, reg->monto);
    }

    fclose(salida);

    //printeo ordenado
    for (reg = puntero; reg < puntero + aux; reg++)
    {
        printf("Año: %d | Trimestre: %d | País: %s | Nombre Pais: %s \t\t| Operación: %s | Monto: %.2f\n",
               reg->anio, reg->trimestre, reg->p.pais_cod, reg->p.pais_desc, reg->operacion, reg->monto);
    }
    free(puntero);
    fclose(file1);

}

void MostrarCantOperacionesPorPais(Transferencias *t, Resumen_Pais *rp)
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
        sscanf(linea, "%d;%d;%2s;%19[^;];%1s;%f",
               &t[aux].anio,
               &t[aux].trimestre,
               t[aux].p.pais_cod,
               t[aux].p.pais_desc,
               t[aux].operacion,
               &t[aux].monto);

        aux++;
    }
    // ahora vuelvo a rp
    rp = malloc(sizeof(Resumen_Pais));
    if(rp == NULL)
    {
        printf("No hay memoria");
        return;
    }

    int cap = 1;
    int i=0;
    int estatico =0;
    int k=0;
    //---- primer registro todo cero -----
    strcpy(rp[0].pais_cod, t[0].p.pais_cod);
    strcpy(rp[0].pais_desc, t[0].p.pais_desc);
    rp[0].registros = 0;
    rp[0].total_credito = 0;
    rp[0].total_debito = 0;
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
                rp = realloc(rp, sizeof(Resumen_Pais) * cap);
                if(rp == NULL)
                {
                    printf("No hay memoria");
                    return;
                }
            }

            strcpy(rp[i].pais_cod, t[estatico].p.pais_cod);
            strcpy(rp[i].pais_desc, t[estatico].p.pais_desc);
            rp[i].registros = 0;
            rp[i].total_credito = 0;
            rp[i].total_debito = 0;

        }

        rp[i].registros = rp[i].registros + 1;
        if(t[k].operacion[0] == 'C')
        {
            rp[i].total_credito += t[k].monto;
        };
        if(t[k].operacion[0] == 'D')
        {
            rp[i].total_debito += t[k].monto;
        };

    }

    fclose(file1);
    free(t);

    //imprimo el vector
    int j;
    for (j = 0; j <= i; j++)
    {
        printf("País: %s | Nombre Pais: %s \t\t| Cant. Operaciones: %ld | Total Credito: %.2f | Total Debito: %.2f\n",
               rp[j].pais_cod,
               rp[j].pais_desc,
               rp[j].registros,
               rp[j].total_credito,
               rp[j].total_debito);
    }

    free(rp);
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
        sscanf(linea, "%2s;%64[^;];%19[^\r\n]", reg.pais_cod, reg.pais_desc, reg.continente);
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

void ProcesarContinentes()
{
    Vector v;
    Resumen_Continente vec[7];
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
        sscanf(linea, "%d;%d;%2s;%64[^;];%1s;%f", &reg.anio, &reg.trimestre, reg.p.pais_cod, reg.p.pais_desc, reg.operacion, &reg.monto);

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

    mostrarVec(vec);

    destruirVector(&v);
    fclose(pf);
}
