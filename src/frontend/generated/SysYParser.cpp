
// Generated from E:/Source/SysYust/src/frontend/SysY.g4 by ANTLR 4.13.1


#include "SysYListener.h"
#include "SysYVisitor.h"

#include "SysYParser.h"


using namespace antlrcpp;

using namespace antlr4;

namespace {

struct SysYParserStaticData final {
  SysYParserStaticData(std::vector<std::string> ruleNames,
                        std::vector<std::string> literalNames,
                        std::vector<std::string> symbolicNames)
      : ruleNames(std::move(ruleNames)), literalNames(std::move(literalNames)),
        symbolicNames(std::move(symbolicNames)),
        vocabulary(this->literalNames, this->symbolicNames) {}

  SysYParserStaticData(const SysYParserStaticData&) = delete;
  SysYParserStaticData(SysYParserStaticData&&) = delete;
  SysYParserStaticData& operator=(const SysYParserStaticData&) = delete;
  SysYParserStaticData& operator=(SysYParserStaticData&&) = delete;

  std::vector<antlr4::dfa::DFA> decisionToDFA;
  antlr4::atn::PredictionContextCache sharedContextCache;
  const std::vector<std::string> ruleNames;
  const std::vector<std::string> literalNames;
  const std::vector<std::string> symbolicNames;
  const antlr4::dfa::Vocabulary vocabulary;
  antlr4::atn::SerializedATNView serializedATN;
  std::unique_ptr<antlr4::atn::ATN> atn;
};

::antlr4::internal::OnceFlag sysyParserOnceFlag;
#if ANTLR4_USE_THREAD_LOCAL_CACHE
static thread_local
#endif
SysYParserStaticData *sysyParserStaticData = nullptr;

void sysyParserInitialize() {
#if ANTLR4_USE_THREAD_LOCAL_CACHE
  if (sysyParserStaticData != nullptr) {
    return;
  }
#else
  assert(sysyParserStaticData == nullptr);
#endif
  auto staticData = std::make_unique<SysYParserStaticData>(
    std::vector<std::string>{
      "compUnit", "decl", "constDef", "varDef", "initVal", "constExp", "constInitVal", 
      "type", "funcDef", "funcFParams", "funcFParam", "block", "blockItem", 
      "stmt", "exp", "cond", "lVal", "primaryExp", "number", "unaryExp", 
      "unaryOP", "funcRParams", "mulExp", "addExp", "relExp", "eqExp", "lAndExp", 
      "lOrExp"
    },
    std::vector<std::string>{
      "", "'const'", "','", "';'", "'['", "']'", "'='", "'{'", "'}'", "'void'", 
      "'int'", "'float'", "'('", "')'", "'if'", "'else'", "'while'", "'break'", 
      "'continue'", "'return'", "'+'", "'-'", "'!'", "'*'", "'/'", "'%'", 
      "'<'", "'>'", "'<='", "'>='", "'=='", "'!='", "'&&'", "'||'"
    },
    std::vector<std::string>{
      "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", 
      "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", 
      "Ident", "IntConst", "FloatConst", "Whitespace", "Newline", "BlockComment", 
      "LineComment"
    }
  );
  static const int32_t serializedATNSegment[] = {
  	4,1,40,355,2,0,7,0,2,1,7,1,2,2,7,2,2,3,7,3,2,4,7,4,2,5,7,5,2,6,7,6,2,
  	7,7,7,2,8,7,8,2,9,7,9,2,10,7,10,2,11,7,11,2,12,7,12,2,13,7,13,2,14,7,
  	14,2,15,7,15,2,16,7,16,2,17,7,17,2,18,7,18,2,19,7,19,2,20,7,20,2,21,7,
  	21,2,22,7,22,2,23,7,23,2,24,7,24,2,25,7,25,2,26,7,26,2,27,7,27,1,0,1,
  	0,5,0,59,8,0,10,0,12,0,62,9,0,1,1,1,1,1,1,1,1,1,1,5,1,69,8,1,10,1,12,
  	1,72,9,1,1,1,1,1,1,1,1,1,1,1,1,1,5,1,80,8,1,10,1,12,1,83,9,1,1,1,1,1,
  	3,1,87,8,1,1,2,1,2,1,2,1,2,1,2,5,2,94,8,2,10,2,12,2,97,9,2,1,2,1,2,1,
  	2,1,3,1,3,1,3,1,3,1,3,5,3,107,8,3,10,3,12,3,110,9,3,1,3,1,3,1,3,1,3,1,
  	3,5,3,117,8,3,10,3,12,3,120,9,3,1,3,1,3,3,3,124,8,3,1,4,1,4,1,4,1,4,1,
  	4,5,4,131,8,4,10,4,12,4,134,9,4,3,4,136,8,4,1,4,3,4,139,8,4,1,5,1,5,1,
  	6,1,6,1,6,1,6,1,6,5,6,148,8,6,10,6,12,6,151,9,6,3,6,153,8,6,1,6,3,6,156,
  	8,6,1,7,1,7,1,8,1,8,1,8,1,8,3,8,164,8,8,1,8,1,8,1,8,1,9,1,9,1,9,5,9,172,
  	8,9,10,9,12,9,175,9,9,1,10,1,10,1,10,1,10,1,10,1,10,1,10,1,10,5,10,185,
  	8,10,10,10,12,10,188,9,10,3,10,190,8,10,1,11,1,11,5,11,194,8,11,10,11,
  	12,11,197,9,11,1,11,1,11,1,12,1,12,3,12,203,8,12,1,13,1,13,1,13,1,13,
  	1,13,1,13,3,13,211,8,13,1,13,1,13,1,13,1,13,1,13,1,13,1,13,1,13,1,13,
  	3,13,222,8,13,1,13,1,13,1,13,1,13,1,13,1,13,1,13,1,13,1,13,1,13,1,13,
  	1,13,3,13,236,8,13,1,13,3,13,239,8,13,1,14,1,14,1,15,1,15,1,16,1,16,1,
  	16,1,16,1,16,5,16,250,8,16,10,16,12,16,253,9,16,1,17,1,17,1,17,1,17,1,
  	17,1,17,3,17,261,8,17,1,18,1,18,3,18,265,8,18,1,19,1,19,1,19,1,19,3,19,
  	271,8,19,1,19,1,19,1,19,1,19,3,19,277,8,19,1,20,1,20,1,21,1,21,1,21,5,
  	21,284,8,21,10,21,12,21,287,9,21,1,22,1,22,1,22,1,22,1,22,1,22,5,22,295,
  	8,22,10,22,12,22,298,9,22,1,23,1,23,1,23,1,23,1,23,1,23,5,23,306,8,23,
  	10,23,12,23,309,9,23,1,24,1,24,1,24,1,24,1,24,1,24,5,24,317,8,24,10,24,
  	12,24,320,9,24,1,25,1,25,1,25,1,25,1,25,1,25,5,25,328,8,25,10,25,12,25,
  	331,9,25,1,26,1,26,1,26,1,26,1,26,1,26,5,26,339,8,26,10,26,12,26,342,
  	9,26,1,27,1,27,1,27,1,27,1,27,1,27,5,27,350,8,27,10,27,12,27,353,9,27,
  	1,27,0,6,44,46,48,50,52,54,28,0,2,4,6,8,10,12,14,16,18,20,22,24,26,28,
  	30,32,34,36,38,40,42,44,46,48,50,52,54,0,6,1,0,9,11,1,0,20,22,1,0,23,
  	25,1,0,20,21,1,0,26,29,1,0,30,31,371,0,60,1,0,0,0,2,86,1,0,0,0,4,88,1,
  	0,0,0,6,123,1,0,0,0,8,138,1,0,0,0,10,140,1,0,0,0,12,155,1,0,0,0,14,157,
  	1,0,0,0,16,159,1,0,0,0,18,168,1,0,0,0,20,176,1,0,0,0,22,191,1,0,0,0,24,
  	202,1,0,0,0,26,238,1,0,0,0,28,240,1,0,0,0,30,242,1,0,0,0,32,244,1,0,0,
  	0,34,260,1,0,0,0,36,264,1,0,0,0,38,276,1,0,0,0,40,278,1,0,0,0,42,280,
  	1,0,0,0,44,288,1,0,0,0,46,299,1,0,0,0,48,310,1,0,0,0,50,321,1,0,0,0,52,
  	332,1,0,0,0,54,343,1,0,0,0,56,59,3,2,1,0,57,59,3,16,8,0,58,56,1,0,0,0,
  	58,57,1,0,0,0,59,62,1,0,0,0,60,58,1,0,0,0,60,61,1,0,0,0,61,1,1,0,0,0,
  	62,60,1,0,0,0,63,64,5,1,0,0,64,65,3,14,7,0,65,70,3,4,2,0,66,67,5,2,0,
  	0,67,69,3,4,2,0,68,66,1,0,0,0,69,72,1,0,0,0,70,68,1,0,0,0,70,71,1,0,0,
  	0,71,73,1,0,0,0,72,70,1,0,0,0,73,74,5,3,0,0,74,87,1,0,0,0,75,76,3,14,
  	7,0,76,81,3,6,3,0,77,78,5,2,0,0,78,80,3,6,3,0,79,77,1,0,0,0,80,83,1,0,
  	0,0,81,79,1,0,0,0,81,82,1,0,0,0,82,84,1,0,0,0,83,81,1,0,0,0,84,85,5,3,
  	0,0,85,87,1,0,0,0,86,63,1,0,0,0,86,75,1,0,0,0,87,3,1,0,0,0,88,95,5,34,
  	0,0,89,90,5,4,0,0,90,91,3,10,5,0,91,92,5,5,0,0,92,94,1,0,0,0,93,89,1,
  	0,0,0,94,97,1,0,0,0,95,93,1,0,0,0,95,96,1,0,0,0,96,98,1,0,0,0,97,95,1,
  	0,0,0,98,99,5,6,0,0,99,100,3,12,6,0,100,5,1,0,0,0,101,108,5,34,0,0,102,
  	103,5,4,0,0,103,104,3,10,5,0,104,105,5,5,0,0,105,107,1,0,0,0,106,102,
  	1,0,0,0,107,110,1,0,0,0,108,106,1,0,0,0,108,109,1,0,0,0,109,124,1,0,0,
  	0,110,108,1,0,0,0,111,118,5,34,0,0,112,113,5,4,0,0,113,114,3,10,5,0,114,
  	115,5,5,0,0,115,117,1,0,0,0,116,112,1,0,0,0,117,120,1,0,0,0,118,116,1,
  	0,0,0,118,119,1,0,0,0,119,121,1,0,0,0,120,118,1,0,0,0,121,122,5,6,0,0,
  	122,124,3,8,4,0,123,101,1,0,0,0,123,111,1,0,0,0,124,7,1,0,0,0,125,139,
  	3,28,14,0,126,135,5,7,0,0,127,132,3,8,4,0,128,129,5,2,0,0,129,131,3,8,
  	4,0,130,128,1,0,0,0,131,134,1,0,0,0,132,130,1,0,0,0,132,133,1,0,0,0,133,
  	136,1,0,0,0,134,132,1,0,0,0,135,127,1,0,0,0,135,136,1,0,0,0,136,137,1,
  	0,0,0,137,139,5,8,0,0,138,125,1,0,0,0,138,126,1,0,0,0,139,9,1,0,0,0,140,
  	141,3,46,23,0,141,11,1,0,0,0,142,156,3,10,5,0,143,152,5,7,0,0,144,149,
  	3,12,6,0,145,146,5,2,0,0,146,148,3,12,6,0,147,145,1,0,0,0,148,151,1,0,
  	0,0,149,147,1,0,0,0,149,150,1,0,0,0,150,153,1,0,0,0,151,149,1,0,0,0,152,
  	144,1,0,0,0,152,153,1,0,0,0,153,154,1,0,0,0,154,156,5,8,0,0,155,142,1,
  	0,0,0,155,143,1,0,0,0,156,13,1,0,0,0,157,158,7,0,0,0,158,15,1,0,0,0,159,
  	160,3,14,7,0,160,161,5,34,0,0,161,163,5,12,0,0,162,164,3,18,9,0,163,162,
  	1,0,0,0,163,164,1,0,0,0,164,165,1,0,0,0,165,166,5,13,0,0,166,167,3,22,
  	11,0,167,17,1,0,0,0,168,173,3,20,10,0,169,170,5,2,0,0,170,172,3,20,10,
  	0,171,169,1,0,0,0,172,175,1,0,0,0,173,171,1,0,0,0,173,174,1,0,0,0,174,
  	19,1,0,0,0,175,173,1,0,0,0,176,177,3,14,7,0,177,189,5,34,0,0,178,179,
  	5,4,0,0,179,186,5,5,0,0,180,181,5,4,0,0,181,182,3,28,14,0,182,183,5,5,
  	0,0,183,185,1,0,0,0,184,180,1,0,0,0,185,188,1,0,0,0,186,184,1,0,0,0,186,
  	187,1,0,0,0,187,190,1,0,0,0,188,186,1,0,0,0,189,178,1,0,0,0,189,190,1,
  	0,0,0,190,21,1,0,0,0,191,195,5,7,0,0,192,194,3,24,12,0,193,192,1,0,0,
  	0,194,197,1,0,0,0,195,193,1,0,0,0,195,196,1,0,0,0,196,198,1,0,0,0,197,
  	195,1,0,0,0,198,199,5,8,0,0,199,23,1,0,0,0,200,203,3,2,1,0,201,203,3,
  	26,13,0,202,200,1,0,0,0,202,201,1,0,0,0,203,25,1,0,0,0,204,205,3,32,16,
  	0,205,206,5,6,0,0,206,207,3,28,14,0,207,208,5,3,0,0,208,239,1,0,0,0,209,
  	211,3,28,14,0,210,209,1,0,0,0,210,211,1,0,0,0,211,212,1,0,0,0,212,239,
  	5,3,0,0,213,239,3,22,11,0,214,215,5,14,0,0,215,216,5,12,0,0,216,217,3,
  	30,15,0,217,218,5,13,0,0,218,221,3,26,13,0,219,220,5,15,0,0,220,222,3,
  	26,13,0,221,219,1,0,0,0,221,222,1,0,0,0,222,239,1,0,0,0,223,224,5,16,
  	0,0,224,225,5,12,0,0,225,226,3,30,15,0,226,227,5,13,0,0,227,228,3,26,
  	13,0,228,239,1,0,0,0,229,230,5,17,0,0,230,239,5,3,0,0,231,232,5,18,0,
  	0,232,239,5,3,0,0,233,235,5,19,0,0,234,236,3,28,14,0,235,234,1,0,0,0,
  	235,236,1,0,0,0,236,237,1,0,0,0,237,239,5,3,0,0,238,204,1,0,0,0,238,210,
  	1,0,0,0,238,213,1,0,0,0,238,214,1,0,0,0,238,223,1,0,0,0,238,229,1,0,0,
  	0,238,231,1,0,0,0,238,233,1,0,0,0,239,27,1,0,0,0,240,241,3,46,23,0,241,
  	29,1,0,0,0,242,243,3,54,27,0,243,31,1,0,0,0,244,251,5,34,0,0,245,246,
  	5,4,0,0,246,247,3,28,14,0,247,248,5,5,0,0,248,250,1,0,0,0,249,245,1,0,
  	0,0,250,253,1,0,0,0,251,249,1,0,0,0,251,252,1,0,0,0,252,33,1,0,0,0,253,
  	251,1,0,0,0,254,255,5,12,0,0,255,256,3,28,14,0,256,257,5,13,0,0,257,261,
  	1,0,0,0,258,261,3,32,16,0,259,261,3,36,18,0,260,254,1,0,0,0,260,258,1,
  	0,0,0,260,259,1,0,0,0,261,35,1,0,0,0,262,265,5,35,0,0,263,265,5,36,0,
  	0,264,262,1,0,0,0,264,263,1,0,0,0,265,37,1,0,0,0,266,277,3,34,17,0,267,
  	268,5,34,0,0,268,270,5,12,0,0,269,271,3,42,21,0,270,269,1,0,0,0,270,271,
  	1,0,0,0,271,272,1,0,0,0,272,277,5,13,0,0,273,274,3,40,20,0,274,275,3,
  	38,19,0,275,277,1,0,0,0,276,266,1,0,0,0,276,267,1,0,0,0,276,273,1,0,0,
  	0,277,39,1,0,0,0,278,279,7,1,0,0,279,41,1,0,0,0,280,285,3,28,14,0,281,
  	282,5,2,0,0,282,284,3,28,14,0,283,281,1,0,0,0,284,287,1,0,0,0,285,283,
  	1,0,0,0,285,286,1,0,0,0,286,43,1,0,0,0,287,285,1,0,0,0,288,289,6,22,-1,
  	0,289,290,3,38,19,0,290,296,1,0,0,0,291,292,10,1,0,0,292,293,7,2,0,0,
  	293,295,3,38,19,0,294,291,1,0,0,0,295,298,1,0,0,0,296,294,1,0,0,0,296,
  	297,1,0,0,0,297,45,1,0,0,0,298,296,1,0,0,0,299,300,6,23,-1,0,300,301,
  	3,44,22,0,301,307,1,0,0,0,302,303,10,1,0,0,303,304,7,3,0,0,304,306,3,
  	44,22,0,305,302,1,0,0,0,306,309,1,0,0,0,307,305,1,0,0,0,307,308,1,0,0,
  	0,308,47,1,0,0,0,309,307,1,0,0,0,310,311,6,24,-1,0,311,312,3,46,23,0,
  	312,318,1,0,0,0,313,314,10,1,0,0,314,315,7,4,0,0,315,317,3,46,23,0,316,
  	313,1,0,0,0,317,320,1,0,0,0,318,316,1,0,0,0,318,319,1,0,0,0,319,49,1,
  	0,0,0,320,318,1,0,0,0,321,322,6,25,-1,0,322,323,3,48,24,0,323,329,1,0,
  	0,0,324,325,10,1,0,0,325,326,7,5,0,0,326,328,3,48,24,0,327,324,1,0,0,
  	0,328,331,1,0,0,0,329,327,1,0,0,0,329,330,1,0,0,0,330,51,1,0,0,0,331,
  	329,1,0,0,0,332,333,6,26,-1,0,333,334,3,50,25,0,334,340,1,0,0,0,335,336,
  	10,1,0,0,336,337,5,32,0,0,337,339,3,50,25,0,338,335,1,0,0,0,339,342,1,
  	0,0,0,340,338,1,0,0,0,340,341,1,0,0,0,341,53,1,0,0,0,342,340,1,0,0,0,
  	343,344,6,27,-1,0,344,345,3,52,26,0,345,351,1,0,0,0,346,347,10,1,0,0,
  	347,348,5,33,0,0,348,350,3,52,26,0,349,346,1,0,0,0,350,353,1,0,0,0,351,
  	349,1,0,0,0,351,352,1,0,0,0,352,55,1,0,0,0,353,351,1,0,0,0,37,58,60,70,
  	81,86,95,108,118,123,132,135,138,149,152,155,163,173,186,189,195,202,
  	210,221,235,238,251,260,264,270,276,285,296,307,318,329,340,351
  };
  staticData->serializedATN = antlr4::atn::SerializedATNView(serializedATNSegment, sizeof(serializedATNSegment) / sizeof(serializedATNSegment[0]));

  antlr4::atn::ATNDeserializer deserializer;
  staticData->atn = deserializer.deserialize(staticData->serializedATN);

  const size_t count = staticData->atn->getNumberOfDecisions();
  staticData->decisionToDFA.reserve(count);
  for (size_t i = 0; i < count; i++) { 
    staticData->decisionToDFA.emplace_back(staticData->atn->getDecisionState(i), i);
  }
  sysyParserStaticData = staticData.release();
}

}

