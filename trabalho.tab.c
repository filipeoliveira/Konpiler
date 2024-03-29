/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.0.4"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
#line 1 "trabalho.y" /* yacc.c:339  */

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
      if(s2.v == '+'){
        string tmp;
        strcpy(tmp, s1.v);
        strcat(tmp, s3.v);
      }
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

void gera_cmd_ifgreater( Atributo& ss,
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


#line 388 "trabalho.tab.c" /* yacc.c:339  */

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif


/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    _ID = 258,
    _BEGIN = 259,
    _END = 260,
    _IMPRIMELN = 261,
    _IMPRIME = 262,
    _READ = 263,
    _READLN = 264,
    _GLOBAL = 265,
    _IF = 266,
    _ELSE = 267,
    _WHILE = 268,
    _PRINCIPAL = 269,
    _IFGREATER = 270,
    _IFLESS = 271,
    _IFEQUAL = 272,
    _FOR = 273,
    _EQUALS = 274,
    _MENOR_IGUAL = 275,
    _MAIOR_IGUAL = 276,
    _DOIS_PTS = 277,
    _DO = 278,
    _DIFERENTE = 279,
    _MOD = 280,
    _AND = 281,
    _OR = 282,
    _PLUS = 283,
    _LESS = 284,
    _TIMES = 285,
    _DIVIDE = 286,
    _INTEGER = 287,
    _STRING = 288,
    _REAL = 289,
    _BOOLEAN = 290,
    _DOUBLE = 291,
    _CHAR = 292,
    _VOID = 293,
    _CTE_STRING = 294,
    _CTE_INTEGER = 295
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);



/* Copy the second part of user declarations.  */

#line 477 "trabalho.tab.c" /* yacc.c:358  */

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif


#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  3
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   288

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  56
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  38
/* YYNRULES -- Number of rules.  */
#define YYNRULES  84
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  172

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   295

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      47,    48,    45,    43,    51,    44,     2,    46,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    52,
      42,    55,    41,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    53,     2,    54,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    49,     2,    50,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   339,   339,   342,   348,   349,   352,   353,   358,   358,
     364,   364,   372,   375,   376,   379,   382,   385,   388,   389,
     392,   393,   396,   399,   400,   401,   402,   403,   404,   407,
     409,   412,   414,   417,   419,   422,   423,   426,   430,   431,
     434,   435,   436,   437,   438,   439,   440,   441,   442,   445,
     446,   447,   450,   453,   454,   457,   460,   460,   465,   472,
     475,   478,   481,   482,   485,   488,   489,   492,   493,   496,
     497,   498,   499,   500,   501,   502,   503,   504,   505,   506,
     507,   511,   512,   513,   514
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "_ID", "_BEGIN", "_END", "_IMPRIMELN",
  "_IMPRIME", "_READ", "_READLN", "_GLOBAL", "_IF", "_ELSE", "_WHILE",
  "_PRINCIPAL", "_IFGREATER", "_IFLESS", "_IFEQUAL", "_FOR", "_EQUALS",
  "_MENOR_IGUAL", "_MAIOR_IGUAL", "_DOIS_PTS", "_DO", "_DIFERENTE", "_MOD",
  "_AND", "_OR", "_PLUS", "_LESS", "_TIMES", "_DIVIDE", "_INTEGER",
  "_STRING", "_REAL", "_BOOLEAN", "_DOUBLE", "_CHAR", "_VOID",
  "_CTE_STRING", "_CTE_INTEGER", "'>'", "'<'", "'+'", "'-'", "'*'", "'/'",
  "'('", "')'", "'{'", "'}'", "','", "';'", "'['", "']'", "'='", "$accept",
  "S", "CABECALHO", "MIOLOS", "MIOLO", "FUNCTION", "$@1", "$@2",
  "BLOCO_FUNCTION", "PARAMETROS", "DECL_FUNCTION", "IDS_FUNCTION", "VARS",
  "VARS_LOCAIS", "DECLS", "DECL", "TIPO", "TAM_STRING", "TAM_CHAR",
  "TAM_INTEGER", "IDS", "MAIN", "CMDS", "CMD", "CMD_ATRIB", "LVALUE",
  "INDICE", "EXPS", "CMD_FUNCTION", "CMD_FOR", "CMD_WHILE", "BLOCO",
  "CMD_IF", "CMD_IFGREATER", "SAIDA", "ENTRADA", "E", "F", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,    62,    60,    43,    45,    42,    47,    40,    41,   123,
     125,    44,    59,    91,    93,    61
};
# endif

