// bs.c for bitset

#include <glib.h>
#include <arpa/inet.h>
#include <gio/gio.h>

#include "bs.h"

#define bitsin(X) (8*sizeof(X))

// must be unsigned
typedef guint8 seg_t;

struct bitset_s {
  gint n_bits; // immutable
  gint n_segs; // immutable
  seg_t *b;
};

static inline seg_t *locate(bitset_t *bs, const glong i){
  // g_message("%d %ld %d", 1, i, (bs->n_bits)-1);
  g_assert_true(1<=i && i<=(bs->n_bits)-1); // #0 is reserver
  return &(bs->b[i/bitsin(seg_t)]);
}

static inline gboolean bs_get(bitset_t *const bs, const glong i){
  return (_Bool)((*(locate(bs, i))) & (((seg_t)1)<<(i%bitsin(seg_t))));
}

void bs_init(){

  // ensure LE

  // https://stackoverflow.com/questions/8671027/c-standard-library-function-to-find-the-byte-order-of-the-hardware
  g_assert_true(1==*((char*)(&(int){1})));

  _Static_assert(__ORDER_LITTLE_ENDIAN__==__BYTE_ORDER__, "");
  _Static_assert(G_LITTLE_ENDIAN==G_BYTE_ORDER, "");

  GRand *const rng=g_rand_new(); g_assert_true(rng);
  const guint16 s=g_rand_int(rng);
  g_assert_true(s!=htons(s));
  g_assert_true(s==ntohs(htons(s)));
  const guint32 l=g_rand_int(rng);
  g_assert_true(l!=htonl(l));
  g_assert_true(l==ntohl(htonl(l)));

}

bitset_t *bs_new(const int n){
  g_assert_true(1<=n && n<=2048);
  bitset_t *bs=g_malloc0(sizeof(bitset_t)); 
  bs->n_bits=n;
  bs->n_segs=(n+bitsin(seg_t)-1)/bitsin(seg_t);
  bs->b=g_malloc0_n(bs->n_segs, sizeof(seg_t));
  return bs;
}

// void bs_on(bitset_t *const bs, const glong i){
//   g_assert_true(!bs_get(bs, i));
//   (*locate(bs, i)) |= ((seg_t)1<<(i%sizeof(seg_t)));
// }

// void bs_off(bitset_t *const bs, const glong i){
//   g_assert_true(bs_get(bs, i));
//   (*locate(bs, i)) |= ((seg_t)1<<(i%sizeof(seg_t)));
// }

void bs_toggle(bitset_t *const bs, const glong i, const gboolean assert_after){
  *locate(bs, i) ^= ((seg_t)1)<<(i%bitsin(seg_t));
  g_assert_true(!assert_after == !bs_get(bs, i));
}

void bs_free(bitset_t *const bs){
  bs->n_bits=0;
  bs->n_segs=0;
  g_assert_true(bs->b); g_free(bs->b); bs->b=NULL;
  g_assert_true(bs); g_free(bs);
}

// display
void bs_test(bitset_t *const bs, const gint width){
  g_assert_true(!( (bs->b[0]) & (((seg_t)1)<<0) ));
  for(gint i=1; i<bs->n_bits; ++i){
    g_print("%c", bs_get(bs, i)?'O':'.');
    if(0==i%width)
      g_print(" %3d~%3d\n", i-width+1, i);
  }
  g_print("\n");
  // g_assert_true(bs->b);
  // set_t *p=bs->b;
  // gint count=0;
  // while(count<(bs->n_bits)){
  //   for(gint i=0; i<8*sizeof(seg_t); ++i){
  //     g_print("%s ", (*p)&(1<<i) ?"O":".")
  //   }
  //   g_print("\n");
  //   count+=8*sizeof(seg_t);
  //   ++p;
  // }
}

void bs_save(const bitset_t *const bs, const gchar *const filename){

  g_message("saving to '%s'", filename);

  // g_assert_true(g_file_set_contents(path, bs->b, bs->n_segs, NULL));
  // g_assert_true(0644==)
  g_assert_true(g_file_set_contents_full(filename, bs->b, bs->n_segs, G_FILE_SET_CONTENTS_CONSISTENT, 0644, NULL));

  // // back up old file (rename/copy?)
  // adwible."$(date +%s)".bak
  // // don't use g_file_query_exists because it is racy
  // // g_file_move
  // // g_file_copy
  // // g_file_delete
  // // g_file_set_display_name

  // // g_file_create
  
  // GFile *gf=g_file_new_for_path(path); g_assert_true(gf);
  // GFileOutputStream *gfo=g_file_replace(gf, NULL, FALSE, G_FILE_CREATE_NONE, NULL, NULL); g_assert_true(gout);
  // GDataOutputStream *gdo=g_data_output_stream_new(gfo);

  // // no need to worry about byte order if 8-bit
  // // https://libsoup.org/glib/glib-Byte-Order-Macros.html
  // // https://docs.gtk.org/gio/property.DataOutputStream.byte-order.html
  // _Static_assert(8==bitsin(seg_t));
  // g_assert_true(G_DATA_STREAM_BYTE_ORDER_HOST_ENDIAN==g_data_output_stream_get_byte_order(gdo));;
  // _Static_assert(8==bitsin(seg_t));


  // g_object_unref(gdo); gdo=NULL;
  // g_object_unref(gfo); gfo=NULL;
  // g_object_unref(gf); gf=NULL;

}

void bs_load(bitset_t *const bs, const gchar *const filename){
  // g_file_query_exists
  // g_file_query_info
  // g_file_load_bytes
  // g_file_load_contents
  // g_file_read

  gchar *contents=NULL;
  gsize length=0;
  GError *e=NULL;
  if(g_file_get_contents(filename, &contents, &length, &e)){
    g_assert_true(contents);
    g_assert_true(!((contents[0]) & ((seg_t)1<<0)));
    g_assert_true(length==(gsize)bs->n_segs);
    g_assert_true(bs->b); g_free(bs->b); bs->b=contents;
    g_message("'%s' loaded.", filename);
  }else{
    g_assert_true(e);
    g_assert_true(G_FILE_ERROR==e->domain);
    g_assert_true(G_FILE_ERROR_NOENT==e->code);
    // g_warning("[EE] %s", e->message);
    g_error_free(e); e=NULL;
    g_message("%s not found, starting over...", filename);
  }

}