SysYParser::SysYParser(TokenStream *input) : SysYParser(input, antlr4::atn::ParserATNSimulatorOptions()) {}

SysYParser::SysYParser(TokenStream *input, const antlr4::atn::ParserATNSimulatorOptions &options) : Parser(input) {
  SysYParser::initialize();
  _interpreter = new atn::ParserATNSimulator(this, *sysyParserStaticData->atn, sysyParserStaticData->decisionToDFA, sysyParserStaticData->sharedContextCache, options);
}

SysYParser::~SysYParser() {
  delete _interpreter;
}

const atn::ATN& SysYParser::getATN() const {
  return *sysyParserStaticData->atn;
}

std::string SysYParser::getGrammarFileName() const {
  return "SysY.g4";
}

const std::vector<std::string>& SysYParser::getRuleNames() const {
  return sysyParserStaticData->ruleNames;
}

const dfa::Vocabulary& SysYParser::getVocabulary() const {
  return sysyParserStaticData->vocabulary;
}

antlr4::atn::SerializedATNView SysYParser::getSerializedATN() const {
  return sysyParserStaticData->serializedATN;
}


//----------------- CompUnitContext ------------------------------------------------------------------

SysYParser::CompUnitContext::CompUnitContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<SysYParser::DeclContext *> SysYParser::CompUnitContext::decl() {
  return getRuleContexts<SysYParser::DeclContext>();
}

SysYParser::DeclContext* SysYParser::CompUnitContext::decl(size_t i) {
  return getRuleContext<SysYParser::DeclContext>(i);
}

std::vector<SysYParser::FuncDefContext *> SysYParser::CompUnitContext::funcDef() {
  return getRuleContexts<SysYParser::FuncDefContext>();
}

SysYParser::FuncDefContext* SysYParser::CompUnitContext::funcDef(size_t i) {
  return getRuleContext<SysYParser::FuncDefContext>(i);
}


size_t SysYParser::CompUnitContext::getRuleIndex() const {
  return SysYParser::RuleCompUnit;
}

void SysYParser::CompUnitContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterCompUnit(this);
}

void SysYParser::CompUnitContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitCompUnit(this);
}


std::any SysYParser::CompUnitContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SysYVisitor*>(visitor))
    return parserVisitor->visitCompUnit(this);
  else
    return visitor->visitChildren(this);
}

SysYParser::CompUnitContext* SysYParser::compUnit() {
  CompUnitContext *_localctx = _tracker.createInstance<CompUnitContext>(_ctx, getState());
  enterRule(_localctx, 0, SysYParser::RuleCompUnit);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(60);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 3586) != 0)) {
      setState(58);
      _errHandler->sync(this);
      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 0, _ctx)) {
      case 1: {
        setState(56);
        decl();
        break;
      }

      case 2: {
        setState(57);
        funcDef();
        break;
      }

      default:
        break;
      }
      setState(62);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- DeclContext ------------------------------------------------------------------

SysYParser::DeclContext::DeclContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t SysYParser::DeclContext::getRuleIndex() const {
  return SysYParser::RuleDecl;
}

void SysYParser::DeclContext::copyFrom(DeclContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- ConstDeclContext ------------------------------------------------------------------

SysYParser::TypeContext* SysYParser::ConstDeclContext::type() {
  return getRuleContext<SysYParser::TypeContext>(0);
}

std::vector<SysYParser::ConstDefContext *> SysYParser::ConstDeclContext::constDef() {
  return getRuleContexts<SysYParser::ConstDefContext>();
}

SysYParser::ConstDefContext* SysYParser::ConstDeclContext::constDef(size_t i) {
  return getRuleContext<SysYParser::ConstDefContext>(i);
}

SysYParser::ConstDeclContext::ConstDeclContext(DeclContext *ctx) { copyFrom(ctx); }

void SysYParser::ConstDeclContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterConstDecl(this);
}
void SysYParser::ConstDeclContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitConstDecl(this);
}

