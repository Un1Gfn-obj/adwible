// bs.c for bitset

#include <glib.h>
#include <arpa/inet.h>

#include "bs.h"

#define nbits(X) (8*sizeof(X))

// must be unsigned !!!
typedef gulong seg_t;
_Static_assert(64==nbits(seg_t));
struct bitset_s {
  gint n;
  seg_t *b;
};

/*static inline */
seg_t *locate(bitset_t *bs, const glong i){
  // g_message("%d %ld %d", 1, i, (bs->n)-1);
  g_assert_true(1<=i && i<=(bs->n)-1); // #0 is reserver
  return &(bs->b[i/nbits(seg_t)]);
}

void bs_init(){
  // ensure little endian
  // https://stackoverflow.com/questions/8671027/c-standard-library-function-to-find-the-byte-order-of-the-hardware
  g_assert_true(1==*((char*)(&(int){1})));
  _Static_assert(__BYTE_ORDER__==__ORDER_LITTLE_ENDIAN__, "");
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
  bitset_t *s=g_malloc0(sizeof(bitset_t)); 
  s->n=n;

  const gint n_segs=(n+nbits(seg_t)-1)/nbits(seg_t);
  // g_print("%d\n", n_segs);
  s->b=g_malloc0_n(n_segs, sizeof(seg_t));
  // s->b=calloc(1, 969/8);

  return s;

}

gboolean bs_get(bitset_t *const bs, const glong i){
  // if(i==32){
  //   g_print("\n");
  //   g_print("0x%016lX\n", *locate(bs, i));
  //   g_print("%ld\n", i%nbits(seg_t));
  //   g_print("0x%016lX\n", ((seg_t)1<<(i%nbits(seg_t))));
  //   g_print("0x%016lX\n",               (*(locate(bs, i))) & (((seg_t)1)<<(i%nbits(seg_t)))  );
  //   g_print("%d\n",          (_Bool)((*(locate(bs, i))) & (((seg_t)1)<<(i%nbits(seg_t))) ));

  //   g_print("\n");
  // }
  // never cast anything into gboolean!!!
  // _Static_assert((gboolean)0x0000000100000000, "");
  return (_Bool)((*(locate(bs, i))) & (((seg_t)1)<<(i%nbits(seg_t))));
}

// void bs_on(bitset_t *const bs, const glong i){
//   g_assert_true(!bs_get(bs, i));
//   *locate(bs, i) |= 1<<(i%sizeof(seg_t));
// }

// void bs_off(bitset_t *const bs, const glong i){
//   g_assert_true(bs_get(bs, i));
//   *locate(bs, i) |= 1<<(i%sizeof(seg_t));
// }

void bs_toggle(bitset_t *const bs, const glong i, const gboolean assert_after){
  // g_print("0x%lX\n", *locate(bs, i));
  *locate(bs, i) ^= ((seg_t)1)<<(i%nbits(seg_t));
  // g_print("0x%lX\n", *locate(bs, i));
  // g_print("%d\n", assert_after);
  // g_print("%d\n", bs_get(bs, i));
  g_assert_true(!assert_after == !bs_get(bs, i));
}

void bs_free(bitset_t *const s){
  g_assert_true(s->b);g_free(s->b); s->b=NULL;
  g_assert_true(s);g_free(s);
}

// display
void bs_test(bitset_t *const bs, const gint width){
  g_assert_true(!( (bs->b[0]) & (((seg_t)1)<<0) ));
  for(gint i=1; i<bs->n; ++i){
    g_print("%c", bs_get(bs, i)?'O':'.');
    if(0==i%width)
      g_print(" %3d~%3d\n", i-width+1, i);
  }
  g_print("\n");
  // g_assert_true(bs->b);
  // set_t *p=bs->b;
  // gint count=0;
  // while(count<(bs->n)){
  //   for(gint i=0; i<8*sizeof(seg_t); ++i){
  //     g_print("%s ", (*p)&(1<<i) ?"O":".")
  //   }
  //   g_print("\n");
  //   count+=8*sizeof(seg_t);
  //   ++p;
  // }
}
