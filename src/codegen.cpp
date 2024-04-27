#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Constant.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"
#include "symtab.hpp"
#include <map>
#include "ast.hpp"
#include "debug.hpp"
#include <sstream>
#include "error.hpp"

int intLiteralToInt(string &s){
  return stoi(s);
};

string stringLiteraltoString(string &s){
  return s.substr(1, s.size() - 2);
}

double floatLiteralToFloat(string &s){
  // cdebug<<"Entered stod "+ s<<endl;
  double temp = stod(s);
  // cdebug<<"temp "<<temp<<endl;
  return temp;
}

std::string stype2str(SymbolType stype) {
  switch(stype) {
    case FP32: return "FP32";
    case FP64: return "FP64";
    case I1: return "I1";
    case I8: return "I8";
    case I16: return "I16";
    case I32: return "I32";
    case I64: return "I64";
    case U8: return "U8";
    case U16: return "U16";
    case U32: return "U32";
    case U64: return "U64";
    case PTR: return "PTR";
    case FUNC: return "FUNC";
    case UNK: return "UNK";
  }
}

using namespace llvm;

namespace ast {

static std::unique_ptr<llvm::LLVMContext> llvm_ctx;
static std::unique_ptr<llvm::Module> llvm_mod;
static std::unique_ptr<llvm::IRBuilder<>> llvm_builder;
static std::map<std::string, AllocaInst*> llvm_st;
static std::map<std::string, GlobalVariable*> global_st;
static std::map<std::string, llvm::Function*> func_st;


bool is_signed_int_type(SymbolType ty) {
  return ty == I8 || ty == I16 || ty == I32 || ty == I64;
}

bool is_bool_type(SymbolType ty) {
  return ty == I1;
}

bool is_unsigned_int_type(SymbolType ty) {
  return ty == U8 || ty == U16 || ty == U32 || ty == U64;
}

bool is_int_type(SymbolType ty) {
  return is_signed_int_type(ty) || is_unsigned_int_type(ty);
}

bool is_fp_type(SymbolType ty) {
  return ty == FP32 || ty == FP64;
}

Value* handleAdd(Value* L, Value* R, SymbolType ty){
  if (is_int_type(ty)) {
    // cout << "INT TYPE" << endl;
    return llvm_builder->CreateAdd(L, R, "temp");
  }
  else if (is_fp_type(ty)) return llvm_builder->CreateFAdd(L, R, "temp");
  else return nullptr;
}

Value* handleSub(Value* L, Value* R, SymbolType ty){
  if (is_int_type(ty)) return llvm_builder->CreateSub(L, R, "temp");
  else if (is_fp_type(ty)) return llvm_builder->CreateFSub(L, R, "temp");
  else return nullptr;
}


Value* handleMul(Value* L, Value* R, SymbolType ty) {
  if (is_int_type(ty)) return llvm_builder->CreateMul(L, R, "temp");
  else if (is_fp_type(ty)) return llvm_builder->CreateFMul(L, R, "temp");
  else return nullptr;
}

Value* handleDiv(Value* L, Value* R, SymbolType ty) {
  if (is_signed_int_type(ty)) return llvm_builder->CreateSDiv(L, R, "temp");
  else if (is_unsigned_int_type(ty)) return llvm_builder->CreateUDiv(L, R, "temp");
  else if (is_fp_type(ty)) return llvm_builder->CreateFDiv(L, R, "temp");
  else return nullptr;
}

Value* handleGE(Value* L, Value* R, SymbolType ty) {
  if (is_signed_int_type(ty)) return llvm_builder->CreateICmpSGE(L, R, "temp");
  else if (is_unsigned_int_type(ty)) return llvm_builder->CreateICmpUGE(L, R, "temp");
  else if (is_fp_type(ty)) return llvm_builder->CreateFCmpOGE(L, R, "temp");
  else return nullptr;
}

Value* handleGT(Value* L, Value* R, SymbolType ty) {
  if (is_signed_int_type(ty)) return llvm_builder->CreateICmpSGT(L, R, "temp");
  else if (is_unsigned_int_type(ty)) return llvm_builder->CreateICmpUGT(L, R, "temp");
  else if (is_fp_type(ty)) return llvm_builder->CreateFCmpOGT(L, R, "temp");
  else return nullptr;
}

Value* handleLE(Value* L, Value* R, SymbolType ty) {
  if (is_signed_int_type(ty)) return llvm_builder->CreateICmpSLE(L, R, "temp");
  else if (is_unsigned_int_type(ty)) return llvm_builder->CreateICmpULE(L, R, "temp");
  else if (is_fp_type(ty)) return llvm_builder->CreateFCmpOLE(L, R, "temp");
  else return nullptr;
}

Value* handleLT(Value* L, Value* R, SymbolType ty) {
  if (is_signed_int_type(ty)) return llvm_builder->CreateICmpSLT(L, R, "temp");
  else if (is_unsigned_int_type(ty)) return llvm_builder->CreateICmpULT(L, R, "temp");
  else if (is_fp_type(ty)) return llvm_builder->CreateFCmpOLT(L, R, "temp");
  else return nullptr;
}

Value* handleEQ(Value* L, Value* R, SymbolType ty) {
  if (is_int_type(ty)) return llvm_builder->CreateICmpEQ(L, R, "temp");
  else if (is_fp_type(ty)) return llvm_builder->CreateFCmpOEQ(L, R, "temp");
  else return nullptr;
}

Value* handleNE(Value* L, Value* R, SymbolType ty) {
  if (is_int_type(ty)) return llvm_builder->CreateICmpNE(L, R, "temp");
  else if (is_fp_type(ty)) return llvm_builder->CreateFCmpONE(L, R, "temp");
  else return nullptr;
}

Value* handleBAnd(Value* L, Value* R, SymbolType ty) {
  if (is_bool_type(ty)) return llvm_builder->CreateAnd(L, R, "temp");
  else return nullptr;
}

Value* handleBOr(Value* L, Value* R, SymbolType ty) {
  if (is_bool_type(ty)) return llvm_builder->CreateOr(L, R, "temp");
  else return nullptr;
}

Value* handleBNot(Value* L, SymbolType ty){
  if (is_bool_type(ty)) return llvm_builder->CreateNot(L);
  else return nullptr;
}

Value* handleAnd(Value* L, Value* R, SymbolType ty) {
  if (is_int_type(ty)) return llvm_builder->CreateAnd(L, R, "temp");
  else return nullptr;
}

Value* handleOr(Value* L, Value* R, SymbolType ty) {
  if (is_int_type(ty)) return llvm_builder->CreateOr(L, R, "temp");
  else return nullptr;
}

Value* handleXor(Value* L, Value* R, SymbolType ty) {
  if (is_int_type(ty)) return llvm_builder->CreateXor(L, R, "temp");
  else return nullptr;
}

Value* handleLshift(Value* L, Value* R, SymbolType ty){
  if (is_int_type(ty)) return llvm_builder->CreateShl(L, R, "lshifttemp");
  else return nullptr;
}

Value* handleRshift(Value* L, Value* R, SymbolType ty) {
  if (is_signed_int_type(ty)) return llvm_builder->CreateAShr(L, R, "srshifttemp");
  else if (is_unsigned_int_type(ty)) return llvm_builder->CreateLShr(L, R, "urshifttemp");
  else return nullptr;
}

SymbolType typespecs2stg(std::set<TypeSpecifier> type_specs) {
  if (type_specs.find(TS_FLOAT) != type_specs.end()) return FP32;
  if (type_specs.find(TS_DOUBLE) != type_specs.end()) return FP64;
  if (type_specs.find(TS_BOOL) != type_specs.end()) return I1;
  if (type_specs.find(TS_UNSIGNED) != type_specs.end()) {
    if (type_specs.find(TS_SHORT) != type_specs.end()) return U16;
    if (type_specs.find(TS_LONG) != type_specs.end()) return U64;
    return U32;
  }
  if (type_specs.find(TS_CHAR) != type_specs.end()) {
    if (type_specs.find(TS_SIGNED) != type_specs.end()) return I8;
    return U8;
  }
  if (type_specs.find(TS_SHORT) != type_specs.end()) return I16;
  if (type_specs.find(TS_LONG) != type_specs.end()) return I64;
  if (type_specs.find(TS_VOID) != type_specs.end()) return VD;
  if (type_specs.find(TS_BOOL) != type_specs.end()) return I1;
  return I32;
}

std::string getVarName(ast::Identifier *ident, std::string prefix){
  int idx = ident->ident_info.idx;
  return prefix + to_string(abs(idx));
}

llvm::Type* getType(SymbolType ts, int ptr_depth){
  llvm::Type* t;
  switch (ts) {
    case FP32: t = Type::getFloatTy(*llvm_ctx); break;
    case FP64: t = Type::getDoubleTy(*llvm_ctx); break;
    case I1:   t = Type::getInt1Ty(*llvm_ctx); break;
    case I8:   
    case U8:   t = Type::getInt8Ty(*llvm_ctx); break;
    case I16:  
    case U16:  t = Type::getInt16Ty(*llvm_ctx); break;
    case I32:  
    case U32:  t = Type::getInt32Ty(*llvm_ctx); break;
    case I64:  
    case U64:  t = Type::getInt64Ty(*llvm_ctx); break;
    case VD: t = Type::getVoidTy(*llvm_ctx); break;
    default: break;
  }

  while (ptr_depth > 0) {
    t = llvm::PointerType::get(t, 0);
    ptr_depth--;
  }
  return t;
}


int get_rank(SymbolType st) {
  if (st == I1) return 0;
  if ((st == I8) || (st == U8)) return 1;
  if ((st == I16) || (st == U16)) return 2;
  if ((st == I32 || st == U32)) return 3;
  if ((st == I64 || st == U64)) return 4;
  if (st == FP32) return 5;
  if (st == FP64) return 6;
  return -1;
}


Value* widenToFloat(Value* v, SymbolType st, llvm::Type* ty){
  if (st == I1) return llvm_builder->CreateUIToFP(v, ty, "widen");
  if (st == I8) return llvm_builder->CreateSIToFP(v, ty, "widen");
  if (st == U8) return llvm_builder->CreateUIToFP(v, ty, "widen");
  if (st == I32) return llvm_builder->CreateSIToFP(v, ty, "widen");
  if (st == U16) return llvm_builder->CreateUIToFP(v, ty, "widen");
  if (st == I16) return llvm_builder->CreateSIToFP(v, ty, "widen");
  if (st == U32) return llvm_builder->CreateUIToFP(v, ty, "widen");
  if (st == I64) return llvm_builder->CreateSIToFP(v, ty, "widen");
  if (st == U64) return llvm_builder->CreateUIToFP(v, ty, "widen");
  if (st == FP32) return llvm_builder->CreateFPExt(v, ty, "widen");
  if (st == FP64) return llvm_builder->CreateFPExt(v, ty, "widen");
}

Value* widenToSInt(Value* v, SymbolType st, llvm::Type* ty){
  if (st == I1)  return llvm_builder->CreateZExt(v, ty, "widen");
  if (st == I8) return llvm_builder->CreateSExt(v, ty, "widen");
  if (st == U8) return llvm_builder->CreateZExt(v, ty, "widen");
  if (st == U16) return llvm_builder->CreateZExt(v, ty, "widen");
  if (st == I16) return llvm_builder->CreateSExt(v, ty, "widen");
  if (st == I32) return llvm_builder->CreateSExt(v, ty, "widen");
  if (st == U32) return llvm_builder->CreateZExt(v, ty, "widen");
  // if (st == I64) return llvm_builder->CreateSIToFP(v, ty, "widen");
  // if (st == U64) return llvm_builder->CreateUIToFP(v, ty, "widen");
}



Value* widenToUInt(Value* v, SymbolType st, llvm::Type* ty){
  if (st == I1) return llvm_builder->CreateZExt(v, ty, "widen");
  if (st == I8) return llvm_builder->CreateZExt(v, ty, "widen");
  if (st == U8) return llvm_builder->CreateZExt(v, ty, "widen");
  if (st == U16) return llvm_builder->CreateZExt(v, ty, "widen");
  if (st == I16) return llvm_builder->CreateZExt(v, ty, "widen");
  if (st == I32) return llvm_builder->CreateZExt(v, ty, "widen");
  if (st == U32) return llvm_builder->CreateZExt(v, ty, "widen");
}


Value* widenOrNarrowToUInt(Value* v, SymbolType st, llvm::Type* ty){
  if (st == I1) return llvm_builder->CreateZExtOrTrunc(v, ty, "widen");
  if (st == I8) return llvm_builder->CreateZExtOrTrunc(v, ty, "widen");
  if (st == U8) return llvm_builder->CreateZExtOrTrunc(v, ty, "widen");
  if (st == U16) return llvm_builder->CreateZExtOrTrunc(v, ty, "widen");
  if (st == I16) return llvm_builder->CreateZExtOrTrunc(v, ty, "widen");
  if (st == I32) return llvm_builder->CreateZExtOrTrunc(v, ty, "widen");
  if (st == U32) return llvm_builder->CreateZExtOrTrunc(v, ty, "widen");
  if (st == I64) return llvm_builder->CreateZExtOrTrunc(v, ty, "widen");
  if (st == U64) return llvm_builder->CreateZExtOrTrunc(v, ty, "widen");
  if (st == FP32) return llvm_builder->CreateFPToUI(v, ty, "fptoint");
  if (st == FP64) return llvm_builder->CreateFPToUI(v, ty, "fptoint");
}

Value* widenOrNarrowToSInt(Value* v, SymbolType st, llvm::Type* ty){
  if (st == I1) return llvm_builder->CreateZExtOrTrunc(v, ty, "widen");
  if (st == I8) return llvm_builder->CreateSExtOrTrunc(v, ty, "widen");
  if (st == U8) return llvm_builder->CreateZExtOrTrunc(v, ty, "widen");
  if (st == U16) return llvm_builder->CreateZExtOrTrunc(v, ty, "widen");
  if (st == I16) return llvm_builder->CreateSExtOrTrunc(v, ty, "widen");
  if (st == I32) return llvm_builder->CreateSExtOrTrunc(v, ty, "widen");
  if (st == U32) return llvm_builder->CreateZExtOrTrunc(v, ty, "widen");
  if (st == I64) return llvm_builder->CreateSExtOrTrunc(v, ty, "widen");
  if (st == U64) return llvm_builder->CreateZExtOrTrunc(v, ty, "widen");
  if (st == FP32) return llvm_builder->CreateFPToSI(v, ty, "widen");
  if (st == FP64) return llvm_builder->CreateFPToSI(v, ty, "widen");
}


Value* widenOrNarrowToFloat(Value* v, SymbolType st, llvm::Type* ty){
  if (st == I1) return llvm_builder->CreateUIToFP(v, ty, "widen");
  if (st == I8) return llvm_builder->CreateSIToFP(v, ty, "widen");
  if (st == U8) return llvm_builder->CreateUIToFP(v, ty, "widen");
  if (st == I16) return llvm_builder->CreateSIToFP(v, ty, "widen");
  if (st == U16) return llvm_builder->CreateUIToFP(v, ty, "widen");
  if (st == I32) return llvm_builder->CreateSIToFP(v, ty, "widen");
  if (st == U32) return llvm_builder->CreateUIToFP(v, ty, "widen");
  if (st == I64) return llvm_builder->CreateSIToFP(v, ty, "widen");
  if (st == U64) return llvm_builder->CreateUIToFP(v, ty, "widen");
  if (st == FP32) return llvm_builder->CreateFPExt(v, ty, "widen");
  if (st == FP64) return llvm_builder->CreateFPTrunc(v, ty, "widen");
}

// widen if not an assignment

void widen_expression(Expression* lhsexp, Expression* rhsexp, Value* lhsval, Value* rhsval, Value** newlhs, Value** newrhs) {
  if (get_rank(rhsexp->type_info.st.stype) > get_rank(lhsexp->type_info.st.stype)) {
    switch (rhsexp->type_info.st.stype) {
      case FP64:  
        *newlhs = widenToFloat(lhsval, lhsexp->type_info.st.stype, Type::getDoubleTy(*llvm_ctx));
        break;
      case FP32:
        *newlhs = widenToFloat(lhsval, lhsexp->type_info.st.stype, Type::getFloatTy(*llvm_ctx));
        break;
      default:
        if(is_signed_int_type(rhsexp->type_info.st.stype)){
          *newlhs = widenToSInt(lhsval, lhsexp->type_info.st.stype, getType(rhsexp->type_info.st.stype, 0));
        }
        else if(is_unsigned_int_type(rhsexp->type_info.st.stype)){
          *newlhs = widenToUInt(lhsval, lhsexp->type_info.st.stype, getType(rhsexp->type_info.st.stype, 0));
        }
    }
    lhsexp->type_info.st.stype = rhsexp->type_info.st.stype;
    *newrhs = rhsval;
  }
  else if (get_rank(lhsexp->type_info.st.stype) > get_rank(rhsexp->type_info.st.stype)) {
    switch (lhsexp->type_info.st.stype) {
      case FP64:  
        *newrhs = widenToFloat(rhsval, rhsexp->type_info.st.stype, Type::getDoubleTy(*llvm_ctx));
        break;
      case FP32:
        *newrhs = widenToFloat(rhsval, rhsexp->type_info.st.stype, Type::getFloatTy(*llvm_ctx));
        break;
      default:
        if(is_signed_int_type(lhsexp->type_info.st.stype)){
          *newrhs = widenToSInt(rhsval, rhsexp->type_info.st.stype, getType(lhsexp->type_info.st.stype, 0));
          // cout << "widened" << endl;
        }
        else if(is_unsigned_int_type(lhsexp->type_info.st.stype)){
          *newrhs = widenToUInt(rhsval, rhsexp->type_info.st.stype, getType(lhsexp->type_info.st.stype, 0));
        }
    }
    rhsexp->type_info.st.stype = lhsexp->type_info.st.stype;
    *newlhs = lhsval;
  }

}

Value* narrowToBool(Value* rhsval, SymbolType st, llvm::Type* ty){
  if (st == I1) return rhsval;
  if (st == I8) return llvm_builder->CreateICmpNE(rhsval, ConstantInt::get(ty, 0));
  if (st == U8) return llvm_builder->CreateICmpNE(rhsval, ConstantInt::get(ty, 0));
  if (st == I16) return llvm_builder->CreateICmpNE(rhsval, ConstantInt::get(ty, 0));
  if (st == U16) return llvm_builder->CreateICmpNE(rhsval, ConstantInt::get(ty, 0));
  if (st == I32) return llvm_builder->CreateICmpNE(rhsval, ConstantInt::get(ty, 0));
  if (st == U32) return llvm_builder->CreateICmpNE(rhsval, ConstantInt::get(ty, 0));
  if (st == I64) return llvm_builder->CreateICmpNE(rhsval, ConstantInt::get(ty, 0));
  if (st == U64) return llvm_builder->CreateICmpNE(rhsval, ConstantInt::get(ty, 0));
  if (st == FP32) return llvm_builder->CreateFCmpONE(rhsval, ConstantFP::get(ty, 0.0));
  if (st == FP64) return llvm_builder->CreateFCmpONE(rhsval, ConstantFP::get(ty, 0.0));
  // if (st == FP64) return llvm_builder->CreateFPTrunc(v, ty, "widen");
}

Value* convertForAssignment(Expression* lhsexp, Expression* rhsexp, Value* rhsval) {
  Value* newrhs;
  switch (lhsexp->type_info.st.stype) {
      case FP64:  
        newrhs =  widenOrNarrowToFloat(rhsval, rhsexp->type_info.st.stype, Type::getDoubleTy(*llvm_ctx));
        break;
      case FP32:
        newrhs = widenOrNarrowToFloat(rhsval, rhsexp->type_info.st.stype, Type::getFloatTy(*llvm_ctx));
        break;
      default:
        if(is_signed_int_type(lhsexp->type_info.st.stype)){
          newrhs = widenOrNarrowToSInt(rhsval, rhsexp->type_info.st.stype, getType(lhsexp->type_info.st.stype, 0));
        }
        else if(is_unsigned_int_type(lhsexp->type_info.st.stype)){
          newrhs = widenOrNarrowToUInt(rhsval, rhsexp->type_info.st.stype, getType(lhsexp->type_info.st.stype, 0));
        }
        else if(is_bool_type(lhsexp->type_info.st.stype)){
          newrhs = narrowToBool(rhsval, rhsexp->type_info.st.stype, getType(rhsexp->type_info.st.stype, 0));
        }
  }
  return newrhs;
}

Value* convertForInit(SymbolType lhstype, Expression* rhsexp, Value* rhsval) {
  Value* newrhs;
  switch (lhstype) {
      case FP64:  
        newrhs =  widenOrNarrowToFloat(rhsval, rhsexp->type_info.st.stype, Type::getDoubleTy(*llvm_ctx));
        break;
      case FP32:
        newrhs = widenOrNarrowToFloat(rhsval, rhsexp->type_info.st.stype, Type::getFloatTy(*llvm_ctx));
        break;
      default:
        if(is_signed_int_type(lhstype)){
          newrhs = widenOrNarrowToSInt(rhsval, rhsexp->type_info.st.stype, getType(lhstype, 0));
        }
        else if(is_unsigned_int_type(lhstype)){
          newrhs = widenOrNarrowToUInt(rhsval, rhsexp->type_info.st.stype, getType(lhstype, 0));
        }
         else if(is_bool_type(lhstype)){
          newrhs = narrowToBool(rhsval, rhsexp->type_info.st.stype, getType(rhsexp->type_info.st.stype, 0));
        }
        
  }
  return newrhs;
}



Constant* getDefaultInitializer(SymbolType ts, int ptr_depth){
  llvm::Type* t;
  if (ptr_depth > 0) return ConstantPointerNull::get(PointerType::get(*llvm_ctx, 0));
  switch (ts) {
    case FP32: return ConstantFP::get(llvm::Type::getFloatTy(*llvm_ctx), APFloat(0.0));
    case FP64: return ConstantFP::get(llvm::Type::getDoubleTy(*llvm_ctx), APFloat(0.0));
    case I1:   return ConstantInt::get(*llvm_ctx, APInt(1, 0));
    case I8:   
    case U8:   return ConstantInt::get(*llvm_ctx, APInt(8, 0));
    case I16:  
    case U16:  return ConstantInt::get(*llvm_ctx, APInt(16, 0));
    case I32:  
    case U32:  return ConstantInt::get(*llvm_ctx, APInt(32, 0));
    case I64:  
    case U64:  return ConstantInt::get(*llvm_ctx, APInt(64, 0));
    default: return nullptr;
  }
}

int getTypeSize(SymbolType ts, int ptr_depth) {
  if (ptr_depth > 0) return 8;
  switch (ts) {
    case FP32: return 4;
    case FP64: return 8;
    case I8:   
    case U8:   return 1;
    case I16:  
    case U16:  return 2;
    case I32:  
    case U32:  return 4;
    case I64:  
    case U64:  return 8;
    default: return 1;
  }
}


AllocaInst *CreateEntryBlockAlloca(llvm::Function *func, DeclarationSpecifiers* decl_specs, int ptr_depth, Identifier* ident) {
  llvm::IRBuilder<> TmpB(&func->getEntryBlock(), func->getEntryBlock().begin());
  return TmpB.CreateAlloca(getType(typespecs2stg(decl_specs->type_specs), ptr_depth), nullptr, getVarName(ident, "l"));
}

llvm::Value* DeclarationStatement::codegen(){
  return decl->codegen();
}

llvm::Value* Declaration::codegen(){

  llvm::Function *func = llvm_builder->GetInsertBlock()->getParent();
  for(auto init_decl: *decl_list){
    AllocaInst* A = CreateEntryBlockAlloca(func, decl_specs, init_decl->ptr_depth, init_decl->ident);
    if(init_decl->init_expr){
      // cout << "START DECL\n";
      Value* init_val = init_decl->init_expr->codegen();
      // cout << "EXPR GEN\n";
      if ((init_decl->init_expr->type_info.st.stype != init_decl->ident->ident_info.stype) 
          && !(init_decl->init_expr->type_info.st.ptr_depth) && !(init_decl->ptr_depth)){
          // cout << "IF" << endl;

          Value* newval = convertForInit(init_decl->ident->ident_info.stype, init_decl->init_expr, init_val);
          llvm_builder->CreateStore(newval, A);
      }
      else{
        // cout << "ELSE" << endl;
        llvm_builder->CreateStore(init_val, A);
      }
    }
    // cout << "CONVERTED\n";
    llvm_st[getVarName(init_decl->ident, "l") ] = A;
    // cout << "DONE DECL" << endl;
  }

  return nullptr;
}


llvm::Value* DeclarationStatement::globalgen(){
  return decl->globalgen();
}

llvm::Value* Declaration::globalgen(){
  // cout<<"HI"<<endl;
  GlobalVariable* A;
  for(auto init_decl: *decl_list){
    Type* t = getType(typespecs2stg(decl_specs->type_specs), init_decl->ptr_depth);
    int tsize = getTypeSize(typespecs2stg(decl_specs->type_specs), init_decl->ptr_depth);
    A = new llvm::GlobalVariable(*llvm_mod, t, false, llvm::GlobalValue::ExternalLinkage, 0, init_decl->ident->name);

    if(init_decl->init_expr) {
      Literal *l;
      if (!(l = dynamic_cast<Literal*>(init_decl->init_expr))) {
        cout << "ERROR: globals can only take constant values" << endl;
        return nullptr;
      }
      Constant* init_val = l->codegen();
      A->setInitializer(init_val);
    }
    else {
      Constant* init = getDefaultInitializer(typespecs2stg(decl_specs->type_specs), init_decl->ptr_depth);
      if (init) A->setInitializer(init);
      else ehdl::err("Could not initialize global value to default value", pos);
    }
    A->setAlignment(MaybeAlign(tsize));
    global_st[init_decl->ident->name ] = A;
  }

  return A;
}

std::string TranslationUnit::codegen() {
  llvm_ctx = std::make_unique<llvm::LLVMContext>();
  llvm_mod = std::make_unique<llvm::Module>("Code Generator", *llvm_ctx);

  // Create a new builder for the module.
  llvm_builder = std::make_unique<llvm::IRBuilder<>>(*llvm_ctx);

  DeclarationStatement* decl_ptr;
  for (auto node_ptr: *nodes){
    // cdebug<<"HI1"<<endl;
    if ((decl_ptr = dynamic_cast<DeclarationStatement*>(node_ptr))) {
      // cout<<"READ FUNC DEF"<<endl;
      Value* v = decl_ptr->globalgen();
      // llvm_mod->print(errs(), nullptr); // print decls
      // v->print(errs());
      // func_ir->print(errs());
    }
  }

  Function* func_ptr;
  for (auto node_ptr: *nodes){
    if ((func_ptr = dynamic_cast<Function*>(node_ptr))) {
      // cout<<"READ FUNC DEF"<<endl;
      llvm::Function* func_ir = func_ptr->codegen();
      // func_ir->print(errs(), nullptr);
    }
  }

  std::string str;
  raw_string_ostream os(str);
  os << *llvm_mod;
  os.flush();
  return os.str();
}

Value* Expression::codegen(){
  ConstantInt::get(*llvm_ctx, APInt(32, 0));
}

Value* Statement::codegen(){
  cout<<"virtual"<<endl;
  return ConstantInt::get(*llvm_ctx, APInt(32, 0));
}

Value* ExpressionStatement::codegen(){
  return expr->codegen();
}

Constant* Literal::codegen() {
  int v;
  float f;
  type_info.is_ref = false;
  type_info.st.ptr_depth = 0;
  llvm::ArrayType* stringType;
  llvm::Constant* stringLiteral;
  llvm::GlobalVariable* stringVar;
  std::vector<llvm::Value *> values;
  llvm::APInt zero(32, 0);
  llvm::APInt one(32, 0);
  switch(ltype) {
    case LT_BOOL:
      type_info.st.stype = I1;
      return ConstantInt::get(*llvm_ctx, APInt(1, data.i));
    case LT_INT32:
      type_info.st.stype = I32;
      return ConstantInt::get(*llvm_ctx, APInt(32, data.i));
    case LT_UINT32:
      type_info.st.stype = U32;
      return ConstantInt::get(*llvm_ctx, APInt(32, data.i));
    case LT_INT64:
      type_info.st.stype = I64;
      return ConstantInt::get(*llvm_ctx, APInt(64, data.l));
    case LT_UINT64:
      type_info.st.stype = U64;
      return ConstantInt::get(*llvm_ctx, APInt(64, data.l));
    case LT_SHORT:
      type_info.st.stype = I16;
      return ConstantInt::get(*llvm_ctx, APInt(16, data.i));
    case LT_CHAR:
      type_info.st.stype = I8;
      return ConstantInt::get(*llvm_ctx, APInt(8, data.c));
    case LT_FLOAT:
      type_info.st.stype = FP32;
      return ConstantFP::get(llvm::Type::getFloatTy(*llvm_ctx), APFloat(data.f));
    case LT_DOUBLE:
      type_info.st.stype = FP64;
      return ConstantFP::get(llvm::Type::getDoubleTy(*llvm_ctx), APFloat(data.d));
    case LT_INT_LIKE:
      cout << "ERROR: should have parsed int_like by now" << endl;
    case LT_FLOAT_LIKE:
      cout << "ERROR: should have parsed float_like by now" << endl;
    case LT_STRING:
      type_info.st.stype = U8;
      type_info.st.ptr_depth = 1;
      return llvm_builder->CreateGlobalStringPtr(value, ".str");
    default:
      cout<<"invalid literal"<<endl;
      return nullptr;
  }
}



Value* Identifier::get_address(){
  type_info.st = ident_info;
  type_info.is_ref = true;

  AllocaInst* A;
  int idx = ident_info.idx;
  if(idx < 0){
    // cout<<idx<<"GLOBAL"<<endl;
    GlobalVariable* A = global_st[ name/*getVarName(this, "g")*/];
    type_info.st= ident_info;
    type_info.is_ref = true;
    return A;
  }
  else{
    A = llvm_st["l" + to_string(idx)];
  }
 
  type_info.st = ident_info;
  type_info.is_ref = true;
  return A;

}


Value *BinaryExpression::codegen() {
  if (op == OP_ASSIGN){

 
    Value* R = rhs->codegen();
    Value* V = (lhs->get_address());
    Value* A = V;
    type_info.st = lhs->type_info.st;
    type_info.is_ref = false;                       
    if(( get_rank(lhs->type_info.st.stype) == get_rank(rhs->type_info.st.stype)) && (lhs->type_info.st.ptr_depth == rhs->type_info.st.ptr_depth) && (lhs->type_info.is_ref)){   // comparing rank as they are internally the same type
      llvm_builder->CreateStore(R, A);
      return R;
    }
    else if ((rhs->type_info.st.ptr_depth == 0) && (lhs->type_info.st.ptr_depth == 0) && lhs->type_info.is_ref){       // widening/narrowing only if rhs is not a pointer
      Value* newrhs = convertForAssignment(lhs, rhs, R);
      rhs->type_info.st = lhs->type_info.st;
      llvm_builder->CreateStore(newrhs, A);
      return newrhs;    // should i return newrhs or R. if R then don't update type info of expression ig.
    }
    else{      
      cout<<"Assignment type mismatch";
      return nullptr;
    }
  }

  Value *oldL = lhs->codegen();
  // cout<<"lhs gened" << endl;
  Value *oldR = rhs->codegen();
  // cout<<"rhs gened" << endl;
  // TODO type check. error if types don't match

  if (!oldL || !oldR)
    return nullptr;

  // handle integer + pointer

  Value* R, *L;   // new L and R

  if(lhs->type_info.st.stype != rhs->type_info.st.stype || lhs->type_info.st.ptr_depth != rhs->type_info.st.ptr_depth || lhs->type_info.st.ptr_depth != 0){
    if (lhs->type_info.st.ptr_depth == 0 &&  rhs->type_info.st.ptr_depth == 0) {
      widen_expression(lhs, rhs, oldL, oldR, &L, &R);
    }
    else{
      cout<<"Type mismatch"<<endl;
      return nullptr;
    }
  }
  else{
    R = oldR;
    L = oldL;
  }

  // if (L) {
  //   cout << "lhs exists";
  // }

  // if(R) {
  //   cout << "rhs exists";
  // }

  type_info.is_ref = false;
  type_info.st.ptr_depth = 0;    // for now
  switch (op) {
    case OP_ADD:
      type_info.st.stype = lhs->type_info.st.stype;
      return handleAdd(L, R, lhs->type_info.st.stype);
    case OP_SUB:
      type_info.st.stype = lhs->type_info.st.stype;
      return handleSub(L, R, lhs->type_info.st.stype);
    case OP_MUL:
      type_info.st.stype = lhs->type_info.st.stype;
      return handleMul(L, R, lhs->type_info.st.stype);
    case OP_DIV:
      type_info.st.stype = lhs->type_info.st.stype;
      return handleDiv(L, R, lhs->type_info.st.stype);
    case OP_LSHIFT:
      type_info.st.stype = lhs->type_info.st.stype;
      return handleLshift(L, R, lhs->type_info.st.stype);
    case OP_RSHIFT:
      type_info.st.stype = lhs->type_info.st.stype;
      return handleRshift(L, R, lhs->type_info.st.stype);
    case OP_GE:
      type_info.st.stype = I1;
      return handleGE(L, R, lhs->type_info.st.stype);
    case OP_GT:
      type_info.st.stype = I1;
      return handleGT(L, R, lhs->type_info.st.stype);
    case OP_LE:
      type_info.st.stype = I1;
      return handleLE(L, R, lhs->type_info.st.stype);
    case OP_LT:
      type_info.st.stype = I1;
      return handleLT(L, R, lhs->type_info.st.stype);
    case OP_EQ:
      type_info.st.stype = I1;
      return handleEQ(L, R, lhs->type_info.st.stype);
    case OP_NE:
      type_info.st.stype = I1;
      return handleNE(L, R, lhs->type_info.st.stype);
    case OP_BOOL_AND:
      type_info.st.stype = I1;
      return handleBAnd(L, R, lhs->type_info.st.stype);
    case OP_BOOL_OR:
      type_info.st.stype = I1;
      return handleBOr(L, R, lhs->type_info.st.stype);
    case OP_AND:
      type_info.st.stype = lhs->type_info.st.stype;
      return handleAnd(L, R, lhs->type_info.st.stype);
    case OP_OR:
      type_info.st.stype = lhs->type_info.st.stype;
      return handleOr(L, R, lhs->type_info.st.stype);
    case OP_XOR:
      type_info.st.stype = lhs->type_info.st.stype;
      return handleXor(L, R, lhs->type_info.st.stype);
    default:
      cout<<"OP NOT FOUND"<<endl;
      return nullptr;
  }
  // TODO code calling this should also do error checking!
}

Value* UnaryExpression::codegen(){
  Value* R;
  switch (op){
    case OP_DEREF:
      // cdebug<<"HAW"<<endl;
      R = expr->codegen();
      type_info= expr->type_info;
      type_info.st.ptr_depth--;  // check if less than zero
      type_info.is_ref = true;
      // removed the getElementType, segfaults now though
      // also changed loc to idx (more descriptive) 
      // llvm::cast<llvm::PointerType>(R->getType()))->getElementType()
      return llvm_builder->CreateLoad(getType(type_info.st.stype, type_info.st.ptr_depth), R, "dereftemp");
    case OP_AND:
      R = expr->get_address();
      type_info= expr->type_info;
      type_info.st.ptr_depth++;  
      type_info.is_ref = false;
      return R;
    default:
      cout<<"Type mismatch"<<endl;
      return nullptr;
  }

}

Value* Expression::get_address(){
  cout<<"get address of non ref type"<<endl;
  return nullptr;
}

Value* UnaryExpression::get_address(){
  Value* R;
  switch (op){
    case OP_DEREF:
      R = expr->codegen();
      type_info= expr->type_info;
      type_info.st.ptr_depth--;  // check if less than zero
      type_info.is_ref = true;
      return R;
    default:
      cout<<"Type mismatch"<<endl;
      return nullptr;
  }
}

Value* ReturnStatement::codegen(){
  Value* ret_val = ret_expr->codegen();
  return llvm_builder->CreateRet(ret_val);

}


Value* BlockStatement::codegen(){
  Value* v;
  for (auto stmt = begin(); stmt != end(); ++stmt) {
        v = (*stmt)->codegen();
        if(dynamic_cast<ReturnStatement*> (*stmt)){
          return nullptr;                                  // if there is a return statement return nullptr else return v. this might create problems when we do error handling.
        }
    }
  return (Value*) 1;
}

llvm::Value* FunctionInvocationExpression::codegen(){
  // cout<<"num args don't match"<<endl;
  int num_params = 0;
  if(params){
    num_params = params->size();
  }

 // Type should be assigned during scopify
  Identifier* ident = (Identifier*) fn;               // is it fine to call codegen func here? nope
  llvm::Function* func = llvm_mod->getFunction(ident->name);

  type_info.is_ref = false;
  type_info.st = ident->ident_info;          
  // cdebug<<"Function invocation with type "<<fn->type_info.st.stype<<endl;
  if (func->arg_size() > num_params || ((func->arg_size() < num_params) && (!func->isVarArg())) ){
    cout<<"num args don't match"<<endl;
    return nullptr;
  }


  vector<Value*> argsV;
  for(int i = 0; i < num_params; i++){
    argsV.push_back((*params)[i]->codegen());                   // type check arg types
  }
  // cout<<"pushed params"<<endl;
  if(func->getFunctionType()->getReturnType()->isVoidTy()){
    return llvm_builder->CreateCall(func, argsV);
  }
  return llvm_builder->CreateCall(func, argsV, "calltmp");
}


llvm::Function *Function::codegen() {
  // Uncommenting this causes a segfault !?
  // cdebug << "Generating function code " << func_decl->ident->name << endl;
  std::string func_name = func_decl->ident->name;
  int num_args = 0;
  if(params){
    num_args = params->params->size();
  }
  llvm::Function *func;
  if(func_st.find(func_name) == func_st.end()){
    
    std::vector<llvm::Type*> argtypes(num_args);
    for(int i = 0; i < num_args; i++){
      PureDeclaration* decl = (*(params->params))[i];
      argtypes[i] = getType(typespecs2stg(decl->decl_specs->type_specs), decl->ptr_depth);
    }
    bool flag = false;
    if(params){
      if(params->has_varargs){
        flag = true;               //varargs
      }
    }
    FunctionType *func_type = FunctionType::get(getType(typespecs2stg(func_decl->decl_specs->type_specs), func_decl->ptr_depth), argtypes, flag);
    
    func = llvm::Function::Create(func_type, llvm::Function::ExternalLinkage, func_name, llvm_mod.get());

    int i=0;
    for (auto &Arg : func->args()){
      Arg.setName(getVarName((*(params->params))[i]->ident, "v"));                // arg vals -> v, global vars -> g, local vars -> l 
      i++;
    }

    func_st[func_name] = func;
    
  }
  else{
    func = func_st[func_name];              // TODO check for redeclaration
  }
  

  if (stmts) {
    BasicBlock *block = BasicBlock::Create(*llvm_ctx, "entry", func);
    llvm_builder->SetInsertPoint(block);
    llvm_st.clear();
    int i = 0;
  for (auto &Arg : func->args()) {

      AllocaInst *Alloca = CreateEntryBlockAlloca(func, (*(params->params))[i]->decl_specs,  (*(params->params))[i]->ptr_depth , (*(params->params))[i]->ident);

      llvm_builder->CreateStore(&Arg, Alloca);

    
      llvm_st[getVarName((*(params->params))[i]->ident, "l")] = Alloca;
      i++;
    }
    
    Value *ret_val = stmts->codegen();
    if(ret_val){
      // add a return statement
      if(typespecs2stg(func_decl->decl_specs->type_specs) == VD){
        llvm_builder->CreateRetVoid();
      }
      else{
        AllocaInst *Alloca = CreateEntryBlockAlloca(func, func_decl->decl_specs, func_decl->ptr_depth , func_decl->ident);
        Value* v = llvm_builder->CreateLoad(getType(typespecs2stg(func_decl->decl_specs->type_specs), func_decl->ptr_depth), Alloca, "returnval");
        llvm_builder->CreateRet(v);
      }
    }
    if (/* Value *ret_val = stmts->codegen() */true) {           // if and while return nullptr for now. change during error handling maybe

      verifyFunction(*func);
      return func;
    }
    func->eraseFromParent();
  }
  else{
    return func;
  }
  return nullptr;
}

Value* Identifier::codegen(){
  // assuming undeclared variables handled in scopify
  AllocaInst* A;
  int idx = ident_info.idx;
  if(idx < 0){
    cout<<idx<<"GLOVAL"<<endl;
    GlobalVariable* A = global_st[ name /*getVarName(this, "g")*/];
    type_info.st= ident_info;
    type_info.is_ref = true;
    return llvm_builder->CreateLoad(A->getValueType(), A, name);
  }
  else{
    A = llvm_st["l" + to_string(idx)];
  }


 
  type_info.st = ident_info;
  type_info.is_ref = true;
  return llvm_builder->CreateLoad(A->getAllocatedType(), A, name);
} 



// control flow

Value *IfStatement::codegen() {
  Value *CondV = cond->codegen();
  CondV = narrowToBool(CondV, cond->type_info.st.stype, getType(cond->type_info.st.stype, 0));
  cond->type_info.st.stype = I1;
  if (!CondV || cond->type_info.st.stype != I1) // sus
    return nullptr;


  llvm::Function *func = llvm_builder->GetInsertBlock()->getParent();

  // Create blocks for the then and else cases.  Insert the 'then' block at the
  // end of the function.
 

  if (true_branch) {
    BasicBlock *ThenBB = BasicBlock::Create(*llvm_ctx, "then", func);
    BasicBlock *ElseBB = BasicBlock::Create(*llvm_ctx, "else");
    BasicBlock *MergeBB = BasicBlock::Create(*llvm_ctx, "ifcont");
    llvm_builder->CreateCondBr(CondV, ThenBB, ElseBB);

    llvm_builder->SetInsertPoint(ThenBB);

    if (true_branch) {
      Value *ThenV = true_branch->codegen();
    }

    llvm_builder->CreateBr(MergeBB);
    ThenBB = llvm_builder->GetInsertBlock();

    func->getBasicBlockList().push_back(ElseBB);
    llvm_builder->SetInsertPoint(ElseBB);
    Value *ElseV = nullptr;
    if(false_branch){
      cdebug << "non empty else" << endl;
      ElseV = false_branch->codegen();
    }
    else{
      cdebug << "empty else" << endl;
    }

    

    llvm_builder->CreateBr(MergeBB);
    ElseBB = llvm_builder->GetInsertBlock();

    func->getBasicBlockList().push_back(MergeBB);
    llvm_builder->SetInsertPoint(MergeBB);
  }
  else{
    Value *ElseV = nullptr;
    if(false_branch){
      cdebug << "non empty else" << endl;
      ElseV = false_branch->codegen();
    }
    else{
      cdebug << "empty else" << endl;
    }
  }

  
  // PHINode *PN = llvm_builder->CreatePHI(Type::getDoubleTy(*TheContext), 2, "iftmp");

  // PN->addIncoming(ThenV, ThenBB);
  // PN->addIncoming(ElseV, ElseBB);               // PHINODE?
  return nullptr;                                                   // might be a problem during error handling
}


Value* WhileStatement::codegen(){
  llvm::Function *func = llvm_builder->GetInsertBlock()->getParent();
  // BasicBlock *Cond = llvm_builder->GetInsertBlock();

  if (stmt) {
    BasicBlock *CondBB = BasicBlock::Create(*llvm_ctx, "loop_cond", func);
    BasicBlock *LoopBB = BasicBlock::Create(*llvm_ctx, "loop_body", func);
    BasicBlock *AfterBB = BasicBlock::Create(*llvm_ctx, "afterloop", func);

    // Insert an explicit fall through from the current block to the LoopBB.
    llvm_builder->CreateBr(CondBB);

    // Start insertion in LoopBB.
    llvm_builder->SetInsertPoint(CondBB);

    // Compute the end condition.
    Value *Cond = cond->codegen();
    llvm_builder->CreateCondBr(Cond, LoopBB, AfterBB);

    llvm_builder->SetInsertPoint(LoopBB);

    stmt->codegen();
    llvm_builder->CreateBr(CondBB);

    llvm_builder->SetInsertPoint(AfterBB);
  }
  else{
    cond->codegen();
  }

 
  return nullptr;
}



}