std::any SysYParser::ConstDeclContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SysYVisitor*>(visitor))
    return parserVisitor->visitConstDecl(this);
  else
    return visitor->visitChildren(this);
}
//----------------- VarDeclContext ------------------------------------------------------------------

SysYParser::TypeContext* SysYParser::VarDeclContext::type() {
  return getRuleContext<SysYParser::TypeContext>(0);
}

std::vector<SysYParser::VarDefContext *> SysYParser::VarDeclContext::varDef() {
  return getRuleContexts<SysYParser::VarDefContext>();
}

SysYParser::VarDefContext* SysYParser::VarDeclContext::varDef(size_t i) {
  return getRuleContext<SysYParser::VarDefContext>(i);
}

SysYParser::VarDeclContext::VarDeclContext(DeclContext *ctx) { copyFrom(ctx); }

void SysYParser::VarDeclContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterVarDecl(this);
}
void SysYParser::VarDeclContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitVarDecl(this);
}

std::any SysYParser::VarDeclContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SysYVisitor*>(visitor))
    return parserVisitor->visitVarDecl(this);
  else
    return visitor->visitChildren(this);
}
SysYParser::DeclContext* SysYParser::decl() {
  DeclContext *_localctx = _tracker.createInstance<DeclContext>(_ctx, getState());
  enterRule(_localctx, 2, SysYParser::RuleDecl);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(86);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case SysYParser::T__0: {
        _localctx = _tracker.createInstance<SysYParser::ConstDeclContext>(_localctx);
        enterOuterAlt(_localctx, 1);
        setState(63);
        match(SysYParser::T__0);
        setState(64);
        type();
        setState(65);
        constDef();
        setState(70);
        _errHandler->sync(this);
        _la = _input->LA(1);
        while (_la == SysYParser::T__1) {
          setState(66);
          match(SysYParser::T__1);
          setState(67);
          constDef();
          setState(72);
          _errHandler->sync(this);
          _la = _input->LA(1);
        }
        setState(73);
        match(SysYParser::T__2);
        break;
      }

      case SysYParser::T__8:
      case SysYParser::T__9:
      case SysYParser::T__10: {
        _localctx = _tracker.createInstance<SysYParser::VarDeclContext>(_localctx);
        enterOuterAlt(_localctx, 2);
        setState(75);
        type();
        setState(76);
        varDef();
        setState(81);
        _errHandler->sync(this);
        _la = _input->LA(1);
        while (_la == SysYParser::T__1) {
          setState(77);
          match(SysYParser::T__1);
          setState(78);
          varDef();
          setState(83);
          _errHandler->sync(this);
          _la = _input->LA(1);
        }
        setState(84);
        match(SysYParser::T__2);
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ConstDefContext ------------------------------------------------------------------

SysYParser::ConstDefContext::ConstDefContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SysYParser::ConstDefContext::Ident() {
  return getToken(SysYParser::Ident, 0);
}

SysYParser::ConstInitValContext* SysYParser::ConstDefContext::constInitVal() {
  return getRuleContext<SysYParser::ConstInitValContext>(0);
}

std::vector<SysYParser::ConstExpContext *> SysYParser::ConstDefContext::constExp() {
  return getRuleContexts<SysYParser::ConstExpContext>();
}

SysYParser::ConstExpContext* SysYParser::ConstDefContext::constExp(size_t i) {
  return getRuleContext<SysYParser::ConstExpContext>(i);
}


size_t SysYParser::ConstDefContext::getRuleIndex() const {
  return SysYParser::RuleConstDef;
}

void SysYParser::ConstDefContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterConstDef(this);
}

void SysYParser::ConstDefContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitConstDef(this);
}


std::any SysYParser::ConstDefContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SysYVisitor*>(visitor))
    return parserVisitor->visitConstDef(this);
  else
    return visitor->visitChildren(this);
}

SysYParser::ConstDefContext* SysYParser::constDef() {
  ConstDefContext *_localctx = _tracker.createInstance<ConstDefContext>(_ctx, getState());
  enterRule(_localctx, 4, SysYParser::RuleConstDef);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(88);
    match(SysYParser::Ident);
    setState(95);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == SysYParser::T__3) {
      setState(89);
      match(SysYParser::T__3);
      setState(90);
      constExp();
      setState(91);
      match(SysYParser::T__4);
      setState(97);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(98);
    match(SysYParser::T__5);
    setState(99);
    constInitVal();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- VarDefContext ------------------------------------------------------------------

SysYParser::VarDefContext::VarDefContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t SysYParser::VarDefContext::getRuleIndex() const {
  return SysYParser::RuleVarDef;
}

void SysYParser::VarDefContext::copyFrom(VarDefContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- UninitVarDefContext ------------------------------------------------------------------

tree::TerminalNode* SysYParser::UninitVarDefContext::Ident() {
  return getToken(SysYParser::Ident, 0);
}

std::vector<SysYParser::ConstExpContext *> SysYParser::UninitVarDefContext::constExp() {
  return getRuleContexts<SysYParser::ConstExpContext>();
}

SysYParser::ConstExpContext* SysYParser::UninitVarDefContext::constExp(size_t i) {
  return getRuleContext<SysYParser::ConstExpContext>(i);
}

SysYParser::UninitVarDefContext::UninitVarDefContext(VarDefContext *ctx) { copyFrom(ctx); }

void SysYParser::UninitVarDefContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterUninitVarDef(this);
}
void SysYParser::UninitVarDefContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitUninitVarDef(this);
}

std::any SysYParser::UninitVarDefContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SysYVisitor*>(visitor))
    return parserVisitor->visitUninitVarDef(this);
  else
    return visitor->visitChildren(this);
}
//----------------- InitVarDefContext ------------------------------------------------------------------

tree::TerminalNode* SysYParser::InitVarDefContext::Ident() {
  return getToken(SysYParser::Ident, 0);
}

SysYParser::InitValContext* SysYParser::InitVarDefContext::initVal() {
  return getRuleContext<SysYParser::InitValContext>(0);
}

std::vector<SysYParser::ConstExpContext *> SysYParser::InitVarDefContext::constExp() {
  return getRuleContexts<SysYParser::ConstExpContext>();
}

SysYParser::ConstExpContext* SysYParser::InitVarDefContext::constExp(size_t i) {
  return getRuleContext<SysYParser::ConstExpContext>(i);
}

SysYParser::InitVarDefContext::InitVarDefContext(VarDefContext *ctx) { copyFrom(ctx); }

void SysYParser::InitVarDefContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterInitVarDef(this);
}
void SysYParser::InitVarDefContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitInitVarDef(this);
}

std::any SysYParser::InitVarDefContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SysYVisitor*>(visitor))
    return parserVisitor->visitInitVarDef(this);
  else
    return visitor->visitChildren(this);
}
SysYParser::VarDefContext* SysYParser::varDef() {
  VarDefContext *_localctx = _tracker.createInstance<VarDefContext>(_ctx, getState());
  enterRule(_localctx, 6, SysYParser::RuleVarDef);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(123);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 8, _ctx)) {
    case 1: {
      _localctx = _tracker.createInstance<SysYParser::UninitVarDefContext>(_localctx);
      enterOuterAlt(_localctx, 1);
      setState(101);
      match(SysYParser::Ident);
      setState(108);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == SysYParser::T__3) {
        setState(102);
        match(SysYParser::T__3);
        setState(103);
        constExp();
        setState(104);
        match(SysYParser::T__4);
        setState(110);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      break;
    }

    case 2: {
      _localctx = _tracker.createInstance<SysYParser::InitVarDefContext>(_localctx);
      enterOuterAlt(_localctx, 2);
      setState(111);
      match(SysYParser::Ident);
      setState(118);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == SysYParser::T__3) {
        setState(112);
        match(SysYParser::T__3);
        setState(113);
        constExp();
        setState(114);
        match(SysYParser::T__4);
        setState(120);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      setState(121);
      match(SysYParser::T__5);
      setState(122);
      initVal();
      break;
    }

    default:
      break;
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- InitValContext ------------------------------------------------------------------

SysYParser::InitValContext::InitValContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t SysYParser::InitValContext::getRuleIndex() const {
  return SysYParser::RuleInitVal;
}

void SysYParser::InitValContext::copyFrom(InitValContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- ListInitContext ------------------------------------------------------------------

std::vector<SysYParser::InitValContext *> SysYParser::ListInitContext::initVal() {
  return getRuleContexts<SysYParser::InitValContext>();
}

SysYParser::InitValContext* SysYParser::ListInitContext::initVal(size_t i) {
  return getRuleContext<SysYParser::InitValContext>(i);
}

SysYParser::ListInitContext::ListInitContext(InitValContext *ctx) { copyFrom(ctx); }

void SysYParser::ListInitContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterListInit(this);
}
void SysYParser::ListInitContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitListInit(this);
}

std::any SysYParser::ListInitContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SysYVisitor*>(visitor))
    return parserVisitor->visitListInit(this);
  else
    return visitor->visitChildren(this);
}
//----------------- ExprInitContext ------------------------------------------------------------------

SysYParser::ExpContext* SysYParser::ExprInitContext::exp() {
  return getRuleContext<SysYParser::ExpContext>(0);
}

SysYParser::ExprInitContext::ExprInitContext(InitValContext *ctx) { copyFrom(ctx); }

void SysYParser::ExprInitContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterExprInit(this);
}
void SysYParser::ExprInitContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitExprInit(this);
}

std::any SysYParser::ExprInitContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SysYVisitor*>(visitor))
    return parserVisitor->visitExprInit(this);
  else
    return visitor->visitChildren(this);
}
SysYParser::InitValContext* SysYParser::initVal() {
  InitValContext *_localctx = _tracker.createInstance<InitValContext>(_ctx, getState());
  enterRule(_localctx, 8, SysYParser::RuleInitVal);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(138);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case SysYParser::T__11:
      case SysYParser::T__19:
      case SysYParser::T__20:
      case SysYParser::T__21:
      case SysYParser::Ident:
      case SysYParser::IntConst:
      case SysYParser::FloatConst: {
        _localctx = _tracker.createInstance<SysYParser::ExprInitContext>(_localctx);
        enterOuterAlt(_localctx, 1);
        setState(125);
        exp();
        break;
      }

      case SysYParser::T__6: {
        _localctx = _tracker.createInstance<SysYParser::ListInitContext>(_localctx);
        enterOuterAlt(_localctx, 2);
        setState(126);
        match(SysYParser::T__6);
        setState(135);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if ((((_la & ~ 0x3fULL) == 0) &&
          ((1ULL << _la) & 120266428544) != 0)) {
          setState(127);
          initVal();
          setState(132);
          _errHandler->sync(this);
          _la = _input->LA(1);
          while (_la == SysYParser::T__1) {
            setState(128);
            match(SysYParser::T__1);
            setState(129);
            initVal();
            setState(134);
            _errHandler->sync(this);
            _la = _input->LA(1);
          }
        }
        setState(137);
        match(SysYParser::T__7);
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ConstExpContext ------------------------------------------------------------------

SysYParser::ConstExpContext::ConstExpContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SysYParser::AddExpContext* SysYParser::ConstExpContext::addExp() {
  return getRuleContext<SysYParser::AddExpContext>(0);
}


size_t SysYParser::ConstExpContext::getRuleIndex() const {
  return SysYParser::RuleConstExp;
}

void SysYParser::ConstExpContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterConstExp(this);
}

void SysYParser::ConstExpContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitConstExp(this);
}


