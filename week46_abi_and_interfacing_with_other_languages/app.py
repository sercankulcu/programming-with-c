# app.py
import ctypes, os

lib = ctypes.CDLL(os.path.abspath("libmathlib.so"))

# Declare every signature. ctypes assumes int otherwise, and a wrong
# assumption about a double is silent corruption, not an error.
lib.ml_add.argtypes = [ctypes.c_int, ctypes.c_int]
lib.ml_add.restype  = ctypes.c_int

lib.ml_mean.argtypes = [ctypes.POINTER(ctypes.c_double), ctypes.c_size_t,
                        ctypes.POINTER(ctypes.c_int)]
lib.ml_mean.restype  = ctypes.c_double

lib.ml_make_range.argtypes = [ctypes.c_double, ctypes.c_double, ctypes.c_size_t]
lib.ml_make_range.restype  = ctypes.POINTER(ctypes.c_double)
lib.ml_free.argtypes = [ctypes.POINTER(ctypes.c_double)]

lib.ml_acc_create.restype   = ctypes.c_void_p        # opaque handle
lib.ml_acc_destroy.argtypes = [ctypes.c_void_p]
lib.ml_acc_add.argtypes     = [ctypes.c_void_p, ctypes.c_double]
lib.ml_acc_mean.argtypes    = [ctypes.c_void_p]
lib.ml_acc_mean.restype     = ctypes.c_double

VISITOR = ctypes.CFUNCTYPE(ctypes.c_int, ctypes.c_double, ctypes.c_void_p)
lib.ml_each.argtypes = [ctypes.POINTER(ctypes.c_double), ctypes.c_size_t,
                        VISITOR, ctypes.c_void_p]

print("ml_add(3, 4) =", lib.ml_add(3, 4))

data = (ctypes.c_double * 5)(1.0, 2.0, 3.0, 4.0, 5.0)
status = ctypes.c_int()
print("ml_mean =", lib.ml_mean(data, 5, ctypes.byref(status)),
      "status", status.value)

# C allocated it, so C must free it.
p = lib.ml_make_range(0.0, 0.5, 6)
print("range =", [p[i] for i in range(6)])
lib.ml_free(p)

acc = lib.ml_acc_create()
for v in (10.0, 20.0, 30.0):
    lib.ml_acc_add(acc, v)
print("accumulator mean =", lib.ml_acc_mean(acc))
lib.ml_acc_destroy(acc)

# A Python function called back from C. The reference must be kept
# alive in a variable — if it is garbage collected mid-call, C jumps
# into freed memory.
total = [0.0]
def on_value(value, _ctx):
    total[0] += value
    return 0
callback = VISITOR(on_value)          # keep this name bound
lib.ml_each(data, 5, callback, None)
print("visited total =", total[0])
