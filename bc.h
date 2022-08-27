// biblical canon

#pragma once

#include "bs.h" // bitset

// bible
//   testament (old, new)
//     group (Torah, Ketuvim, Gospel, ...)
//       book (Genesis, Revelation, ...)
//         chapter (1, 2, 3, ...)

typedef struct bc_testament_s bc_testament_t;
typedef struct bc_group_s     bc_group_t;
typedef struct bc_book_s      bc_book_t;

// err "array type has incomplete element type"
// typedef const bc_group_t bc_testament_t[];

struct bc_testament_s {
  const char *const progress;
  const char *const autoexpand;
  const int n_total_groups;
  const int n_total_books;
  const int n_total_chapters;
  const bc_group_t *const groups;
};

struct bc_group_s {
  const int n_books;
  const char *const title;
  const char *const description;
  bc_book_t *const books;
};

struct bc_book_s {
  const int n_chapters;
  const char *const title;
  const char *const subtitle;
};

// old testament
extern const bc_testament_t tanakh;
extern bitset_t *bs_tanakh;

extern void bc_init();
