grammar SysY;

options { language=Cpp; }

compUnit     : (decl | funcDef)*;

decl         : 'const' type constDef (',' constDef)* ';' #constDecl
             | type varDef (',' varDef)* ';' #varDecl
             ;
constDef     : Ident ('[' constExp ']')* '=' constInitVal;
varDef       : Ident ('[' constExp ']')* #uninitVarDef
             | Ident ('[' constExp ']')* '=' initVal #initVarDef
             ;
initVal      : exp #exprInit
             | '{' (initVal (',' initVal)*)? '}' #listInit
             ;
constExp     : addExp;
constInitVal : constExp #constInit
             | '{' (constInitVal (',' constInitVal)*)? '}' #constListInit
             ;

type         : 'void' | 'int' | 'float';
funcDef      : type Ident '(' (funcFParams)? ')' block;
funcFParams  : funcFParam (',' funcFParam)*;
funcFParam   : type Ident ('[' ']' ('[' exp ']')*)?;
block        : '{' (blockItem)* '}';
blockItem    : decl | stmt;
stmt         : lVal '=' exp ';' #Assign
             | (exp)? ';' #Expr
             | block #BlockStmt
             | 'if' '(' cond ')' stmt ('else' stmt)? #If
             | 'while' '(' cond ')' stmt #While
             | 'break' ';' #Break
             | 'continue' ';' #Continue
             | 'return' (exp)? ';' #Return
             ;
exp          : addExp;
cond         : lOrExp;
lVal         : Ident ('[' exp ']')*;
primaryExp   : '(' exp ')' #closedExpr
             | lVal #lValValue
             | number #literalValue
             ;
number       : IntConst #IntNumber
             | FloatConst #FloatNumber
             ;
unaryExp     : primaryExp #fromPrimary
             | Ident '(' funcRParams? ')' #call
             | unaryOP unaryExp #opUnary
             ;
unaryOP      : '+' | '-' | '!';
funcRParams  : exp (',' exp)*;
mulExp       : unaryExp #fromUnary
             | mulExp op=('*' | '/' | '%') unaryExp #mulOp
             ;
addExp       : mulExp #fromMul
             | addExp op=('+' | '-') mulExp #addOp
             ;
relExp       : addExp #fromAdd
             | relExp op=('<' | '>' | '<=' | '>=') addExp #relOp
             ;
eqExp        : relExp #fromRel
             | eqExp op=('==' | '!=') relExp #eqOp
             ;
lAndExp      : eqExp #fromEq
             | lAndExp '&&' eqExp #lAndOp
             ;
lOrExp       : lAndExp #fromLAnd
             | lOrExp '||' lAndExp #lOrOp
             ;

Ident        : Nondigit (Nondigit | Digit)*;
fragment Nondigit     : [a-zA-Z_];
fragment Digit        : [0-9];


IntConst:
	DecimalConstant
	| OctalConstant
	| HexadecimalConstant;

fragment DecimalConstant: NonzeroDigit Digit*;

fragment NonzeroDigit: [1-9];

fragment OctalConstant: '0' OctalDigit*;

fragment OctalDigit: [0-7];

fragment HexadecimalConstant:
	HexadecimalPrefix HexadecimalDigit+;

fragment HexadecimalPrefix: '0' [xX];

fragment HexadecimalDigit: [0-9a-fA-F];

FloatConst:
	DecimalFloatingConstant
	| HexadecimalFloatingConstant;

fragment DecimalFloatingConstant:
	FractionalConstant ExponentPart?
	| Digit+ ExponentPart;

fragment FractionalConstant: Digit* '.' Digit+ | Digit+ '.';

fragment ExponentPart: [eE] Sign? Digit+;

fragment Sign: [+-];

fragment HexadecimalFloatingConstant:
	HexadecimalPrefix (HexadecimalFractionalConstant | HexadecimalDigit+) BinaryExponentPart;

fragment HexadecimalFractionalConstant:
	HexadecimalDigit* '.' HexadecimalDigit+
	| HexadecimalDigit+ '.';

fragment BinaryExponentPart: [pP] Sign? Digit+;

Whitespace: [ \t]+ -> skip;

Newline: ('\r' '\n'? | '\n') -> skip;

BlockComment: '/*' .*? '*/' -> skip;

LineComment: '//' ~[\r\n]* -> skip;

