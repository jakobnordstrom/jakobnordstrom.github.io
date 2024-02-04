
#ifndef BOOLEAN_H
#define BOOLEAN_H

typedef unsigned    Boolean;

#undef TRUE
#undef FALSE

#define TRUE    1
#define FALSE   0


/*
 *  XOR (A,B) computes the exclusive or of A and B.
 *  
 * PRE:
 *   A and B are set to either TRUE or FALSE.
 *  
 */

#undef XOR
#define XOR(A, B)    (A) ^ (B)

#endif				/* #ifndef BOOLEAN_H */


