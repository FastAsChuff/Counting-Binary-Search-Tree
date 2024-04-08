#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <assert.h>
#include <time.h>
#include </home/simon/cbst.c>

// gcc CSESNestedRanges2.c -o CSESNestedRanges2.bin -O3 -march=native -Wall 

// Inspired by https://stackoverflow.com/questions/78282577/counting-nested-ranges/78287297#78287297

typedef struct {
  uint32_t x;
  uint32_t y;
  uint32_t contains;
  uint32_t contained;
  uint32_t inputpos;
} CSESRange_t;

void solve2(CSESRange_t *Ranges, uint32_t Rangessize) {
  /*
    Sorted Ranges on x then y and updated xysortedpos.
    Now for each element i, need to know the number of elements before it with greater or equal y.
    Get this number (Ranges[i].contained) by finding the index position to insert it at from array of previous elements sorted on y.    
  */
  struct CBSTnode *CBSTroot = NULL;
  uint32_t lt, eq, gt;
  for (uint32_t i=0; i<Rangessize; i++) {
    CountingBSTinsert(&CBSTroot, -1, &lt, &eq, &gt, 1, Ranges[i].y);
    Ranges[i].contained = gt + eq - 1;
  }
  CountingBSTfree(&CBSTroot);
  /*
    Sorted Ranges on x then y and updated xysortedpos.
    Backwards for each element i, need to know the number of elements before it with less than or equal y.
    Get this number (Ranges[i].contains) by finding the index position to insert it at from array of previous elements sorted on y.    
  */
  for (int32_t i=Rangessize-1; i>=0; i--) {
    CountingBSTinsert(&CBSTroot, -1, &lt, &eq, &gt, 1, Ranges[i].y);
    Ranges[i].contains = lt + eq - 1;
  }
  CountingBSTfree(&CBSTroot);
}

void solve(CSESRange_t *Ranges, uint32_t Rangessize) {
  // Nodes on an array version.
  /*
    Sorted Ranges on x then y and updated xysortedpos.
    Now for each element i, need to know the number of elements before it with greater or equal y.
    Get this number (Ranges[i].contained) by finding the index position to insert it at from array of previous elements sorted on y.    
  */  
  struct CBSTnode *CBSTroot = malloc(Rangessize*sizeof(CountingBSTnode_t));
  uint32_t lt, eq, gt;
  for (int32_t i=0; i<Rangessize; i++) {
    CountingBSTinsert(&CBSTroot, i, &lt, &eq, &gt, 1, Ranges[i].y);
    Ranges[i].contained = gt + eq - 1;
  }
  /*
    Sorted Ranges on x then y and updated xysortedpos.
    Backwards for each element i, need to know the number of elements before it with less than or equal y.
    Get this number (Ranges[i].contains) by finding the index position to insert it at from array of previous elements sorted on y.    
  */
  for (int32_t i=Rangessize-1; i>=0; i--) {
    CountingBSTinsert(&CBSTroot, Rangessize-1-i, &lt, &eq, &gt, 1, Ranges[i].y);
    Ranges[i].contains = lt + eq - 1;
  }
  free(CBSTroot);
}

void naivesolve(CSESRange_t *Ranges, uint32_t Rangessize) {
  for (uint32_t i=0; i<Rangessize; i++) {
    uint32_t x = Ranges[i].x;
    uint32_t y = Ranges[i].y;
    for (uint32_t j=0; j<Rangessize; j++) {
      if ((Ranges[j].x <= x) && (Ranges[j].y >= y)) Ranges[i].contained++;
      if ((Ranges[j].x >= x) && (Ranges[j].y <= y)) Ranges[i].contains++;
    }
    Ranges[i].contained--;
    Ranges[i].contains--;
  }
}

void printsolution(CSESRange_t *Ranges, uint32_t Rangessize) {
  for (uint32_t i=0; i<Rangessize; i++) {
    printf("[%u, %u] ", Ranges[i].x, Ranges[i].y);
  }
  printf("\n");
  for (uint32_t i=0; i<Rangessize; i++) {
    printf("%u ", Ranges[i].contains);
  }
  printf("\n");
  for (uint32_t i=0; i<Rangessize; i++) {
    printf("%u ", Ranges[i].contained);
  }
  printf("\n");
}

int sortonxy(const void*RangeA_ptr, const void*RangeB_ptr) {
  // Increasing x, decreasing y.
  const CSESRange_t *RangeA = RangeA_ptr;
  const CSESRange_t *RangeB = RangeB_ptr;
  if (RangeA->x < RangeB->x) return -1;
  if (RangeA->x == RangeB->x) {
    assert(RangeA->y != RangeB->y);
    if (RangeA->y > RangeB->y) return -1;
    if (RangeA->y == RangeB->y) return 0;
  }
  return 1;
}

int sortoninputpos(const void*RangeA_ptr, const void*RangeB_ptr) {
  const CSESRange_t *RangeA = RangeA_ptr;
  const CSESRange_t *RangeB = RangeB_ptr;
  if (RangeA->inputpos < RangeB->inputpos) return -1;
  return 1;
}

int main(int argc, char*argv[]) {
  uint32_t maxy = 1000000000;
  srand(55555555);
  //srand(time(0));
  if (argc == 1) {
    printf("This program solves the CSES Nested Ranges [intervals] Problem for n random intervals [x_i, y_i].\n");
    printf("There are two output arrays.\n");
    printf("The first is the number of ranges the interval contains.\n");
    printf("The second is the number of ranges contained by the interval.\n");
    printf("[a,b] contains [c,d] iff ((c >= a) && (d <= b)).\n");
    printf("Ranges are assumed to be distinct.\n");
    printf("Author: Simon Goater Mar 2024.\n");
    printf("Usage: %s n\n", argv[0]);
    printf("1 <= n <= 2*10^5\n");
    printf("1 <= x_i < y_i <= 10^9\n");
    exit(0);
  }
  int64_t Rangessize = atol(argv[1]);
  assert(Rangessize >= 1);
  assert(Rangessize <= 200000);
  
  CSESRange_t *Ranges = calloc(Rangessize, sizeof(CSESRange_t));
  for (uint32_t i=0; i<Rangessize; ) {
    Ranges[i].x = 1+(rand() % maxy);
    Ranges[i].y = 1+(rand() % maxy);
    Ranges[i].inputpos = i;
    if (Ranges[i].x < Ranges[i].y) i++;
  }  
  if ((argc > 2) && (strcmp("naive", argv[2]) == 0)) {
    naivesolve(Ranges, Rangessize);
  } else {
    qsort(Ranges, Rangessize, sizeof(CSESRange_t),
            &sortonxy);
    solve(Ranges, Rangessize);
    qsort(Ranges, Rangessize, sizeof(CSESRange_t),
            &sortoninputpos);
  }
  printsolution(Ranges, Rangessize);
}
