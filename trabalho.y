%{
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <map>
#include <vector>

using namespace std;

struct Range {
  int inicio, fim;
};

struct Tipo {
  string nome;  // O nome na sua linguagem
  string decl;  // A declaração correspondente em c-assembly
  string fmt;   // O formato para "printf"
  vector< Range > dim; // Dimensões (se não for array, fica vazio)
};

Tipo Integer = { "inteiro", "int", "d" };
Tipo Real =    { "real", "float", "f" };
Tipo Double =  { "double", "double", "lf" };
Tipo Boolean = { "boolean", "int", "d" };
Tipo String =  { "string", "char", "s" };
Tipo Char =    { "char", "char", "c" };

struct Atributo {
  string v, c;
  Tipo t;
  vector<string> lst;
};

#define YYSTYPE Atributo

int yylex();
int yyparse();
void yyerror(const char *);
void erro( string );

// Tabela de Símbolos (variáveis)
vector< map<string,Tipo> > ts;
map<string,Tipo> tf;
map< string, map< string, Tipo > > tro; // tipo_resultado_operacao;

// contadores para variáveis temporariras
map< string, int > temp_global;
map< string, int > temp_local;
map< string, int > nlabel;
bool escopo_local = false;

string toString( int n ) {
  char buf[256] = "";

  sprintf( buf, "%d", n );

  return buf;
}

int toInt( string valor ) {
  int aux = 0;

  sscanf( valor.c_str(), "%d", &aux );

  return aux;
}

void empilha_nova_tabela_de_simbolos() {
  ts.push_back( map< string, Tipo >() );
}

void desempilha_tabela_de_simbolos() {
  ts.pop_back();
}

string gera_nome_var( Tipo t ) {
  return "t_" + t.nome + "_" +
   toString( ++(escopo_local ? temp_local : temp_global)[t.nome] );
}

string gera_nome_label( string cmd ) {
  return "L_" + cmd + "_" + toString( ++nlabel[cmd] );
}

ostream& operator << ( ostream& o, const vector<string>& st ) {
  cout << "[ ";
  for( vector<string>::const_iterator itr = st.begin();
       itr != st.end(); ++itr )
    cout << *itr << " ";

  cout << "]";
  return o;
}

string trata_dimensoes_decl_var( Tipo t ) {
  string aux;

  for( int i = 0; i < t.dim.size(); i++ )
    aux += "[" + toString( t.dim[i].fim - t.dim[i].inicio + 1 )+ "]";

  return aux;
}

// 'Atributo&': o '&' significa passar por referência (modifica).
void declara_variavel( Atributo& ss,
                       vector<string> lst,
                       Tipo tipo ) {
  ss.c = "";
  for( int i = 0; i < lst.size(); i++ ) {
    if( ts[ts.size()-1].find( lst[i] ) != ts[ts.size()-1].end() )
      erro( "Variável já declarada: " + lst[i] );
    else {
      ts[ts.size()-1][ lst[i] ] = tipo;
      ss.c += tipo.decl + " " + lst[i]
              + trata_dimensoes_decl_var( tipo ) + ";\n";
    }
  }
}

void declara_variavel_function( Atributo& ss,
                       		vector<string> lst,
                       		Tipo tipo ) {
  ss.c = "";
  for( int i = 0; i < lst.size(); i++ ) {
    if( ts[ts.size()-1].find( lst[i] ) != ts[ts.size()-1].end() )
      erro( "Variável já declarada: " + lst[i] );
    else {
      ts[ts.size()-1][ lst[i] ] = tipo;
      ss.c += tipo.decl + " " + lst[i]
              + trata_dimensoes_decl_var( tipo ) + "\n";
    }
  }
}


void declara_variavel( Atributo& ss, string nome, Tipo tipo ) {
  vector<string> lst;
  lst.push_back( nome );
  declara_variavel( ss, lst, tipo );
}

void busca_tipo_da_variavel( Atributo& ss, const Atributo& s1 ) {
  if( ts[ts.size()-1].find( s1.v ) == ts[ts.size()-1].end() )
        erro( "Variável não declarada: " + s1.v );
  else {
    ss.t = ts[ts.size()-1][ s1.v ];
    ss.v = s1.v;
  }
}

void gera_codigo_atribuicao( Atributo& ss,
                             const Atributo& s1,
                             const Atributo& s3 ) {
  if( s1.t.nome == s3.t.nome )
      {
    ss.c = s1.c + s3.c + "  " + s1.v + " = " + s3.v + ";\n";
  }
  else if( s1.t.nome == s3.t.nome &&  s1.t.nome == "string" ) {
    ss.c = s1.c + s3.c + "  "
           + "strncpy( " + s1.v + ", " + s3.v + ", " +
           toString( s1.t.dim[0].fim ) + " );\n";
  }
}

string par( Tipo a, Tipo b ) {
  return a.nome + "," + b.nome;
}

void gera_codigo_operador( Atributo& ss,
                           const Atributo& s1,
                           const Atributo& s2,
                           const Atributo& s3 ) {
  if( tro.find( s2.v ) != tro.end() ) {
    if( tro[s2.v].find( par( s1.t, s3.t ) ) != tro[s2.v].end() ) {
      ss.t = tro[s2.v][par( s1.t, s3.t )];
      ss.v = gera_nome_var( ss.t );
      ss.c = s1.c + s3.c + "  " + ss.v + " = " + s1.v + s2.v + s3.v
             + ";\n";
    }
    else
      erro( "O operador '" + s2.v + "' não está definido para os tipos " + s1.t.nome + " e " + s3.t.nome + "." );
  }
  else
    erro( "Operador '" + s2.v + "' não definido." );
}

string declara_nvar_temp( Tipo t, int qtde ) {
  string aux = "";

  for( int i = 1; i <= qtde; i++ )
    aux += t.decl + " t_" + t.nome + "_" + toString( i ) + ";\n";

  return aux;
}

string declara_var_temp( map< string, int >& temp ) {
  string decls =
    declara_nvar_temp( Integer, temp[Integer.nome] ) +
    declara_nvar_temp( Real, temp[Real.nome] ) +
    declara_nvar_temp( Double, temp[Double.nome] ) +
    declara_nvar_temp( String, temp[String.nome] ) +
    declara_nvar_temp( Char, temp[Char.nome] ) +
    declara_nvar_temp( Boolean, temp[Boolean.nome] ) +
    "\n";

  temp.clear();

  return decls;
}


void gera_cmd_if( Atributo& ss,
                  const Atributo& exp,
                  const Atributo& cmd_then,
                  const Atributo& cmd_else ) {
  string lbl_then = gera_nome_label( "then" );
  string lbl_end_if = gera_nome_label( "end_if" );

  if( exp.t.nome != Boolean.nome )
    erro( "A expressão do SE deve ser booleana!" );

  ss.c = exp.c +
         "\nif( " + exp.v + " ) goto " + lbl_then + ";\n" +
         cmd_else.c + "  goto " + lbl_end_if + ";\n\n" +
         lbl_then + ":\n" +
         cmd_then.c + ";\n" +
         lbl_end_if + ":;\n";
}


void gera_cmd_for( Atributo& ss,
                  const Atributo& id,
                  const Atributo& atrib,
          const Atributo& exp,
                  const Atributo& cmd_loop ) {

  string lbl_start = gera_nome_label( "start" );
  string lbl_loop = gera_nome_label( "loop" );
  string lbl_end_for = gera_nome_label( "end_for" );

   if( exp.t.nome != Boolean.nome )
    erro( "A expressão do PARA deve ser booleana!" );
   //if( exp.v.find("=="))// || exp.v == _MOD || exp.v == _DIFERENTE)
    //erro( "A expressão booleana não é válida!\n Favor usar '<', '>', '<=' ou '>='. \n" );

    if( id.t.nome != Integer.nome )
    erro( "A variavel do PARA deve ser inteiro!" );

  ss.c = id.v + '=' + atrib.v + ';' + "\n" + exp.c +
         "\n" + lbl_start + ": if( " + exp.v + " ) goto " + lbl_loop + ";\n" +
         "goto " + lbl_end_for + ";\n\n" +
         lbl_loop + ":\n" +
         cmd_loop.c + "\n" +
     	 id.v + "=" + id.v + " + 1;" + "\n"
         + "goto " +
         lbl_start + ";\n"+
     lbl_end_for +":;\n";
}

void gera_codigo_funcao( Atributo& ss,
                         const Atributo& retorno,
                         string nome,
                         string params,
                         string bloco ) {
  ss.c = retorno.t.decl + " " + nome + "( " + params + " )" +
         "{\n" +
         retorno.c +
         declara_var_temp( temp_local ) +
         bloco +
         "return Result;\n}\n";
}


void gera_cmd_while( Atributo& ss,
                     const Atributo& exp,
                     const Atributo& cmd_loop ) {

  string lbl_start = gera_nome_label( "start" );
  string lbl_loop = gera_nome_label( "loop" );
  string lbl_end_while = gera_nome_label( "end_while" );

   if( exp.t.nome != Boolean.nome )
    erro( "A expressão do ENQUANTO deve ser booleana!" );


  ss.c = exp.c +
         "\n" + lbl_start + ": if( " + exp.v + " ) goto " + lbl_loop + ";\n" +
         "goto" + lbl_end_while + ";\n\n" +
         lbl_loop + ":\n" +
         cmd_loop.c + "\n" +

         "goto " +
         lbl_start + ";\n" +
	 lbl_end_while + ":;\n";
}

%}

