// biblical canon

#pragma once

#include <gtk/gtk.h> // GtkWidget

#include "bs.h" // bitset

// bible
//   testament (old, new)
//     group (Torah, Ketuvim, Gospel, ...)
//       book (Genesis, Revelation, ...)
//         chapter (1, 2, 3, ...)

typedef struct bc_testament_s bc_testament_t;
typedef struct bc_group_s     bc_group_t;
typedef struct bc_book_s      bc_book_t;
typedef struct bc_dyn_s       bc_dyn_t;

struct bc_dyn_s {
  //
  bitset_t *bs;
  const char *autoexpand;
  //
  gboolean   scroll_do;
  GObject   *scroll_gsw; // GtkScrolledWindow
  GtkWidget *scroll_aer; // AdwExpanderRow
  GtkWidget *scroll_apg; // AdwPreferencesGroup
};

struct bc_testament_s {
  bc_dyn_t *const dyn;
  const char *const fn_progress;
  const char *const fn_autoexpand;
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

extern const bc_testament_t bc_tanakh;
extern const bc_testament_t bc_newtestament;

void bc_init(const bc_testament_t *const);