std::any SysYParser::ConstExpContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SysYVisitor*>(visitor))
    return parserVisitor->visitConstExp(this);
  else
    return visitor->visitChildren(this);
}

SysYParser::ConstExpContext* SysYParser::constExp() {
  ConstExpContext *_localctx = _tracker.createInstance<ConstExpContext>(_ctx, getState());
  enterRule(_localctx, 10, SysYParser::RuleConstExp);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(140);
    addExp(0);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ConstInitValContext ------------------------------------------------------------------

SysYParser::ConstInitValContext::ConstInitValContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t SysYParser::ConstInitValContext::getRuleIndex() const {
  return SysYParser::RuleConstInitVal;
}

void SysYParser::ConstInitValContext::copyFrom(ConstInitValContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- ConstListInitContext ------------------------------------------------------------------

std::vector<SysYParser::ConstInitValContext *> SysYParser::ConstListInitContext::constInitVal() {
  return getRuleContexts<SysYParser::ConstInitValContext>();
}

SysYParser::ConstInitValContext* SysYParser::ConstListInitContext::constInitVal(size_t i) {
  return getRuleContext<SysYParser::ConstInitValContext>(i);
}

SysYParser::ConstListInitContext::ConstListInitContext(ConstInitValContext *ctx) { copyFrom(ctx); }

void SysYParser::ConstListInitContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterConstListInit(this);
}
void SysYParser::ConstListInitContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitConstListInit(this);
}

std::any SysYParser::ConstListInitContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SysYVisitor*>(visitor))
    return parserVisitor->visitConstListInit(this);
  else
    return visitor->visitChildren(this);
}
//----------------- ConstInitContext ------------------------------------------------------------------

SysYParser::ConstExpContext* SysYParser::ConstInitContext::constExp() {
  return getRuleContext<SysYParser::ConstExpContext>(0);
}

SysYParser::ConstInitContext::ConstInitContext(ConstInitValContext *ctx) { copyFrom(ctx); }

void SysYParser::ConstInitContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterConstInit(this);
}
void SysYParser::ConstInitContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitConstInit(this);
}

std::any SysYParser::ConstInitContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SysYVisitor*>(visitor))
    return parserVisitor->visitConstInit(this);
  else
    return visitor->visitChildren(this);
}
SysYParser::ConstInitValContext* SysYParser::constInitVal() {
  ConstInitValContext *_localctx = _tracker.createInstance<ConstInitValContext>(_ctx, getState());
  enterRule(_localctx, 12, SysYParser::RuleConstInitVal);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(155);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case SysYParser::T__11:
      case SysYParser::T__19:
      case SysYParser::T__20:
      case SysYParser::T__21:
      case SysYParser::Ident:
      case SysYParser::IntConst:
      case SysYParser::FloatConst: {
        _localctx = _tracker.createInstance<SysYParser::ConstInitContext>(_localctx);
        enterOuterAlt(_localctx, 1);
        setState(142);
        constExp();
        break;
      }

      case SysYParser::T__6: {
        _localctx = _tracker.createInstance<SysYParser::ConstListInitContext>(_localctx);
        enterOuterAlt(_localctx, 2);
        setState(143);
        match(SysYParser::T__6);
        setState(152);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if ((((_la & ~ 0x3fULL) == 0) &&
          ((1ULL << _la) & 120266428544) != 0)) {
          setState(144);
          constInitVal();
          setState(149);
          _errHandler->sync(this);
          _la = _input->LA(1);
          while (_la == SysYParser::T__1) {
            setState(145);
            match(SysYParser::T__1);
            setState(146);
            constInitVal();
            setState(151);
            _errHandler->sync(this);
            _la = _input->LA(1);
          }
        }
        setState(154);
        match(SysYParser::T__7);
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- TypeContext ------------------------------------------------------------------

SysYParser::TypeContext::TypeContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t SysYParser::TypeContext::getRuleIndex() const {
  return SysYParser::RuleType;
}

void SysYParser::TypeContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterType(this);
}

void SysYParser::TypeContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitType(this);
}


std::any SysYParser::TypeContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SysYVisitor*>(visitor))
    return parserVisitor->visitType(this);
  else
    return visitor->visitChildren(this);
}

SysYParser::TypeContext* SysYParser::type() {
  TypeContext *_localctx = _tracker.createInstance<TypeContext>(_ctx, getState());
  enterRule(_localctx, 14, SysYParser::RuleType);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(157);
    _la = _input->LA(1);
    if (!((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 3584) != 0))) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- FuncDefContext ------------------------------------------------------------------

SysYParser::FuncDefContext::FuncDefContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SysYParser::TypeContext* SysYParser::FuncDefContext::type() {
  return getRuleContext<SysYParser::TypeContext>(0);
}

tree::TerminalNode* SysYParser::FuncDefContext::Ident() {
  return getToken(SysYParser::Ident, 0);
}

SysYParser::BlockContext* SysYParser::FuncDefContext::block() {
  return getRuleContext<SysYParser::BlockContext>(0);
}

SysYParser::FuncFParamsContext* SysYParser::FuncDefContext::funcFParams() {
  return getRuleContext<SysYParser::FuncFParamsContext>(0);
}


size_t SysYParser::FuncDefContext::getRuleIndex() const {
  return SysYParser::RuleFuncDef;
}

void SysYParser::FuncDefContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterFuncDef(this);
}

void SysYParser::FuncDefContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitFuncDef(this);
}


std::any SysYParser::FuncDefContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SysYVisitor*>(visitor))
    return parserVisitor->visitFuncDef(this);
  else
    return visitor->visitChildren(this);
}

SysYParser::FuncDefContext* SysYParser::funcDef() {
  FuncDefContext *_localctx = _tracker.createInstance<FuncDefContext>(_ctx, getState());
  enterRule(_localctx, 16, SysYParser::RuleFuncDef);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(159);
    type();
    setState(160);
    match(SysYParser::Ident);
    setState(161);
    match(SysYParser::T__11);
    setState(163);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 3584) != 0)) {
      setState(162);
      funcFParams();
    }
    setState(165);
    match(SysYParser::T__12);
    setState(166);
    block();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- FuncFParamsContext ------------------------------------------------------------------

SysYParser::FuncFParamsContext::FuncFParamsContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<SysYParser::FuncFParamContext *> SysYParser::FuncFParamsContext::funcFParam() {
  return getRuleContexts<SysYParser::FuncFParamContext>();
}

SysYParser::FuncFParamContext* SysYParser::FuncFParamsContext::funcFParam(size_t i) {
  return getRuleContext<SysYParser::FuncFParamContext>(i);
}


size_t SysYParser::FuncFParamsContext::getRuleIndex() const {
  return SysYParser::RuleFuncFParams;
}

void SysYParser::FuncFParamsContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterFuncFParams(this);
}

void SysYParser::FuncFParamsContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitFuncFParams(this);
}


std::any SysYParser::FuncFParamsContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SysYVisitor*>(visitor))
    return parserVisitor->visitFuncFParams(this);
  else
    return visitor->visitChildren(this);
}

SysYParser::FuncFParamsContext* SysYParser::funcFParams() {
  FuncFParamsContext *_localctx = _tracker.createInstance<FuncFParamsContext>(_ctx, getState());
  enterRule(_localctx, 18, SysYParser::RuleFuncFParams);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(168);
    funcFParam();
    setState(173);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == SysYParser::T__1) {
      setState(169);
      match(SysYParser::T__1);
      setState(170);
      funcFParam();
      setState(175);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- FuncFParamContext ------------------------------------------------------------------

SysYParser::FuncFParamContext::FuncFParamContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SysYParser::TypeContext* SysYParser::FuncFParamContext::type() {
  return getRuleContext<SysYParser::TypeContext>(0);
}

tree::TerminalNode* SysYParser::FuncFParamContext::Ident() {
  return getToken(SysYParser::Ident, 0);
}

std::vector<SysYParser::ExpContext *> SysYParser::FuncFParamContext::exp() {
  return getRuleContexts<SysYParser::ExpContext>();
}

SysYParser::ExpContext* SysYParser::FuncFParamContext::exp(size_t i) {
  return getRuleContext<SysYParser::ExpContext>(i);
}


size_t SysYParser::FuncFParamContext::getRuleIndex() const {
  return SysYParser::RuleFuncFParam;
}

void SysYParser::FuncFParamContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterFuncFParam(this);
}

void SysYParser::FuncFParamContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitFuncFParam(this);
}


std::any SysYParser::FuncFParamContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SysYVisitor*>(visitor))
    return parserVisitor->visitFuncFParam(this);
  else
    return visitor->visitChildren(this);
}

SysYParser::FuncFParamContext* SysYParser::funcFParam() {
  FuncFParamContext *_localctx = _tracker.createInstance<FuncFParamContext>(_ctx, getState());
  enterRule(_localctx, 20, SysYParser::RuleFuncFParam);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(176);
    type();
    setState(177);
    match(SysYParser::Ident);
    setState(189);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == SysYParser::T__3) {
      setState(178);
      match(SysYParser::T__3);
      setState(179);
      match(SysYParser::T__4);
      setState(186);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == SysYParser::T__3) {
        setState(180);
        match(SysYParser::T__3);
        setState(181);
        exp();
        setState(182);
        match(SysYParser::T__4);
        setState(188);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- BlockContext ------------------------------------------------------------------

SysYParser::BlockContext::BlockContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<SysYParser::BlockItemContext *> SysYParser::BlockContext::blockItem() {
  return getRuleContexts<SysYParser::BlockItemContext>();
}

SysYParser::BlockItemContext* SysYParser::BlockContext::blockItem(size_t i) {
  return getRuleContext<SysYParser::BlockItemContext>(i);
}


size_t SysYParser::BlockContext::getRuleIndex() const {
  return SysYParser::RuleBlock;
}

void SysYParser::BlockContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterBlock(this);
}

void SysYParser::BlockContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitBlock(this);
}


std::any SysYParser::BlockContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SysYVisitor*>(visitor))
    return parserVisitor->visitBlock(this);
  else
    return visitor->visitChildren(this);
}

SysYParser::BlockContext* SysYParser::block() {
  BlockContext *_localctx = _tracker.createInstance<BlockContext>(_ctx, getState());
  enterRule(_localctx, 22, SysYParser::RuleBlock);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(191);
    match(SysYParser::T__6);
    setState(195);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 120267431562) != 0)) {
      setState(192);
      blockItem();
      setState(197);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(198);
    match(SysYParser::T__7);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- BlockItemContext ------------------------------------------------------------------

SysYParser::BlockItemContext::BlockItemContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SysYParser::DeclContext* SysYParser::BlockItemContext::decl() {
  return getRuleContext<SysYParser::DeclContext>(0);
}

SysYParser::StmtContext* SysYParser::BlockItemContext::stmt() {
  return getRuleContext<SysYParser::StmtContext>(0);
}


size_t SysYParser::BlockItemContext::getRuleIndex() const {
  return SysYParser::RuleBlockItem;
}

void SysYParser::BlockItemContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterBlockItem(this);
}

void SysYParser::BlockItemContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitBlockItem(this);
}


std::any SysYParser::BlockItemContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SysYVisitor*>(visitor))
    return parserVisitor->visitBlockItem(this);
  else
    return visitor->visitChildren(this);
}

