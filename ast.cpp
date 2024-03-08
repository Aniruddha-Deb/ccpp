#include "ast.hpp"
using namespace ast;

string op2str(Operator op) {
   switch (op) {
       case OP_ADD: return "OP_ADD";
       case OP_SUB: return "OP_SUB";
       case OP_MUL: return "OP_MUL";
       case OP_DIV: return "OP_DIV";
       case OP_MOD: return "OP_MOD";
       case OP_UNARY_PLUS: return "OP_UNARY_PLUS";
       case OP_UNARY_MINUS: return "OP_UNARY_MINUS";
       case OP_PRE_INCR: return "OP_PRE_INCR";
       case OP_PRE_DECR: return "OP_PRE_DECR";
       case OP_POST_INCR: return "OP_POST_INCR";
       case OP_POST_DECR: return "OP_POST_DECR";
       case OP_DEREF: return "OP_DEREF";
       case OP_SIZEOF: return "OP_SIZEOF";
       case OP_ALIGNOF: return "OP_ALIGNOF";
       case OP_TYPECAST: return "OP_TYPECAST";
       case OP_BOOL_AND: return "OP_BOOL_AND";
       case OP_BOOL_OR: return "OP_BOOL_OR";
       case OP_BOOL_NOT: return "OP_BOOL_NOT";
       case OP_EQ: return "OP_EQ";
       case OP_GE: return "OP_GE";
       case OP_GT: return "OP_GT";
       case OP_LT: return "OP_LT";
       case OP_LE: return "OP_LE";
       case OP_NE: return "OP_NE";
       case OP_AND: return "OP_AND";
       case OP_OR: return "OP_OR";
       case OP_XOR: return "OP_XOR";
       case OP_NOT: return "OP_NOT";
       case OP_LSHIFT: return "OP_LSHIFT";
       case OP_RSHIFT: return "OP_RSHIFT";
       case OP_ASSIGN: return "OP_ASSIGN";
       case OP_MUL_ASSIGN: return "OP_MUL_ASSIGN";
       case OP_DIV_ASSIGN: return "OP_DIV_ASSIGN";
       case OP_MOD_ASSIGN: return "OP_MOD_ASSIGN";
       case OP_ADD_ASSIGN: return "OP_ADD_ASSIGN";
       case OP_SUB_ASSIGN: return "OP_SUB_ASSIGN";
       case OP_LEFT_ASSIGN: return "OP_LEFT_ASSIGN";
       case OP_RIGHT_ASSIGN: return "OP_RIGHT_ASSIGN";
       case OP_AND_ASSIGN: return "OP_AND_ASSIGN";
       case OP_XOR_ASSIGN: return "OP_XOR_ASSIGN";
       case OP_OR_ASSIGN: return "OP_OR_ASSIGN";
       case OP_SEQ: return "OP_SEQ";
       default: return "UNKNOWN_OPERATOR";
   }
}