%token _ID _BEGIN _END _IMPRIMELN _IMPRIME _READ _READLN _GLOBAL _IF _ELSE _WHILE _PRINCIPAL
%token _FOR _EQUALS _MENOR_IGUAL _MAIOR_IGUAL _DOIS_PTS _DO _DIFERENTE _MOD
%token _INTEGER _STRING _REAL _BOOLEAN _DOUBLE _CHAR _VOID

%token _CTE_STRING _CTE_INTEGER

%nonassoc '>' '<' _EQUALS _MENOR_IGUAL _MAIOR_IGUAL _DIFERENTE
%left '+' '-'
%left '*' '/' _MOD

%start S

%%

// TESTAR DECLARAÇAO DE VARIAVEL TALVEZ USANDO TIPOS COM TODAS AS LETRAS EM MINUSCULAS PRA VARIAVEIS E TIPOS COM A PRIMEIRA LETRA EM MAIUSCULA E O RESTO TANTO FAZ PARA FUNCOES

S :  CABECALHO MIOLOS MAIN { cout << $1.c << declara_var_temp( temp_global ) << $2.c << $3.c << endl; }
  ;

CABECALHO : { $$.c = "#include <stdlib.h>\n"
               	     "#include <string.h>\n"
                     "#include <stdio.h>\n\n";
            }
         ;