SysYParser::BlockItemContext* SysYParser::blockItem() {
  BlockItemContext *_localctx = _tracker.createInstance<BlockItemContext>(_ctx, getState());
  enterRule(_localctx, 24, SysYParser::RuleBlockItem);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(202);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case SysYParser::T__0:
      case SysYParser::T__8:
      case SysYParser::T__9:
      case SysYParser::T__10: {
        enterOuterAlt(_localctx, 1);
        setState(200);
        decl();
        break;
      }

      case SysYParser::T__2:
      case SysYParser::T__6:
      case SysYParser::T__11:
      case SysYParser::T__13:
      case SysYParser::T__15:
      case SysYParser::T__16:
      case SysYParser::T__17:
      case SysYParser::T__18:
      case SysYParser::T__19:
      case SysYParser::T__20:
      case SysYParser::T__21:
      case SysYParser::Ident:
      case SysYParser::IntConst:
      case SysYParser::FloatConst: {
        enterOuterAlt(_localctx, 2);
        setState(201);
        stmt();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- StmtContext ------------------------------------------------------------------

SysYParser::StmtContext::StmtContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t SysYParser::StmtContext::getRuleIndex() const {
  return SysYParser::RuleStmt;
}

void SysYParser::StmtContext::copyFrom(StmtContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- ReturnContext ------------------------------------------------------------------

SysYParser::ExpContext* SysYParser::ReturnContext::exp() {
  return getRuleContext<SysYParser::ExpContext>(0);
}

SysYParser::ReturnContext::ReturnContext(StmtContext *ctx) { copyFrom(ctx); }

void SysYParser::ReturnContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterReturn(this);
}
void SysYParser::ReturnContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitReturn(this);
}

std::any SysYParser::ReturnContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SysYVisitor*>(visitor))
    return parserVisitor->visitReturn(this);
  else
    return visitor->visitChildren(this);
}
//----------------- ExprContext ------------------------------------------------------------------

SysYParser::ExpContext* SysYParser::ExprContext::exp() {
  return getRuleContext<SysYParser::ExpContext>(0);
}

SysYParser::ExprContext::ExprContext(StmtContext *ctx) { copyFrom(ctx); }

void SysYParser::ExprContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterExpr(this);
}
void SysYParser::ExprContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitExpr(this);
}

std::any SysYParser::ExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SysYVisitor*>(visitor))
    return parserVisitor->visitExpr(this);
  else
    return visitor->visitChildren(this);
}
//----------------- BlockStmtContext ------------------------------------------------------------------

SysYParser::BlockContext* SysYParser::BlockStmtContext::block() {
  return getRuleContext<SysYParser::BlockContext>(0);
}

SysYParser::BlockStmtContext::BlockStmtContext(StmtContext *ctx) { copyFrom(ctx); }

void SysYParser::BlockStmtContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterBlockStmt(this);
}
void SysYParser::BlockStmtContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitBlockStmt(this);
}

std::any SysYParser::BlockStmtContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SysYVisitor*>(visitor))
    return parserVisitor->visitBlockStmt(this);
  else
    return visitor->visitChildren(this);
}
//----------------- BreakContext ------------------------------------------------------------------

SysYParser::BreakContext::BreakContext(StmtContext *ctx) { copyFrom(ctx); }

void SysYParser::BreakContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterBreak(this);
}
void SysYParser::BreakContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitBreak(this);
}

std::any SysYParser::BreakContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SysYVisitor*>(visitor))
    return parserVisitor->visitBreak(this);
  else
    return visitor->visitChildren(this);
}
//----------------- AssignContext ------------------------------------------------------------------

SysYParser::LValContext* SysYParser::AssignContext::lVal() {
  return getRuleContext<SysYParser::LValContext>(0);
}

SysYParser::ExpContext* SysYParser::AssignContext::exp() {
  return getRuleContext<SysYParser::ExpContext>(0);
}

SysYParser::AssignContext::AssignContext(StmtContext *ctx) { copyFrom(ctx); }

void SysYParser::AssignContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterAssign(this);
}
void SysYParser::AssignContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitAssign(this);
}

std::any SysYParser::AssignContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SysYVisitor*>(visitor))
    return parserVisitor->visitAssign(this);
  else
    return visitor->visitChildren(this);
}
//----------------- WhileContext ------------------------------------------------------------------

SysYParser::CondContext* SysYParser::WhileContext::cond() {
  return getRuleContext<SysYParser::CondContext>(0);
}

SysYParser::StmtContext* SysYParser::WhileContext::stmt() {
  return getRuleContext<SysYParser::StmtContext>(0);
}

SysYParser::WhileContext::WhileContext(StmtContext *ctx) { copyFrom(ctx); }

void SysYParser::WhileContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterWhile(this);
}
void SysYParser::WhileContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitWhile(this);
}

std::any SysYParser::WhileContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SysYVisitor*>(visitor))
    return parserVisitor->visitWhile(this);
  else
    return visitor->visitChildren(this);
}
//----------------- ContinueContext ------------------------------------------------------------------

SysYParser::ContinueContext::ContinueContext(StmtContext *ctx) { copyFrom(ctx); }

void SysYParser::ContinueContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterContinue(this);
}
void SysYParser::ContinueContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitContinue(this);
}

std::any SysYParser::ContinueContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SysYVisitor*>(visitor))
    return parserVisitor->visitContinue(this);
  else
    return visitor->visitChildren(this);
}
//----------------- IfContext ------------------------------------------------------------------

SysYParser::CondContext* SysYParser::IfContext::cond() {
  return getRuleContext<SysYParser::CondContext>(0);
}

std::vector<SysYParser::StmtContext *> SysYParser::IfContext::stmt() {
  return getRuleContexts<SysYParser::StmtContext>();
}

SysYParser::StmtContext* SysYParser::IfContext::stmt(size_t i) {
  return getRuleContext<SysYParser::StmtContext>(i);
}

SysYParser::IfContext::IfContext(StmtContext *ctx) { copyFrom(ctx); }

void SysYParser::IfContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterIf(this);
}
void SysYParser::IfContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitIf(this);
}

std::any SysYParser::IfContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SysYVisitor*>(visitor))
    return parserVisitor->visitIf(this);
  else
    return visitor->visitChildren(this);
}
SysYParser::StmtContext* SysYParser::stmt() {
  StmtContext *_localctx = _tracker.createInstance<StmtContext>(_ctx, getState());
  enterRule(_localctx, 26, SysYParser::RuleStmt);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(238);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 24, _ctx)) {
    case 1: {
      _localctx = _tracker.createInstance<SysYParser::AssignContext>(_localctx);
      enterOuterAlt(_localctx, 1);
      setState(204);
      lVal();
      setState(205);
      match(SysYParser::T__5);
      setState(206);
      exp();
      setState(207);
      match(SysYParser::T__2);
      break;
    }

    case 2: {
      _localctx = _tracker.createInstance<SysYParser::ExprContext>(_localctx);
      enterOuterAlt(_localctx, 2);
      setState(210);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if ((((_la & ~ 0x3fULL) == 0) &&
        ((1ULL << _la) & 120266428416) != 0)) {
        setState(209);
        exp();
      }
      setState(212);
      match(SysYParser::T__2);
      break;
    }

    case 3: {
      _localctx = _tracker.createInstance<SysYParser::BlockStmtContext>(_localctx);
      enterOuterAlt(_localctx, 3);
      setState(213);
      block();
      break;
    }

    case 4: {
      _localctx = _tracker.createInstance<SysYParser::IfContext>(_localctx);
      enterOuterAlt(_localctx, 4);
      setState(214);
      match(SysYParser::T__13);
      setState(215);
      match(SysYParser::T__11);
      setState(216);
      cond();
      setState(217);
      match(SysYParser::T__12);
      setState(218);
      stmt();
      setState(221);
      _errHandler->sync(this);

      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 22, _ctx)) {
      case 1: {
        setState(219);
        match(SysYParser::T__14);
        setState(220);
        stmt();
        break;
      }

      default:
        break;
      }
      break;
    }

    case 5: {
      _localctx = _tracker.createInstance<SysYParser::WhileContext>(_localctx);
      enterOuterAlt(_localctx, 5);
      setState(223);
      match(SysYParser::T__15);
      setState(224);
      match(SysYParser::T__11);
      setState(225);
      cond();
      setState(226);
      match(SysYParser::T__12);
      setState(227);
      stmt();
      break;
    }

    case 6: {
      _localctx = _tracker.createInstance<SysYParser::BreakContext>(_localctx);
      enterOuterAlt(_localctx, 6);
      setState(229);
      match(SysYParser::T__16);
      setState(230);
      match(SysYParser::T__2);
      break;
    }

    case 7: {
      _localctx = _tracker.createInstance<SysYParser::ContinueContext>(_localctx);
      enterOuterAlt(_localctx, 7);
      setState(231);
      match(SysYParser::T__17);
      setState(232);
      match(SysYParser::T__2);
      break;
    }

    case 8: {
      _localctx = _tracker.createInstance<SysYParser::ReturnContext>(_localctx);
      enterOuterAlt(_localctx, 8);
      setState(233);
      match(SysYParser::T__18);
      setState(235);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if ((((_la & ~ 0x3fULL) == 0) &&
        ((1ULL << _la) & 120266428416) != 0)) {
        setState(234);
        exp();
      }
      setState(237);
      match(SysYParser::T__2);
      break;
    }

    default:
      break;
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ExpContext ------------------------------------------------------------------

SysYParser::ExpContext::ExpContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SysYParser::AddExpContext* SysYParser::ExpContext::addExp() {
  return getRuleContext<SysYParser::AddExpContext>(0);
}


size_t SysYParser::ExpContext::getRuleIndex() const {
  return SysYParser::RuleExp;
}

void SysYParser::ExpContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterExp(this);
}

void SysYParser::ExpContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitExp(this);
}


std::any SysYParser::ExpContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SysYVisitor*>(visitor))
    return parserVisitor->visitExp(this);
  else
    return visitor->visitChildren(this);
}

SysYParser::ExpContext* SysYParser::exp() {
  ExpContext *_localctx = _tracker.createInstance<ExpContext>(_ctx, getState());
  enterRule(_localctx, 28, SysYParser::RuleExp);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(240);
    addExp(0);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- CondContext ------------------------------------------------------------------

SysYParser::CondContext::CondContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SysYParser::LOrExpContext* SysYParser::CondContext::lOrExp() {
  return getRuleContext<SysYParser::LOrExpContext>(0);
}


size_t SysYParser::CondContext::getRuleIndex() const {
  return SysYParser::RuleCond;
}

void SysYParser::CondContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterCond(this);
}

void SysYParser::CondContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitCond(this);
}


std::any SysYParser::CondContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SysYVisitor*>(visitor))
    return parserVisitor->visitCond(this);
  else
    return visitor->visitChildren(this);
}

SysYParser::CondContext* SysYParser::cond() {
  CondContext *_localctx = _tracker.createInstance<CondContext>(_ctx, getState());
  enterRule(_localctx, 30, SysYParser::RuleCond);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(242);
    lOrExp(0);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- LValContext ------------------------------------------------------------------

SysYParser::LValContext::LValContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SysYParser::LValContext::Ident() {
  return getToken(SysYParser::Ident, 0);
}

std::vector<SysYParser::ExpContext *> SysYParser::LValContext::exp() {
  return getRuleContexts<SysYParser::ExpContext>();
}

SysYParser::ExpContext* SysYParser::LValContext::exp(size_t i) {
  return getRuleContext<SysYParser::ExpContext>(i);
}


size_t SysYParser::LValContext::getRuleIndex() const {
  return SysYParser::RuleLVal;
}

void SysYParser::LValContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterLVal(this);
}

void SysYParser::LValContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitLVal(this);
}


std::any SysYParser::LValContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SysYVisitor*>(visitor))
    return parserVisitor->visitLVal(this);
  else
    return visitor->visitChildren(this);
}