#define YYPACT_NINF -96

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-96)))

#define YYTABLE_NINF -57

#define yytable_value_is_error(Yytable_value) \
  (!!((Yytable_value) == (-57)))

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     -96,     5,   251,   -96,   105,   -45,   -44,   -96,   -96,   -96,
     -43,     0,   251,   -96,   -96,    13,   -96,   -35,    16,   -20,
     -96,   -14,   -96,   -12,   -96,    34,   -96,   -96,    -8,   105,
     -96,    -4,    27,    30,    31,   105,    23,    37,   -96,    84,
     -96,   -96,   -96,    24,   -96,   105,   105,   -96,   -96,    41,
      45,    46,    49,    50,     4,     4,     4,   102,    24,   104,
      55,   -96,   -49,   -96,   -96,   -96,   -96,   -96,   -96,   -96,
     -96,    60,    66,   116,    24,     4,     4,     4,   102,   102,
     -96,   -96,   -96,     4,   179,   -96,   186,   193,   -96,    65,
      75,   -96,    24,     4,     9,    71,    37,   105,   -96,   -96,
      79,    70,   103,   111,    85,    86,   141,     4,     4,     4,
      94,     4,     4,     4,     4,     4,     4,     4,     4,    94,
      94,     4,   -96,   -96,    96,   228,    41,   -96,   228,     4,
     -96,   -96,   -96,   -96,     4,   -96,   -96,   -96,   -96,   -96,
     235,   235,   235,   146,   235,   -96,   235,   235,    20,    20,
     -96,   -96,   -96,   151,    58,   114,   228,   149,   148,   152,
       4,     4,   -96,    94,    94,   221,   110,   -96,   -96,    94,
     -96,   -96
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       3,     0,     5,     1,     0,    34,    30,    25,    26,    27,
      32,     0,     5,     7,     6,     0,    17,     0,     0,     0,
      23,     0,    24,     0,    28,     0,     2,     4,     8,    21,
      36,    22,     0,     0,     0,    19,     0,     0,    20,     0,
      33,    29,    31,    39,    18,     0,    19,    11,    35,    52,
       0,     0,     0,     0,     0,     0,     0,     0,    39,     0,
       0,    47,     0,    48,    44,    45,    46,    42,    43,    40,
      41,     0,    14,     0,    39,     0,     0,     0,     0,     0,
      83,    81,    82,     0,     0,    80,     0,     0,    52,     0,
       0,    37,    39,     0,    56,     0,     0,     0,    16,    15,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    61,    38,     0,    55,    83,    51,    50,     0,
       9,    13,    12,    57,     0,    66,    65,    68,    67,    84,
      75,    76,    77,    62,    78,    79,    73,    74,    69,    70,
      71,    72,    60,     0,     0,    54,    49,     0,     0,     0,
       0,     0,    58,     0,     0,     0,     0,    63,    64,     0,
      53,    59
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -96,   -96,   -96,   160,   -96,   -96,   -96,   -96,    80,    81,
     -96,   -96,   -96,   131,     7,   -96,     1,   -96,   -96,   -96,
     -96,   -96,   -40,   -96,   -96,    -7,   -96,    18,    87,   -96,
     -96,   -95,   -96,   -96,   -96,   -96,   -54,   -96
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,     1,     2,    11,    12,    13,    36,    37,    47,    71,
      72,    99,    14,    43,    44,    17,    18,    22,    24,    20,
      31,    26,    59,    60,    61,    62,    95,   124,    63,    64,
      65,    66,    67,    68,    69,    70,   125,    85
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      84,    86,    87,    15,    93,     3,    94,    80,    19,    21,
      23,    16,   126,    15,    25,   143,    28,    29,    90,    30,
      32,   101,   102,   103,   152,   153,    33,    49,    34,   106,
      50,    51,    52,    53,   100,    54,    38,    55,    35,    56,
     128,   -10,    57,    81,    82,   112,    73,    39,    81,    82,
      89,    83,   123,   140,   141,   142,    83,   144,   145,   146,
     147,   148,   149,   150,   151,   117,   118,   154,   167,   168,
      45,   104,   105,    58,   171,   156,   -56,   107,   108,   109,
     157,    40,   111,   112,    41,    42,    46,    48,    75,   107,
     108,   109,    76,    77,   111,   112,    78,    79,    73,   113,
     114,   115,   116,   117,   118,    88,   165,    92,    96,    91,
     160,   113,   114,   115,   116,   117,   118,    97,   133,    98,
     121,   134,   107,   108,   109,   122,   129,   111,   112,   132,
     107,   108,   109,   137,   138,   111,   112,     5,     6,     7,
       8,     9,    10,    58,   113,   114,   115,   116,   117,   118,
     155,   135,   113,   114,   115,   116,   117,   118,   158,   136,
     107,   108,   109,   159,   170,   111,   112,   161,   107,   108,
     109,   163,    27,   111,   112,   164,   130,    74,   131,   166,
       0,   127,   113,   114,   115,   116,   117,   118,     0,   139,
     113,   114,   115,   116,   117,   118,     0,   162,   107,   108,
     109,     0,   110,   111,   112,   107,   108,   109,     0,   119,
     111,   112,   107,   108,   109,     0,   120,   111,   112,     0,
     113,   114,   115,   116,   117,   118,     0,   113,   114,   115,
     116,   117,   118,     0,   113,   114,   115,   116,   117,   118,
     107,   108,   109,     0,   169,   111,   112,   107,   108,   109,
       0,     0,   111,   112,   -57,   -57,   -57,     0,     0,   -57,
     112,     4,   113,   114,   115,   116,   117,   118,     0,   113,
     114,   115,   116,   117,   118,     0,   -57,   -57,   115,   116,
     117,   118,     0,     5,     6,     7,     8,     9,    10
};

