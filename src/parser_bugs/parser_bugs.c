/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

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
#define YYBISON_VERSION "2.3"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Using locations.  */
#define YYLSP_NEEDED 0

/* Substitute the variable and function names.  */
#define yyparse bugsparse
#define yylex   bugslex
#define yyerror bugserror
#define yylval  bugslval
#define yychar  bugschar
#define yydebug bugsdebug
#define yynerrs bugsnerrs


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     VAR = 258,
     DATA = 259,
     MODEL = 260,
     NAME = 261,
     FUNC = 262,
     SPECIAL = 263,
     IN = 264,
     ARROW = 265,
     FOR = 266,
     GT = 267,
     GE = 268,
     LT = 269,
     LE = 270,
     EQ = 271,
     NE = 272,
     AND = 273,
     OR = 274,
     LENGTH = 275,
     DIM = 276,
     DOUBLE = 277,
     INT = 278,
     NOT = 279,
     NEG = 280,
     ENDL = 281
   };
#endif
/* Tokens.  */
#define VAR 258
#define DATA 259
#define MODEL 260
#define NAME 261
#define FUNC 262
#define SPECIAL 263
#define IN 264
#define ARROW 265
#define FOR 266
#define GT 267
#define GE 268
#define LT 269
#define LE 270
#define EQ 271
#define NE 272
#define AND 273
#define OR 274
#define LENGTH 275
#define DIM 276
#define DOUBLE 277
#define INT 278
#define NOT 279
#define NEG 280
#define ENDL 281




/* Copy the first part of user declarations.  */
#line 1 "parser_bugs.y"

#include <stdio.h>
#include <string.h>
#include "bugs_node.h"

BUGS_NODE* g_bugsnode = NULL;



/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif

#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 15 "parser_bugs.y"
{
  double val;
  char *stringptr;
  struct _BUGS_NODE *pnode;
}
/* Line 193 of yacc.c.  */
#line 172 "parser_bugs.tab.c"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 216 of yacc.c.  */
#line 185 "parser_bugs.tab.c"

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
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
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
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
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
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int i)
#else
static int
YYID (i)
    int i;