SysYParser::LValContext* SysYParser::lVal() {
  LValContext *_localctx = _tracker.createInstance<LValContext>(_ctx, getState());
  enterRule(_localctx, 32, SysYParser::RuleLVal);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(244);
    match(SysYParser::Ident);
    setState(251);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 25, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        setState(245);
        match(SysYParser::T__3);
        setState(246);
        exp();
        setState(247);
        match(SysYParser::T__4); 
      }
      setState(253);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 25, _ctx);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- PrimaryExpContext ------------------------------------------------------------------

SysYParser::PrimaryExpContext::PrimaryExpContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t SysYParser::PrimaryExpContext::getRuleIndex() const {
  return SysYParser::RulePrimaryExp;
}

void SysYParser::PrimaryExpContext::copyFrom(PrimaryExpContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- ClosedExprContext ------------------------------------------------------------------

SysYParser::ExpContext* SysYParser::ClosedExprContext::exp() {
  return getRuleContext<SysYParser::ExpContext>(0);
}

SysYParser::ClosedExprContext::ClosedExprContext(PrimaryExpContext *ctx) { copyFrom(ctx); }

void SysYParser::ClosedExprContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterClosedExpr(this);
}
void SysYParser::ClosedExprContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitClosedExpr(this);
}

std::any SysYParser::ClosedExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SysYVisitor*>(visitor))
    return parserVisitor->visitClosedExpr(this);
  else
    return visitor->visitChildren(this);
}
//----------------- LValValueContext ------------------------------------------------------------------

SysYParser::LValContext* SysYParser::LValValueContext::lVal() {
  return getRuleContext<SysYParser::LValContext>(0);
}

SysYParser::LValValueContext::LValValueContext(PrimaryExpContext *ctx) { copyFrom(ctx); }

void SysYParser::LValValueContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterLValValue(this);
}
void SysYParser::LValValueContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitLValValue(this);
}

std::any SysYParser::LValValueContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SysYVisitor*>(visitor))
    return parserVisitor->visitLValValue(this);
  else
    return visitor->visitChildren(this);
}
//----------------- LiteralValueContext ------------------------------------------------------------------

SysYParser::NumberContext* SysYParser::LiteralValueContext::number() {
  return getRuleContext<SysYParser::NumberContext>(0);
}

SysYParser::LiteralValueContext::LiteralValueContext(PrimaryExpContext *ctx) { copyFrom(ctx); }

void SysYParser::LiteralValueContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterLiteralValue(this);
}
void SysYParser::LiteralValueContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitLiteralValue(this);
}

std::any SysYParser::LiteralValueContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SysYVisitor*>(visitor))
    return parserVisitor->visitLiteralValue(this);
  else
    return visitor->visitChildren(this);
}
SysYParser::PrimaryExpContext* SysYParser::primaryExp() {
  PrimaryExpContext *_localctx = _tracker.createInstance<PrimaryExpContext>(_ctx, getState());
  enterRule(_localctx, 34, SysYParser::RulePrimaryExp);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(260);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case SysYParser::T__11: {
        _localctx = _tracker.createInstance<SysYParser::ClosedExprContext>(_localctx);
        enterOuterAlt(_localctx, 1);
        setState(254);
        match(SysYParser::T__11);
        setState(255);
        exp();
        setState(256);
        match(SysYParser::T__12);
        break;
      }

      case SysYParser::Ident: {
        _localctx = _tracker.createInstance<SysYParser::LValValueContext>(_localctx);
        enterOuterAlt(_localctx, 2);
        setState(258);
        lVal();
        break;
      }

      case SysYParser::IntConst:
      case SysYParser::FloatConst: {
        _localctx = _tracker.createInstance<SysYParser::LiteralValueContext>(_localctx);
        enterOuterAlt(_localctx, 3);
        setState(259);
        number();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- NumberContext ------------------------------------------------------------------

SysYParser::NumberContext::NumberContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t SysYParser::NumberContext::getRuleIndex() const {
  return SysYParser::RuleNumber;
}

void SysYParser::NumberContext::copyFrom(NumberContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- FloatNumberContext ------------------------------------------------------------------

tree::TerminalNode* SysYParser::FloatNumberContext::FloatConst() {
  return getToken(SysYParser::FloatConst, 0);
}

SysYParser::FloatNumberContext::FloatNumberContext(NumberContext *ctx) { copyFrom(ctx); }

void SysYParser::FloatNumberContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterFloatNumber(this);
}
void SysYParser::FloatNumberContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitFloatNumber(this);
}

std::any SysYParser::FloatNumberContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SysYVisitor*>(visitor))
    return parserVisitor->visitFloatNumber(this);
  else
    return visitor->visitChildren(this);
}
//----------------- IntNumberContext ------------------------------------------------------------------

tree::TerminalNode* SysYParser::IntNumberContext::IntConst() {
  return getToken(SysYParser::IntConst, 0);
}

SysYParser::IntNumberContext::IntNumberContext(NumberContext *ctx) { copyFrom(ctx); }

void SysYParser::IntNumberContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterIntNumber(this);
}
void SysYParser::IntNumberContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitIntNumber(this);
}

std::any SysYParser::IntNumberContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SysYVisitor*>(visitor))
    return parserVisitor->visitIntNumber(this);
  else
    return visitor->visitChildren(this);
}
SysYParser::NumberContext* SysYParser::number() {
  NumberContext *_localctx = _tracker.createInstance<NumberContext>(_ctx, getState());
  enterRule(_localctx, 36, SysYParser::RuleNumber);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(264);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case SysYParser::IntConst: {
        _localctx = _tracker.createInstance<SysYParser::IntNumberContext>(_localctx);
        enterOuterAlt(_localctx, 1);
        setState(262);
        match(SysYParser::IntConst);
        break;
      }

      case SysYParser::FloatConst: {
        _localctx = _tracker.createInstance<SysYParser::FloatNumberContext>(_localctx);
        enterOuterAlt(_localctx, 2);
        setState(263);
        match(SysYParser::FloatConst);
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- UnaryExpContext ------------------------------------------------------------------

SysYParser::UnaryExpContext::UnaryExpContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t SysYParser::UnaryExpContext::getRuleIndex() const {
  return SysYParser::RuleUnaryExp;
}

void SysYParser::UnaryExpContext::copyFrom(UnaryExpContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- CallContext ------------------------------------------------------------------

tree::TerminalNode* SysYParser::CallContext::Ident() {
  return getToken(SysYParser::Ident, 0);
}

SysYParser::FuncRParamsContext* SysYParser::CallContext::funcRParams() {
  return getRuleContext<SysYParser::FuncRParamsContext>(0);
}

SysYParser::CallContext::CallContext(UnaryExpContext *ctx) { copyFrom(ctx); }

void SysYParser::CallContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterCall(this);
}
void SysYParser::CallContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitCall(this);
}

std::any SysYParser::CallContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SysYVisitor*>(visitor))
    return parserVisitor->visitCall(this);
  else
    return visitor->visitChildren(this);
}
//----------------- FromPrimaryContext ------------------------------------------------------------------

SysYParser::PrimaryExpContext* SysYParser::FromPrimaryContext::primaryExp() {
  return getRuleContext<SysYParser::PrimaryExpContext>(0);
}

SysYParser::FromPrimaryContext::FromPrimaryContext(UnaryExpContext *ctx) { copyFrom(ctx); }

void SysYParser::FromPrimaryContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterFromPrimary(this);
}
void SysYParser::FromPrimaryContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitFromPrimary(this);
}

std::any SysYParser::FromPrimaryContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SysYVisitor*>(visitor))
    return parserVisitor->visitFromPrimary(this);
  else
    return visitor->visitChildren(this);
}
//----------------- OpUnaryContext ------------------------------------------------------------------

SysYParser::UnaryOPContext* SysYParser::OpUnaryContext::unaryOP() {
  return getRuleContext<SysYParser::UnaryOPContext>(0);
}

SysYParser::UnaryExpContext* SysYParser::OpUnaryContext::unaryExp() {
  return getRuleContext<SysYParser::UnaryExpContext>(0);
}

SysYParser::OpUnaryContext::OpUnaryContext(UnaryExpContext *ctx) { copyFrom(ctx); }

void SysYParser::OpUnaryContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterOpUnary(this);
}
void SysYParser::OpUnaryContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitOpUnary(this);
}

std::any SysYParser::OpUnaryContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SysYVisitor*>(visitor))
    return parserVisitor->visitOpUnary(this);
  else
    return visitor->visitChildren(this);
}
SysYParser::UnaryExpContext* SysYParser::unaryExp() {
  UnaryExpContext *_localctx = _tracker.createInstance<UnaryExpContext>(_ctx, getState());
  enterRule(_localctx, 38, SysYParser::RuleUnaryExp);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(276);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 29, _ctx)) {
    case 1: {
      _localctx = _tracker.createInstance<SysYParser::FromPrimaryContext>(_localctx);
      enterOuterAlt(_localctx, 1);
      setState(266);
      primaryExp();
      break;
    }

    case 2: {
      _localctx = _tracker.createInstance<SysYParser::CallContext>(_localctx);
      enterOuterAlt(_localctx, 2);
      setState(267);
      match(SysYParser::Ident);
      setState(268);
      match(SysYParser::T__11);
      setState(270);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if ((((_la & ~ 0x3fULL) == 0) &&
        ((1ULL << _la) & 120266428416) != 0)) {
        setState(269);
        funcRParams();
      }
      setState(272);
      match(SysYParser::T__12);
      break;
    }

    case 3: {
      _localctx = _tracker.createInstance<SysYParser::OpUnaryContext>(_localctx);
      enterOuterAlt(_localctx, 3);
      setState(273);
      unaryOP();
      setState(274);
      unaryExp();
      break;
    }

    default:
      break;
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- UnaryOPContext ------------------------------------------------------------------

SysYParser::UnaryOPContext::UnaryOPContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t SysYParser::UnaryOPContext::getRuleIndex() const {
  return SysYParser::RuleUnaryOP;
}

void SysYParser::UnaryOPContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterUnaryOP(this);
}

void SysYParser::UnaryOPContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitUnaryOP(this);
}


std::any SysYParser::UnaryOPContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SysYVisitor*>(visitor))
    return parserVisitor->visitUnaryOP(this);
  else
    return visitor->visitChildren(this);
}

SysYParser::UnaryOPContext* SysYParser::unaryOP() {
  UnaryOPContext *_localctx = _tracker.createInstance<UnaryOPContext>(_ctx, getState());
  enterRule(_localctx, 40, SysYParser::RuleUnaryOP);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(278);
    _la = _input->LA(1);
    if (!((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 7340032) != 0))) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- FuncRParamsContext ------------------------------------------------------------------

SysYParser::FuncRParamsContext::FuncRParamsContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<SysYParser::ExpContext *> SysYParser::FuncRParamsContext::exp() {
  return getRuleContexts<SysYParser::ExpContext>();
}

SysYParser::ExpContext* SysYParser::FuncRParamsContext::exp(size_t i) {
  return getRuleContext<SysYParser::ExpContext>(i);
}


size_t SysYParser::FuncRParamsContext::getRuleIndex() const {
  return SysYParser::RuleFuncRParams;
}

void SysYParser::FuncRParamsContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterFuncRParams(this);
}

void SysYParser::FuncRParamsContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitFuncRParams(this);
}


std::any SysYParser::FuncRParamsContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SysYVisitor*>(visitor))
    return parserVisitor->visitFuncRParams(this);
  else
    return visitor->visitChildren(this);
}

