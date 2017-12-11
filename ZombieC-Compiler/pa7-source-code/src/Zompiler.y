%{
#include "Node.h"
#include <iostream>
#include <string>
extern int yylex();
extern void yyerror(const char* s);

// Global for the main block
NBlock* gMainBlock = nullptr;

// Disable the warnings that Bison creates
#pragma warning(disable: 4065)
%}

/* Add one union member for each Node* type */
%union {
    Node* node;
	NBlock* block;
	NStatement* statement;
	NNumeric* numeric;
	NBoolean* boolean;
	std::string* string;
	int token;
}

%error-verbose

/* Terminal symbols */
%token <string> TINTEGER
%token <token> TLBRACE TRBRACE TSEMI TLPAREN TRPAREN
%token <token> TMAIN TROTATE TFORWARD TIF TELSE TISHUMAN TISZOMBIE TISPASSABLE TISRANDOM TISWALL TATTACK TRANGED

/* Statements */
%type <block> main_loop block ifelse
%type <statement> statement rotate forward attack ranged
 
/* Expressions */
%type <numeric> numeric
%type <boolean> boolean is_human is_zombie is_passable is_random is_wall

%%

main_loop	: TMAIN TLBRACE block TRBRACE { std::cout << "Main entry point found!" << std::endl; ($3)->SetMainBlock(); gMainBlock = $3; }
;

block		: statement { std::cout << "Single statement" << std::endl; $$ = new NBlock(); ($$)->AddStatement($1); }
			| block statement { std::cout << "Multiple statements" << std::endl; ($1)->AddStatement($2); }
;

statement	: rotate TSEMI | forward TSEMI | ifelse | attack TSEMI | ranged TSEMI
;
			
rotate		: TROTATE TLPAREN numeric TRPAREN { std::cout << "Rotate command" << std::endl; $$ = new NRotate($3); }
;
			
numeric		: TINTEGER { std::cout << "Numeric value of " << *($1) << std::endl; $$ = new NNumeric(*($1)); }
;

forward		: TFORWARD TLPAREN TRPAREN { std::cout << "Forward command" << std::endl; $$ = new NForward(); }
;

ifelse		: TIF TLPAREN boolean TRPAREN TLBRACE block TRBRACE TELSE TLBRACE block TRBRACE { std::cout << "If-else command" << std::endl; $$ = new NIfElse($3, $6, $10); }
;

boolean		: is_human | is_zombie | is_passable | is_random | is_wall
;

is_human	: TISHUMAN TLPAREN numeric TRPAREN { std::cout << "Is_human command" << std::endl; $$ = new NIsHuman($3); }
;

is_zombie	: TISZOMBIE TLPAREN numeric TRPAREN { std::cout << "Is_zombie command" << std::endl; $$ = new NIsZombie($3); }
;

is_passable	: TISPASSABLE TLPAREN TRPAREN { std::cout << "Is_passable command" << std::endl; $$ = new NIsPassable(); }
;

is_random	: TISRANDOM TLPAREN TRPAREN { std::cout << "Is_random command" << std::endl; $$ = new NIsRandom(); }
;

is_wall		: TISWALL TLPAREN TRPAREN { std::cout << "Is_wall command" << std::endl; $$ = new NIsWall(); }
;

attack		: TATTACK TLPAREN TRPAREN { std::cout << "Attack command" << std::endl; $$ = new NAttack(); }
;

ranged		: TRANGED TLPAREN TRPAREN { std::cout << "Ranged command" << std::endl; $$ = new NRanged(); }
;

%%
