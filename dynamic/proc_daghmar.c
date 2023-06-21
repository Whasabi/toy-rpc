#include "../rpc.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct
{
    reportable_t parent;

    double arg;
    double base;
    double result;
} my_struct_t; //Estructura empleada para almacenar los valores de entrada y el resultado

void *parse_parameters(void *data) //Analisis de parametros
{
    const char *buf = (const char *)(data);

    my_struct_t *d = (my_struct_t *)(malloc(sizeof(my_struct_t))); //Asigna memroio a la estructura

    if (d)
    {
        int count = sscanf(buf, "%lf %lf", &d->arg, &d->base);
        if (count == 1) {
            d->base = 10.0; //Si se ingresa un solo parametro el valor de base se establece como 10 por defecto
        }
    }

    return (void *)d;
}

void *do_work(void *data)
{
    my_struct_t *d = (my_struct_t *)(data);

    double arg = d->arg;
    double base = d->base;

    d->result = log(arg) / log(base);

    return data;
}

reportable_t *report(void *data)
{
    my_struct_t *d = (my_struct_t *)(data);

    d->parent.data = (char *)(malloc(255 * sizeof(char)));

    snprintf(d->parent.data, 255, "El logaritmo de %lf en base %lf es %lf\n", d->arg, d->base, d->result);
    d->parent.len = strlen(d->parent.data);

    return (reportable_t *)(data);
}

void clean_up(void *params, void *result, reportable_t *report)
{
    if (report && report->data)
    {
        free(report->data);
    }

    if (params)
    {
        free(params);
    }
}