SysYParser::FuncRParamsContext* SysYParser::funcRParams() {
  FuncRParamsContext *_localctx = _tracker.createInstance<FuncRParamsContext>(_ctx, getState());
  enterRule(_localctx, 42, SysYParser::RuleFuncRParams);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(280);
    exp();
    setState(285);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == SysYParser::T__1) {
      setState(281);
      match(SysYParser::T__1);
      setState(282);
      exp();
      setState(287);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- MulExpContext ------------------------------------------------------------------

SysYParser::MulExpContext::MulExpContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t SysYParser::MulExpContext::getRuleIndex() const {
  return SysYParser::RuleMulExp;
}

void SysYParser::MulExpContext::copyFrom(MulExpContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- MulOpContext ------------------------------------------------------------------

SysYParser::MulExpContext* SysYParser::MulOpContext::mulExp() {
  return getRuleContext<SysYParser::MulExpContext>(0);
}

SysYParser::UnaryExpContext* SysYParser::MulOpContext::unaryExp() {
  return getRuleContext<SysYParser::UnaryExpContext>(0);
}

SysYParser::MulOpContext::MulOpContext(MulExpContext *ctx) { copyFrom(ctx); }

void SysYParser::MulOpContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterMulOp(this);
}
void SysYParser::MulOpContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitMulOp(this);
}

std::any SysYParser::MulOpContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SysYVisitor*>(visitor))
    return parserVisitor->visitMulOp(this);
  else
    return visitor->visitChildren(this);
}
//----------------- FromUnaryContext ------------------------------------------------------------------

SysYParser::UnaryExpContext* SysYParser::FromUnaryContext::unaryExp() {
  return getRuleContext<SysYParser::UnaryExpContext>(0);
}

SysYParser::FromUnaryContext::FromUnaryContext(MulExpContext *ctx) { copyFrom(ctx); }

void SysYParser::FromUnaryContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterFromUnary(this);
}
void SysYParser::FromUnaryContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitFromUnary(this);
}

std::any SysYParser::FromUnaryContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SysYVisitor*>(visitor))
    return parserVisitor->visitFromUnary(this);
  else
    return visitor->visitChildren(this);
}

SysYParser::MulExpContext* SysYParser::mulExp() {
   return mulExp(0);
}

SysYParser::MulExpContext* SysYParser::mulExp(int precedence) {
  ParserRuleContext *parentContext = _ctx;
  size_t parentState = getState();
  SysYParser::MulExpContext *_localctx = _tracker.createInstance<MulExpContext>(_ctx, parentState);
  SysYParser::MulExpContext *previousContext = _localctx;
  (void)previousContext; // Silence compiler, in case the context is not used by generated code.
  size_t startState = 44;
  enterRecursionRule(_localctx, 44, SysYParser::RuleMulExp, precedence);

    size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    unrollRecursionContexts(parentContext);
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    _localctx = _tracker.createInstance<FromUnaryContext>(_localctx);
    _ctx = _localctx;
    previousContext = _localctx;

    setState(289);
    unaryExp();
    _ctx->stop = _input->LT(-1);
    setState(296);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 31, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        if (!_parseListeners.empty())
          triggerExitRuleEvent();
        previousContext = _localctx;
        auto newContext = _tracker.createInstance<MulOpContext>(_tracker.createInstance<MulExpContext>(parentContext, parentState));
        _localctx = newContext;
        pushNewRecursionContext(newContext, startState, RuleMulExp);
        setState(291);

        if (!(precpred(_ctx, 1))) throw FailedPredicateException(this, "precpred(_ctx, 1)");
        setState(292);
        antlrcpp::downCast<MulOpContext *>(_localctx)->op = _input->LT(1);
        _la = _input->LA(1);
        if (!((((_la & ~ 0x3fULL) == 0) &&
          ((1ULL << _la) & 58720256) != 0))) {
          antlrcpp::downCast<MulOpContext *>(_localctx)->op = _errHandler->recoverInline(this);
        }
        else {
          _errHandler->reportMatch(this);
          consume();
        }
        setState(293);
        unaryExp(); 
      }
      setState(298);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 31, _ctx);
    }
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }
  return _localctx;
}

//----------------- AddExpContext ------------------------------------------------------------------

SysYParser::AddExpContext::AddExpContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t SysYParser::AddExpContext::getRuleIndex() const {
  return SysYParser::RuleAddExp;
}

void SysYParser::AddExpContext::copyFrom(AddExpContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- AddOpContext ------------------------------------------------------------------

SysYParser::AddExpContext* SysYParser::AddOpContext::addExp() {
  return getRuleContext<SysYParser::AddExpContext>(0);
}

SysYParser::MulExpContext* SysYParser::AddOpContext::mulExp() {
  return getRuleContext<SysYParser::MulExpContext>(0);
}

SysYParser::AddOpContext::AddOpContext(AddExpContext *ctx) { copyFrom(ctx); }

void SysYParser::AddOpContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterAddOp(this);
}
void SysYParser::AddOpContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitAddOp(this);
}

std::any SysYParser::AddOpContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SysYVisitor*>(visitor))
    return parserVisitor->visitAddOp(this);
  else
    return visitor->visitChildren(this);
}
//----------------- FromMulContext ------------------------------------------------------------------

SysYParser::MulExpContext* SysYParser::FromMulContext::mulExp() {
  return getRuleContext<SysYParser::MulExpContext>(0);
}

SysYParser::FromMulContext::FromMulContext(AddExpContext *ctx) { copyFrom(ctx); }

void SysYParser::FromMulContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterFromMul(this);
}
void SysYParser::FromMulContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitFromMul(this);
}

std::any SysYParser::FromMulContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SysYVisitor*>(visitor))
    return parserVisitor->visitFromMul(this);
  else
    return visitor->visitChildren(this);
}

SysYParser::AddExpContext* SysYParser::addExp() {
   return addExp(0);
}

SysYParser::AddExpContext* SysYParser::addExp(int precedence) {
  ParserRuleContext *parentContext = _ctx;
  size_t parentState = getState();
  SysYParser::AddExpContext *_localctx = _tracker.createInstance<AddExpContext>(_ctx, parentState);
  SysYParser::AddExpContext *previousContext = _localctx;
  (void)previousContext; // Silence compiler, in case the context is not used by generated code.
  size_t startState = 46;
  enterRecursionRule(_localctx, 46, SysYParser::RuleAddExp, precedence);

    size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    unrollRecursionContexts(parentContext);
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    _localctx = _tracker.createInstance<FromMulContext>(_localctx);
    _ctx = _localctx;
    previousContext = _localctx;

    setState(300);
    mulExp(0);
    _ctx->stop = _input->LT(-1);
    setState(307);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 32, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        if (!_parseListeners.empty())
          triggerExitRuleEvent();
        previousContext = _localctx;
        auto newContext = _tracker.createInstance<AddOpContext>(_tracker.createInstance<AddExpContext>(parentContext, parentState));
        _localctx = newContext;
        pushNewRecursionContext(newContext, startState, RuleAddExp);
        setState(302);

        if (!(precpred(_ctx, 1))) throw FailedPredicateException(this, "precpred(_ctx, 1)");
        setState(303);
        antlrcpp::downCast<AddOpContext *>(_localctx)->op = _input->LT(1);
        _la = _input->LA(1);
        if (!(_la == SysYParser::T__19

        || _la == SysYParser::T__20)) {
          antlrcpp::downCast<AddOpContext *>(_localctx)->op = _errHandler->recoverInline(this);
        }
        else {
          _errHandler->reportMatch(this);
          consume();
        }
        setState(304);
        mulExp(0); 
      }
      setState(309);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 32, _ctx);
    }
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }
  return _localctx;
}

//----------------- RelExpContext ------------------------------------------------------------------

SysYParser::RelExpContext::RelExpContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t SysYParser::RelExpContext::getRuleIndex() const {
  return SysYParser::RuleRelExp;
}

void SysYParser::RelExpContext::copyFrom(RelExpContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- FromAddContext ------------------------------------------------------------------

SysYParser::AddExpContext* SysYParser::FromAddContext::addExp() {
  return getRuleContext<SysYParser::AddExpContext>(0);
}

SysYParser::FromAddContext::FromAddContext(RelExpContext *ctx) { copyFrom(ctx); }

void SysYParser::FromAddContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterFromAdd(this);
}
void SysYParser::FromAddContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitFromAdd(this);
}

std::any SysYParser::FromAddContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SysYVisitor*>(visitor))
    return parserVisitor->visitFromAdd(this);
  else
    return visitor->visitChildren(this);
}
//----------------- RelOpContext ------------------------------------------------------------------

SysYParser::RelExpContext* SysYParser::RelOpContext::relExp() {
  return getRuleContext<SysYParser::RelExpContext>(0);
}

SysYParser::AddExpContext* SysYParser::RelOpContext::addExp() {
  return getRuleContext<SysYParser::AddExpContext>(0);
}

SysYParser::RelOpContext::RelOpContext(RelExpContext *ctx) { copyFrom(ctx); }

void SysYParser::RelOpContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterRelOp(this);
}
void SysYParser::RelOpContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitRelOp(this);
}

std::any SysYParser::RelOpContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SysYVisitor*>(visitor))
    return parserVisitor->visitRelOp(this);
  else
    return visitor->visitChildren(this);
}

SysYParser::RelExpContext* SysYParser::relExp() {
   return relExp(0);
}

SysYParser::RelExpContext* SysYParser::relExp(int precedence) {
  ParserRuleContext *parentContext = _ctx;
  size_t parentState = getState();
  SysYParser::RelExpContext *_localctx = _tracker.createInstance<RelExpContext>(_ctx, parentState);
  SysYParser::RelExpContext *previousContext = _localctx;
  (void)previousContext; // Silence compiler, in case the context is not used by generated code.
  size_t startState = 48;
  enterRecursionRule(_localctx, 48, SysYParser::RuleRelExp, precedence);

    size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    unrollRecursionContexts(parentContext);
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    _localctx = _tracker.createInstance<FromAddContext>(_localctx);
    _ctx = _localctx;
    previousContext = _localctx;

    setState(311);
    addExp(0);
    _ctx->stop = _input->LT(-1);
    setState(318);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 33, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        if (!_parseListeners.empty())
          triggerExitRuleEvent();
        previousContext = _localctx;
        auto newContext = _tracker.createInstance<RelOpContext>(_tracker.createInstance<RelExpContext>(parentContext, parentState));
        _localctx = newContext;
        pushNewRecursionContext(newContext, startState, RuleRelExp);
        setState(313);

        if (!(precpred(_ctx, 1))) throw FailedPredicateException(this, "precpred(_ctx, 1)");
        setState(314);
        antlrcpp::downCast<RelOpContext *>(_localctx)->op = _input->LT(1);
        _la = _input->LA(1);
        if (!((((_la & ~ 0x3fULL) == 0) &&
          ((1ULL << _la) & 1006632960) != 0))) {
          antlrcpp::downCast<RelOpContext *>(_localctx)->op = _errHandler->recoverInline(this);
        }
        else {
          _errHandler->reportMatch(this);
          consume();
        }
        setState(315);
        addExp(0); 
      }
      setState(320);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 33, _ctx);
    }
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }
  return _localctx;
}

//----------------- EqExpContext ------------------------------------------------------------------

SysYParser::EqExpContext::EqExpContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t SysYParser::EqExpContext::getRuleIndex() const {
  return SysYParser::RuleEqExp;
}

void SysYParser::EqExpContext::copyFrom(EqExpContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- EqOpContext ------------------------------------------------------------------

SysYParser::EqExpContext* SysYParser::EqOpContext::eqExp() {
  return getRuleContext<SysYParser::EqExpContext>(0);
}

SysYParser::RelExpContext* SysYParser::EqOpContext::relExp() {
  return getRuleContext<SysYParser::RelExpContext>(0);
}

SysYParser::EqOpContext::EqOpContext(EqExpContext *ctx) { copyFrom(ctx); }

void SysYParser::EqOpContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterEqOp(this);
}
void SysYParser::EqOpContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitEqOp(this);
}

