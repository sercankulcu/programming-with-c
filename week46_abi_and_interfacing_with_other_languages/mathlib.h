/* mathlib.h */
#ifndef MATHLIB_H
#define MATHLIB_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    ML_OK = 0,
    ML_ERR_NULL     = 1,
    ML_ERR_EMPTY    = 2,
    ML_ERR_NOMEM    = 3
} MlStatus;

/* Scalars: the easiest thing to call from anywhere. */
int    ml_add(int a, int b);
double ml_mean(const double *values, size_t count, MlStatus *status);

/* A caller-supplied buffer: no ownership question at all. */
MlStatus ml_scale(const double *in, double *out, size_t count, double factor);

/* C allocates, C frees — the pair must be used together. */
double  *ml_make_range(double start, double step, size_t count);
void     ml_free(double *p);

/* An opaque handle: a foreign caller never needs the layout. */
typedef struct MlAccumulator MlAccumulator;

MlAccumulator *ml_acc_create(void);
void           ml_acc_destroy(MlAccumulator *acc);
MlStatus       ml_acc_add(MlAccumulator *acc, double value);
double         ml_acc_mean(const MlAccumulator *acc);
size_t         ml_acc_count(const MlAccumulator *acc);

/* A callback: explicit context, no hidden state. */
typedef int (*MlVisitor)(double value, void *context);
MlStatus ml_each(const double *values, size_t count,
                 MlVisitor visit, void *context);

const char *ml_status_string(MlStatus s);

#ifdef __cplusplus
}
#endif
#endif /* MATHLIB_H */
