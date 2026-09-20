mkdir -p /tmp/consumer && cd /tmp/consumer
cat > app.c <<'EOF'
#include <intarray.h>
#include <stdio.h>

int main(void)
{
    printf("compiled against %d.%d.%d, running %s\n",
           INTARRAY_VERSION_MAJOR, INTARRAY_VERSION_MINOR,
           INTARRAY_VERSION_PATCH, intarray_version_string());

    IntArray *a = intarray_create(4);
    if (a == NULL) return 1;
    for (int i = 0; i < 10; i++) intarray_push(a, i * i);
    printf("count %zu\n", intarray_count(a));
    intarray_destroy(a);
    return 0;
}
EOF

export PKG_CONFIG_PATH=/tmp/stage/usr/lib/pkgconfig
pkg-config --cflags --libs intarray
gcc app.c $(pkg-config --cflags --libs intarray) -o app
LD_LIBRARY_PATH=/tmp/stage/usr/lib ./app
ldd app | grep intarray