std::any SysYParser::EqOpContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SysYVisitor*>(visitor))
    return parserVisitor->visitEqOp(this);
  else
    return visitor->visitChildren(this);
}
//----------------- FromRelContext ------------------------------------------------------------------

SysYParser::RelExpContext* SysYParser::FromRelContext::relExp() {
  return getRuleContext<SysYParser::RelExpContext>(0);
}

SysYParser::FromRelContext::FromRelContext(EqExpContext *ctx) { copyFrom(ctx); }

void SysYParser::FromRelContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterFromRel(this);
}
void SysYParser::FromRelContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitFromRel(this);
}

std::any SysYParser::FromRelContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SysYVisitor*>(visitor))
    return parserVisitor->visitFromRel(this);
  else
    return visitor->visitChildren(this);
}

SysYParser::EqExpContext* SysYParser::eqExp() {
   return eqExp(0);
}

SysYParser::EqExpContext* SysYParser::eqExp(int precedence) {
  ParserRuleContext *parentContext = _ctx;
  size_t parentState = getState();
  SysYParser::EqExpContext *_localctx = _tracker.createInstance<EqExpContext>(_ctx, parentState);
  SysYParser::EqExpContext *previousContext = _localctx;
  (void)previousContext; // Silence compiler, in case the context is not used by generated code.
  size_t startState = 50;
  enterRecursionRule(_localctx, 50, SysYParser::RuleEqExp, precedence);

    size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    unrollRecursionContexts(parentContext);
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    _localctx = _tracker.createInstance<FromRelContext>(_localctx);
    _ctx = _localctx;
    previousContext = _localctx;

    setState(322);
    relExp(0);
    _ctx->stop = _input->LT(-1);
    setState(329);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 34, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        if (!_parseListeners.empty())
          triggerExitRuleEvent();
        previousContext = _localctx;
        auto newContext = _tracker.createInstance<EqOpContext>(_tracker.createInstance<EqExpContext>(parentContext, parentState));
        _localctx = newContext;
        pushNewRecursionContext(newContext, startState, RuleEqExp);
        setState(324);

        if (!(precpred(_ctx, 1))) throw FailedPredicateException(this, "precpred(_ctx, 1)");
        setState(325);
        antlrcpp::downCast<EqOpContext *>(_localctx)->op = _input->LT(1);
        _la = _input->LA(1);
        if (!(_la == SysYParser::T__29

        || _la == SysYParser::T__30)) {
          antlrcpp::downCast<EqOpContext *>(_localctx)->op = _errHandler->recoverInline(this);
        }
        else {
          _errHandler->reportMatch(this);
          consume();
        }
        setState(326);
        relExp(0); 
      }
      setState(331);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 34, _ctx);
    }
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }
  return _localctx;
}

//----------------- LAndExpContext ------------------------------------------------------------------

SysYParser::LAndExpContext::LAndExpContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t SysYParser::LAndExpContext::getRuleIndex() const {
  return SysYParser::RuleLAndExp;
}

void SysYParser::LAndExpContext::copyFrom(LAndExpContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- FromEqContext ------------------------------------------------------------------

SysYParser::EqExpContext* SysYParser::FromEqContext::eqExp() {
  return getRuleContext<SysYParser::EqExpContext>(0);
}

SysYParser::FromEqContext::FromEqContext(LAndExpContext *ctx) { copyFrom(ctx); }

void SysYParser::FromEqContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterFromEq(this);
}
void SysYParser::FromEqContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitFromEq(this);
}

std::any SysYParser::FromEqContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SysYVisitor*>(visitor))
    return parserVisitor->visitFromEq(this);
  else
    return visitor->visitChildren(this);
}
//----------------- LAndOpContext ------------------------------------------------------------------

SysYParser::LAndExpContext* SysYParser::LAndOpContext::lAndExp() {
  return getRuleContext<SysYParser::LAndExpContext>(0);
}

SysYParser::EqExpContext* SysYParser::LAndOpContext::eqExp() {
  return getRuleContext<SysYParser::EqExpContext>(0);
}

SysYParser::LAndOpContext::LAndOpContext(LAndExpContext *ctx) { copyFrom(ctx); }

void SysYParser::LAndOpContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterLAndOp(this);
}
void SysYParser::LAndOpContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitLAndOp(this);
}

std::any SysYParser::LAndOpContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SysYVisitor*>(visitor))
    return parserVisitor->visitLAndOp(this);
  else
    return visitor->visitChildren(this);
}

SysYParser::LAndExpContext* SysYParser::lAndExp() {
   return lAndExp(0);
}

SysYParser::LAndExpContext* SysYParser::lAndExp(int precedence) {
  ParserRuleContext *parentContext = _ctx;
  size_t parentState = getState();
  SysYParser::LAndExpContext *_localctx = _tracker.createInstance<LAndExpContext>(_ctx, parentState);
  SysYParser::LAndExpContext *previousContext = _localctx;
  (void)previousContext; // Silence compiler, in case the context is not used by generated code.
  size_t startState = 52;
  enterRecursionRule(_localctx, 52, SysYParser::RuleLAndExp, precedence);

    

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    unrollRecursionContexts(parentContext);
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    _localctx = _tracker.createInstance<FromEqContext>(_localctx);
    _ctx = _localctx;
    previousContext = _localctx;

    setState(333);
    eqExp(0);
    _ctx->stop = _input->LT(-1);
    setState(340);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 35, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        if (!_parseListeners.empty())
          triggerExitRuleEvent();
        previousContext = _localctx;
        auto newContext = _tracker.createInstance<LAndOpContext>(_tracker.createInstance<LAndExpContext>(parentContext, parentState));
        _localctx = newContext;
        pushNewRecursionContext(newContext, startState, RuleLAndExp);
        setState(335);

        if (!(precpred(_ctx, 1))) throw FailedPredicateException(this, "precpred(_ctx, 1)");
        setState(336);
        match(SysYParser::T__31);
        setState(337);
        eqExp(0); 
      }
      setState(342);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 35, _ctx);
    }
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }
  return _localctx;
}

//----------------- LOrExpContext ------------------------------------------------------------------

SysYParser::LOrExpContext::LOrExpContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t SysYParser::LOrExpContext::getRuleIndex() const {
  return SysYParser::RuleLOrExp;
}

void SysYParser::LOrExpContext::copyFrom(LOrExpContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- LOrOpContext ------------------------------------------------------------------

SysYParser::LOrExpContext* SysYParser::LOrOpContext::lOrExp() {
  return getRuleContext<SysYParser::LOrExpContext>(0);
}

SysYParser::LAndExpContext* SysYParser::LOrOpContext::lAndExp() {
  return getRuleContext<SysYParser::LAndExpContext>(0);
}

SysYParser::LOrOpContext::LOrOpContext(LOrExpContext *ctx) { copyFrom(ctx); }

void SysYParser::LOrOpContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterLOrOp(this);
}
void SysYParser::LOrOpContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitLOrOp(this);
}

std::any SysYParser::LOrOpContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SysYVisitor*>(visitor))
    return parserVisitor->visitLOrOp(this);
  else
    return visitor->visitChildren(this);
}
//----------------- FromLAndContext ------------------------------------------------------------------

SysYParser::LAndExpContext* SysYParser::FromLAndContext::lAndExp() {
  return getRuleContext<SysYParser::LAndExpContext>(0);
}

SysYParser::FromLAndContext::FromLAndContext(LOrExpContext *ctx) { copyFrom(ctx); }

void SysYParser::FromLAndContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterFromLAnd(this);
}
void SysYParser::FromLAndContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SysYListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitFromLAnd(this);
}

std::any SysYParser::FromLAndContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SysYVisitor*>(visitor))
    return parserVisitor->visitFromLAnd(this);
  else
    return visitor->visitChildren(this);
}

SysYParser::LOrExpContext* SysYParser::lOrExp() {
   return lOrExp(0);
}

SysYParser::LOrExpContext* SysYParser::lOrExp(int precedence) {
  ParserRuleContext *parentContext = _ctx;
  size_t parentState = getState();
  SysYParser::LOrExpContext *_localctx = _tracker.createInstance<LOrExpContext>(_ctx, parentState);
  SysYParser::LOrExpContext *previousContext = _localctx;
  (void)previousContext; // Silence compiler, in case the context is not used by generated code.
  size_t startState = 54;
  enterRecursionRule(_localctx, 54, SysYParser::RuleLOrExp, precedence);

    

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    unrollRecursionContexts(parentContext);
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    _localctx = _tracker.createInstance<FromLAndContext>(_localctx);
    _ctx = _localctx;
    previousContext = _localctx;

    setState(344);
    lAndExp(0);
    _ctx->stop = _input->LT(-1);
    setState(351);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 36, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        if (!_parseListeners.empty())
          triggerExitRuleEvent();
        previousContext = _localctx;
        auto newContext = _tracker.createInstance<LOrOpContext>(_tracker.createInstance<LOrExpContext>(parentContext, parentState));
        _localctx = newContext;
        pushNewRecursionContext(newContext, startState, RuleLOrExp);
        setState(346);

        if (!(precpred(_ctx, 1))) throw FailedPredicateException(this, "precpred(_ctx, 1)");
        setState(347);
        match(SysYParser::T__32);
        setState(348);
        lAndExp(0); 
      }
      setState(353);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 36, _ctx);
    }
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }
  return _localctx;
}

bool SysYParser::sempred(RuleContext *context, size_t ruleIndex, size_t predicateIndex) {
  switch (ruleIndex) {
    case 22: return mulExpSempred(antlrcpp::downCast<MulExpContext *>(context), predicateIndex);
    case 23: return addExpSempred(antlrcpp::downCast<AddExpContext *>(context), predicateIndex);
    case 24: return relExpSempred(antlrcpp::downCast<RelExpContext *>(context), predicateIndex);
    case 25: return eqExpSempred(antlrcpp::downCast<EqExpContext *>(context), predicateIndex);
    case 26: return lAndExpSempred(antlrcpp::downCast<LAndExpContext *>(context), predicateIndex);
    case 27: return lOrExpSempred(antlrcpp::downCast<LOrExpContext *>(context), predicateIndex);

  default:
    break;
  }
  return true;
}

bool SysYParser::mulExpSempred(MulExpContext *_localctx, size_t predicateIndex) {
  switch (predicateIndex) {
    case 0: return precpred(_ctx, 1);

  default:
    break;
  }
  return true;
}

bool SysYParser::addExpSempred(AddExpContext *_localctx, size_t predicateIndex) {
  switch (predicateIndex) {
    case 1: return precpred(_ctx, 1);

  default:
    break;
  }
  return true;
}

bool SysYParser::relExpSempred(RelExpContext *_localctx, size_t predicateIndex) {
  switch (predicateIndex) {
    case 2: return precpred(_ctx, 1);

  default:
    break;
  }
  return true;
}

bool SysYParser::eqExpSempred(EqExpContext *_localctx, size_t predicateIndex) {
  switch (predicateIndex) {
    case 3: return precpred(_ctx, 1);

  default:
    break;
  }
  return true;
}

bool SysYParser::lAndExpSempred(LAndExpContext *_localctx, size_t predicateIndex) {
  switch (predicateIndex) {
    case 4: return precpred(_ctx, 1);

  default:
    break;
  }
  return true;
}

bool SysYParser::lOrExpSempred(LOrExpContext *_localctx, size_t predicateIndex) {
  switch (predicateIndex) {
    case 5: return precpred(_ctx, 1);

  default:
    break;
  }
  return true;
}

void SysYParser::initialize() {
#if ANTLR4_USE_THREAD_LOCAL_CACHE
  sysyParserInitialize();
#else
  ::antlr4::internal::call_once(sysyParserOnceFlag, sysyParserInitialize);
#endif
}
