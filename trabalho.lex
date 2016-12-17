%{
int yyrowno = 1;
void atributo();
char *strupr( char* st );
%}

WS      [\t ]
DIGITO  [0-9]
LETRA   [A-Za-z_]
ID      {LETRA}({LETRA}|{DIGITO})*

MAIN	[Mm][Aa][Ii][Nn]
SCHREIBLN	[Ss][Cc][Hh][Rr][Ee][Ii][Bb][Ll][Nn]
SCHREIB		[Ss][Cc][Hh][Rr][Ee][Ii][Bb]
LESELN		[Ll][Ee][Ss][Ee][Ll][Nn]
LESE			[Ll][Ee][Ss][Ee]
WELTWEIT		[Ww][Ee][Ll][Tt][Ww][Ee][Ii][Tt]
OB			[Oo][Bb]
SONST		[Ss][Oo][Nn][Ss][Tt]
FUER			[Ff][Uu][Ee][Rr]
MACH			[Mm][Aa][Cc][Hh]
WAEHREND	[Ww][Aa][Ee][Hh][Rr][Ee][Nn][Dd]
MOD			[Mm][Oo][Dd]

GANZZAHL		[Gg][Aa][Nn][Zz][Zz][Aa][Hh][Ll]
SCHNUR		[Ss][Cc][Hh][Nn][Uu][Rr]
ECHT		[Ee][Cc][Hh][Tt]
BOOLESCH		[Bb][Oo][Oo][Ll][Ee][Ss][Cc][Hh]
DOPPEL		[Dd][Oo][Pp][Pp][Ee][Ll]
ZEICHEN		[Zz][Ee][Ii][Cc][Hh][Ee][Nn]
LEER			[Ll][Ee][Ee][Rr]


CTE_STRING	"\""([^\"\n]|"\\\"")*"\""
CTE_INTEGER {DIGITO}+
CTE_DOUBLE  {NUMERO}+("."{NUMERO}+)?

%%

"\n" { yylineno++; yyrowno = 1; }
{WS} { yyrowno += 1; }


{MAIN} 	{ atributo(); return _PRINCIPAL; }
{SCHREIBLN} 	{ atributo(); return _IMPRIMELN; }
{SCHREIB} 	{ atributo(); return _IMPRIME; }
{LESELN} 	{ atributo(); return _READLN; }
{LESE} 		{ atributo(); return _READ; }
{SCHNUR} 	{ atributo(); return _STRING; }
{GANZZAHL} 	{ atributo(); return _INTEGER; }
{DOPPEL} 	{ atributo(); return _DOUBLE; }
{ZEICHEN}	 	{ atributo(); return _CHAR; }
{LEER}     { atributo(); return _VOID; }
{ECHT} 		{ atributo(); return _REAL; }
{BOOLESCH} 	{ atributo(); return _BOOLEAN; }
{WELTWEIT} 	{ atributo(); return _GLOBAL; }
{OB} 		{ atributo(); return _IF; }
{SONST}		{ atributo(); return _ELSE; }
{MACH}		{ atributo(); return _DO; }
{FUER} 		{ atributo(); return _FOR; }
{WAEHREND} 	{ atributo(); return _WHILE; }
{MOD} 		{ atributo(); yylval.v = "%"; return _MOD; }

{CTE_STRING} 	{ atributo(); return _CTE_STRING; }
{CTE_INTEGER} 	{ atributo(); return _CTE_INTEGER; }

":{"		{ atributo(); return _BEGIN; }
"}:" 		{ atributo(); return _END; }
"=="		{ atributo(); return _EQUALS; }
"<="		{ atributo(); return _MENOR_IGUAL; }
">="		{ atributo(); return _MAIOR_IGUAL; }
"!="		{ atributo(); return _DIFERENTE; }


{ID}  { atributo(); return _ID; }

.     { atributo(); return yytext[0]; }

%%

void atributo() {
yylval.v = yytext;
  yylval.t.nome = "";
  yylval.t.decl = "";
  yylval.t.fmt = "";
  yylval.c = "";
  yylval.lst.clear();

  yyrowno += strlen( yytext );
}

char* strupr( char* st ) {
  char * aux = st;

  while( *st ) {
    *st = toupper( *st );
    ++st;
  }
  return aux;
}
