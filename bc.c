// biblical canon

// #include <stddef.h> // NULL
#include <glib.h> // g_assert_true

#include "bc.h"

#define NULLBOOK 0,NULL,NULL
#define NULLGRP 0,NULL,NULL,NULL

// old testament
const bc_testament_t tanakh={
  {
    5,
    "Torah",
    "Law Pentateuch",
    (bc_book_t[]){
      { 50 , "Genesis"     , "Bereshit" }, // 1-, 12-
      { 40 , "Exodus"      , "Shemot"   }, // 1-, 20-
      { 27 , "Leviticus"   , "Vayikra"  },
      { 36 , "Numbers"     , "Bamidbar" },
      { 34 , "Deuteronomy" , "Devarim"  },
      { NULLBOOK }
    }
  },
  {
    12,
    "Former",
    "Prophets/Nevi'im Historical",
    (bc_book_t[]){
      { 24 , "Joshua"       , "Yehoshua"         },
      { 21 , "Judges"       , "Shofetim"         },
      {  4 , "Ruth"         , "Rut"              },
      { 31 , "1 Samuel"     , "1 Shemuel"        },
      { 24 , "2 Samuel"     , "2 Shemuel"        },
      { 22 , "1 Kings"      , "1 Melakhim"       },
      { 25 , "2 Kings"      , "2 Melakhim"       },
      { 29 , "1 Chronicles" , "1 Divrei Hayamim" },
      { 36 , "2 Chronicles" , "2 Divrei Hayamim" },
      { 10 , "Ezra"         , NULL               },
      { 13 , "Nehemiah"     , "Nəhemyah"         },
      { 10 , "Esther"       , "Ester"            },
      { NULLBOOK }
    }
  },
  {
    5,
    "Ketuvim",
    "Wisdom Poetic Sapiential",
    (bc_book_t[]){
      {  42 , "Job"           , "Iyov"           },
      { 150 , "Psalms"        , "Tehillim"       }, // 1-, 42-, 73-, 90-, 107-, 
      {  31 , "Proverbs"      , "Mishlei"        },
      {  12 , "Ecclesiastes"  , "Qohelet"        },
      {   8 , "Song of Songs" , "Shir Hashirim"  },
      { NULLBOOK }
    }
  },
  {
    5,
    "Major",
    "Prophets/Nevi'im Latter",
    (bc_book_t[]){
      { 66 , "Isaiah"       , "Yeshayahu" },
      { 52 , "Jeremiah"     , "Yirmeyahu" },
      {  5 , "Lamentations" , "Eikhah"    },
      { 48 , "Ezekiel"      , "Yekhezqel" },
      { 12 , "Daniel"       , "Daniyyel"  },
      { NULLBOOK }
    }
  },
  {
    12,
    "Minor",
    "Trei Asar",
    (bc_book_t[]){
      { 14 , "[01] Hosea"     , NULL       },
      {  3 , "[02] Joel"      , "Yoel"     },
      {  9 , "[03] Amos"      , NULL       },
      {  1 , "[04] Obadiah"   , "Obadyahu" },
      {  4 , "[05] Jonah"     , NULL       },
      {  7 , "[06] Micah"     , "Mikayahu" },
      {  3 , "[07] Nahum"     , NULL       },
      {  3 , "[08] Habakkuk"  , NULL       },
      {  3 , "[09] Zephaniah" , "Səfanya"  },
      {  2 , "[10] Haggai"    , "Haggay"   },
      { 14 , "[11] Zechariah" , "Zəkarya"  },
      {  4 , "[12] Malachi"   , "Malaki"   },
      { NULLBOOK }
    }
  },
  {
    NULLGRP
  }
};

void bc_validate(){

  // print("%s(): stub", __FUNC__);
  // https://justwordministries.com/wp-content/uploads/2020/02/Bible-Books-Chapters-and-Verses-Chart.pdf

  // 5 groups
  // 39 books
  // 929 chapters
  int total_books=0;
  int total_chapters=0;
  const bc_group_t *g=tanakh;
  for(; 0!=g->n_books; ++g){
    total_books+=g->n_books;
    const bc_book_t *b=g->books;
    for(; 0!=b->n_chapters; ++b)
      total_chapters+=b->n_chapters;
    g_assert_true(g->n_books==b-g->books);
  }
  // _Static_assert(5==sizeof(tanakh)/sizeof(bc_group_t), "");
  g_assert_true(5==g-tanakh);
  // printf("%d\n", total_books);
  g_assert_true(39==total_books);
  g_assert_true(929==total_chapters);

}


// #define A 27158, 12200, 24510
// #define N 32032

// int bc_a_0[]={A};
// // int *bc_a_1={A}; // err
// int *bc_a_2=(int[]){A};
// int *bc_a_3=&(((int[]){A})[0]);

// int bc_b_0=N;
// int *bc_b_1=&(int){N};

// struct {
//   int x;
// } bc_c_0={N};
// struct {
//   int *x;
// } bc_c_1={&(int){N}};

// struct bc_d_0_s {
//   void *_; // avoid "flexible array member in a struct with no named members"
//   int x[];
// } bc_d_0={._=NULL, .x={A}};
// struct {
//   void *_; // avoid "flexible array member in a struct with no named members"
//   int *x;
// } bc_d_1={._=NULL, .x=(int[]){A}};