MIOLOS : MIOLO MIOLOS  { $$.c = $1.c + $2.c; }
       |               { $$.c = ""; }
       ;

MIOLO : VARS
      | FUNCTION
      ;



FUNCTION :  TIPO _ID { escopo_local = true; empilha_nova_tabela_de_simbolos(); declara_variavel( $1, "Result", $1.t ); tf[$2.v] = $1.t;}
           '(' PARAMETROS ')' BLOCO_FUNCTION
	     { gera_codigo_funcao( $$, $1, $2.v, $5.c, $7.c );
             escopo_local = false;
             desempilha_tabela_de_simbolos(); }

         | TIPO _ID { escopo_local = true; empilha_nova_tabela_de_simbolos(); declara_variavel( $1, "Result", $1.t ); tf[$2.v] = $1.t;}
         BLOCO_FUNCTION
	   { gera_codigo_funcao( $$, $1, $2.v, "", $4.c );
            escopo_local = false;
            desempilha_tabela_de_simbolos(); }
         ;


BLOCO_FUNCTION:'{' VARS_LOCAIS CMDS '}' {$$.c = $2.c + $3.c;}
          ;

PARAMETROS : DECL_FUNCTION ',' PARAMETROS { $$.c = $1.c + "," + $3.c;}
           | DECL_FUNCTION {$$.c = $1.c;}
       ;

DECL_FUNCTION: TIPO IDS_FUNCTION { declara_variavel_function( $$, $2.lst, $1.t ); }
	     ;

IDS_FUNCTION: _ID        { $$.lst.push_back( $1.v ); }
	    ;

VARS :  _GLOBAL DECLS { $$.c = $2.c; }
     ;

VARS_LOCAIS: DECLS {$$.c = $1.c;}
           |
           ;

