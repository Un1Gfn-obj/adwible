// bs.h for bitset
#pragma once

// this type is incomplete on purpose
typedef struct bitset_s bitset_t;

void bs_init();
bitset_t *bs_new(const int);
void bs_toggle(bitset_t *const, const glong, const gboolean);
void bs_free(bitset_t *const);