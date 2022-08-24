// biblical canon

#include <stddef.h> // NULL

#include "bc.h"

#define A 27158, 12200, 24510
#define N 32032

int bc_a_0[]={A};
// int *bc_a_1={A}; // err
int *bc_a_2=(int[]){A};
int *bc_a_3=&(((int[]){A})[0]);

int bc_b_0=N;
int *bc_b_1=&(int){N};

struct {
  int x;
} bc_c_0={N};
struct {
  int *x;
} bc_c_1={&(int){N}};

struct bc_d_0_s {
  void *_; // avoid "flexible array member in a struct with no named members"
  int x[];
} bc_d_0={._=NULL, .x={A}};
struct {
  void *_; // avoid "flexible array member in a struct with no named members"
  int *x;
} bc_d_1={._=NULL, .x=(int[]){A}};

void bc_init(){

}
