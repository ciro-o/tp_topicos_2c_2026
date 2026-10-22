#include "DeclaracionDeFunciones.h"

void MostrarArchivo(Transferencias *t)
{

    FILE *file1 = fopen("transferencias_personales_clean.csv", "r");
    if (file1 == NULL)
    {
        printf("Error al abrir archivo\n");
        return;
    }

    char linea[256];
    fgets(linea, sizeof(linea), file1);
    while (fgets(linea, sizeof(linea), file1))
    {
        // parsear CSV: anio, trimestre, pais_cod, pais_des, operacion, monto
        sscanf(linea, "%d;%d;%2s;%64[^;];%1s;%f",
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

void MostrarArchivoPorPais(const Vector *transferencias)
{

    Transferencias *t;
    Transferencias temp;
    Transferencias *reg;
    int aux;
    int k;
    FILE *salida;

    aux = transferencias->cantElem;
    t = malloc(sizeof(Transferencias) * aux);
    if (t == NULL)
    {
        printf("No hay memoria");
        return;
    }
    memcpy(t, transferencias->vec, sizeof(Transferencias) * aux);

    // ordenamiento
    for (k = 0; k < aux - 1; k++)
    {
        for (reg = t + 1; reg < t + aux; reg++)
        {
            if (strcmp((reg - 1)->p.pais_cod, reg->p.pais_cod) > 0)
            {
                temp = *reg;
                *reg = *(reg - 1);
                *(reg - 1) = temp;
            }
        }
    }

    salida = fopen("paises_ordenado.txt", "w");
    if (salida == NULL)
    {
        printf("No se pudo crear el archivo\n");
        return;
    }

    // Escritura de archivo

    for (reg = t; reg < t + aux; reg++)
    {
        fprintf(salida, "%d;%d;%s;%s;%s;%.2f\n",
                reg->anio, reg->trimestre, reg->p.pais_cod, reg->p.pais_desc, reg->operacion, reg->monto);
    }

    fclose(salida);

    // printeo ordenado
    /* for (reg = puntero; reg < puntero + aux; reg++)
    {
        printf("Año: %d | Trimestre: %d | País: %s | Nombre Pais: %s \t\t| Operación: %s | Monto: %.2f\n",
               reg->anio, reg->trimestre, reg->p.pais_cod, reg->p.pais_desc, reg->operacion, reg->monto);
    } */
    free(t);
    fclose(salida);
}

void MostrarCantOperacionesPorPais(Transferencias *t, Resumen_Pais **rp)
{

    FILE *file1;
    file1 = fopen("paises_ordenado.txt", "r");
    if (file1 == NULL)
    {
        printf("Archivo corrupto");
        return;
    }
    t = malloc(sizeof(Transferencias));
    if (t == NULL)
    {
        printf("No hay memoria");
        return;
    }

    // Aca tengo el archivo en vector
    int aux = 0;
    int capacidad = 1;
    char linea[256];
    fgets(linea, sizeof(linea), file1);
    while (fgets(linea, sizeof(linea), file1))
    {
        // realloc si esta lleno
        if (aux == capacidad)
        {
            capacidad++;
            t = realloc(t, sizeof(Transferencias) * capacidad);
            if (t == NULL)
            {
                printf("No hay memoria");
                return;
            }
        }

        // parsear CSV: anio, trimestre, pais_cod, pais_des, operacion, monto
        if (parsear(linea, &t[aux]))
        {
            aux++;
        }
    }

    // creo el arch de todos los paises y sus montos

    // ahora vuelvo a rp
    *rp = malloc(sizeof(Resumen_Pais));
    if (*rp == NULL)
    {
        printf("No hay memoria");
        return;
    }

    int cap = 1;
    int i = 0;
    int estatico = 0;
    int k = 0;
    //---- primer registro todo cero -----
    strcpy((*rp)[0].pais_cod, t[0].p.pais_cod);
    strcpy((*rp)[0].pais_desc, t[0].p.pais_desc);
    (*rp)[0].registros = 0;
    (*rp)[0].total_credito = 0;
    (*rp)[0].total_debito = 0;
    //----- recorro los registros -----
    for (k = 0; k < aux; k++)
    {
        if (strcmp(t[estatico].p.pais_cod, t[k].p.pais_cod) == 0)
        {
            // Vacio, sumo afuera
        }
        else
        {

            i++;
            estatico = k;

            if (i == cap)
            {
                cap++;
                *rp = realloc(*rp, sizeof(Resumen_Pais) * cap);
                if (*rp == NULL)
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
        if (t[k].operacion[0] == 'C')
        {
            (*rp)[i].total_credito += t[k].monto;
        };
        if (t[k].operacion[0] == 'D')
        {
            (*rp)[i].total_debito += t[k].monto;
        };
    }

    fclose(file1);
    free(t);

    // imprimo el vector
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

void ImporteDebitoPorPais(Transferencias *t, Resumen_Pais **rp)
{

    FILE *file1;
    file1 = fopen("paises_ordenado.txt", "r");
    if (file1 == NULL)
    {
        printf("Archivo corrupto");
        return;
    }
    t = malloc(sizeof(Transferencias));
    if (t == NULL)
    {
        printf("No hay memoria");
        return;
    }

    // Aca tengo el archivo en vector
    int aux = 0;
    int capacidad = 1;
    char linea[256];
    fgets(linea, sizeof(linea), file1);
    while (fgets(linea, sizeof(linea), file1))
    {
        // realloc si esta lleno
        if (aux == capacidad)
        {
            capacidad++;
            t = realloc(t, sizeof(Transferencias) * capacidad);
            if (t == NULL)
            {
                printf("No hay memoria");
                return;
            }
        }

        // parsear CSV: anio, trimestre, pais_cod, pais_des, operacion, monto
        if (parsear(linea, &t[aux]))
        {
            aux++;
        }
    }
    // ahora vuelvo a rp
    *rp = malloc(sizeof(Resumen_Pais));
    if (*rp == NULL)
    {
        printf("No hay memoria");
        return;
    }

    int cap = 1;
    int i = 0;
    int estatico = 0;
    int k = 0;
    //---- primer registro todo cero -----
    strcpy((*rp)[0].pais_cod, t[0].p.pais_cod);
    strcpy((*rp)[0].pais_desc, t[0].p.pais_desc);
    (*rp)[0].registros = 0;
    (*rp)[0].total_credito = 0;
    (*rp)[0].total_debito = 0;
    //----- recorro los registros -----
    for (k = 0; k < aux; k++)
    {
        if (strcmp(t[estatico].p.pais_cod, t[k].p.pais_cod) == 0)
        {
            // Vacio, sumo afuera
        }
        else
        {

            i++;
            estatico = k;

            if (i == cap)
            {
                cap++;
                *rp = realloc(*rp, sizeof(Resumen_Pais) * cap);
                if (*rp == NULL)
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
        if (t[k].operacion[0] == 'C')
        {
            (*rp)[i].total_credito += t[k].monto;
        };
        if (t[k].operacion[0] == 'D')
        {
            (*rp)[i].total_debito += t[k].monto;
        };
    }

    fclose(file1);
    free(t);

    // imprimo el vector
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

void ImporteCreditoPorPais(Transferencias *t, Resumen_Pais **rp)
{

    FILE *file1;
    file1 = fopen("paises_ordenado.txt", "r");
    if (file1 == NULL)
    {
        printf("Archivo corrupto");
        return;
    }
    t = malloc(sizeof(Transferencias));
    if (t == NULL)
    {
        printf("No hay memoria");
        return;
    }

    // Aca tengo el archivo en vector
    int aux = 0;
    int capacidad = 1;
    char linea[256];
    fgets(linea, sizeof(linea), file1);
    while (fgets(linea, sizeof(linea), file1))
    {
        // realloc si esta lleno
        if (aux == capacidad)
        {
            capacidad++;
            t = realloc(t, sizeof(Transferencias) * capacidad);
            if (t == NULL)
            {
                printf("No hay memoria");
                return;
            }
        }

        // parsear CSV: anio, trimestre, pais_cod, pais_des, operacion, monto
        if (parsear(linea, &t[aux]))
        {
            aux++;
        }
    }
    // ahora vuelvo a rp
    *rp = malloc(sizeof(Resumen_Pais));
    if (*rp == NULL)
    {
        printf("No hay memoria");
        return;
    }

    int cap = 1;
    int i = 0;
    int estatico = 0;
    int k = 0;
    //---- primer registro todo cero -----
    strcpy((*rp)[0].pais_cod, t[0].p.pais_cod);
    strcpy((*rp)[0].pais_desc, t[0].p.pais_desc);
    (*rp)[0].registros = 0;
    (*rp)[0].total_credito = 0;
    (*rp)[0].total_debito = 0;
    //----- recorro los registros -----
    for (k = 0; k < aux; k++)
    {
        if (strcmp(t[estatico].p.pais_cod, t[k].p.pais_cod) == 0)
        {
            // Vacio, sumo afuera
        }
        else
        {

            i++;
            estatico = k;

            if (i == cap)
            {
                cap++;
                *rp = realloc(*rp, sizeof(Resumen_Pais) * cap);
                if (*rp == NULL)
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
        if (t[k].operacion[0] == 'C')
        {
            (*rp)[i].total_credito += t[k].monto;
        };
        if (t[k].operacion[0] == 'D')
        {
            (*rp)[i].total_debito += t[k].monto;
        };
    }

    fclose(file1);
    free(t);

    // imprimo el vector
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

void OrdenarPorMonto(Transferencias *t, OrdenMonto *m)
{

    // aca tengo el archivo en el vector
    FILE *file1;
    file1 = fopen("paises_ordenado.txt", "r");
    if (file1 == NULL)
    {
        printf("Archivo corrupto");
        return;
    }
    t = malloc(sizeof(Transferencias));
    if (t == NULL)
    {
        printf("No hay memoria");
        return;
    }

    // Aca tengo el archivo en vector
    int aux = 0;
    int capacidad = 1;
    char linea[256];
    // fgets(linea, sizeof(linea), file1);
    while (fgets(linea, sizeof(linea), file1))
    {
        // realloc si esta lleno
        if (aux == capacidad)
        {
            capacidad++;
            t = realloc(t, sizeof(Transferencias) * capacidad);
            if (t == NULL)
            {
                printf("No hay memoria");
                return;
            }
        }

        // parsear CSV: anio, trimestre, pais_cod, pais_des, operacion, monto
        if (parsear(linea, &t[aux]))
            aux++;
    }

    // ahora tengo que contar los montos por vector de cada pais, comparando el code pais

    m = malloc(sizeof(OrdenMonto));
    capacidad = 1;

    int i = 0;
    int estatico = 0;
    int k = 0;
    //---- primer registro todo cero -----
    strcpy(m[0].pais_cod, t[0].p.pais_cod);
    strcpy(m[0].pais_desc, t[0].p.pais_desc);
    m[0].monto = 0;
    //----- recorro los registros -----
    for (k = 0; k < aux; k++)
    {
        if (strcmp(t[estatico].p.pais_cod, t[k].p.pais_cod) == 0)
        {
            // Vacio, sumo afuera
        }
        else
        {
            i++;
            estatico = k;
            if (i == capacidad)
            {
                capacidad++;
                m = realloc(m, sizeof(OrdenMonto) * capacidad);
                if (m == NULL)
                {
                    printf("No hay memoria");
                    return;
                }
            }
            strcpy(m[i].pais_cod, t[estatico].p.pais_cod);
            strcpy(m[i].pais_desc, t[estatico].p.pais_desc);
            m[i].monto = 0;
        }

        if (t[k].operacion[0] == 'C')
            m[i].monto += t[k].monto;
        if (t[k].operacion[0] == 'D')
            m[i].monto += t[k].monto;
    }

    // despues ordeno el vector
    OrdenMonto maux;
    for (k = 0; k < capacidad; k++)
    {
        for (i = 0; i < capacidad - 1; i++)
        {
            if (m[i].monto < m[i + 1].monto)
            {
                maux = m[i + 1];
                m[i + 1] = m[i];
                m[i] = maux;
            }
        }
    }

    // por ultimo imprimo el vector ordenado

    // imprimo el vector
    int j;
    for (j = 0; j <= i; j++)
    {
        printf("País: %s | Nombre Pais: %-34s | Total Monto: %10.2f\n",
               m[j].pais_cod,
               m[j].pais_desc,
               m[j].monto);
    }
}

bool parsear(char *linea, Transferencias *reg)
{
    char *act;

    act = strchr(linea, '\n');
    if (act)
        *act = '\0';
    act = strchr(linea, '\r');
    if (act)
        *act = '\0';

    act = strrchr(linea, ';');
    if (!act || sscanf(act + 1, "%f", &reg->monto) != 1)
        return false;
    *act = '\0';

    act = strrchr(linea, ';');
    if (!act)
        return false;
    *reg->operacion = *(act + 1);
    *(reg->operacion + 1) = '\0';
    *act = '\0';

    act = strrchr(linea, ';');
    if (!act)
        return false;
    strncpy(reg->p.pais_desc, act + 1, sizeof(reg->p.pais_desc) - 1);
    reg->p.pais_desc[sizeof(reg->p.pais_desc) - 1] = '\0';
    *act = '\0';

    act = strrchr(linea, ';');
    if (!act)
        return false;
    strncpy(reg->p.pais_cod, act + 1, sizeof(reg->p.pais_cod) - 1);
    reg->p.pais_cod[sizeof(reg->p.pais_cod) - 1] = '\0';
    *act = '\0';

    act = strrchr(linea, ';');
    if (!act || sscanf(act + 1, "%d", &reg->trimestre) != 1)
        return false;
    *act = '\0';

    return sscanf(linea, "%d", &reg->anio) == 1;
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
    v->vec = (void *)malloc(tamElem * cap);
    if (!v->vec)
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
    void *nVec = (void *)realloc(v->vec, nuevaCap * v->tamElem);
    if (!nVec)
        return false;

    v->vec = nVec;
    v->cap = nuevaCap;

    return true;
}

void insertarAlFinal(Vector *v, void *elem)
{
    if (v->cantElem == v->cap)
    {
        if (!redimensionar(v, v->cap * 2))
        {
            printf("Error en la memoria\n");
            exit(ERROR);
        }
    }

    void *ult = v->vec + (v->cantElem * v->tamElem);
    memcpy(ult, elem, v->tamElem);
    v->cantElem++;
}

void vectorRecorrer(const Vector *v, Accion accion)
{
    void *ult = v->vec + (v->cantElem - 1) * v->tamElem;
    for (void *i = v->vec; i <= ult; i += v->tamElem)
        accion(i);
}

void cargarVec(Vector *v, char *nombArch)
{
    FILE *fp = fopen(nombArch, "rt");
    if (!fp)
    {
        printf("Error al abrir el archivo\n");
        exit(ERROR);
    }

    char linea[TAMLINEA];
    P_Continentes reg;

    fgets(linea, TAMLINEA, fp); // Salteamos encabezados
    while (fgets(linea, TAMLINEA, fp))
    {
        parsearCont(linea, &reg);
        insertarAlFinal(v, (void *)&reg);
    }

    fclose(fp);
}

int cmpResumenCreditoDesc(const void *a, const void *b)
{
    const Resumen_Continente *r1 = (const Resumen_Continente *)a;
    const Resumen_Continente *r2 = (const Resumen_Continente *)b;

    if (r1->total_credito < r2->total_credito)
        return 1;
    if (r1->total_credito > r2->total_credito)
        return -1;

    
    return strcmp(r1->continente, r2->continente);
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
            return p;
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
            return i;
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
        printf("%-15s %12ld %15.2f %15.2f\n", inicio->continente, inicio->registros, inicio->total_credito, inicio->total_debito);
    }
}

void procesarContinentes(const Vector *transferencias, Resumen_Continente *vec)
{
    Vector v;
    Transferencias *actual;
    Transferencias *fin;
    P_Continentes *pais_hallado;
    Resumen_Continente *resumen_hallado;

    crearVector(&v, sizeof(P_Continentes), 10);
    cargarVec(&v, "paises_continentes.csv");
    inicializarResumen(vec);

    actual = (Transferencias *)transferencias->vec;
    fin = actual + transferencias->cantElem;

    for (; actual < fin; actual++)
    {
        pais_hallado = buscarPorPaisDesc(&v, actual->p.pais_desc);

        resumen_hallado = buscarEnResumen(vec, vec + 7, pais_hallado->continente);

        if (resumen_hallado != NULL)
        {
            resumen_hallado->registros++;
            if (*(actual->operacion) == 'C')
                resumen_hallado->total_credito += actual->monto;
            else
                resumen_hallado->total_debito += actual->monto;
        }
    }

    destruirVector(&v);
    ordenarInsercion(vec, 7, sizeof(Resumen_Continente), cmpResumenCreditoDesc);
}

void Ranking_pais(const Vector *transferencias, Resumen_Pais **rp, int *contPaises)
{

    // declaraciones del punto
    Transferencias *t;
    Transferencias *reg;
    Transferencias temp;
    Transferencias *base;
    Transferencias *actual;
    Resumen_Pais *destino;
    int aux;
    int k;
    int cap;
    int offset;
    int cantPaises;
    char tipoOperacion;

    aux = transferencias->cantElem;
    t = malloc(sizeof(Transferencias) * aux);
    if (t == NULL)
    {
        printf("No hay memoria");
        return;
    }
    memcpy(t, transferencias->vec, sizeof(Transferencias) * aux);
    // TERMINA: tengo todo el archivo leido en un vector

    // ordeno todo el archivo por pais, registro, credito y debito
    for (k = 0; k < aux - 1; k++)
    {
        for (reg = t + 1; reg < t + aux; reg++)
        {
            if (strcmp((reg - 1)->p.pais_cod, reg->p.pais_cod) > 0)
            {
                temp = *reg;
                *reg = *(reg - 1);
                *(reg - 1) = temp;
            }
        }
    }

    // agrupo por pais
    cap = 1;
    *rp = malloc(sizeof(Resumen_Pais) * cap);
    if (*rp == NULL)
    {
        printf("No hay memoria");
        return;
    }

    destino = *rp;
    base = t;
    strcpy(destino->pais_cod, base->p.pais_cod);
    strcpy(destino->pais_desc, base->p.pais_desc);
    destino->registros = 0;
    destino->total_credito = 0;
    destino->total_debito = 0;

    for (actual = t; actual < t + aux; actual++)
    {
        if (strcmp(base->p.pais_cod, actual->p.pais_cod) != 0)
        {
            base = actual;
            destino++;

            if (destino == *rp + cap)
            {
                offset = destino - *rp;
                cap++;
                *rp = realloc(*rp, sizeof(Resumen_Pais) * cap);
                if (*rp == NULL)
                {
                    printf("No hay memoria");
                    return;
                }
                destino = *rp + offset;
            }

            strcpy(destino->pais_cod, base->p.pais_cod);
            strcpy(destino->pais_desc, base->p.pais_desc);
            destino->registros = 0;
            destino->total_credito = 0;
            destino->total_debito = 0;
        }

        destino->registros++;
        if (actual->operacion[0] == 'C')
            destino->total_credito += actual->monto;
        if (actual->operacion[0] == 'D')
            destino->total_debito += actual->monto;
    }
    *contPaises = destino - *rp + 1;

    printf("Elija la cantidad de paises: ");
    scanf("%d", &cantPaises);
    printf("Elija el tipo de Operacion: ");
    scanf(" %c", &tipoOperacion);
    while (cantPaises < 1 || cantPaises > *contPaises)
    {
        printf("Reingrese la cantidad de paises: ");
        scanf("%d", &cantPaises);
    }
    while (tipoOperacion != 'c' && tipoOperacion != 'C' && tipoOperacion != 'd' && tipoOperacion != 'D')
    {
        printf("Reingrese el tipo de Operacion: ");
        scanf(" %c", &tipoOperacion);
    }

    if (tipoOperacion == 'c' || tipoOperacion == 'C')
    {
        // ordeno por monto credito
        Resumen_Pais Raux;
        Resumen_Pais *p1;
        Resumen_Pais *finRp = *rp + *contPaises;
        int pasada;

        for (pasada = 0; pasada < *contPaises; pasada++)
        {
            for (p1 = *rp; p1 < finRp - 1; p1++)
            {
                if (p1->total_credito < (p1 + 1)->total_credito)
                {
                    Raux = *(p1 + 1);
                    *(p1 + 1) = *p1;
                    *p1 = Raux;
                }
            }
        }
        // imprimo el vector
        Resumen_Pais *actualImprimir;
        int puesto = 1;
        for (actualImprimir = *rp; actualImprimir < *rp + cantPaises; actualImprimir++, puesto++)
        {
            printf("Top %d | País: %s | Nombre Pais: %-34s | Cant. Operaciones: %4ld | Total Credito: %10.2f\n",
                   puesto, actualImprimir->pais_cod, actualImprimir->pais_desc,
                   actualImprimir->registros, actualImprimir->total_credito);
        }
    }

    if (tipoOperacion == 'd' || tipoOperacion == 'D')
    {
        // ordeno por monto credito
        Resumen_Pais Raux;
        Resumen_Pais *p1;
        Resumen_Pais *finRp = *rp + *contPaises;
        int pasada;

        for (pasada = 0; pasada < *contPaises; pasada++)
        {
            for (p1 = *rp; p1 < finRp - 1; p1++)
            {
                if (p1->total_debito < (p1 + 1)->total_debito)
                {
                    Raux = *(p1 + 1);
                    *(p1 + 1) = *p1;
                    *p1 = Raux;
                }
            }
        }
        // imprimo el vector
        Resumen_Pais *actualImprimir;
        int puesto = 1;
        for (actualImprimir = *rp; actualImprimir < *rp + cantPaises; actualImprimir++, puesto++)
        {
            printf("Top %d | País: %s | Nombre Pais: %-34s | Cant. Operaciones: %4ld | Total Debito: %10.2f\n",
                   puesto, actualImprimir->pais_cod, actualImprimir->pais_desc,
                   actualImprimir->registros, actualImprimir->total_debito);
        }
    }

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

    for (ini = vec; ini < fin; ini++)
    {
        total_c += ini->total_credito;
        total_d += ini->total_debito;
    }

    for (ini = vec; ini < fin; ini++)
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
    Participacion_Continente *pr = reg;
    int i;

    fread(&reg, sizeof(Participacion_Continente), 7, pf);

    ordenarInsercion(reg, 7, sizeof(Participacion_Continente), compararPorContinente);

    printf("%-18s %8s %8s\n", "CONTINENTE", "C", "D");
    printf("------------------------------------\n");
    for (i = 0; i < 7; i++)
    {
        printf("%-18s %8.2f %8.2f\n", pr->continente, pr->porc_credito, pr->porc_debito);
        pr++;
    }
    printf("\n");

    fclose(pf);
}

void GenerarArchivoIntermedio(const Vector *transferencias, P_Continentes *pcParam, Intermedio_Arch *interParam)
{

    Transferencias *t;
    Transferencias *regT;
    P_Continentes *pc;
    P_Continentes *regPC;
    P_Continentes *escribirPC;
    P_Continentes *finPC;
    Intermedio_Arch *inter;
    Intermedio_Arch *regInter;
    Intermedio_Arch *reg;
    FILE *file2;
    FILE *salida;
    char linea[256];
    int aux;
    int capacidad2;
    int offsetPC;
    int j;

    (void)pcParam;
    (void)interParam;

    file2 = fopen("paises_continentes.csv", "r");
    if (file2 == NULL)
    {
        printf("Archivo corrupto");
        return;
    }

    // ya esta cargado una vez en memoria (Vector transferencias): solo lo copio, no reabro el archivo principal
    aux = transferencias->cantElem;
    t = malloc(sizeof(Transferencias) * aux);
    if (t == NULL)
    {
        printf("No hay memoria");
        fclose(file2);
        return;
    }
    memcpy(t, transferencias->vec, sizeof(Transferencias) * aux);

    capacidad2 = 1;
    pc = malloc(sizeof(P_Continentes) * capacidad2);
    if (pc == NULL)
    {
        printf("No hay memoria");
        free(t);
        fclose(file2);
        return;
    }

    escribirPC = pc;
    finPC = pc + capacidad2;

    fgets(linea, sizeof(linea), file2);
    while (fgets(linea, sizeof(linea), file2))
    {
        if (escribirPC == finPC)
        {
            offsetPC = escribirPC - pc;
            capacidad2++;
            pc = realloc(pc, sizeof(P_Continentes) * capacidad2);
            if (pc == NULL)
            {
                printf("No hay memoria");
                free(t);
                fclose(file2);
                return;
            }
            escribirPC = pc + offsetPC;
            finPC = pc + capacidad2;
        }
        parsearCont(linea, escribirPC);
        escribirPC++;
    }
    fclose(file2);

    inter = malloc(sizeof(Intermedio_Arch) * aux);
    if (inter == NULL)
    {
        printf("No hay memoria");
        free(t);
        free(pc);
        return;
    }

    regInter = inter;
    for (regT = t; regT < t + aux; regT++, regInter++)
    {
        for (regPC = pc; regPC < escribirPC; regPC++)
        {
            if (strcmp(regPC->pais_cod, regT->p.pais_cod) == 0)
            {
                regInter->anio = regT->anio;
                regInter->trimestre = regT->trimestre;
                strcpy(regInter->pais_cod, regT->p.pais_cod);
                strcpy(regInter->pais_desc, regT->p.pais_desc);
                strcpy(regInter->continente, regPC->continente);
                strcpy(regInter->operacion, regT->operacion);
                regInter->monto = regT->monto;
            }
        }
    }

    for (j = 0; j < aux; j++)
    {
        printf("%d %d %s %-20.20s %s %s %.2f\n", inter[j].anio, inter[j].trimestre, inter[j].pais_cod, inter[j].pais_desc, inter[j].continente, inter[j].operacion, inter[j].monto);
    }

    salida = fopen("transferencias_base_larga.csv", "w");
    if (salida == NULL)
    {
        printf("No se pudo crear el archivo\n");
        free(t);
        free(inter);
        free(pc);
        return;
    }

    for (reg = inter; reg < inter + aux; reg++)
    {
        fprintf(salida, "%d;%d;%s;%s;%s;%s;%.2f\n",
                reg->anio, reg->trimestre, reg->pais_cod, reg->pais_desc, reg->continente, reg->operacion, reg->monto);
    }

    fclose(salida);
    free(t);
    free(inter);
    free(pc);
}

//==================================================================
//  PUNTO 3: Saldo trimestral por pais
//==================================================================

// Lee el CSV principal UNA sola vez y agrega cada registro valido al vector v (de Transferencias).
// El vector lo crea quien llama (crearVector). Devuelve false si no se pudo abrir el archivo.
bool cargarTransferencias(Vector *v, const char *nombArch)
{
    FILE *archivo = fopen(nombArch, "r");
    Transferencias t;
    char linea[TAMLINEA];
    int descartadas = 0;

    if (!archivo)
        return false;

    t.p.continente[0] = '\0'; // el CSV principal no trae continente

    fgets(linea, sizeof(linea), archivo); // descarta el encabezado

    while (fgets(linea, sizeof(linea), archivo))
    {
        // las lineas vacias se ignoran; parsear modifica la linea, por eso se mira antes
        if (*linea != '\n' && *linea != '\r')
        {
            if (parsear(linea, &t))
                insertarAlFinal(v, &t);
            else
                descartadas++;
        }
    }

    fclose(archivo);

    if (descartadas > 0)
        printf("Aviso: se descartaron %d lineas con formato invalido.\n", descartadas);

    return true;
}

// Compara dos SaldoPais por nombre de pais: mantiene el vector ordenado alfabeticamente
int cmpSaldoPais(const void *a, const void *b)
{
    return strcmp(((const SaldoPais *)a)->p.pais_desc, ((const SaldoPais *)b)->p.pais_desc);
}

// Recorre el vector de transferencias UNA vez y acumula creditos y debitos por pais y trimestre del anio pedido.
// El vector saldos queda ordenado por nombre de pais. Devuelve false si no hay memoria.
bool acumularSaldosPorPais(const Vector *transf, int anio, Vector *saldos)
{
    const Transferencias *actual = (const Transferencias *)transf->vec;
    const Transferencias *fin = actual + transf->cantElem;
    SaldoPais clave;
    SaldoPais *saldo;

    memset(&clave, 0, sizeof(clave)); // montos en cero: es el valor inicial de cada pais nuevo

    while (actual < fin)
    {
        if (actual->anio == anio && actual->trimestre >= 1 && actual->trimestre <= CANT_TRIMESTRES)
        {
            clave.p = actual->p;
            saldo = (SaldoPais *)obtenerOInsertarEnOrden(saldos, &clave, cmpSaldoPais);
            if (!saldo)
                return false;

            if (actual->operacion[0] == 'C')
                *(saldo->creditos + actual->trimestre - 1) += actual->monto;
            else if (actual->operacion[0] == 'D')
                *(saldo->debitos + actual->trimestre - 1) += actual->monto;
        }

        actual++;
    }

    return true;
}

// Escribe la tabla pais x trimestre en destino (stdout o un archivo ya abierto): saldo = creditos - debitos
void imprimirSaldos(FILE *destino, const Vector *saldos, int anio)
{
    const SaldoPais *actual = (const SaldoPais *)saldos->vec;
    const SaldoPais *fin = actual + saldos->cantElem;
    char etiqueta[TAM_ETIQUETA];
    int relleno;
    int trimestre;
    int i;

    fprintf(destino, "SALDO TRIMESTRAL POR PAIS - ANIO %d\n", anio);
    fprintf(destino, "(en millones de dolares: creditos - debitos)\n\n");

    fprintf(destino, "%-*s", ANCHO_COL_PAIS, "Pais");
    for (trimestre = 1; trimestre <= CANT_TRIMESTRES; trimestre++)
    {
        sprintf(etiqueta, "T%d", trimestre);
        fprintf(destino, "%*s", ANCHO_COL_SALDO, etiqueta);
    }
    fprintf(destino, "\n");

    for (i = 0; i < ANCHO_COL_PAIS + CANT_TRIMESTRES * ANCHO_COL_SALDO; i++)
        fputc('-', destino);
    fprintf(destino, "\n");

    while (actual < fin)
    {
        // %-*s cuenta bytes y una tilde ocupa 2 en UTF-8: el relleno se calcula contando caracteres
        relleno = ANCHO_COL_PAIS - contarCaracteres(actual->p.pais_desc);
        if (relleno < 0)
            relleno = 0;
        fprintf(destino, "%s%*s", actual->p.pais_desc, relleno, "");

        for (trimestre = 0; trimestre < CANT_TRIMESTRES; trimestre++)
            fprintf(destino, "%*.2f", ANCHO_COL_SALDO,
                    *(actual->creditos + trimestre) - *(actual->debitos + trimestre));
        fprintf(destino, "\n");

        actual++;
    }
}

// Pide un anio positivo por teclado; repite si el valor es invalido. Devuelve false si se termina la entrada.
bool pedirAnio(int *anio)
{
    int c;

    printf("Ingrese el anio a consultar: ");
    while (scanf("%d", anio) != 1 || *anio <= 0)
    {
        // descarta el resto de la linea para no repetir siempre el mismo error
        while ((c = getchar()) != '\n' && c != EOF)
            ;
        if (c == EOF)
            return false;

        printf("Valor invalido. Ingrese un anio positivo: ");
    }

    return true;
}

// Opcion 3 del menu: pide el anio, genera saldo_trimestral_<anio>.txt y muestra por pantalla el contenido de ese archivo
void OpcionSaldoTrimestralPorPais(const Vector *transf)
{
    Vector saldos;
    FILE *archivo;
    char nombre[TAM_NOMBRE_ARCHIVO];
    int anio;

    if (!pedirAnio(&anio))
        return;

    crearVector(&saldos, sizeof(SaldoPais), 64);

    if (!acumularSaldosPorPais(transf, anio, &saldos))
        printf("Error: no hay memoria suficiente.\n");
    else if (saldos.cantElem == 0)
        printf("No hay operaciones registradas para el anio %d.\n", anio);
    else
    {
        sprintf(nombre, "saldo_trimestral_%d.txt", anio);
        archivo = fopen(nombre, "w");
        if (!archivo)
            printf("Error: no se pudo crear el archivo %s\n", nombre);
        else
        {
            imprimirSaldos(archivo, &saldos, anio);
            fclose(archivo);

            // lo que se ve por pantalla es el contenido del archivo ya generado
            printf("\n");
            if (mostrarArchivoTexto(nombre))
                printf("\nSe exportaron %d paises al archivo %s\n", saldos.cantElem, nombre);
            else
                printf("Error: no se pudo leer el archivo %s\n", nombre);
        }
    }

    destruirVector(&saldos);
}

//==================================================================
//  PUNTO 4: Totales anuales y variacion interanual
//==================================================================

// Compara dos TotalAnual por anio: mantiene el vector ordenado cronologicamente
int cmpTotalAnual(const void *a, const void *b)
{
    return ((const TotalAnual *)a)->anio - ((const TotalAnual *)b)->anio;
}

// Recorre el vector de transferencias UNA vez y acumula creditos y debitos de cada anio.
// El vector totales queda ordenado por anio. Devuelve false si no hay memoria.
bool acumularTotalesPorAnio(const Vector *transf, Vector *totales)
{
    const Transferencias *actual = (const Transferencias *)transf->vec;
    const Transferencias *fin = actual + transf->cantElem;
    TotalAnual clave;
    TotalAnual *total;

    memset(&clave, 0, sizeof(clave)); // montos en cero: es el valor inicial de cada anio nuevo

    while (actual < fin)
    {
        clave.anio = actual->anio;
        total = (TotalAnual *)obtenerOInsertarEnOrden(totales, &clave, cmpTotalAnual);
        if (!total)
            return false;

        if (actual->operacion[0] == 'C')
            total->creditos += actual->monto;
        else if (actual->operacion[0] == 'D')
            total->debitos += actual->monto;

        actual++;
    }

    return true;
}

// Muestra la tabla anio / creditos / debitos / saldo / variacion interanual del saldo.
// Variacion (%) = ((saldo actual - saldo anio anterior) / saldo anio anterior) * 100
// Para el primer anio no hay anio previo (y si el saldo anterior fuera 0 no se puede dividir): se muestra "-"
void imprimirTotalesAnuales(const Vector *totales)
{
    const TotalAnual *actual = (const TotalAnual *)totales->vec;
    const TotalAnual *fin = actual + totales->cantElem;
    const TotalAnual *primero = actual;
    double saldo;
    double saldoAnterior = 0;

    printf("TOTALES ANUALES Y VARIACION INTERANUAL (en millones de dolares)\n\n");
    printf("%*s%*s%*s%*s%*s\n", ANCHO_COL_ANIO, "ANIO", ANCHO_COL_MONTO, "C", ANCHO_COL_MONTO, "D",
           ANCHO_COL_MONTO, "SALDO", ANCHO_COL_VARIACION, "VAR_PCT_SALDO");

    while (actual < fin)
    {
        saldo = actual->creditos - actual->debitos;

        printf("%*d%*.2f%*.2f%*.2f", ANCHO_COL_ANIO, actual->anio, ANCHO_COL_MONTO, actual->creditos,
               ANCHO_COL_MONTO, actual->debitos, ANCHO_COL_MONTO, saldo);

        if (actual > primero && saldoAnterior != 0)
            printf("%*.2f\n", ANCHO_COL_VARIACION, (saldo - saldoAnterior) / saldoAnterior * 100);
        else
            printf("%*s\n", ANCHO_COL_VARIACION, "-");

        saldoAnterior = saldo;
        actual++;
    }
}

// Opcion 4 del menu: totales anuales y variacion interanual, por pantalla, sobre el vector ya cargado
void OpcionTotalesAnuales(const Vector *transf)
{
    Vector totales;

    crearVector(&totales, sizeof(TotalAnual), 16);

    if (!acumularTotalesPorAnio(transf, &totales))
        printf("Error: no hay memoria suficiente.\n");
    else if (totales.cantElem == 0)
        printf("No hay operaciones registradas.\n");
    else
        imprimirTotalesAnuales(&totales);

    destruirVector(&totales);
}

//==================================================================
//  PUNTO 8: Promedio trimestral por pais y tipo de operacion
//==================================================================

// Convierte una linea de transferencias_base_larga.csv (anio;trimestre;cod;pais;continente;operacion;monto)
// en un registro. Corta los campos desde el final, asi acepta el codigo de pais vacio (Namibia).
// Devuelve false si la linea no tiene el formato esperado.
bool parsearIntermedio(char *linea, Intermedio_Arch *reg)
{
    char *act;

    act = strchr(linea, '\n');
    if (act)
        *act = '\0';
    act = strchr(linea, '\r');
    if (act)
        *act = '\0';

    act = strrchr(linea, ';');
    if (!act || sscanf(act + 1, "%lf", &reg->monto) != 1)
        return false;
    *act = '\0';

    act = strrchr(linea, ';');
    if (!act)
        return false;
    *reg->operacion = *(act + 1);
    *(reg->operacion + 1) = '\0';
    *act = '\0';

    act = strrchr(linea, ';');
    if (!act)
        return false;
    strncpy(reg->continente, act + 1, sizeof(reg->continente) - 1);
    reg->continente[sizeof(reg->continente) - 1] = '\0';
    *act = '\0';

    act = strrchr(linea, ';');
    if (!act)
        return false;
    strncpy(reg->pais_desc, act + 1, sizeof(reg->pais_desc) - 1);
    reg->pais_desc[sizeof(reg->pais_desc) - 1] = '\0';
    *act = '\0';

    act = strrchr(linea, ';');
    if (!act)
        return false;
    strncpy(reg->pais_cod, act + 1, sizeof(reg->pais_cod) - 1);
    reg->pais_cod[sizeof(reg->pais_cod) - 1] = '\0';
    *act = '\0';

    act = strrchr(linea, ';');
    if (!act || sscanf(act + 1, "%d", &reg->trimestre) != 1)
        return false;
    *act = '\0';

    return sscanf(linea, "%d", &reg->anio) == 1;
}

// Compara dos PromedioPais por nombre de pais: mantiene el vector ordenado alfabeticamente mientras se acumula
int cmpPaisPromedio(const void *a, const void *b)
{
    return strcmp(((const PromedioPais *)a)->pais_desc, ((const PromedioPais *)b)->pais_desc);
}

// Promedio trimestral de la operacion op (0 = credito, 1 = debito): suma / trimestres con monto > 0.
// Si no hay ningun trimestre con monto positivo devuelve 0 (evita dividir por cero).
double promedioTrimestral(const PromedioPais *p, int op)
{
    if (*(p->trimestres + op) > 0)
        return *(p->suma + op) / *(p->trimestres + op);

    return 0;
}

// Compara dos PromedioPais para ordenar de MAYOR a MENOR segun el promedio de las operaciones de credito.
// Si hay empate no cambia el orden (queda el alfabetico, porque el ordenamiento es estable).
int cmpPromedioCredito(const void *a, const void *b)
{
    double promA = promedioTrimestral((const PromedioPais *)a, 0);
    double promB = promedioTrimestral((const PromedioPais *)b, 0);

    if (promA < promB)
        return 1;
    if (promA > promB)
        return -1;

    return 0;
}

// Lee transferencias_base_larga.csv UNA sola vez y acumula, por pais y tipo de operacion, el monto de los
// trimestres con monto mayor que cero y la cantidad de esos trimestres. El vector promedios queda ordenado por pais.
// Devuelve false si no se pudo abrir el archivo o no hay memoria.
bool cargarPromedios(const char *nombArch, Vector *promedios)
{
    FILE *archivo = fopen(nombArch, "r");
    Intermedio_Arch reg;
    PromedioPais clave;
    PromedioPais *prom;
    char linea[TAMLINEA];
    int op;
    int descartadas = 0;
    int numLinea = 0;

    if (!archivo)
        return false;

    memset(&clave, 0, sizeof(clave)); // todo en cero: es el valor inicial de cada pais nuevo

    while (fgets(linea, sizeof(linea), archivo))
    {
        numLinea++;

        // se ignoran las lineas vacias y el encabezado (la primera linea, si no empieza con el anio)
        if (*linea != '\n' && *linea != '\r' && (numLinea > 1 || isdigit((unsigned char)*linea)))
        {
            if (parsearIntermedio(linea, &reg) && (*reg.operacion == 'C' || *reg.operacion == 'D'))
            {
                strcpy(clave.pais_cod, reg.pais_cod);
                strcpy(clave.pais_desc, reg.pais_desc);

                prom = (PromedioPais *)obtenerOInsertarEnOrden(promedios, &clave, cmpPaisPromedio);
                if (!prom)
                {
                    fclose(archivo);
                    return false;
                }

                op = (*reg.operacion == 'D'); // 0 = credito, 1 = debito
                *(prom->hay + op) = true;

                if (reg.monto > 0)
                {
                    *(prom->suma + op) += reg.monto;
                    *(prom->trimestres + op) += 1;
                }
            }
            else
                descartadas++;
        }
    }

    fclose(archivo);

    if (descartadas > 0)
        printf("Aviso: se descartaron %d lineas con formato invalido.\n", descartadas);

    return true;
}

// Muestra, por cada pais y cada operacion que tenga registros: codigo, nombre, operacion,
// cantidad de trimestres considerados y monto promedio trimestral
void imprimirPromedios(const Vector *promedios)
{
    const PromedioPais *actual = (const PromedioPais *)promedios->vec;
    const PromedioPais *fin = actual + promedios->cantElem;
    int relleno;
    int op;
    int i;

    printf("PROMEDIO TRIMESTRAL POR PAIS Y TIPO DE OPERACION (en millones de dolares)\n");
    printf("(solo se consideran los trimestres con monto mayor que cero)\n\n");

    printf("%-*s%-*s%*s%*s%*s\n", ANCHO_COL_COD, "COD", ANCHO_COL_PAIS, "PAIS", ANCHO_COL_OP, "OPERACION",
           ANCHO_COL_TRIM, "TRIMESTRES", ANCHO_COL_PROM, "PROMEDIO");
    for (i = 0; i < ANCHO_COL_COD + ANCHO_COL_PAIS + ANCHO_COL_OP + ANCHO_COL_TRIM + ANCHO_COL_PROM; i++)
        putchar('-');
    putchar('\n');

    while (actual < fin)
    {
        // %-*s cuenta bytes y una tilde ocupa 2 en UTF-8: el relleno se calcula contando caracteres
        relleno = ANCHO_COL_PAIS - contarCaracteres(actual->pais_desc);
        if (relleno < 0)
            relleno = 0;

        for (op = 0; op < 2; op++)
        {
            if (*(actual->hay + op))
                printf("%-*s%s%*s%*s%*d%*.2f\n", ANCHO_COL_COD, actual->pais_cod, actual->pais_desc, relleno, "",
                       ANCHO_COL_OP, op == 0 ? "C" : "D", ANCHO_COL_TRIM, *(actual->trimestres + op),
                       ANCHO_COL_PROM, promedioTrimestral(actual, op));
        }

        actual++;
    }
}

// Opcion 8 del menu: promedio trimestral por pais y operacion, de mayor a menor segun el promedio de credito
void OpcionPromedioTrimestralPorPais()
{
    Vector promedios;

    crearVector(&promedios, sizeof(PromedioPais), 64);

    if (!cargarPromedios("transferencias_base_larga.csv", &promedios))
        printf("Error: no se pudo leer transferencias_base_larga.csv. Genere el archivo con la opcion 7.\n");
    else if (promedios.cantElem == 0)
        printf("El archivo transferencias_base_larga.csv no tiene registros.\n");
    else if (!ordenarInsercion(promedios.vec, promedios.cantElem, promedios.tamElem, cmpPromedioCredito))
        printf("Error: no hay memoria suficiente.\n");
    else
        imprimirPromedios(&promedios);

    destruirVector(&promedios);
}

// Busca elem en un vector ORDENADO segun cmp (busqueda binaria). Si existe, devuelve el puntero a ese elemento
// del vector; si no existe, lo inserta en el lugar que le corresponde y devuelve el puntero al elemento nuevo.
// Devuelve NULL si no hay memoria. Sirve para cualquier tipo de elemento: solo cambia la funcion cmp.
void *obtenerOInsertarEnOrden(Vector *v, const void *elem, int (*cmp)(const void *, const void *))
{
    char *ini = (char *)v->vec;
    char *fin = ini + v->cantElem * v->tamElem;
    char *medio;
    size_t desplazamiento;
    int comparacion;

    while (ini < fin)
    {
        medio = ini + ((fin - ini) / v->tamElem / 2) * v->tamElem;
        comparacion = cmp(elem, medio);

        if (comparacion == 0)
            return medio;

        if (comparacion < 0)
            fin = medio;
        else
            ini = medio + v->tamElem;
    }

    // no estaba: ini es la posicion donde corresponde insertarlo.
    // redimensionar puede mover el bloque, por eso guardo la distancia al inicio y no el puntero
    desplazamiento = ini - (char *)v->vec;

    if (v->cantElem == v->cap && !redimensionar(v, v->cap * 2))
        return NULL;

    ini = (char *)v->vec + desplazamiento;
    memmove(ini + v->tamElem, ini, v->cantElem * v->tamElem - desplazamiento);
    memcpy(ini, elem, v->tamElem);
    v->cantElem++;

    return ini;
}

// Ordena un arreglo de cantidad elementos de tamano tam segun cmp (insercion: estable, sin funciones de biblioteca).
// Misma firma que qsort. Devuelve false (y no ordena) si no hay memoria para el auxiliar.
bool ordenarInsercion(void *base, size_t cant, size_t tam, int (*cmp)(const void *, const void *))
{
    char *ini = (char *)base;
    char *fin = ini + cant * tam;
    char *i;
    char *j;
    char *aux;

    if (cant < 2)
        return true;

    aux = (char *)malloc(tam);
    if (!aux)
        return false;

    for (i = ini + tam; i < fin; i += tam)
    {
        memcpy(aux, i, tam);

        j = i;
        while (j > ini && cmp(j - tam, aux) > 0)
        {
            memcpy(j, j - tam, tam);
            j -= tam;
        }

        memcpy(j, aux, tam);
    }

    free(aux);

    return true;
}

// Cuenta caracteres (no bytes) de un texto UTF-8: los bytes 10xxxxxx son la continuacion de un caracter, no uno nuevo.
// Sirve para alinear columnas con nombres con tildes, porque %-*s cuenta bytes.
int contarCaracteres(const char *texto)
{
    int cantidad = 0;

    while (*texto)
    {
        if ((*texto & 0xC0) != 0x80)
            cantidad++;
        texto++;
    }

    return cantidad;
}

// Muestra por pantalla el contenido de un archivo de texto, caracter a caracter (sin limite de largo de linea).
// Devuelve false si no se pudo abrir el archivo.
bool mostrarArchivoTexto(const char *nombArch)
{
    FILE *archivo = fopen(nombArch, "r");
    int c;

    if (!archivo)
        return false;

    while ((c = fgetc(archivo)) != EOF)
        putchar(c);

    fclose(archivo);

    return true;
}

int cmpPromedioContinente(const void *a, const void *b)
{
    const PromedioContinente *p1 = (const PromedioContinente *)a;
    const PromedioContinente *p2 = (const PromedioContinente *)b;
    int cmpCont;

    // Orden por año ascendente
    if (p1->anio != p2->anio)
        return p1->anio - p2->anio;

    // Orden alfabetico de los continentes
    cmpCont = compararPorContinente(p1->continente, p2->continente);
    if (cmpCont != 0)
        return cmpCont;

    // Orden por operacion, primero 'C' despues 'D'
    return strcmp(p1->operacion, p2->operacion);
}

bool cargarPromediosContinente(const char *nombArch, Vector *promedios)
{
    FILE *pf = fopen(nombArch, "r");
    Intermedio_Arch reg;
    PromedioContinente clave;
    PromedioContinente *prom;
    char linea[TAMLINEA];

    if (pf == NULL)
    {
        printf("No se pudo leer el archivo %s", nombArch);
        return 0;
    }

    while (fgets(linea, sizeof(linea), pf))
    {
        if (*linea != '\n' && *linea != '\r')
        {
            if (parsearIntermedio(linea, &reg) && (*reg.operacion == 'C' || *reg.operacion == 'D'))
            {
                if (reg.monto > 0)
                {
                    clave.anio = reg.anio;
                    strncpy(clave.continente, reg.continente, sizeof(clave.continente) - 1);
                    *(clave.continente + sizeof(clave.continente) - 1) = '\0';
                    strncpy(clave.operacion, reg.operacion, sizeof(clave.operacion) - 1);
                    *(clave.operacion + sizeof(clave.operacion) - 1) = '\0';
                    clave.suma_monto = 0.0;
                    clave.cant_trimestres = 0;

                    prom = (PromedioContinente *)obtenerOInsertarEnOrden(promedios, &clave, cmpPromedioContinente);
                    if (!prom)
                    {
                        fclose(pf);
                        return 0;
                    }

                    prom->suma_monto += reg.monto;
                    prom->cant_trimestres += 1;
                }
            }
        }
    }

    fclose(pf);
    return 1;
}

void imprimirPromediosContinente(const Vector *promedios)
{
    const PromedioContinente *ini = (const PromedioContinente *)promedios->vec;
    const PromedioContinente *fin = ini + promedios->cantElem;
    double promedio;

    printf("\n%-6s %-15s %-10s %12s %16s %10s\n",
           "ANIO", "CONTINENTE", "OPERACION", "suma_monto", "cant_trimestres", "PROMEDIO");
    printf("----------------------------------------------------------------------------\n");

    while (ini < fin)
    {
        if (ini->cant_trimestres > 0)
            promedio = ini->suma_monto / ini->cant_trimestres;
        else
            promedio = 0.0;

        printf("%-6d %-15s %-10s %12.2f %16d %10.2f\n", ini->anio, ini->continente, ini->operacion, ini->suma_monto, ini->cant_trimestres, promedio);

        ini++;
    }
    printf("\n");
}

void OpcionPromedioContinentePorAnio(void)
{
    Vector promedios;

    crearVector(&promedios, sizeof(PromedioContinente), 64);

    if (!cargarPromediosContinente("transferencias_base_larga.csv", &promedios))
        printf("\nGenerelo con la opcion 7.");
    else
        imprimirPromediosContinente(&promedios);

    destruirVector(&promedios);
}

void Menu(Transferencias *t, OrdenMonto *m, Resumen_Continente vec[7],
          Vector *transferencias, Resumen_Pais **rp, int *contPaises,
          P_Continentes *pc, Intermedio_Arch *inter)
{

    int opcion;

    do
    {
        printf("\n\n1. Totales por país.\n"
               "2. Totales por continente.\n"
               "3. Saldo trimestral por país.\n"
               "4. Totales anuales y variación interanual.\n"
               "5. Ranking de países.\n"
               "6. Participación porcentual por continente.\n"
               "7. Generación del archivo intermedio transferencias_base_larga.csv.\n"
               "8. Promedio trimestral por país.\n"
               "9. Promedio trimestral por continente, operación y año.\n"
               "10. Salir.\n\n");

        printf("Elija una opcion: ");
        scanf("%d", &opcion);

        switch (opcion)
        {
        case 1:
            MostrarArchivoPorPais(transferencias);
            OrdenarPorMonto(t, m);
            break;

        case 2:
            procesarContinentes(transferencias, vec);
            mostrarVec(vec);
            break;

        case 3:
            OpcionSaldoTrimestralPorPais(transferencias);
            break;

        case 4:
            OpcionTotalesAnuales(transferencias);
            break;

        case 5:
            Ranking_pais(transferencias, rp, contPaises);
            break;

        case 6:
            procesarContinentes(transferencias, vec);
            GenerarArchivoParticipacion(vec);
            MostrarParticipacionDesdeArchivo();
            break;

        case 7:
            GenerarArchivoIntermedio(transferencias, pc, inter);
            break;

        case 8:
            OpcionPromedioTrimestralPorPais();
            break;

        case 9:
            OpcionPromedioContinentePorAnio();
            break;

        case 10:
            break;

        default:
            printf("Opcion invalida.\n");
            break;
        }

    } while (opcion != 10);
}