#endif
{
  return i;
}
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
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
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
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
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
  yytype_int16 yyss;
  YYSTYPE yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  14
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   224

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  42
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  22
/* YYNRULES -- Number of rules.  */
#define YYNRULES  68
/* YYNRULES -- Number of states.  */
#define YYNSTATES  131

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   281

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      38,    39,    27,    25,    33,    26,     2,    28,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    41,    32,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    34,     2,    35,    30,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    36,     2,    37,    40,     2,     2,     2,
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
      29,    31
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint8 yyprhs[] =
{
       0,     0,     3,     4,     6,     9,    12,    16,    19,    23,
      25,    29,    31,    36,    38,    42,    45,    48,    52,    55,
      57,    60,    62,    64,    66,    69,    72,    75,    78,    85,
      89,    92,    99,   105,   109,   113,   117,   121,   125,   127,
     129,   134,   139,   144,   146,   150,   152,   156,   159,   163,
     167,   171,   175,   179,   183,   187,   191,   195,   199,   203,
     205,   209,   210,   212,   216,   221,   223,   228,   232
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      43,     0,    -1,    -1,    49,    -1,    44,    49,    -1,    48,
      49,    -1,    44,    48,    49,    -1,     3,    45,    -1,     3,
      45,    32,    -1,    46,    -1,    45,    33,    46,    -1,     6,
      -1,     6,    34,    47,    35,    -1,    59,    -1,    47,    33,
      59,    -1,     4,    50,    -1,     5,    50,    -1,    36,    51,
      37,    -1,    36,    37,    -1,    52,    -1,    51,    52,    -1,
      56,    -1,    55,    -1,    53,    -1,    56,    32,    -1,    55,
      32,    -1,     1,    32,    -1,    54,    50,    -1,    11,    38,
       6,     9,    61,    39,    -1,    63,    10,    59,    -1,    63,
       1,    -1,     7,    38,    63,    39,    10,    59,    -1,     7,
      38,    63,    39,     1,    -1,    63,    40,    62,    -1,    59,
      27,    59,    -1,    57,    27,    59,    -1,    59,    25,    59,
      -1,    58,    25,    59,    -1,    63,    -1,    22,    -1,    20,
      38,    63,    39,    -1,    21,    38,    63,    39,    -1,     7,
      38,    60,    39,    -1,    57,    -1,    59,    28,    59,    -1,
      58,    -1,    59,    26,    59,    -1,    26,    59,    -1,    59,
      12,    59,    -1,    59,    13,    59,    -1,    59,    14,    59,
      -1,    59,    15,    59,    -1,    59,    16,    59,    -1,    59,
      17,    59,    -1,    59,    18,    59,    -1,    59,    19,    59,
      -1,    59,    30,    59,    -1,    59,     8,    59,    -1,    38,
      59,    39,    -1,    59,    -1,    60,    33,    59,    -1,    -1,
      59,    -1,    59,    41,    59,    -1,     7,    38,    60,    39,
      -1,     6,    -1,     6,    34,    60,    35,    -1,     6,    34,
      35,    -1,     6,    34,     1,    35,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,    63,    63,    64,    68,    72,    76,    82,    83,    86,
      87,    90,    91,    94,    95,    98,   103,   108,   112,   117,
     121,   127,   128,   129,   130,   131,   132,   135,   142,   149,
     154,   161,   169,   178,   185,   191,   197,   203,   209,   212,
     216,   220,   224,   229,   235,   241,   244,   250,   255,   261,
     267,   273,   279,   285,   291,   297,   303,   309,   315,   320,
     324,   339,   340,   344,   351,   359,   363,   369,   373
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "VAR", "DATA", "MODEL", "NAME", "FUNC",
  "SPECIAL", "IN", "ARROW", "FOR", "GT", "GE", "LT", "LE", "EQ", "NE",
  "AND", "OR", "LENGTH", "DIM", "DOUBLE", "INT", "NOT", "'+'", "'-'",
  "'*'", "'/'", "NEG", "'^'", "ENDL", "';'", "','", "'['", "']'", "'{'",
  "'}'", "'('", "')'", "'~'", "':'", "$accept", "input", "var_stmt",
  "dec_list", "node_dec", "dim_list", "data_stmt", "model_stmt",
  "relations", "relation_list", "relation", "for_loop", "counter",
  "determ_relation", "stoch_relation", "product", "sum", "expression",
  "expression_list", "range_element", "distribution", "var", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,    43,    45,    42,    47,   280,
      94,   281,    59,    44,    91,    93,   123,   125,    40,    41,
     126,    58
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    42,    43,    43,    43,    43,    43,    44,    44,    45,
      45,    46,    46,    47,    47,    48,    49,    50,    50,    51,
      51,    52,    52,    52,    52,    52,    52,    53,    54,    55,
      55,    55,    55,    56,    57,    57,    58,    58,    59,    59,
      59,    59,    59,    59,    59,    59,    59,    59,    59,    59,
      59,    59,    59,    59,    59,    59,    59,    59,    59,    60,
      60,    61,    61,    61,    62,    63,    63,    63,    63
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     1,     2,     2,     3,     2,     3,     1,
       3,     1,     4,     1,     3,     2,     2,     3,     2,     1,
       2,     1,     1,     1,     2,     2,     2,     2,     6,     3,
       2,     6,     5,     3,     3,     3,     3,     3,     1,     1,
       4,     4,     4,     1,     3,     1,     3,     2,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     1,
       3,     0,     1,     3,     4,     1,     4,     3,     4
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       2,     0,     0,     0,     0,     0,     0,     3,    11,     7,
       9,     0,    15,    16,     1,     0,     4,     5,     0,     8,
       0,     0,    65,     0,     0,    18,     0,    19,    23,     0,
      22,    21,     0,     6,     0,     0,     0,    39,     0,     0,
       0,    43,    45,    13,    38,    10,    26,     0,     0,     0,
      17,    20,    27,    25,    24,    30,     0,     0,     0,     0,
       0,    47,     0,     0,    12,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    67,    59,     0,     0,     0,    29,     0,    33,
       0,     0,     0,    58,    14,    35,    37,    57,    48,    49,
      50,    51,    52,    53,    54,    55,    36,    46,    34,    44,
      56,    68,     0,    66,     0,    61,     0,    42,    40,    41,
      60,    32,     0,    62,     0,     0,    31,     0,    28,    64,
      63
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,     4,     5,     9,    10,    40,     6,     7,    12,    26,
      27,    28,    29,    30,    31,    41,    42,    83,    84,   124,
      89,    44
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -54
static const yytype_int16 yypact[] =
{
     132,     2,   -25,   -25,    33,    21,    34,   -54,    10,    37,
     -54,    30,   -54,   -54,   -54,    34,   -54,   -54,    -3,   -54,
       2,    41,    45,    28,    42,   -54,    71,   -54,   -54,   -25,
      51,    52,     0,   -54,    58,    64,    68,   -54,    -3,    -3,
      60,    80,    88,   131,   -54,   -54,   -54,     8,   117,   120,
     -54,   -54,   -54,   -54,   -54,   -54,    -3,   125,    -3,   117,
     117,   104,   103,    -3,   -54,    -3,    -3,    -3,    -3,    -3,
      -3,    -3,    -3,    -3,    -3,    -3,    -3,    -3,    -3,    -3,
      -3,   105,   -54,   131,    77,    99,   142,   131,   114,   -54,
      32,   102,   115,   -54,   131,    -6,    97,   104,   173,   173,
     173,   173,   173,   173,   194,   152,    97,    97,    -6,    -6,
     104,   -54,    -3,   -54,     6,    -3,    -3,   -54,   -54,   -54,
     131,   -54,    -3,    73,   116,    35,   131,    -3,   -54,   -54,
     131
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -54,   -54,   -54,   -54,   133,   -54,   157,     7,     3,   -54,
     137,   -54,   -54,   -54,   -54,   -54,   -54,   -18,   -53,   -54,
     -54,    16
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const yytype_int16 yytable[] =
{
      43,    55,    67,    22,    34,    90,    13,   121,     8,    81,
      56,    11,    16,    17,    22,    34,   122,    35,    36,    37,
      61,    62,    33,    38,    80,     2,     3,    32,    35,    36,
      37,    21,    52,    14,    38,    39,    22,    23,    87,     3,
      57,    24,    32,    82,    18,    94,    39,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   125,    85,   112,    48,    25,   112,    19,
      20,   117,    21,    46,   129,    91,    92,    22,    23,    47,
      49,    67,    24,    53,    54,    68,    69,    70,    71,    72,
      73,    74,    75,    63,   120,    64,    58,   123,    76,    77,
      78,    79,    59,    80,   126,    67,    60,    65,    50,   130,
     112,    67,   113,    66,   127,    68,    69,    70,    71,    72,
      73,    74,    75,    22,    78,    79,    86,    80,    76,    77,
      78,    79,    88,    80,    80,     1,     2,     3,   114,    67,
     111,   118,    93,    68,    69,    70,    71,    72,    73,    74,
      75,   115,   116,    45,   119,   128,    76,    77,    78,    79,
      67,    80,    15,    51,    68,    69,    70,    71,    72,    73,
      74,     0,     0,     0,     0,     0,     0,    76,    77,    78,
      79,    67,    80,     0,     0,    -1,    -1,    -1,    -1,    -1,
      -1,     0,     0,     0,     0,     0,     0,     0,    76,    77,
      78,    79,    67,    80,     0,     0,    68,    69,    70,    71,
      72,    73,     0,     0,     0,     0,     0,     0,     0,    76,
      77,    78,    79,     0,    80
};

static const yytype_int8 yycheck[] =
{
      18,     1,     8,     6,     7,    58,     3,     1,     6,     1,
      10,    36,     5,     6,     6,     7,    10,    20,    21,    22,
      38,    39,    15,    26,    30,     4,     5,    11,    20,    21,
      22,     1,    29,     0,    26,    38,     6,     7,    56,     5,
      40,    11,    26,    35,    34,    63,    38,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,   116,    48,    33,    38,    37,    33,    32,
      33,    39,     1,    32,    39,    59,    60,     6,     7,    34,
      38,     8,    11,    32,    32,    12,    13,    14,    15,    16,
      17,    18,    19,    33,   112,    35,    38,   115,    25,    26,
      27,    28,    38,    30,   122,     8,    38,    27,    37,   127,
      33,     8,    35,    25,    41,    12,    13,    14,    15,    16,
      17,    18,    19,     6,    27,    28,     6,    30,    25,    26,
      27,    28,     7,    30,    30,     3,     4,     5,    39,     8,
      35,    39,    39,    12,    13,    14,    15,    16,    17,    18,
      19,     9,    38,    20,    39,    39,    25,    26,    27,    28,
       8,    30,     5,    26,    12,    13,    14,    15,    16,    17,
      18,    -1,    -1,    -1,    -1,    -1,    -1,    25,    26,    27,
      28,     8,    30,    -1,    -1,    12,    13,    14,    15,    16,
      17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,    26,
      27,    28,     8,    30,    -1,    -1,    12,    13,    14,    15,
      16,    17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,
      26,    27,    28,    -1,    30
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,     4,     5,    43,    44,    48,    49,     6,    45,
      46,    36,    50,    50,     0,    48,    49,    49,    34,    32,
      33,     1,     6,     7,    11,    37,    51,    52,    53,    54,
      55,    56,    63,    49,     7,    20,    21,    22,    26,    38,
      47,    57,    58,    59,    63,    46,    32,    34,    38,    38,
      37,    52,    50,    32,    32,     1,    10,    40,    38,    38,
      38,    59,    59,    33,    35,    27,    25,     8,    12,    13,
      14,    15,    16,    17,    18,    19,    25,    26,    27,    28,
      30,     1,    35,    59,    60,    63,     6,    59,     7,    62,
      60,    63,    63,    39,    59,    59,    59,    59,    59,    59,
      59,    59,    59,    59,    59,    59,    59,    59,    59,    59,
      59,    35,    33,    35,    39,     9,    38,    39,    39,    39,
      59,     1,    10,    59,    61,    60,    59,    41,    39,    39,
      59
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *bottom, yytype_int16 *top)
#else
static void
yy_stack_print (bottom, top)
    yytype_int16 *bottom;
    yytype_int16 *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      fprintf (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      fprintf (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

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
#ifndef	YYINITDEPTH
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
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
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
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
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

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */

#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */



/* The look-ahead symbol.  */
int yychar;

/* The semantic value of the look-ahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
  
  int yystate;
  int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Look-ahead token as an internal (translated) token number.  */
  int yytoken = 0;
#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  yytype_int16 yyssa[YYINITDEPTH];
  yytype_int16 *yyss = yyssa;
  yytype_int16 *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  YYSTYPE *yyvsp;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;


  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss;
  yyvsp = yyvs;

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
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);

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

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     look-ahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to look-ahead token.  */
  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a look-ahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid look-ahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
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
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the look-ahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

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
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 3:
#line 64 "parser_bugs.y"
    { 
	g_bugsnode = bugs_node_create(BN_LIST);
	bugs_node_addparam(g_bugsnode, (yyvsp[(1) - (1)].pnode)); 
;}
    break;

  case 4:
#line 68 "parser_bugs.y"
    {
	g_bugsnode = bugs_node_create(BN_LIST);
	bugs_node_addparam(g_bugsnode, (yyvsp[(2) - (2)].pnode)); 
;}
    break;

  case 5:
#line 72 "parser_bugs.y"
    {
	g_bugsnode = bugs_node_create(BN_LIST);
	bugs_node_addparam(g_bugsnode, (yyvsp[(2) - (2)].pnode)); 
;}
    break;

  case 6:
#line 76 "parser_bugs.y"
    {
	g_bugsnode = bugs_node_create(BN_LIST);
	bugs_node_addparam(g_bugsnode, (yyvsp[(3) - (3)].pnode)); 
;}
    break;

  case 15:
#line 98 "parser_bugs.y"
    {
	(yyval.pnode) = (yyvsp[(2) - (2)].pnode);
;}
    break;

  case 16:
#line 103 "parser_bugs.y"
    {
	(yyval.pnode) = (yyvsp[(2) - (2)].pnode);
;}
    break;

  case 17:
#line 108 "parser_bugs.y"
    {
	(yyval.pnode) = bugs_node_create(BN_RELATIONS);
	(yyval.pnode)->params = (yyvsp[(2) - (3)].pnode)->params;
;}
    break;

  case 18:
#line 112 "parser_bugs.y"
    {
	(yyval.pnode) = bugs_node_create(BN_RELATIONS);
;}
    break;

  case 19:
#line 117 "parser_bugs.y"
    {
	(yyval.pnode) = bugs_node_create(BN_LIST);
	bugs_node_addparam((yyval.pnode), (yyvsp[(1) - (1)].pnode));
;}
    break;

  case 20:
#line 121 "parser_bugs.y"
    {
	(yyval.pnode) = (yyvsp[(1) - (2)].pnode);
	bugs_node_addparam((yyval.pnode), (yyvsp[(2) - (2)].pnode));
;}
    break;

  case 26:
#line 132 "parser_bugs.y"
    { yyerrok; ;}
    break;

  case 27:
#line 135 "parser_bugs.y"
    {
	(yyval.pnode) = bugs_node_create(BN_FOR);
	bugs_node_addparam((yyval.pnode), (yyvsp[(1) - (2)].pnode));
	bugs_node_addparam((yyval.pnode), (yyvsp[(2) - (2)].pnode));
;}
    break;

  case 28:
#line 142 "parser_bugs.y"
    {
	(yyval.pnode) = bugs_node_create(BN_COUNTER);
	bugs_node_setname((yyval.pnode), (yyvsp[(3) - (6)].stringptr));
	bugs_node_addparam((yyval.pnode), (yyvsp[(5) - (6)].pnode));
;}
    break;

  case 29:
#line 149 "parser_bugs.y"
    {
	(yyval.pnode) = bugs_node_create(BN_DETERMREL);
	bugs_node_addparam((yyval.pnode), (yyvsp[(1) - (3)].pnode));
	bugs_node_addparam((yyval.pnode), (yyvsp[(3) - (3)].pnode));
;}
    break;

  case 30:
#line 154 "parser_bugs.y"
    {
	char buf[1024];
	sprintf(buf, "variable '%s' has no right expression", (yyvsp[(1) - (2)].pnode)->name);
	yyerror(buf);
	YYERROR;
	yyerrok;
;}
    break;

  case 31:
#line 161 "parser_bugs.y"
    {
	BUGS_NODE *p = bugs_node_create(BN_LINK);
	bugs_node_setname(p, (yyvsp[(1) - (6)].stringptr));
	bugs_node_addparam(p, (yyvsp[(6) - (6)].pnode));
	(yyval.pnode) = bugs_node_create(BN_DETERMREL);
	bugs_node_addparam((yyval.pnode), (yyvsp[(3) - (6)].pnode));
	bugs_node_addparam((yyval.pnode), p);
;}
    break;

  case 32:
#line 169 "parser_bugs.y"
    {
	char buf[1024];
	sprintf(buf, "variable '%s(%s)' has no right expression", (yyvsp[(1) - (5)].stringptr), (yyvsp[(3) - (5)].pnode)->name);
	yyerror(buf);
	YYERROR;
	yyerrok;
;}
    break;

  case 33:
#line 178 "parser_bugs.y"
    {
	(yyval.pnode) = bugs_node_create(BN_STOCHREL);
	bugs_node_addparam((yyval.pnode), (yyvsp[(1) - (3)].pnode));
	bugs_node_addparam((yyval.pnode), (yyvsp[(3) - (3)].pnode));
;}
    break;

  case 34:
#line 185 "parser_bugs.y"
    {
	(yyval.pnode) = bugs_node_create(BN_FUNC);
	bugs_node_setname((yyval.pnode), "*");
	bugs_node_addparam((yyval.pnode), (yyvsp[(1) - (3)].pnode));
	bugs_node_addparam((yyval.pnode), (yyvsp[(3) - (3)].pnode));
;}
    break;

  case 35:
#line 191 "parser_bugs.y"
    {
	(yyval.pnode) = (yyvsp[(1) - (3)].pnode);
	bugs_node_addparam((yyval.pnode), (yyvsp[(3) - (3)].pnode));
;}
    break;

  case 36:
#line 197 "parser_bugs.y"
    {
	(yyval.pnode) = bugs_node_create(BN_FUNC);
	bugs_node_setname((yyval.pnode), "+");
	bugs_node_addparam((yyval.pnode), (yyvsp[(1) - (3)].pnode));
	bugs_node_addparam((yyval.pnode), (yyvsp[(3) - (3)].pnode));
;}
    break;

  case 37:
#line 203 "parser_bugs.y"
    {
	(yyval.pnode) = (yyvsp[(1) - (3)].pnode);
	bugs_node_addparam((yyval.pnode), (yyvsp[(3) - (3)].pnode));
;}
    break;

  case 38:
#line 209 "parser_bugs.y"
    {
	(yyval.pnode) = (yyvsp[(1) - (1)].pnode);
;}
    break;

  case 39:
#line 212 "parser_bugs.y"
    {
	(yyval.pnode) = bugs_node_create(BN_VALUE);
	bugs_node_addvalue((yyval.pnode), (yyvsp[(1) - (1)].val));
;}
    break;

  case 40:
#line 216 "parser_bugs.y"
    {
	(yyval.pnode) = bugs_node_create(BN_LENGTH);
	bugs_node_addparam((yyval.pnode), (yyvsp[(3) - (4)].pnode));
;}
    break;

  case 41:
#line 220 "parser_bugs.y"
    {
	(yyval.pnode) = bugs_node_create(BN_DIM);
	bugs_node_addparam((yyval.pnode), (yyvsp[(3) - (4)].pnode));
;}
    break;

  case 42:
#line 224 "parser_bugs.y"
    {
	(yyval.pnode) = bugs_node_create(BN_FUNC);
	bugs_node_setname((yyval.pnode), (yyvsp[(1) - (4)].stringptr));
	(yyval.pnode)->params = (yyvsp[(3) - (4)].pnode)->params;
;}
    break;

  case 43:
#line 229 "parser_bugs.y"
    {
	(yyval.pnode) = (yyvsp[(1) - (1)].pnode);
//	bugs_node_create(BN_FUNC);
//	bugs_node_setname($$, "*");
//	bugs_node_addparam($$, $1);
;}
    break;

  case 44:
#line 235 "parser_bugs.y"
    {
	(yyval.pnode) = bugs_node_create(BN_FUNC);
	bugs_node_setname((yyval.pnode), "/");
	bugs_node_addparam((yyval.pnode), (yyvsp[(1) - (3)].pnode));
	bugs_node_addparam((yyval.pnode), (yyvsp[(3) - (3)].pnode));
;}
    break;

  case 45:
#line 241 "parser_bugs.y"
    {
	(yyval.pnode) = (yyvsp[(1) - (1)].pnode);
;}
    break;

  case 46:
#line 244 "parser_bugs.y"
    {
	(yyval.pnode) = bugs_node_create(BN_FUNC);
	bugs_node_setname((yyval.pnode), "-");
	bugs_node_addparam((yyval.pnode), (yyvsp[(1) - (3)].pnode));
	bugs_node_addparam((yyval.pnode), (yyvsp[(3) - (3)].pnode));
;}
    break;

  case 47:
#line 250 "parser_bugs.y"
    {
	(yyval.pnode) = bugs_node_create(BN_FUNC);
	bugs_node_setname((yyval.pnode), "NEG");
	bugs_node_addparam((yyval.pnode), (yyvsp[(2) - (2)].pnode));
;}
    break;

  case 48:
#line 255 "parser_bugs.y"
    { 
	(yyval.pnode) = bugs_node_create(BN_FUNC);
	bugs_node_setname((yyval.pnode), ">");
	bugs_node_addparam((yyval.pnode), (yyvsp[(1) - (3)].pnode));
	bugs_node_addparam((yyval.pnode), (yyvsp[(3) - (3)].pnode));
;}
    break;

  case 49:
#line 261 "parser_bugs.y"
    {
	(yyval.pnode) = bugs_node_create(BN_FUNC);
	bugs_node_setname((yyval.pnode), ">=");
	bugs_node_addparam((yyval.pnode), (yyvsp[(1) - (3)].pnode));
	bugs_node_addparam((yyval.pnode), (yyvsp[(3) - (3)].pnode));
;}
    break;

  case 50:
#line 267 "parser_bugs.y"
    {
	(yyval.pnode) = bugs_node_create(BN_FUNC);
	bugs_node_setname((yyval.pnode), "<");
	bugs_node_addparam((yyval.pnode), (yyvsp[(1) - (3)].pnode));
	bugs_node_addparam((yyval.pnode), (yyvsp[(3) - (3)].pnode));
;}
    break;

  case 51:
#line 273 "parser_bugs.y"
    {
	(yyval.pnode) = bugs_node_create(BN_FUNC);
	bugs_node_setname((yyval.pnode), "<=");
	bugs_node_addparam((yyval.pnode), (yyvsp[(1) - (3)].pnode));
	bugs_node_addparam((yyval.pnode), (yyvsp[(3) - (3)].pnode));
;}
    break;

  case 52:
#line 279 "parser_bugs.y"
    {
	(yyval.pnode) = bugs_node_create(BN_FUNC);
	bugs_node_setname((yyval.pnode), "==");
	bugs_node_addparam((yyval.pnode), (yyvsp[(1) - (3)].pnode));
	bugs_node_addparam((yyval.pnode), (yyvsp[(3) - (3)].pnode));
;}
    break;

  case 53:
#line 285 "parser_bugs.y"
    {
	(yyval.pnode) = bugs_node_create(BN_FUNC);
	bugs_node_setname((yyval.pnode), "!=");
	bugs_node_addparam((yyval.pnode), (yyvsp[(1) - (3)].pnode));
	bugs_node_addparam((yyval.pnode), (yyvsp[(3) - (3)].pnode));
;}
    break;

  case 54:
#line 291 "parser_bugs.y"
    {
	(yyval.pnode) = bugs_node_create(BN_FUNC);
	bugs_node_setname((yyval.pnode), "^");
	bugs_node_addparam((yyval.pnode), (yyvsp[(1) - (3)].pnode));
	bugs_node_addparam((yyval.pnode), (yyvsp[(3) - (3)].pnode));
;}
    break;

  case 55:
#line 297 "parser_bugs.y"
    {
	(yyval.pnode) = bugs_node_create(BN_FUNC);
	bugs_node_setname((yyval.pnode), "^");
	bugs_node_addparam((yyval.pnode), (yyvsp[(1) - (3)].pnode));
	bugs_node_addparam((yyval.pnode), (yyvsp[(3) - (3)].pnode));
;}
    break;

  case 56:
#line 303 "parser_bugs.y"
    {
	(yyval.pnode) = bugs_node_create(BN_FUNC);
	bugs_node_setname((yyval.pnode), "^");
	bugs_node_addparam((yyval.pnode), (yyvsp[(1) - (3)].pnode));
	bugs_node_addparam((yyval.pnode), (yyvsp[(3) - (3)].pnode));
;}
    break;

  case 57:
#line 309 "parser_bugs.y"
    {
	(yyval.pnode) = bugs_node_create(BN_FUNC);
	bugs_node_setname((yyval.pnode), (yyvsp[(2) - (3)].stringptr));
	bugs_node_addparam((yyval.pnode), (yyvsp[(1) - (3)].pnode));
	bugs_node_addparam((yyval.pnode), (yyvsp[(3) - (3)].pnode));
;}
    break;

  case 58:
#line 315 "parser_bugs.y"
    {
	(yyval.pnode) = (yyvsp[(2) - (3)].pnode);
;}
    break;

  case 59:
#line 320 "parser_bugs.y"
    {
	(yyval.pnode) = bugs_node_create(BN_LIST);
	bugs_node_addparam((yyval.pnode), (yyvsp[(1) - (1)].pnode));
;}
    break;

  case 60:
#line 324 "parser_bugs.y"
    {
	(yyval.pnode) = (yyvsp[(1) - (3)].pnode);
	bugs_node_addparam((yyval.pnode), (yyvsp[(3) - (3)].pnode));
;}
    break;

  case 62:
#line 340 "parser_bugs.y"
    {
	(yyval.pnode) = bugs_node_create(BN_RANGE);
	bugs_node_addparam((yyval.pnode), (yyvsp[(1) - (1)].pnode));
;}
    break;

  case 63:
#line 344 "parser_bugs.y"
    {
	(yyval.pnode) = bugs_node_create(BN_RANGE);
	bugs_node_addparam((yyval.pnode), (yyvsp[(1) - (3)].pnode));
	bugs_node_addparam((yyval.pnode), (yyvsp[(3) - (3)].pnode));
;}
    break;

  case 64:
#line 351 "parser_bugs.y"
    {
	(yyval.pnode) = bugs_node_create(BN_FUNC);
	bugs_node_setname((yyval.pnode), (yyvsp[(1) - (4)].stringptr));
	(yyval.pnode)->params = (yyvsp[(3) - (4)].pnode)->params;
	//bugs_node_addparam($$, $3);
;}
    break;

  case 65:
#line 359 "parser_bugs.y"
    {
	(yyval.pnode) = bugs_node_create(BN_VAR);
	bugs_node_setname((yyval.pnode), (yyvsp[(1) - (1)].stringptr));
;}
    break;

  case 66:
#line 363 "parser_bugs.y"
    {
	(yyval.pnode) = bugs_node_create(BN_VAR);
	bugs_node_setname((yyval.pnode), (yyvsp[(1) - (4)].stringptr));
	(yyval.pnode)->params = (yyvsp[(3) - (4)].pnode)->params;
	//bugs_node_addparam($$, $3);
;}
    break;

  case 67:
#line 369 "parser_bugs.y"
    {
	(yyval.pnode) = bugs_node_create(BN_VAR);
	bugs_node_setname((yyval.pnode), (yyvsp[(1) - (3)].stringptr));
;}
    break;

  case 68:
#line 373 "parser_bugs.y"
    {
	char buf[1024];
	sprintf(buf, "index expression of an array '%s' has error", (yyvsp[(1) - (4)].stringptr));
	yyerror(buf);
	YYERROR;
	yyerrok;
;}
    break;


/* Line 1267 of yacc.c.  */
#line 1973 "parser_bugs.tab.c"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;


  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (yymsg);
	  }
	else
	  {
	    yyerror (YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse look-ahead token after an
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

  /* Else will try to reuse look-ahead token after shifting the error
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

  /* Do not reclaim the symbols of the rule which action triggered
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
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
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

  if (yyn == YYFINAL)
    YYACCEPT;

  *++yyvsp = yylval;


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

#ifndef yyoverflow
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEOF && yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval);
  /* Do not reclaim the symbols of the rule which action triggered
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
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}


#line 382 "parser_bugs.y"


/*
int main(int argc, char **argv)
{
#ifdef  YYDEBUG
        extern int bugsdebug;
        bugsdebug = 0;
#endif

        GC_INIT();

        g_bugsnode = NULL;
        if(bugsparse() != 0)
        {
                printf("Err\n");
                return -1;
        }

        printf("ok\n");
        bugs_node_dump(g_bugsnode);

        return 0;
}
*/

extern int bugslineno;
extern char* bugstext;
extern int bugsnerrs;
int yyerror (const char *s)
{
	fprintf(stderr, "Error#%d: %s (near '%s' [line:%d])\n", bugsnerrs, s, bugstext, bugslineno);
}

int bugswrap()
{
        return 1;
}


