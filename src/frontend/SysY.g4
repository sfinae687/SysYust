grammar SysY;

options { language=Cpp; }

compUnit     : (decl | funcDef)*;

decl         : constDecl | varDecl;
constDecl    : 'const' type constDef (',' constDef)* ';';
type         : 'void' | 'int' | 'float';
constDef     : Ident ('[' constExp ']')* '=' constInitVal;
constInitVal : constExp | '{' (constInitVal (',' constInitVal)*)? '}';
varDecl      : type varDef (',' varDef)* ';';
varDef       : Ident ('[' constExp ']')* | Ident ('[' constExp ']')* '=' initVal;
initVal      : exp | '{' (initVal (',' initVal)*)? '}';
funcDef      : type Ident '(' (funcFParams)? ')' block;
funcFParams  : funcFParam (',' funcFParam)*;
funcFParam   : type Ident ('[' ']' ('[' exp ']')*)?;
block        : '{' (blockItem)* '}';
blockItem    : decl | stmt;
stmt         : lVal '=' exp ';' | (exp)? ';' | block 
             | 'if' '(' cond ')' stmt ('else' stmt)?
             | 'while' '(' cond ')' stmt
             | 'break' ';' | 'continue' ';'
             | 'return' (exp)? ';';
exp          : addExp;
cond         : lOrExp;
lVal         : Ident ('[' exp ']')*;
primaryExp   : '(' exp ')' | lVal | number;
number       : IntConst | FloatConst;
unaryExp     : primaryExp | Ident '(' funcRParams? ')' | unaryOP unaryExp;
unaryOP      : '+' | '-' | '!';
funcRParams  : exp (',' exp)*;
mulExp       : unaryExp | mulExp ('*' | '/' | '%') unaryExp;
addExp       : mulExp | addExp ('+' | '-') mulExp;
relExp       : addExp | relExp ('<' | '>' | '<=' | '>=') addExp;
eqExp        : relExp | eqExp ('==' | '!=') relExp;
lAndExp      : eqExp | lAndExp '&&' eqExp;
lOrExp       : lAndExp | lOrExp '||' lAndExp;
constExp     : addExp;

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

