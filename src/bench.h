#ifndef BENCH_H
#define BENCH_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

#ifndef uint
#define uint unsigned int
#endif

#ifndef ulong
#define ulong unsigned long
#endif

/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- *
 *   B e n c h 4                                                           *
 * ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- *
 *
 * vector structure over 4 layers of 4 bits entry (16)
 * 
 * ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- */

typedef struct {
  uint start;
  uint size;
  void* _dsc[16];
} bench4;

bench4* bench4_create( bench4* self )
{
    self->start= 0;
    self->size= 0;
    memset(self->_dsc, 0, 128);
    return self;
}

bench4* bench4_destroy( bench4* self )
{
    return self;
}

static inline bench4* newBench4(){ return bench4_create( (bench4*)(malloc( sizeof(bench4) )) ); };
static inline uint deleteBench4(bench4* self){
  bench4_destroy(self);
  free(self);
  return 1;
};

// Accessor :

static inline uint bench4_size( bench4* self ){ return self->size; };

#endif // BENCH_H
