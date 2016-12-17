#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int t_inteiro_1;

int mdc( int x
,int y
 ){
int Result;
int t_inteiro_1;
int t_inteiro_2;
int t_inteiro_3;
int t_boolean_1;

  t_inteiro_1 = x%y;
  t_boolean_1 = t_inteiro_1==0;

if( t_boolean_1 ) goto L_then_1;
  t_inteiro_2 = x%y;
  t_inteiro_3 = mdc( y,t_inteiro_2 );
  Result = t_inteiro_3;
  goto L_end_if_1;

L_then_1:
  Result = y;
;
L_end_if_1:;
return Result;
}
int main() {
  a = 30;
  b = 45;
  t_inteiro_1 = mdc( a,b );
  return 0;
}

