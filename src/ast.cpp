#include "ast.hpp"
#include "debug.hpp"
#include "error.hpp"
#include <cmath>
#include <sstream>

#define I32_MOD (1ULL<<32)

namespace ast {

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

string lt2str(LiteralType lt) {
  switch(lt) {
    case LT_INT64: return "LT_INT64";
    case LT_UINT64: return "LT_UINT64";
    case LT_INT32: return "LT_INT32";
    case LT_UINT32: return "LT_UINT32";
    case LT_CHAR: return "LT_CHAR";
    case LT_FLOAT: return "LT_FLOAT";
    case LT_DOUBLE: return "LT_DOUBLE";
    case LT_BOOL: return "LT_BOOL";
    case LT_INT_LIKE: return "LT_INT_LIKE";
    case LT_FLOAT_LIKE: return "LT_FLOAT_LIKE";
    case LT_STRING: return "LT_STRING";
  }
}

string ss2str(StorageSpecifier ss) {
  switch(ss) {
    case SS_EXTERN: return "SS_EXTERN";
    case SS_STATIC: return "SS_STATIC";
    case SS_THREADLOCAL: return "SS_THREADLOCAL";
    case SS_AUTO: return "SS_AUTO";
    case SS_REGISTER: return "SS_REGISTER";
  }
}

string ts2str(TypeSpecifier ts) {
  switch(ts) {
    case TS_VOID: return "TS_VOID";
    case TS_CHAR: return "TS_CHAR";
    case TS_SHORT: return "TS_SHORT";
    case TS_INT: return "TS_INT";
    case TS_LONG: return "TS_LONG";
    case TS_FLOAT: return "TS_FLOAT";
    case TS_DOUBLE: return "TS_DOUBLE";
    case TS_SIGNED: return "TS_SIGNED";
    case TS_UNSIGNED: return "TS_UNSIGNED";
    case TS_BOOL: return "TS_BOOL";
    case TS_COMPLEX: return "TS_COMPLEX";
    case TS_IMAGINARY: return "TS_IMAGINARY";
    case TS_STRUCT: return "TS_STRUCT";
    case TS_UNION: return "TS_UNION";
    case TS_UNDEFINED: return "TS_UNDEFINED";
  }
}

string tq2str(TypeQualifier tq) {
  switch(tq) {
    case TQ_CONST: return "TQ_CONST";
    case TQ_RESTRICT: return "TQ_RESTRICT";
    case TQ_VOLATILE: return "TQ_VOLATILE";
    case TQ_ATOMIC: return "TQ_ATOMIC";
  }
}

string fs2str(FunctionSpecifier fs) {
  switch(fs) {
    case FS_INLINE: return "FS_INLINE";
    case FS_NORETURN: return "FS_NORETURN";
  }
}

Identifier::Identifier(string _name) : name(_name) {    // is this fine? Jai
    cdebug << "Identifier constructor called with name: " << _name << endl;
}

TernaryExpression::TernaryExpression(Expression *_cond,
                                     Expression *_true_branch,
                                     Expression *_false_branch)
    : cond(_cond), true_branch(_true_branch), false_branch(_false_branch) {
    cdebug << "TernaryExpression constructor called" << endl;
}

TernaryExpression::~TernaryExpression() {
    cdebug << "TernaryExpression destructor called" << endl;
    delete cond;
    delete true_branch;
    delete false_branch;
}

FunctionInvocationExpression::FunctionInvocationExpression(Expression *_fn)
    : fn(_fn) {
    cdebug << "FunctionInvocationExpression constructor called" << endl;
}

FunctionInvocationExpression::FunctionInvocationExpression(
    Expression *_fn, vector<Expression *> *_params)
    : fn(_fn), params(_params) {
    cdebug << "FunctionInvocationExpression constructor called with params" << endl;
}

FunctionInvocationExpression::~FunctionInvocationExpression() {
    cdebug << "FunctionInvocationExpression destructor called" << endl;
    delete fn;
    for (auto expr : *params) {
        delete expr;
    }
    delete params;
}

BinaryExpression::BinaryExpression(Expression *_lhs, Operator _op,
                                   Expression *_rhs)
    : lhs(_lhs), op(_op), rhs(_rhs) {
    cdebug << "BinaryExpression constructor called" << endl;
}

BinaryExpression::~BinaryExpression() {
    cdebug << "BinaryExpression destructor called" << endl;
    delete lhs;
    delete rhs;
}

UnaryExpression::UnaryExpression(Operator _op, Expression *_expr)
    : op(_op), expr(_expr) {
    cdebug << "UnaryExpression constructor called" << endl;
}

int hex2int(char c) {
  if (c >= '0' && c <= '9') return c - '0';
  if (c >= 'A' && c <= 'F') return c - 'A' + 10;
  if (c >= 'a' && c <= 'f') return c - 'a' + 10;
  return 0;
}

char get_esc_char(char c) {
  char esctab[256] = {0};
  esctab['a'] = 0x07;
  esctab['b'] = 0x08;
  esctab['e'] = 0x1B;
  esctab['f'] = 0x0C;
  esctab['n'] = 0x0A;
  esctab['r'] = 0x0D;
  esctab['t'] = 0x09;
  esctab['v'] = 0x0B;
  esctab['\\'] = 0x5C;
  esctab['\''] = 0x27;
  return esctab[c];
}

void parse_int_literal(Literal* literal) {
  string value = literal->value;
  int i = 0;
  if (value[0] == '0') {
    i = 1;
    if (value.size() == 1) { literal->data.i = 0; literal->ltype = LT_INT32; return; }
    if (value[1] == 'x' || value[1] == 'X') { // hex 
      i = 2;
      for (; i<value.size(); i++) {
        if (!isxdigit(value[i])) break;
        literal->data.l = (literal->data.l*16) + hex2int(value[i]);
      }
    }
    else {
      for (; i<value.size(); i++) {
        if (value[i] < '0' || value[i] > '7') break;
        literal->data.l = (literal->data.l*8) + (value[i] - '0');
      }
    }
  }
  else if (isdigit(value[0])) {
    for (; i<value.size(); i++) {
      if (value[i] < '0' || value[i] > '9') break;
      literal->data.l = (literal->data.l*10) + (value[i] - '0');
    }
  }
  else if (value[0] == 'u' || value[0] == '\'') {
    // char
    i = 1;
    if (value[i] == '\'') i++;
    // check escapes and stuff
    // we don't handle unicode sequences
    if (value[i] == '\\') {
      i++;
      literal->data.c = get_esc_char(value[i]);
    }
    else literal->data.c = value[i];
    literal->ltype = LT_CHAR;
    return;
  }

  if (i >= value.size()) { literal->ltype = LT_INT32; return; }
  if (value[i] == 'u' || value[i] == 'U') {
    i++;
    if (i >= value.size()) { literal->ltype = LT_UINT32; return; }
    literal->ltype = LT_UINT64; return; // can only be l, L, ll or LL
  }
  else if (value[i] == 'l' || value[i] == 'L') {
    i++;
    if (i >= value.size()) { literal->ltype = LT_INT64; return; }
    i++;
    if (i >= value.size()) { literal->ltype = LT_INT64; return; }
    if (value[i] == 'U' || value[i] == 'u') { literal->ltype = LT_UINT64; return; }
    i++;
    if (i >= value.size()) { literal->ltype = LT_INT64; return; }
    if (value[i] == 'U' || value[i] == 'u') { literal->ltype = LT_UINT64; return; }
  }
  ehdl::err("Could not parse int literal", literal->pos);
  return;
}

void parse_float_literal(Literal* literal) {
  double d = 0.0;
  string value = literal->value;

  // floats are guaranteed to have two chars atleast
  int fpos=0, i=0;
  bool hexfloat = false;
  if (value[0] == '0' && (value[1] == 'x' || value[1] == 'X')) {
    // hexfloat
    // find the position of the fp first 
    hexfloat = true;
    fpos = i = 2;
    int neg = -1;
    while (fpos < value.size() && value[fpos] != '.') fpos++;
    for (; i<value.size(); i++) {
      if (!isxdigit(value[i]) && value[i] != '.') break;
      if (value[i] == '.') { neg = 0; continue; }
      d += double(hex2int(value[i])) * pow(16, fpos-i+neg);
    }
  }
  else {
    // intfloat
    int neg = -1;
    while (fpos < value.size() && value[fpos] != '.') fpos++;
    for (; i<value.size(); i++) {
      if (!isdigit(value[i]) && value[i] != '.') break;
      if (value[i] == '.') {neg = 0; continue; }
      d += double(value[i] - '0') * pow(10, fpos-i+neg);
    }
  }
  if (i < value.size() && (value[i] == 'p' || value[i] == 'P' || value[i] == 'e' || value[i] == 'E')) {
    // exponent
    i++;
    int exp = 0;
    int sign = 1;
    if (i < value.size()) {
      if (value[i] == '-') { sign = -1; i++; }
      else if (value[i] == '+') { i++; }
    }
    for (;i < value.size(); i++) {
      if (!isdigit(value[i])) break;
      exp = (exp*10) + (value[i] - '0');
    }
    exp *= sign;
    if (hexfloat) d *= pow(2, exp);
    else d *= pow(10, exp);
  }
  if (i < value.size() && value[i] == 'f') {
    literal->ltype = LT_FLOAT;
    literal->data.f = d;
  }
  else {
    literal->ltype = LT_DOUBLE;
    literal->data.d = d;
  }
}

void parse_string_literal(Literal *literal) {
  string value = literal->value.substr(1, literal->value.size()-2);
  stringstream val_parsed;
  for (int i=0; i<value.size(); i++) {
    if (value[i] == '\\') {
      // escape seq
      val_parsed << get_esc_char(value[++i]);
    }
    else val_parsed << value[i];
  }
  literal->value = val_parsed.str();
}

Literal::Literal(string _value, LiteralType _ltype)
    : value(_value), ltype(_ltype), data{0} {
  cdebug << "Literal constructor called with value: " << _value << endl;
  // parse literal to int/float etc
  // has to be done here for us to be able to evaluate literal expressions 
  // instantaneously
  switch (ltype) {
    case LT_INT_LIKE:
      parse_int_literal(this);
      break;
    case LT_FLOAT_LIKE:
      parse_float_literal(this);
      break;
    case LT_STRING:
      parse_string_literal(this);
      break;
  }
}

Literal::Literal(long _data, LiteralType _ltype) :
  data{0}, ltype(_ltype), value("") { data.l = _data; }

Literal::Literal(float _data, LiteralType _ltype) :
  data{0}, ltype(_ltype), value("") { data.f = _data; }

Literal::Literal(double _data, LiteralType _ltype) :
  data{0}, ltype(_ltype), value("") { data.d = _data; }

int get_rank(LiteralType ltype) {
  if (ltype == LT_BOOL) return 0;
  if (ltype == LT_CHAR) return 1;
  if (ltype == LT_SHORT) return 2;
  if (ltype == LT_INT32 || ltype == LT_UINT32) return 3;
  if (ltype == LT_INT64 || ltype == LT_UINT64) return 4;
  if (ltype == LT_FLOAT) return 5;
  if (ltype == LT_DOUBLE) return 6;
  return -1;
}

int get_symbol_rank(SymbolType st) {
  if (st == I1) return 0;
  if ((st == I8) || (st == U8)) return 1;
  if ((st == I16) || (st == U16)) return 2;
  if ((st == I32 || st == U32)) return 3;
  if ((st == I64 || st == U64)) return 4;
  if (st == FP32) return 5;
  if (st == FP64) return 6;
  return -1;
}

void widen_literals(Literal* lhslit, Literal* rhslit) {
  if (get_rank(rhslit->ltype) > get_rank(lhslit->ltype)) {
    switch (rhslit->ltype) {
      case LT_DOUBLE: 
        if(lhslit->ltype == LT_FLOAT) {
          lhslit->data.d = double(lhslit->data.f);
        }
        else{
          lhslit->data.d = double(lhslit->data.l);
        }
        break;
      
      case LT_FLOAT: lhslit->data.f = float(lhslit->data.l); break;
    }
    lhslit->ltype = rhslit->ltype;
  }
  else if (get_rank(lhslit->ltype) > get_rank(rhslit->ltype)) {
    switch (lhslit->ltype) {
      case LT_DOUBLE: 
        if(rhslit->ltype == LT_FLOAT) {
          rhslit->data.d = double(rhslit->data.f);
        }
        else{
          rhslit->data.d = double(rhslit->data.l);
        }
        break;
      case LT_FLOAT: rhslit->data.f = float(rhslit->data.l); break;
    }
    rhslit->ltype = lhslit->ltype;
  }
}

LiteralType symbol_to_literal(SymbolType st){
  if (st == FP64) return LT_DOUBLE;
  if (st == FP32) return LT_FLOAT;
  if (st == I64) return LT_INT64;
  if (st == I32) return LT_INT32;
  if (st == I16) return LT_SHORT;
  if (st == I8) return LT_CHAR;
  if (st == I1) return LT_BOOL;
  if (st == U64) return LT_UINT64;
  if (st == U32) return LT_UINT32;
  if (st == U16) return LT_SHORT;
  if (st == U8) return LT_CHAR;
}

void assign_literals(SymbolType lhstype, Literal* rhslit) {
  if (rhslit->ltype == LT_STRING) {

    return;
  }
  if (get_rank(rhslit->ltype) > get_symbol_rank(lhstype)) {
    if (lhstype == I1) {
      switch (rhslit->ltype) {
        case LT_DOUBLE: 
          rhslit->data.l = rhslit->data.d != 0;
          break;
        case LT_FLOAT: 
          rhslit->data.l = (rhslit->data.f != 0); 
          break;
        default:
          rhslit->data.l = (rhslit->data.l != 0);
          break;
      }
    }
    else{
      switch (rhslit->ltype) {
        case LT_DOUBLE: 
          if (lhstype == FP32) {
            rhslit->data.f = float(rhslit->data.d); 
            // cout << rhslit->data.f << endl;
          }
          else{
            rhslit->data.l = floor(rhslit->data.d);
          }
        break;
        case LT_FLOAT: 
   
          rhslit->data.l = long(rhslit->data.f); 
          
          break;
      }
    }
    rhslit->ltype = symbol_to_literal(lhstype);
    // cout << rhslit->data.l << endl;
  }
  else if (get_symbol_rank(lhstype) > get_rank(rhslit->ltype)) {
    switch (lhstype) {
      case FP64: 
        if(rhslit->ltype == LT_FLOAT) {
          rhslit->data.d = double(rhslit->data.f);
        }
        else{
          rhslit->data.d = double(rhslit->data.l);
        }
        break;
      case FP32: rhslit->data.f = float(rhslit->data.l); break;
    }
    rhslit->ltype = symbol_to_literal(lhstype);
  }
}

void add_literals(Literal* lhs, Literal* rhs) {
  if (lhs->ltype == LT_CHAR) lhs->data.c += rhs->data.c;
  else if (lhs->ltype == LT_SHORT) lhs->data.s += rhs->data.s;
  else if (lhs->ltype == LT_INT32 || lhs->ltype == LT_UINT32) lhs->data.i += rhs->data.i;
  else if (lhs->ltype == LT_FLOAT) lhs->data.f += rhs->data.f;
  else if (lhs->ltype == LT_DOUBLE) lhs->data.d += rhs->data.d;
  else lhs->data.l += rhs->data.l;
}

void sub_literals(Literal* lhs, Literal* rhs) {
  if (lhs->ltype == LT_CHAR) lhs->data.c -= rhs->data.c;
  else if (lhs->ltype == LT_SHORT) lhs->data.s -= rhs->data.s;
  else if (lhs->ltype == LT_INT32 || lhs->ltype == LT_UINT32) lhs->data.i -= rhs->data.i;
  else if (lhs->ltype == LT_FLOAT) lhs->data.f -= rhs->data.f;
  else if (lhs->ltype == LT_DOUBLE) lhs->data.d -= rhs->data.d;
  else lhs->data.l -= rhs->data.l;
}

void mul_literals(Literal* lhs, Literal* rhs) {
  if (lhs->ltype == LT_CHAR) lhs->data.c *= rhs->data.c;
  else if (lhs->ltype == LT_SHORT) lhs->data.s *= rhs->data.s;
  else if (lhs->ltype == LT_INT32 || lhs->ltype == LT_UINT32) lhs->data.i *= rhs->data.i;
  else if (lhs->ltype == LT_FLOAT) lhs->data.f *= rhs->data.f;
  else if (lhs->ltype == LT_DOUBLE) lhs->data.d *= rhs->data.d;
  else lhs->data.l *= rhs->data.l;
}

// Udiv different from sdiv?
void div_literals(Literal* lhs, Literal* rhs) {
  // cout << rhs->ltype << " " << rhs->data.s << endl;
  if (lhs->ltype == LT_CHAR) lhs->data.c /= rhs->data.c;
  else if (lhs->ltype == LT_SHORT) lhs->data.s /= rhs->data.s;
  else if (lhs->ltype == LT_INT32 || lhs->ltype == LT_UINT32) lhs->data.i /= rhs->data.i;
  else if (lhs->ltype == LT_FLOAT) lhs->data.f /= rhs->data.f;
  else if (lhs->ltype == LT_DOUBLE) lhs->data.d /= rhs->data.d;
  else lhs->data.l /= rhs->data.l;
  // cout << lhs->ltype << " " << lhs->data.s << endl;
}

void mod_literals(Literal* lhs, Literal* rhs) {
  if (lhs->ltype == LT_FLOAT || lhs->ltype == LT_DOUBLE) ehdl::err("Can't take modulo of floating point literals", lhs->pos);
  else if (lhs->ltype == LT_CHAR) lhs->data.c %= rhs->data.c;
  else if (lhs->ltype == LT_SHORT) lhs->data.s %= rhs->data.s;
  else if (lhs->ltype == LT_INT32 || lhs->ltype == LT_UINT32) lhs->data.i %= rhs->data.i;
  else lhs->data.l %= rhs->data.l;
}

void gt_literals(Literal* lhs, Literal* rhs) {
  if (lhs->ltype == LT_FLOAT) lhs->data.f = (lhs->data.f > rhs->data.f);
  else if (lhs->ltype == LT_DOUBLE) lhs->data.d = (lhs->data.d > rhs->data.d);
  else lhs->data.l = (lhs->data.l > rhs->data.l);
  lhs->ltype = LT_BOOL;
}

void ge_literals(Literal* lhs, Literal* rhs) {
  if (lhs->ltype == LT_FLOAT) lhs->data.f = (lhs->data.f >= rhs->data.f);
  else if (lhs->ltype == LT_DOUBLE) lhs->data.d = (lhs->data.d >= rhs->data.d);
  else lhs->data.l = (lhs->data.l >= rhs->data.l);
  lhs->ltype = LT_BOOL;
}

void lt_literals(Literal* lhs, Literal* rhs) {
  if (lhs->ltype == LT_FLOAT) lhs->data.f = (lhs->data.f < rhs->data.f);
  else if (lhs->ltype == LT_DOUBLE) lhs->data.d = (lhs->data.d < rhs->data.d);
  else lhs->data.l = (lhs->data.l < rhs->data.l);
  lhs->ltype = LT_BOOL;
}

void le_literals(Literal* lhs, Literal* rhs) {
  if (lhs->ltype == LT_FLOAT) lhs->data.f = (lhs->data.f <= rhs->data.f);
  else if (lhs->ltype == LT_DOUBLE) lhs->data.d = (lhs->data.d <= rhs->data.d);
  else lhs->data.l = (lhs->data.l <= rhs->data.l);
  lhs->ltype = LT_BOOL;
}

void eq_literals(Literal* lhs, Literal* rhs) {
  if (lhs->ltype == LT_FLOAT) lhs->data.f = (lhs->data.f == rhs->data.f);
  else if (lhs->ltype == LT_DOUBLE) lhs->data.d = (lhs->data.d == rhs->data.d);
  else lhs->data.l = (lhs->data.l == rhs->data.l);
  lhs->ltype = LT_BOOL;
}

void ne_literals(Literal* lhs, Literal* rhs) {
  if (lhs->ltype == LT_FLOAT) lhs->data.f = (lhs->data.f != rhs->data.f);
  else if (lhs->ltype == LT_DOUBLE) lhs->data.d = (lhs->data.d != rhs->data.d);
  else lhs->data.l = (lhs->data.l != rhs->data.l);
  lhs->ltype = LT_BOOL;
}

void and_literals(Literal* lhs, Literal* rhs) {
  if (lhs->ltype == LT_FLOAT || lhs->ltype == LT_DOUBLE) ehdl::err("Can't take and of floating point literals", rhs->pos);
  else lhs->data.l &= rhs->data.l;
}

void or_literals(Literal* lhs, Literal* rhs) {
  if (lhs->ltype == LT_FLOAT || lhs->ltype == LT_DOUBLE) ehdl::err("Can't take or of floating point literals", rhs->pos);
  else if (lhs->ltype == LT_CHAR) lhs->data.c |= rhs->data.c;
  else if (lhs->ltype == LT_SHORT) lhs->data.s |= rhs->data.s;
  else if (lhs->ltype == LT_INT32 || lhs->ltype == LT_UINT32) lhs->data.i |= rhs->data.i;
  else lhs->data.l |= rhs->data.l;
}

void xor_literals(Literal* lhs, Literal* rhs) {
  if (lhs->ltype == LT_FLOAT || lhs->ltype == LT_DOUBLE) ehdl::err("Can't take xor of floating point literals", rhs->pos);
  else if (lhs->ltype == LT_CHAR) lhs->data.c ^= rhs->data.c;
  else if (lhs->ltype == LT_SHORT) lhs->data.s ^= rhs->data.s;
  else if (lhs->ltype == LT_INT32 || lhs->ltype == LT_UINT32) lhs->data.i ^= rhs->data.i;
  else lhs->data.l ^= rhs->data.l;
}

void lshift_literals(Literal* lhs, Literal* rhs) {
  if (lhs->ltype == LT_FLOAT || lhs->ltype == LT_DOUBLE) ehdl::err("Can't left shift floating point literals", rhs->pos);
  else if (lhs->ltype == LT_CHAR) lhs->data.c <<= rhs->data.c;
  else if (lhs->ltype == LT_SHORT) lhs->data.s <<= rhs->data.s;
  else if (lhs->ltype == LT_INT32 || lhs->ltype == LT_UINT32) lhs->data.i <<= rhs->data.i;
  else lhs->data.l <<= rhs->data.l;
}

void rshift_literals(Literal* lhs, Literal* rhs) {
  if (lhs->ltype == LT_FLOAT || lhs->ltype == LT_DOUBLE) ehdl::err("Can't right shift floating point literals", rhs->pos);
  else if (lhs->ltype == LT_CHAR) lhs->data.c >>= rhs->data.c;
  else if (lhs->ltype == LT_SHORT) lhs->data.s >>= rhs->data.s;
  else if (lhs->ltype == LT_INT32 || lhs->ltype == LT_UINT32) lhs->data.i >>= rhs->data.i;
  else lhs->data.l >>= rhs->data.l;
}

bool lit2bool(Literal* l) {
  if (l->ltype == LT_STRING) return true;
  if (l->ltype == LT_FLOAT) return (l->data.f != 0.f && l->data.f != -0.f);
  if (l->ltype == LT_DOUBLE) return (l->data.d != 0.f && l->data.d != -0.f);
  return (l->data.l != 0);
}

void booland_literals(Literal* lhs, Literal* rhs) {
  lhs->data.l = (lit2bool(lhs) && lit2bool(rhs));
  lhs->ltype = LT_BOOL;
}

void boolor_literals(Literal* lhs, Literal* rhs) {
  lhs->data.l = (lit2bool(lhs) || lit2bool(rhs));
  lhs->ltype = LT_BOOL;
}

void boolnot_literal(Literal* l) {
  l->data.l = !lit2bool(l);
  l->ltype = LT_BOOL;
}

void not_literal(Literal* l) {
  if (l->ltype == LT_FLOAT || l->ltype == LT_DOUBLE) ehdl::err("Can't take not of floating point literals", l->pos);
  else if (l->ltype == LT_CHAR) l->data.c = ~l->data.c;
  else if (l->ltype == LT_SHORT) l->data.s = ~l->data.s;
  else if (l->ltype == LT_INT32 || l->ltype == LT_UINT32) l->data.i = ~l->data.i;
  else l->data.l = ~l->data.l;
}

void negate_literal_value(Literal* l) {
  if (l->ltype == LT_CHAR) l->data.c *= -1;
  else if (l->ltype == LT_SHORT) l->data.s *= -1;
  else if (l->ltype == LT_INT32 || l->ltype == LT_UINT32) l->data.i *= -1;
  else if (l->ltype == LT_FLOAT) l->data.f *= -1;
  else if (l->ltype == LT_DOUBLE) l->data.d *= -1;
  else l->data.l *= -1;
}

// TODO add more operators (bitwise, relational, logical and unary)




Expression* FoldConstants(Expression* expr){
  BinaryExpression* bin_exp;
  UnaryExpression* un_exp;
  if ((bin_exp = dynamic_cast<BinaryExpression*>(expr))) {
    Expression* result = allocateBinaryExpression(FoldConstants(bin_exp->lhs), bin_exp->op, FoldConstants(bin_exp->rhs));
    return result;
  }
  else if((un_exp = dynamic_cast<UnaryExpression*>(expr))){
    // cout << "unary expression" << endl;
    Expression* result = allocateUnaryExpression(un_exp->op, FoldConstants(un_exp->expr));
    return result;
  }
  return expr;
}

Expression* allocateBinaryExpression(Expression* lhs, Operator op, Expression* rhs) {
  Identifier *ident;
  if ((ident = dynamic_cast<Identifier*>(lhs))) {
    // assignments can get constant folded because of this
    switch(op) {
      case OP_ADD_ASSIGN:   return allocateBinaryExpression(ident, OP_ASSIGN, allocateBinaryExpression(ident, OP_ADD,    rhs));
      case OP_SUB_ASSIGN:   return allocateBinaryExpression(ident, OP_ASSIGN, allocateBinaryExpression(ident, OP_SUB,    rhs));
      case OP_MUL_ASSIGN:   return allocateBinaryExpression(ident, OP_ASSIGN, allocateBinaryExpression(ident, OP_MUL,    rhs));
      case OP_DIV_ASSIGN:   return allocateBinaryExpression(ident, OP_ASSIGN, allocateBinaryExpression(ident, OP_DIV,    rhs));
      case OP_MOD_ASSIGN:   return allocateBinaryExpression(ident, OP_ASSIGN, allocateBinaryExpression(ident, OP_MOD,    rhs));
      case OP_AND_ASSIGN:   return allocateBinaryExpression(ident, OP_ASSIGN, allocateBinaryExpression(ident, OP_AND,    rhs));
      case OP_OR_ASSIGN:    return allocateBinaryExpression(ident, OP_ASSIGN, allocateBinaryExpression(ident, OP_OR,     rhs));
      case OP_XOR_ASSIGN:   return allocateBinaryExpression(ident, OP_ASSIGN, allocateBinaryExpression(ident, OP_XOR,    rhs));
      case OP_LEFT_ASSIGN:  return allocateBinaryExpression(ident, OP_ASSIGN, allocateBinaryExpression(ident, OP_LSHIFT, rhs));
      case OP_RIGHT_ASSIGN: return allocateBinaryExpression(ident, OP_ASSIGN, allocateBinaryExpression(ident, OP_RSHIFT, rhs));
    }
  }
  Literal *lhslit, *rhslit;
  if ((lhslit = dynamic_cast<Literal*>(lhs)) && (rhslit = dynamic_cast<Literal*>(rhs))) {
    widen_literals(lhslit, rhslit);
    switch(op) {
      case OP_ADD: add_literals(lhslit, rhslit); break;
      case OP_SUB: sub_literals(lhslit, rhslit); break;
      case OP_MUL: mul_literals(lhslit, rhslit); break;
      case OP_DIV: div_literals(lhslit, rhslit); break;
      case OP_MOD: mod_literals(lhslit, rhslit); break;
      case OP_BOOL_AND: booland_literals(lhslit, rhslit); break;
      case OP_BOOL_OR: boolor_literals(lhslit, rhslit); break;
      case OP_AND: and_literals(lhslit, rhslit); break;
      case OP_OR: or_literals(lhslit, rhslit); break;
      case OP_XOR: xor_literals(lhslit, rhslit); break;
      case OP_LSHIFT: lshift_literals(lhslit, rhslit); break;
      case OP_RSHIFT: rshift_literals(lhslit, rhslit); break;
      case OP_LT: lt_literals(lhslit, rhslit); break;
      case OP_LE: le_literals(lhslit, rhslit); break;
      case OP_GT: gt_literals(lhslit, rhslit); break;
      case OP_GE: ge_literals(lhslit, rhslit); break;
      case OP_EQ: eq_literals(lhslit, rhslit); break;
      case OP_NE: ne_literals(lhslit, rhslit); break;
      case OP_ASSIGN:
      case OP_ADD_ASSIGN:
      case OP_SUB_ASSIGN:
      case OP_MUL_ASSIGN:
      case OP_DIV_ASSIGN:
      case OP_MOD_ASSIGN:
      case OP_AND_ASSIGN:
      case OP_OR_ASSIGN:
      case OP_XOR_ASSIGN:
      case OP_LEFT_ASSIGN:
      case OP_RIGHT_ASSIGN: ehdl::err("Cannot assign a constant to a value", lhs->pos); break;
      default: return new BinaryExpression(lhslit, op, rhslit);
    }
    delete rhs;
    return lhs;
  }
  return new BinaryExpression(lhs, op, rhs);
}


Expression* allocateUnaryExpression(Operator op, Expression* expr) {
  Literal *lit;
  if ((lit = dynamic_cast<Literal*>(expr))) {
    switch(op) {
      case OP_UNARY_PLUS: return lit;
      case OP_UNARY_MINUS: negate_literal_value(lit); return lit;
      case OP_BOOL_NOT: boolnot_literal(lit); return lit;
      case OP_NOT: not_literal(lit); return lit;
      default: return new UnaryExpression(op, expr);
    }
  }
  return new UnaryExpression(op, expr);
}

ExpressionStatement::ExpressionStatement(Expression *_expr) : expr(_expr) {
    cdebug << "ExpressionStatement constructor called" << endl;
}

ExpressionStatement::~ExpressionStatement() {
    cdebug << "ExpressionStatement destructor called" << endl;
    delete expr;
}

IfStatement::IfStatement(Expression *_cond, Statement *_true_branch,
                         Statement *_false_branch)
    : cond(_cond), true_branch(_true_branch), false_branch(_false_branch) {
    cdebug << "IfStatement constructor called" << endl;
}

IfStatement::~IfStatement() {
    cdebug << "IfStatement destructor called" << endl;
    delete cond;
    delete true_branch;
    delete false_branch;
}

SwitchStatement::~SwitchStatement() {
    cdebug << "SwitchStatement destructor called" << endl;
    delete expr;
    delete stmt;
}

WhileStatement::WhileStatement(Expression *_cond, Statement *_stmt)
    : cond(_cond), stmt(_stmt) {
    cdebug << "WhileStatement constructor called" << endl;
}

WhileStatement::~WhileStatement() {
    cdebug << "WhileStatement destructor called" << endl;
    delete cond;
    delete stmt;
}

DoWhileStatement::DoWhileStatement(Expression *_cond, Statement *_stmt)
    : cond(_cond), stmt(_stmt) {
    cdebug << "DoWhileStatement constructor called" << endl;
}

DoWhileStatement::~DoWhileStatement() {
    cdebug << "DoWhileStatement destructor called" << endl;
    delete cond;
    delete stmt;
}

ReturnStatement::ReturnStatement(Expression *_ret_expr) : ret_expr(_ret_expr) {
    cdebug << "ReturnStatement constructor called" << endl;
}

ReturnStatement::~ReturnStatement() {
    cdebug << "ReturnStatement destructor called" << endl;
    delete ret_expr;
}

GotoStatement::GotoStatement(char *_label) : label(_label) {
    cdebug << "GotoStatement constructor called" << endl;
}

DeclarationSpecifiers::DeclarationSpecifiers()
    : storage_specs(), type_specs{}, type_quals(), func_specs() {
    cdebug << "DeclarationSpecifiers constructor called" << endl;
}

void DeclarationSpecifiers::add_type_specifier(TypeSpecifier ts) {
    // FLOAT cannot be combined with unsigned/signed
    if (ts == TS_FLOAT || ts == TS_DOUBLE) {
        if (!type_specs.empty()) {
          ehdl::err("Cannot combine " + ts2str(ts) + " with previous decls", pos);
            return;
        }
    }
    else if (type_specs.find(TS_FLOAT) != type_specs.end() || type_specs.find(TS_DOUBLE) != type_specs.end()) {
        ehdl::err("cannot combine a type specifier with floating point types", pos);
        return;
    }

    if (ts == TS_CHAR) {
        if (type_specs.size() == 1 && !(*type_specs.begin() == TS_SIGNED || *type_specs.begin() == TS_UNSIGNED)) {
            ehdl::err("cannot combine char with previous decls", pos);
            return;
        }
        else if (type_specs.size() > 1) {
            ehdl::err("cannot combine char with previous decls", pos);
            return;
        }
    }
    else if (ts == TS_SIGNED || ts == TS_UNSIGNED) {
        if ((ts == TS_SIGNED && type_specs.find(TS_UNSIGNED) != type_specs.end()) ||
            (ts == TS_UNSIGNED && type_specs.find(TS_SIGNED) != type_specs.end())) {
            ehdl::err("cannot combine signed with unsigned", pos);
            return;
        }
    }
    else if (ts == TS_SHORT) {
        if (type_specs.find(TS_CHAR) != type_specs.end() || type_specs.find(TS_LONG) != type_specs.end()) {
            ehdl::err("cannot combine short with char, long or llong", pos);
            return;
        }
    }
    else if (ts == TS_LONG) {
        if (type_specs.find(TS_CHAR) != type_specs.end() || type_specs.find(TS_SHORT) != type_specs.end()) {
            ehdl::err("cannot combine long with char or short", pos);
            return;
        }
    }
    else if (ts == TS_INT) {
        if (type_specs.find(TS_CHAR) != type_specs.end()) {
            ehdl::err("cannot combine int with char", pos);
        }
    }
    type_specs.insert(ts);
}

void DeclarationSpecifiers::add_storage_specifier(StorageSpecifier ss) {
    storage_specs.insert(ss);
}

void DeclarationSpecifiers::add_type_qualifier(TypeQualifier tq) {
    type_quals.insert(tq);
}

void DeclarationSpecifiers::add_func_specifier(FunctionSpecifier fs) {
    func_specs.insert(fs);
}

PureDeclaration::PureDeclaration(DeclarationSpecifiers *_decl_specs,
                                 int _ptr_depth, Identifier *_ident)
    : decl_specs{_decl_specs}, ptr_depth{_ptr_depth}, ident{_ident} {
    cdebug << "PureDeclaration constructor called" << endl;
}

FunctionParameterList::FunctionParameterList(std::vector<PureDeclaration*>* _params, bool _has_varargs)
    : params{_params}, has_varargs{_has_varargs} {
    cdebug << "FunctionParameterList constructor called" << endl;
}

InitDeclarator::InitDeclarator(int _ptr_depth, Identifier *_ident,
                               Expression *_init_expr)
    : ptr_depth{_ptr_depth}, ident{_ident}, init_expr{_init_expr} {
    cdebug << "InitDeclarator constructor called" << endl;
}

Declaration::Declaration(DeclarationSpecifiers *_decl_specs,
                         vector<InitDeclarator *> *_decl_list)
    : decl_specs(_decl_specs), decl_list(_decl_list) {
    cdebug << "Declaration constructor called" << endl;
}

Declaration::~Declaration() {
    cdebug << "Declaration destructor called" << endl;
    // TODO delete stuff
}

DeclarationStatement::DeclarationStatement(Declaration *_decl) : decl(_decl) {
    cdebug << "DeclarationStatement constructor called" << endl;
}

DeclarationStatement::~DeclarationStatement() {
    cdebug << "DeclarationStatement destructor called" << endl;
    delete decl;
}

BlockStatement::~BlockStatement() {
    cdebug << "BlockStatement destructor called" << endl;
    for (auto stmt : *this) {
        delete stmt;
    }
}

LabeledStatement::LabeledStatement(Identifier *_label, Statement *_stmt)
    : label{_label}, stmt{_stmt} {
    cdebug << "LabeledStatement constructor called" << endl;
}

LabeledStatement::~LabeledStatement() {
    cdebug << "LabeledStatement destructor called" << endl;
    delete label;
    delete stmt;
}

CaseStatement::CaseStatement(Expression *_const_expr, Statement *_stmt)
    : const_expr{_const_expr}, stmt{_stmt} {
    cdebug << "CaseStatement constructor called" << endl;
}

CaseStatement::~CaseStatement() {
    cdebug << "CaseStatement destructor called" << endl;
    delete const_expr;
    delete stmt;
}

PureDeclaration::~PureDeclaration() {
    cdebug << "PureDeclaration destructor called" << endl;
    delete decl_specs;
    delete ident;
}

SwitchStatement::SwitchStatement(Expression* _expr, Statement* _stmt)
    : expr{_expr}, stmt{_stmt} {
    cdebug << "SwitchStatement constructor called" << endl;
}

FunctionParameterList::~FunctionParameterList() {
    cdebug << "FunctionParameterList destructor called" << endl;
    for (auto pd : *params) {
        delete pd;
    }
}

Function::Function(PureDeclaration *_func_decl, FunctionParameterList *_params,
                   BlockStatement *_stmts)
    : func_decl{_func_decl}, params{_params}, stmts{_stmts} {
    cdebug << "Function constructor called" << endl;
}

Function::~Function() {
    cdebug << "Function destructor called" << endl;
    delete func_decl;
    delete params;
    delete stmts;
}

TranslationUnit::TranslationUnit()
    : nodes(new vector<Node *>) {
    cdebug << "TranslationUnit constructor called" << endl;
}

void TranslationUnit::add_function(Function *func) {
    nodes->push_back(func);
}

void TranslationUnit::add_declaration(DeclarationStatement *decl) {
    nodes->push_back(decl);
}

TranslationUnit::~TranslationUnit() {
    cdebug << "TranslationUnit destructor called" << endl;
    for (auto *node : *nodes) {
        delete node;
    }
    delete nodes;
}


//copy

Expression* Expression::copy_exp(){
  cout << "virtual copyexp" << endl;
  return nullptr;
}

Expression* Identifier::copy_exp(){
  Identifier* newexp = new Identifier("");
  if (true) {
    newexp->ident_info = ident_info;
    newexp->name = name;
    newexp->type_info = type_info;
    return newexp;
  }
  return nullptr;
}

Expression* Literal::copy_exp(){
  Literal* newexp = new Literal("", LT_CHAR);
  if (true) {
    newexp->data = data;
    newexp->ltype = ltype;
    newexp->type_info = type_info;
    newexp->value = value;
    return newexp;
  }
}

Expression* UnaryExpression::copy_exp(){
  UnaryExpression* newexp = new UnaryExpression(OP_ADD, nullptr);
  if (true) {
    newexp->expr = expr->copy_exp();
    newexp->op = op;
    newexp->type_info = type_info;
    return newexp;
  }
  return nullptr;
}

Expression* BinaryExpression::copy_exp(){
  BinaryExpression* newexp = new BinaryExpression(lhs, op, rhs);
  if (true) {
    newexp->lhs = lhs->copy_exp();
    newexp->rhs = rhs->copy_exp();
    newexp->op = op;
    newexp->type_info = type_info;
    return newexp;
  }
  return nullptr;
}


Expression* FunctionInvocationExpression::copy_exp(){
  FunctionInvocationExpression* newexp = new FunctionInvocationExpression(this);

  newexp->fn = fn->copy_exp();
  newexp->type_info = type_info;

  if (params) {
    newexp->params = new vector<Expression*>;
    for(auto itr: *params){
      newexp->params->push_back(itr->copy_exp());
    }
  }
  else{
    newexp->params = nullptr;
  }

  return newexp;

}

} // namespace ast
