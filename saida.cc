#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int t_inteiro_1;

int mdc( int a
,int b
 ){
int Result;
int t_inteiro_1;
int t_inteiro_2;
int t_boolean_1;

  t_boolean_1 = b==0;

if( t_boolean_1 ) goto L_then_1;
  t_inteiro_1 = a%b;
  t_inteiro_2 = mdc( a,t_inteiro_1 );
  Result = t_inteiro_2;
  goto L_end_if_1;

L_then_1:
  Result = a;
;
L_end_if_1:;
return Result;
}
int main() {
  printf( "%s\n", "Programa MDC" );
  printf( "%s\n", "Digite o primeiro numero:" );
  scanf( "%d", &a );
  printf( "%s\n", "Digite o segundo numero:" );
  scanf( "%d", &b );
  t_inteiro_1 = mdc( a,b );
  result = t_inteiro_1;
  printf( "%s\n", "O resultado do mdc entre os dois numeros é: " );
  printf( "%d\n", result );
  return 0;
}

