// bs.c for bitset

#include <glib.h>
#include <arpa/inet.h>

#include "bs.h"

// must be unsigned !!!
typedef gulong seg_t;

// private
_Static_assert(64/8==sizeof(seg_t));
struct bitset_s {
  gint n;
  seg_t *b;
};

static inline seg_t *locate(bitset_t *bs, const glong i){
  g_assert_true(1<=i && i<=(bs->n)-1); // #0 is reserver
  return &(bs->b[i/sizeof(seg_t)]);
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

  // each seg provides [x] bits
  static const int x=8*sizeof(seg_t);

  // need [(n+x-1) div x] segments
  s->b=g_malloc0_n((n+x-1)/x, sizeof(seg_t));

  return s;

}

gboolean bs_get(bitset_t *const bs, const glong i){
  return *(locate(bs, i)) & (1<<(i%sizeof(seg_t)));
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
  *locate(bs, i) ^= 1<<(i%sizeof(seg_t));
  g_assert_true(!assert_after == !bs_get(bs, i));
}

void bs_free(bitset_t *const s){
  g_assert_true(s->b);g_free(s->b); s->b=NULL;
  g_assert_true(s);g_free(s);
}