static const yytype_int16 yycheck[] =
{
      54,    55,    56,     2,    53,     0,    55,     3,    53,    53,
      53,     4,     3,    12,    14,   110,     3,    52,    58,     3,
      40,    75,    76,    77,   119,   120,    40,     3,    40,    83,
       6,     7,     8,     9,    74,    11,    29,    13,     4,    15,
      94,    49,    18,    39,    40,    25,    45,    51,    39,    40,
      57,    47,    92,   107,   108,   109,    47,   111,   112,   113,
     114,   115,   116,   117,   118,    45,    46,   121,   163,   164,
      47,    78,    79,    49,   169,   129,    52,    19,    20,    21,
     134,    54,    24,    25,    54,    54,    49,     3,    47,    19,
      20,    21,    47,    47,    24,    25,    47,    47,    97,    41,
      42,    43,    44,    45,    46,     3,   160,    52,    48,     5,
      52,    41,    42,    43,    44,    45,    46,    51,    48,     3,
      55,    51,    19,    20,    21,    50,    55,    24,    25,    50,
      19,    20,    21,    48,    48,    24,    25,    32,    33,    34,
      35,    36,    37,    49,    41,    42,    43,    44,    45,    46,
      54,    48,    41,    42,    43,    44,    45,    46,    12,    48,
      19,    20,    21,    12,    54,    24,    25,    53,    19,    20,
      21,    23,    12,    24,    25,    23,    96,    46,    97,   161,
      -1,    94,    41,    42,    43,    44,    45,    46,    -1,    48,
      41,    42,    43,    44,    45,    46,    -1,    48,    19,    20,
      21,    -1,    23,    24,    25,    19,    20,    21,    -1,    23,
      24,    25,    19,    20,    21,    -1,    23,    24,    25,    -1,
      41,    42,    43,    44,    45,    46,    -1,    41,    42,    43,
      44,    45,    46,    -1,    41,    42,    43,    44,    45,    46,
      19,    20,    21,    -1,    23,    24,    25,    19,    20,    21,
      -1,    -1,    24,    25,    19,    20,    21,    -1,    -1,    24,
      25,    10,    41,    42,    43,    44,    45,    46,    -1,    41,
      42,    43,    44,    45,    46,    -1,    41,    42,    43,    44,
      45,    46,    -1,    32,    33,    34,    35,    36,    37
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    57,    58,     0,    10,    32,    33,    34,    35,    36,
      37,    59,    60,    61,    68,    72,    70,    71,    72,    53,
      75,    53,    73,    53,    74,    14,    77,    59,     3,    52,
       3,    76,    40,    40,    40,     4,    62,    63,    70,    51,
      54,    54,    54,    69,    70,    47,    49,    64,     3,     3,
       6,     7,     8,     9,    11,    13,    15,    18,    49,    78,
      79,    80,    81,    84,    85,    86,    87,    88,    89,    90,
      91,    65,    66,    72,    69,    47,    47,    47,    47,    47,
       3,    39,    40,    47,    92,    93,    92,    92,     3,    81,
      78,     5,    52,    53,    55,    82,    48,    51,     3,    67,
      78,    92,    92,    92,    81,    81,    92,    19,    20,    21,
      23,    24,    25,    41,    42,    43,    44,    45,    46,    23,
      23,    55,    50,    78,    83,    92,     3,    84,    92,    55,
      64,    65,    50,    48,    51,    48,    48,    48,    48,    48,
      92,    92,    92,    87,    92,    92,    92,    92,    92,    92,
      92,    92,    87,    87,    92,    54,    92,    92,    12,    12,
      52,    53,    48,    23,    23,    92,    83,    87,    87,    23,
      54,    87
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    56,    57,    58,    59,    59,    60,    60,    62,    61,
      63,    61,    64,    65,    65,    66,    67,    68,    69,    69,
      70,    70,    71,    72,    72,    72,    72,    72,    72,    73,
      73,    74,    74,    75,    75,    76,    76,    77,    78,    78,
      79,    79,    79,    79,    79,    79,    79,    79,    79,    80,
      80,    80,    81,    82,    82,    83,    84,    84,    84,    85,
      86,    87,    88,    88,    89,    90,    90,    91,    91,    92,
      92,    92,    92,    92,    92,    92,    92,    92,    92,    92,
      92,    93,    93,    93,    93
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     3,     0,     2,     0,     1,     1,     0,     7,
       0,     4,     4,     3,     1,     2,     1,     2,     1,     0,
       3,     2,     2,     2,     2,     1,     1,     1,     2,     3,
       0,     3,     0,     3,     0,     3,     1,     5,     3,     0,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     4,
       3,     3,     1,     6,     3,     1,     0,     4,     6,     8,
       4,     3,     4,     7,     7,     4,     4,     4,     4,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       1,     1,     1,     1,     3
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
do                                                              \
  if (yychar == YYEMPTY)                                        \
    {                                                           \
      yychar = (Token);                                         \
      yylval = (Value);                                         \
      YYPOPSTACK (yylen);                                       \
      yystate = *yyssp;                                         \
      goto yybackup;                                            \
    }                                                           \
  else                                                          \
    {                                                           \
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;                                                  \
    }                                                           \
while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, int yyrule)
{
  unsigned long int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &(yyvsp[(yyi + 1) - (yynrhs)])
                                              );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
