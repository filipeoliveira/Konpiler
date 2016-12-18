#include <stdlib.h>
#include <string.h>
#include <stdio.h>

char t_string_1;

char formata( char a[256]
,char b[256]
 ){
char Result[256];
char t_string_1;
char t_string_2;
int t_boolean_1;

  t_boolean_1 = a>b;

if( t_boolean_1 ) goto L_then_1;
  t_string_2 = b+a;
  Result = t_string_2;
  goto L_end_if_1;

L_then_1:
  t_string_1 = a+b;
  Result = t_string_1;
;
L_end_if_1:;
return Result;
}
int main() {
  printf( "%s\n", "Digite o seu nome: " );
  scanf( "%s", &nome1 );
  printf( "%s\n", "Digite o seu sobrenome" );
  scanf( "%s", &nome2 );
  printf( "%s\n", "\n" );
  printf( "%s\n", "Bom dia, " );
  t_string_1 = formata( nome1,nome2 );
  return 0;
}

