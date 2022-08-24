// biblical canon

#pragma once

// bible
//   testament (old, new)
//     group (Torah, Ketuvim, Gospel, ...)
//       book (Genesis, Revelation, ...)
//         chapter (1, 2, 3, ...)

typedef struct bc_group_s bc_group_t;
typedef struct bc_book_s bc_book_t;

// err "array type has incomplete element type"
// typedef const bc_group_t bc_testament_t[];

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

typedef const bc_group_t bc_testament_t[];

// old testament
extern const bc_testament_t tanakh;
extern void bc_validate();