DECLS : DECL ';' DECLS { $$.c = $1.c + $3.c; }
      | DECL ';'
      ;

DECL : TIPO IDS { declara_variavel( $$, $2.lst, $1.t ); }  // add tamanho do array depois de tipos
     ;

TIPO : _INTEGER TAM_INTEGER    	{ $$.t = $2.t; }    // faltam os tipos boolean, char, double e float.
     | _STRING TAM_STRING    	{ $$.t = $2.t; }    // passar esse tam_array somente para tipos de variaveis
     | _REAL             	{ $$.t = Real; }
     | _BOOLEAN        		{ $$.t = Boolean; }
     | _DOUBLE        		{ $$.t = Double; }
     | _CHAR TAM_CHAR    	{ $$.t = $2.t; }
     ;

TAM_STRING : '[' _CTE_INTEGER ']'
             { $$.t = String; $$.t.dim[0].fim = toInt( $2.v ); }
           | { $$.t = String; }
           ;

TAM_CHAR : '[' _CTE_INTEGER ']'
             { $$.t = String; $$.t.dim[0].fim = toInt( $2.v ); }
           | { $$.t = Char; }
           ;

TAM_INTEGER: '[' _CTE_INTEGER ']'
             { $$.t = String; $$.t.dim[0].fim = toInt( $2.v ); }
           | { $$.t = Integer; }
           ;

IDS : IDS ',' _ID    { $$.lst = $1.lst; $$.lst.push_back( $3.v ); }
    | _ID        { $$.lst.push_back( $1.v ); }
    ;

MAIN : _PRINCIPAL _BEGIN VARS_LOCAIS CMDS _END
         { $$.c = "int main() {\n" + $4.c + "  return 0;\n}\n"; }
          ;

CMDS : CMD ';' CMDS     { $$.c = $1.c + $3.c; }
     |             	{ $$.c = ""; }
     ;

CMD : SAIDA        // faltam os comandos while e do while.
    | ENTRADA
    | CMD_IF
    | CMD_FOR
    | CMD_WHILE
    | BLOCO
    | CMD_ATRIB
    | CMD_FUNCTION
    ;

CMD_ATRIB : LVALUE INDICE '=' E //{ gera_codigo_atribuicao( $$, $1, $4);}
      	  | LVALUE '=' E { gera_codigo_atribuicao( $$, $1, $3 ); }
	  | LVALUE '=' CMD_FUNCTION {gera_codigo_atribuicao( $$, $1, $3 );}
          ;

LVALUE: _ID { busca_tipo_da_variavel( $$, $1 ); }
      ;

INDICE : '[' EXPS ']' '[' EXPS ']' {$$.c = "[" + $2.c + "]" + "[" + $5.c + "]";}
       | '[' EXPS ']'  {$$.c = "[" + $2.c + "]";}
       ;

EXPS : E {$$.c = $1.c;}
     ;

CMD_FUNCTION: | _ID '(' E ')'   { $$.v = gera_nome_var( tf[$1.v] );
                      $$.c = $3.c +
                      "  " + $$.v + " = " + $1.v + "( " + $3.v + " );\n";
                      $$.t = tf[$1.v]; }

	      | _ID '(' E ',' E ')'   { $$.v = gera_nome_var( tf[$1.v] );
                      $$.c = $3.c + $5.c +
                      "  " + $$.v + " = " + $1.v + "( " + $3.v + "," + $5.v + " );\n";
                      $$.t = tf[$1.v]; }

	    ;

CMD_FOR : _FOR LVALUE '=' E ';'  E _DO BLOCO { gera_cmd_for($$, $2, $4, $6, $8);}
        ;

CMD_WHILE : _WHILE E _DO BLOCO { gera_cmd_while($$, $2, $4);}
        ;

BLOCO : '{' CMDS '}' { $$.c = $2.c ;}
      ;

CMD_IF : _IF E _DO BLOCO     //alterado - mudar declaração de IF
       | _IF E _DO BLOCO _ELSE _DO BLOCO { gera_cmd_if( $$, $2, $4, $7 ); }
       ;

