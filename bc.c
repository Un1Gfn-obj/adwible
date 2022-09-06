// biblical canon

// #include <stddef.h> // NULL
#include <glib.h> // g_assert_true

#include "bc.h" // biblical canon
#include "bs.h" // bitset

#define NULLBOOK 0,NULL,NULL
#define NULLGRP 0,NULL,NULL,NULL

static bc_dyn_t dyn_tanakh={0};
static bc_dyn_t dyn_nt={0};

// old testament
const bc_testament_t bc_tanakh={
  .dyn=&dyn_tanakh,
  .fn_progress="./tanakh.bin",
  .fn_autoexpand="./tanakh.expand",
  // https://justwordministries.com/wp-content/uploads/2020/02/Bible-Books-Chapters-and-Verses-Chart.pdf
  .n_total_groups=5,
  .n_total_books=39,
  .n_total_chapters=929,
  .groups=(bc_group_t[]){
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
  }
};

const bc_testament_t bc_newtestament={
  .dyn=&dyn_nt,
  .fn_progress="./newtestament.bin",
  .fn_autoexpand="./newtestament.expand",
  // https://justwordministries.com/wp-content/uploads/2020/02/Bible-Books-Chapters-and-Verses-Chart.pdf
  .n_total_groups=5,
  .n_total_books=27,
  .n_total_chapters=260,
  .groups=(bc_group_t[]){
    {
      4,
      "Canonical gospels",
      "Quattuor Evangelia",
      (bc_book_t[]){
        { 28 , "Matthew" , "Evangelium secundum Matthaeum" },
        { 16 , "Mark"    , "Evangelium secundum Marcum"    },
        { 24 , "Luke"    , "Evangelium secundum Lucam"     },
        { 21 , "John"    , "Evangelium secundum Ioannem"   },
        { NULLBOOK }
      }
    },
    {
      1,
      "Acts of apostles",
      "Actus Apostolorum",
      (bc_book_t[]){
        { 28 , "Acts" , "Actus" },
        { NULLBOOK }
      }
    },
    {
      13,
      "Pauline epistles",
      "Epistulae Paulinae",
      (bc_book_t[]){
        { 16 , "Romans"          , "Epistula ad Romanos"             },
        { 16 , "1 Corinthians"   , "Epistula I ad Corinthios"        },
        { 13 , "2 Corinthians"   , "Epistula II ad Corinthios"       },
        {  6 , "Galatians"       , "Epistula ad Galatas"             },
        {  6 , "Ephesians"       , "Epistula ad Ephesios"            },
        {  4 , "Philippians"     , "Epistula ad Philippenses"        },
        {  4 , "Colossians"      , "Epistula ad Colossenses"         },
        {  5 , "1 Thessalonians" , "Epistula I ad Thessalonicenses"  },
        {  3 , "2 Thessalonians" , "Epistula II ad Thessalonicenses" },
        {  6 , "1 Timothy"       , "Epistula I ad Timotheum"         },
        {  4 , "2 Timothy"       , "Epistula II ad Timotheum"        },
        {  3 , "Titus"           , "Epistula ad Titum"               },
        {  1 , "Philemon"        , "Epistulam ad Philemonem"         },
        { NULLBOOK }
      }
    },
    {
      8,
      "Catholic epistles",
      "Epistulae catholicae",
      (bc_book_t[]){
        {  13 , "Hebrews" , "Epistula ad Hebraeos" },
        {   5 , "James"   , "Epistula Iacobi"      },
        {   5 , "1 Peter" , "Epistula I Petri"     },
        {   3 , "2 Peter" , "Epistula II Petri"    },
        {   5 , "1 John"  , "Epistula I Ioannis"   },
        {   1 , "2 John"  , "Epistula II Ioannis"  },
        {   1 , "3 John"  , "Epistula III Ioannis" },
        {   1 , "Jude"    , "Epistula Iudae"       },
        { NULLBOOK }
      }
    },
    {
      1,
      "Apocalypse",
      "Apocalypsis",
      (bc_book_t[]){
        { 22 , "Revelation" , "Apocalypsis Ioannis / Apokalypsis" },
        { NULLBOOK }
      }
    },
  }
};

void bc_init(const bc_testament_t *const t){
  int cnt_books=0;
  int cnt_chapters=0;
  const bc_group_t *g=t->groups;
  for(; 0!=g->n_books; ++g){
    cnt_books+=g->n_books;
    const bc_book_t *b=g->books;
    for(; 0!=b->n_chapters; ++b)
      cnt_chapters+=b->n_chapters;
    g_assert_true(g->n_books==b-g->books);
  }
  g_assert_true(t->n_total_groups   == g-t->groups);
  g_assert_true(t->n_total_books    == cnt_books);
  g_assert_true(t->n_total_chapters == cnt_chapters);

  t->dyn->bs=bs_new(t->n_total_chapters+1);
  bs_load(t->dyn->bs, t->fn_progress);
  // bs_test(t->dyn->bs);

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
