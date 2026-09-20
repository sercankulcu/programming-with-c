/* mathlib.c */
#include "mathlib.h"
#include <stdlib.h>

struct MlAccumulator {          /* definition stays here */
    double total;
    size_t count;
};

int ml_add(int a, int b) { return a + b; }

double ml_mean(const double *values, size_t count, MlStatus *status)
{
    if (values == NULL) { if (status) *status = ML_ERR_NULL;  return 0.0; }
    if (count == 0)     { if (status) *status = ML_ERR_EMPTY; return 0.0; }

    double total = 0.0;
    for (size_t i = 0; i < count; i++) total += values[i];
    if (status) *status = ML_OK;
    return total / (double)count;
}

MlStatus ml_scale(const double *in, double *out, size_t count, double factor)
{
    if (in == NULL || out == NULL) return ML_ERR_NULL;
    for (size_t i = 0; i < count; i++) out[i] = in[i] * factor;
    return ML_OK;
}

double *ml_make_range(double start, double step, size_t count)
{
    if (count == 0) return NULL;
    double *v = malloc(count * sizeof *v);
    if (v == NULL) return NULL;
    for (size_t i = 0; i < count; i++) v[i] = start + (double)i * step;
    return v;                      /* caller must call ml_free */
}

void ml_free(double *p) { free(p); }

MlAccumulator *ml_acc_create(void)
{
    MlAccumulator *a = calloc(1, sizeof *a);
    return a;
}

void ml_acc_destroy(MlAccumulator *a) { free(a); }

MlStatus ml_acc_add(MlAccumulator *a, double v)
{
    if (a == NULL) return ML_ERR_NULL;
    a->total += v;
    a->count++;
    return ML_OK;
}

double ml_acc_mean(const MlAccumulator *a)
{
    return (a == NULL || a->count == 0) ? 0.0 : a->total / (double)a->count;
}

size_t ml_acc_count(const MlAccumulator *a)
{
    return (a == NULL) ? 0 : a->count;
}

MlStatus ml_each(const double *values, size_t count,
                 MlVisitor visit, void *context)
{
    if (values == NULL || visit == NULL) return ML_ERR_NULL;
    for (size_t i = 0; i < count; i++) {
        if (visit(values[i], context) != 0) break;   /* non-zero stops */
    }
    return ML_OK;
}

const char *ml_status_string(MlStatus s)
{
    switch (s) {
    case ML_OK:        return "ok";
    case ML_ERR_NULL:  return "null pointer";
    case ML_ERR_EMPTY: return "empty input";
    case ML_ERR_NOMEM: return "out of memory";
    }
    return "unknown";
}