SAIDA : _IMPRIME'(' E ')'     { $$.c = "  printf( \"%"+ $3.t.fmt + "\", " + $3.v + " );\n"; }
      | _IMPRIMELN '(' E ')'  { $$.c = "  printf( \"%"+ $3.t.fmt + "\\n\", " + $3.v + " );\n"; }
      ;

ENTRADA : _READLN '(' LVALUE ')' { $$.c = "  scanf( \"%"+ $3.t.fmt + "\", &"+ $3.v + " );\n"; }
        | _READ '(' LVALUE ')'   { $$.c = "  scanf( \"%"+ $3.t.fmt + "\", &"+ $3.v + " );\n"; }
        ;

E : E '+' E     	{ gera_codigo_operador( $$, $1, $2, $3 ); }
  | E '-' E     	{ gera_codigo_operador( $$, $1, $2, $3 ); }
  | E '*' E     	{ gera_codigo_operador( $$, $1, $2, $3 ); }
  | E '/' E     	{ gera_codigo_operador( $$, $1, $2, $3 ); }
  | E '>' E 	        { gera_codigo_operador( $$, $1, $2, $3 ); }
  | E '<' E       	{ gera_codigo_operador( $$, $1, $2, $3 ); }
  | E _EQUALS E        	{ gera_codigo_operador( $$, $1, $2, $3 ); }
  | E _MENOR_IGUAL E    { gera_codigo_operador( $$, $1, $2, $3 ); }
  | E _MAIOR_IGUAL E    { gera_codigo_operador( $$, $1, $2, $3 ); }
  | E _DIFERENTE E    	{ gera_codigo_operador( $$, $1, $2, $3 ); }
  | E _MOD E  		{ gera_codigo_operador( $$, $1, $2, $3 ); }
  | F
  ;


F : _CTE_STRING     { $$ = $1; $$.t = String; }
  | _CTE_INTEGER    { $$ = $1; $$.t = Integer; }
  | _ID            { busca_tipo_da_variavel( $$, $1 );  }
  | '(' E ')'        { $$ = $2;}
 ;

%%

#include "lex.yy.c"

void erro( string st ) {
  yyerror( st.c_str() );
  exit( 1 );
}

void yyerror( const char* st )
{
   if( strlen( yytext ) == 0 )
     fprintf( stderr, "%s\nNo final do arquivo\n", st );
   else
     fprintf( stderr, "%s\nProximo a: %s\nlinha/coluna: %d/%d\n", st,
              yytext, yylineno, yyrowno - (int) strlen( yytext ) );
}

void inicializa_tabela_de_resultado_de_operacoes() {
  map< string, Tipo > r;

  // OBS: a ordem é muito importante!!
  r[par(Integer, Integer)] = Integer;
  tro[ "%" ] = r;


  r[par(Integer, Real)] = Real;
  r[par(Integer, Double)] = Double;
  r[par(Real, Integer)] = Real;
  r[par(Real, Real)] = Real;
  r[par(Real, Double)] = Double;
  r[par(Double, Integer)] = Double;
  r[par(Double, Real)] = Double;
  r[par(Double, Double)] = Double;

  tro[ "-" ] = r;
  tro[ "*" ] = r;
  tro[ "/" ] = r;

  r[par(Char, Char)] = String;
  r[par(String, Char)] = String;
  r[par(Char, String)] = String;
  r[par(String, String)] = String;
  tro[ "+" ] = r;

  r.clear();
  r[par(Integer, Integer)] = Boolean;
  r[par(Real, Real)] = Boolean;
  r[par(Real, Double)] = Boolean;
  r[par(Double, Real)] = Boolean;
  r[par(Double, Double)] = Boolean;
  r[par(Char, Char)] = Boolean;
  r[par(String, Char)] = Boolean;
  r[par(Char, String)] = Boolean;
  r[par(String, String)] = Boolean;
  r[par(Boolean, Boolean)] = Boolean;
  tro["=="] = r;
  tro["!="] = r;
  tro[">="] = r;
  tro[">"] = r;
  tro["<"] = r;
  tro["<="] = r;
}


void inicializa_tipos() {
  Range r = { 0, 255 };

  String.dim.push_back( r );
}


int main( int argc, char* argv[] )
{
  inicializa_tipos();
  inicializa_tabela_de_resultado_de_operacoes();
  empilha_nova_tabela_de_simbolos();
  yyparse();
}