yystrlen (const char *yystr)
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            /* Fall through.  */
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        YYSTYPE *yyvs1 = yyvs;
        yytype_int16 *yyss1 = yyss;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * sizeof (*yyssp),
                    &yyvs1, yysize * sizeof (*yyvsp),
                    &yystacksize);

        yyss = yyss1;
        yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yytype_int16 *yyss1 = yyss;
        union yyalloc *yyptr =
          (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
                  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 339 "trabalho.y" /* yacc.c:1646  */
    { cout << (yyvsp[-2]).c << declara_var_temp( temp_global ) << (yyvsp[-1]).c << (yyvsp[0]).c << endl; }
#line 1700 "trabalho.tab.c" /* yacc.c:1646  */
    break;

  case 3:
#line 342 "trabalho.y" /* yacc.c:1646  */
    { (yyval).c = "#include <stdlib.h>\n"
               	     "#include <string.h>\n"
                     "#include <stdio.h>\n\n";
            }
#line 1709 "trabalho.tab.c" /* yacc.c:1646  */
    break;

  case 4:
#line 348 "trabalho.y" /* yacc.c:1646  */
    { (yyval).c = (yyvsp[-1]).c + (yyvsp[0]).c; }
#line 1715 "trabalho.tab.c" /* yacc.c:1646  */
    break;

  case 5:
#line 349 "trabalho.y" /* yacc.c:1646  */
    { (yyval).c = ""; }
#line 1721 "trabalho.tab.c" /* yacc.c:1646  */
    break;

  case 8:
#line 358 "trabalho.y" /* yacc.c:1646  */
    { escopo_local = true; empilha_nova_tabela_de_simbolos(); declara_variavel( (yyvsp[-1]), "Result", (yyvsp[-1]).t ); tf[(yyvsp[0]).v] = (yyvsp[-1]).t;}
#line 1727 "trabalho.tab.c" /* yacc.c:1646  */
    break;

  case 9:
#line 360 "trabalho.y" /* yacc.c:1646  */
    { gera_codigo_funcao( (yyval), (yyvsp[-6]), (yyvsp[-5]).v, (yyvsp[-2]).c, (yyvsp[0]).c );
             escopo_local = false;
             desempilha_tabela_de_simbolos(); }
#line 1735 "trabalho.tab.c" /* yacc.c:1646  */
    break;

  case 10:
#line 364 "trabalho.y" /* yacc.c:1646  */
    { escopo_local = true; empilha_nova_tabela_de_simbolos(); declara_variavel( (yyvsp[-1]), "Result", (yyvsp[-1]).t ); tf[(yyvsp[0]).v] = (yyvsp[-1]).t;}
#line 1741 "trabalho.tab.c" /* yacc.c:1646  */
    break;

  case 11:
#line 366 "trabalho.y" /* yacc.c:1646  */
    { gera_codigo_funcao( (yyval), (yyvsp[-3]), (yyvsp[-2]).v, "", (yyvsp[0]).c );
            escopo_local = false;
            desempilha_tabela_de_simbolos(); }
#line 1749 "trabalho.tab.c" /* yacc.c:1646  */
    break;

  case 12:
#line 372 "trabalho.y" /* yacc.c:1646  */
    {(yyval).c = (yyvsp[-2]).c + (yyvsp[-1]).c;}
#line 1755 "trabalho.tab.c" /* yacc.c:1646  */
    break;

  case 13:
#line 375 "trabalho.y" /* yacc.c:1646  */
    { (yyval).c = (yyvsp[-2]).c + "," + (yyvsp[0]).c;}
#line 1761 "trabalho.tab.c" /* yacc.c:1646  */
    break;

  case 14:
#line 376 "trabalho.y" /* yacc.c:1646  */
    {(yyval).c = (yyvsp[0]).c;}
#line 1767 "trabalho.tab.c" /* yacc.c:1646  */
    break;

  case 15:
#line 379 "trabalho.y" /* yacc.c:1646  */
    { declara_variavel_function( (yyval), (yyvsp[0]).lst, (yyvsp[-1]).t ); }
#line 1773 "trabalho.tab.c" /* yacc.c:1646  */
    break;

  case 16:
#line 382 "trabalho.y" /* yacc.c:1646  */
    { (yyval).lst.push_back( (yyvsp[0]).v ); }
#line 1779 "trabalho.tab.c" /* yacc.c:1646  */
    break;

  case 17:
#line 385 "trabalho.y" /* yacc.c:1646  */
    { (yyval).c = (yyvsp[0]).c; }
#line 1785 "trabalho.tab.c" /* yacc.c:1646  */
    break;

  case 18:
#line 388 "trabalho.y" /* yacc.c:1646  */
    {(yyval).c = (yyvsp[0]).c;}
#line 1791 "trabalho.tab.c" /* yacc.c:1646  */
    break;

  case 20:
#line 392 "trabalho.y" /* yacc.c:1646  */
    { (yyval).c = (yyvsp[-2]).c + (yyvsp[0]).c; }
#line 1797 "trabalho.tab.c" /* yacc.c:1646  */
    break;

  case 22:
#line 396 "trabalho.y" /* yacc.c:1646  */
    { declara_variavel( (yyval), (yyvsp[0]).lst, (yyvsp[-1]).t ); }
#line 1803 "trabalho.tab.c" /* yacc.c:1646  */
    break;

  case 23:
#line 399 "trabalho.y" /* yacc.c:1646  */
    { (yyval).t = (yyvsp[0]).t; }
#line 1809 "trabalho.tab.c" /* yacc.c:1646  */
    break;

  case 24:
#line 400 "trabalho.y" /* yacc.c:1646  */
    { (yyval).t = (yyvsp[0]).t; }
#line 1815 "trabalho.tab.c" /* yacc.c:1646  */
    break;

  case 25:
#line 401 "trabalho.y" /* yacc.c:1646  */
    { (yyval).t = Real; }
#line 1821 "trabalho.tab.c" /* yacc.c:1646  */
    break;

  case 26:
#line 402 "trabalho.y" /* yacc.c:1646  */
    { (yyval).t = Boolean; }
#line 1827 "trabalho.tab.c" /* yacc.c:1646  */
    break;

  case 27:
#line 403 "trabalho.y" /* yacc.c:1646  */
    { (yyval).t = Double; }
#line 1833 "trabalho.tab.c" /* yacc.c:1646  */
    break;

  case 28:
#line 404 "trabalho.y" /* yacc.c:1646  */
    { (yyval).t = (yyvsp[0]).t; }
#line 1839 "trabalho.tab.c" /* yacc.c:1646  */
    break;

  case 29:
#line 408 "trabalho.y" /* yacc.c:1646  */
    { (yyval).t = String; (yyval).t.dim[0].fim = toInt( (yyvsp[-1]).v ); }
#line 1845 "trabalho.tab.c" /* yacc.c:1646  */
    break;

  case 30:
#line 409 "trabalho.y" /* yacc.c:1646  */
    { (yyval).t = String; }
#line 1851 "trabalho.tab.c" /* yacc.c:1646  */
    break;

  case 31:
#line 413 "trabalho.y" /* yacc.c:1646  */
    { (yyval).t = String; (yyval).t.dim[0].fim = toInt( (yyvsp[-1]).v ); }
#line 1857 "trabalho.tab.c" /* yacc.c:1646  */
    break;

  case 32:
#line 414 "trabalho.y" /* yacc.c:1646  */
    { (yyval).t = Char; }
#line 1863 "trabalho.tab.c" /* yacc.c:1646  */
    break;

  case 33:
#line 418 "trabalho.y" /* yacc.c:1646  */
    { (yyval).t = String; (yyval).t.dim[0].fim = toInt( (yyvsp[-1]).v ); }
#line 1869 "trabalho.tab.c" /* yacc.c:1646  */
    break;

  case 34:
#line 419 "trabalho.y" /* yacc.c:1646  */
    { (yyval).t = Integer; }
#line 1875 "trabalho.tab.c" /* yacc.c:1646  */
    break;

  case 35:
#line 422 "trabalho.y" /* yacc.c:1646  */
    { (yyval).lst = (yyvsp[-2]).lst; (yyval).lst.push_back( (yyvsp[0]).v ); }
#line 1881 "trabalho.tab.c" /* yacc.c:1646  */
    break;

  case 36:
#line 423 "trabalho.y" /* yacc.c:1646  */
    { (yyval).lst.push_back( (yyvsp[0]).v ); }
#line 1887 "trabalho.tab.c" /* yacc.c:1646  */
    break;

  case 37:
#line 427 "trabalho.y" /* yacc.c:1646  */
    { (yyval).c = "int main() {\n" + (yyvsp[-1]).c + "  return 0;\n}\n"; }
#line 1893 "trabalho.tab.c" /* yacc.c:1646  */
    break;

  case 38:
#line 430 "trabalho.y" /* yacc.c:1646  */
    { (yyval).c = (yyvsp[-2]).c + (yyvsp[0]).c; }
#line 1899 "trabalho.tab.c" /* yacc.c:1646  */
    break;

  case 39:
#line 431 "trabalho.y" /* yacc.c:1646  */
    { (yyval).c = ""; }
#line 1905 "trabalho.tab.c" /* yacc.c:1646  */
    break;

  case 50:
#line 446 "trabalho.y" /* yacc.c:1646  */
    { gera_codigo_atribuicao( (yyval), (yyvsp[-2]), (yyvsp[0]) ); }
#line 1911 "trabalho.tab.c" /* yacc.c:1646  */
    break;

  case 51:
#line 447 "trabalho.y" /* yacc.c:1646  */
    {gera_codigo_atribuicao( (yyval), (yyvsp[-2]), (yyvsp[0]) );}
#line 1917 "trabalho.tab.c" /* yacc.c:1646  */
    break;

  case 52:
#line 450 "trabalho.y" /* yacc.c:1646  */
    { busca_tipo_da_variavel( (yyval), (yyvsp[0]) ); }
#line 1923 "trabalho.tab.c" /* yacc.c:1646  */
    break;

  case 53:
#line 453 "trabalho.y" /* yacc.c:1646  */
    {(yyval).c = "[" + (yyvsp[-4]).c + "]" + "[" + (yyvsp[-1]).c + "]";}
#line 1929 "trabalho.tab.c" /* yacc.c:1646  */
    break;

  case 54:
#line 454 "trabalho.y" /* yacc.c:1646  */
    {(yyval).c = "[" + (yyvsp[-1]).c + "]";}
#line 1935 "trabalho.tab.c" /* yacc.c:1646  */
    break;

  case 55:
#line 457 "trabalho.y" /* yacc.c:1646  */
    {(yyval).c = (yyvsp[0]).c;}
#line 1941 "trabalho.tab.c" /* yacc.c:1646  */
    break;

  case 57:
#line 460 "trabalho.y" /* yacc.c:1646  */
    { (yyval).v = gera_nome_var( tf[(yyvsp[-3]).v] );
                      (yyval).c = (yyvsp[-1]).c +
                      "  " + (yyval).v + " = " + (yyvsp[-3]).v + "( " + (yyvsp[-1]).v + " );\n";
                      (yyval).t = tf[(yyvsp[-3]).v]; }
#line 1950 "trabalho.tab.c" /* yacc.c:1646  */
    break;

  case 58:
#line 465 "trabalho.y" /* yacc.c:1646  */
    { (yyval).v = gera_nome_var( tf[(yyvsp[-5]).v] );
                      (yyval).c = (yyvsp[-3]).c + (yyvsp[-1]).c +
                      "  " + (yyval).v + " = " + (yyvsp[-5]).v + "( " + (yyvsp[-3]).v + "," + (yyvsp[-1]).v + " );\n";
                      (yyval).t = tf[(yyvsp[-5]).v]; }
#line 1959 "trabalho.tab.c" /* yacc.c:1646  */
    break;

  case 59:
#line 472 "trabalho.y" /* yacc.c:1646  */
    { gera_cmd_for((yyval), (yyvsp[-6]), (yyvsp[-4]), (yyvsp[-2]), (yyvsp[0]));}
#line 1965 "trabalho.tab.c" /* yacc.c:1646  */
    break;

  case 60:
#line 475 "trabalho.y" /* yacc.c:1646  */
    { gera_cmd_while((yyval), (yyvsp[-2]), (yyvsp[0]));}
#line 1971 "trabalho.tab.c" /* yacc.c:1646  */
    break;

  case 61:
#line 478 "trabalho.y" /* yacc.c:1646  */
    { (yyval).c = (yyvsp[-1]).c ;}
#line 1977 "trabalho.tab.c" /* yacc.c:1646  */
    break;

  case 63:
#line 482 "trabalho.y" /* yacc.c:1646  */
    { gera_cmd_if( (yyval), (yyvsp[-5]), (yyvsp[-3]), (yyvsp[0]) ); }
#line 1983 "trabalho.tab.c" /* yacc.c:1646  */
    break;

  case 64:
#line 485 "trabalho.y" /* yacc.c:1646  */
    { gera_cmd_ifgreater( (yyval), (yyvsp[-5]), (yyvsp[-3]), (yyvsp[0]) ); }
#line 1989 "trabalho.tab.c" /* yacc.c:1646  */
    break;

  case 65:
#line 488 "trabalho.y" /* yacc.c:1646  */
    { (yyval).c = "  printf( \"%"+ (yyvsp[-1]).t.fmt + "\", " + (yyvsp[-1]).v + " );\n"; }
#line 1995 "trabalho.tab.c" /* yacc.c:1646  */
    break;

  case 66:
#line 489 "trabalho.y" /* yacc.c:1646  */
    { (yyval).c = "  printf( \"%"+ (yyvsp[-1]).t.fmt + "\\n\", " + (yyvsp[-1]).v + " );\n"; }
#line 2001 "trabalho.tab.c" /* yacc.c:1646  */
    break;

  case 67:
#line 492 "trabalho.y" /* yacc.c:1646  */
    { (yyval).c = "  scanf( \"%"+ (yyvsp[-1]).t.fmt + "\", &"+ (yyvsp[-1]).v + " );\n"; }
#line 2007 "trabalho.tab.c" /* yacc.c:1646  */
    break;

  case 68:
#line 493 "trabalho.y" /* yacc.c:1646  */
    { (yyval).c = "  scanf( \"%"+ (yyvsp[-1]).t.fmt + "\", &"+ (yyvsp[-1]).v + " );\n"; }
#line 2013 "trabalho.tab.c" /* yacc.c:1646  */
    break;

  case 69:
#line 496 "trabalho.y" /* yacc.c:1646  */
    { gera_codigo_operador( (yyval), (yyvsp[-2]), (yyvsp[-1]), (yyvsp[0]) ); }
#line 2019 "trabalho.tab.c" /* yacc.c:1646  */
    break;

  case 70:
#line 497 "trabalho.y" /* yacc.c:1646  */
    { gera_codigo_operador( (yyval), (yyvsp[-2]), (yyvsp[-1]), (yyvsp[0]) ); }
#line 2025 "trabalho.tab.c" /* yacc.c:1646  */
    break;

  case 71:
#line 498 "trabalho.y" /* yacc.c:1646  */
    { gera_codigo_operador( (yyval), (yyvsp[-2]), (yyvsp[-1]), (yyvsp[0]) ); }
#line 2031 "trabalho.tab.c" /* yacc.c:1646  */
    break;

  case 72:
#line 499 "trabalho.y" /* yacc.c:1646  */
    { gera_codigo_operador( (yyval), (yyvsp[-2]), (yyvsp[-1]), (yyvsp[0]) ); }
#line 2037 "trabalho.tab.c" /* yacc.c:1646  */
    break;

  case 73:
#line 500 "trabalho.y" /* yacc.c:1646  */
    { gera_codigo_operador( (yyval), (yyvsp[-2]), (yyvsp[-1]), (yyvsp[0]) ); }
#line 2043 "trabalho.tab.c" /* yacc.c:1646  */
    break;

  case 74:
#line 501 "trabalho.y" /* yacc.c:1646  */
    { gera_codigo_operador( (yyval), (yyvsp[-2]), (yyvsp[-1]), (yyvsp[0]) ); }
#line 2049 "trabalho.tab.c" /* yacc.c:1646  */
    break;

  case 75:
#line 502 "trabalho.y" /* yacc.c:1646  */
    { gera_codigo_operador( (yyval), (yyvsp[-2]), (yyvsp[-1]), (yyvsp[0]) ); }
#line 2055 "trabalho.tab.c" /* yacc.c:1646  */
    break;

  case 76:
#line 503 "trabalho.y" /* yacc.c:1646  */
    { gera_codigo_operador( (yyval), (yyvsp[-2]), (yyvsp[-1]), (yyvsp[0]) ); }
#line 2061 "trabalho.tab.c" /* yacc.c:1646  */
    break;

  case 77:
#line 504 "trabalho.y" /* yacc.c:1646  */
    { gera_codigo_operador( (yyval), (yyvsp[-2]), (yyvsp[-1]), (yyvsp[0]) ); }
#line 2067 "trabalho.tab.c" /* yacc.c:1646  */
    break;

  case 78:
#line 505 "trabalho.y" /* yacc.c:1646  */
    { gera_codigo_operador( (yyval), (yyvsp[-2]), (yyvsp[-1]), (yyvsp[0]) ); }
#line 2073 "trabalho.tab.c" /* yacc.c:1646  */
    break;

  case 79:
#line 506 "trabalho.y" /* yacc.c:1646  */
    { gera_codigo_operador( (yyval), (yyvsp[-2]), (yyvsp[-1]), (yyvsp[0]) ); }
#line 2079 "trabalho.tab.c" /* yacc.c:1646  */
    break;

  case 81:
#line 511 "trabalho.y" /* yacc.c:1646  */
    { (yyval) = (yyvsp[0]); (yyval).t = String; }
#line 2085 "trabalho.tab.c" /* yacc.c:1646  */
    break;

  case 82:
#line 512 "trabalho.y" /* yacc.c:1646  */
    { (yyval) = (yyvsp[0]); (yyval).t = Integer; }
#line 2091 "trabalho.tab.c" /* yacc.c:1646  */
    break;

  case 83:
#line 513 "trabalho.y" /* yacc.c:1646  */
    { busca_tipo_da_variavel( (yyval), (yyvsp[0]) );  }
#line 2097 "trabalho.tab.c" /* yacc.c:1646  */
    break;

  case 84:
#line 514 "trabalho.y" /* yacc.c:1646  */
    { (yyval) = (yyvsp[-1]);}
#line 2103 "trabalho.tab.c" /* yacc.c:1646  */
    break;


#line 2107 "trabalho.tab.c" /* yacc.c:1646  */
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
#line 517 "trabalho.y" /* yacc.c:1906  */


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
