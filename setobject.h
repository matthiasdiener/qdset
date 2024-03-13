// #ifndef Py_CPYTHON_SETOBJECT_H
// #  error "this header file must not be included directly"
// #endif

/* There are three kinds of entries in the table:

1. Unused:  key == NULL and hash == 0
2. Dummy:   key == dummy and hash == -1
3. Active:  key != NULL and key != dummy and hash != -1

The hash field of Unused slots is always zero.

The hash field of Dummy slots are set to -1
meaning that dummy entries can be detected by
either entry->key==dummy or by entry->hash==-1.
*/

#define PySet_MINSIZE 8

typedef struct {
    PyObject *key;
    Py_hash_t hash;             /* Cached hash code of the key */
} qdsetentry;

/* The SetObject data structure is shared by set and frozenset objects.

Invariant for sets:
 - hash is -1

Invariants for frozensets:
 - data is immutable.
 - hash is the hash of the frozenset or -1 if not computed yet.

*/

// typedef struct
// {
//     PyObject_HEAD
//     Py_ssize_t fill; /* Number active and dummy entries*/
//     Py_ssize_t used;    /* Number active entries */

//     /* The table contains mask + 1 slots, and that's a power of 2.
//      * We store the mask instead of the size because the mask is more
//      * frequently needed.
//      */
//     Py_ssize_t mask;

//     /* The table points to a fixed-size smalltable for small tables
//      * or to additional malloc'ed memory for bigger tables.
//      * The table pointer is never NULL which saves us from repeated
//      * runtime null-tests.
//      */
//     qdsetentry *table;
//     Py_hash_t hash;    /* Only used by frozenset objects */
//     Py_ssize_t finger; /* Search finger for pop() */

//     qdsetentry smalltable[PySet_MINSIZE];
//     PyObject *weakreflist; /* List of weak references */
// } QdPySetObject;

typedef struct
{
    Py_ssize_t dk_refcnt;

    /* Size of the hash table (dk_indices). It must be a power of 2. */
    uint8_t dk_log2_size;

    /* Size of the hash table (dk_indices) by bytes. */
    uint8_t dk_log2_index_bytes;

    /* Kind of keys */
    uint8_t dk_kind;

    /* Version number -- Reset to 0 by any modification to keys */
    uint32_t dk_version;

    /* Number of usable entries in dk_entries. */
    Py_ssize_t dk_usable;

    /* Number of used entries in dk_entries. */
    Py_ssize_t used;

    // ADDED
    qdsetentry *table;
    Py_hash_t hash; /* Only used by frozenset objects */

    /* Actual hash table of dk_size entries. It holds indices in dk_entries,
        or DKIX_EMPTY(-1) or DKIX_DUMMY(-2).

        Indices must be: 0 <= indice < USABLE_FRACTION(dk_size).

        The size in bytes of an indice depends on dk_size:

        - 1 byte if dk_size <= 0xff (char*)
        - 2 bytes if dk_size <= 0xffff (int16_t*)
        - 4 bytes if dk_size <= 0xffffffff (int32_t*)
        - 8 bytes otherwise (int64_t*)

        Dynamically sized, SIZEOF_VOID_P is minimum. */
    char dk_indices[]; /* char is required to avoid strict aliasing. */


    /* "PyDictKeyEntry or PyDictUnicodeEntry dk_entries[USABLE_FRACTION(DK_SIZE(dk))];" array follows:
        see the DK_ENTRIES() macro */
} QdPySetObject;

#define DK_LOG_SIZE(dk) ((dk)->dk_log2_size)
#if SIZEOF_VOID_P > 4
#define DK_SIZE(dk) (((int64_t)1) << DK_LOG_SIZE(dk))
#else
#define DK_SIZE(dk) (1 << DK_LOG_SIZE(dk))
#endif
#define DK_ENTRIES(dk) \
    (qdsetentry *)(&((int8_t *)((dk)->dk_indices))[(size_t)1 << (dk)->dk_log2_index_bytes])

#define DK_MASK(dk) (DK_SIZE(dk) - 1)

#define PySet_GET_SIZE(so) \
    (assert(PyAnySet_Check(so)), (((PySetObject *)(so))->used))

PyAPI_DATA(PyObject *) _PySet_Dummy;

PyAPI_FUNC(int) _PySet_NextEntry(PyObject *set, Py_ssize_t *pos, PyObject **key, Py_hash_t *hash);
PyAPI_FUNC(int) _PySet_Update(PyObject *set, PyObject *iterable);
