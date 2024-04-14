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
#include "codegen.hpp"
#include "symtab.hpp"
#include "expressiongen.hpp"
#include <map>
#include "ast.hpp"
#include "debug.hpp"

int intLiteralToInt(string &s){
  return stoi(s);
};

double floatLiteralToFloat(string &s){
  cout<<"Entered stod "+ s<<endl;
  double temp = stod(s);
  cout<<"temp "<<temp<<endl;
  return temp;
}

using namespace llvm;


namespace ast {

 std::unique_ptr<llvm::LLVMContext> llvm_ctx;
 std::unique_ptr<llvm::Module> llvm_mod;
 std::unique_ptr<llvm::IRBuilder<>> llvm_builder;
 std::map<std::string, AllocaInst*> llvm_st;
 std::map<std::string, AllocaInst*> global_st;



SymbolType typespecs2stg(std::set<TypeSpecifier> type_specs) {
  if (type_specs.find(TS_FLOAT) != type_specs.end()) return FP32;
  if (type_specs.find(TS_DOUBLE) != type_specs.end()) return FP64;
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
  return I32;
}



std::string getVarName(ast::Identifier *ident, std::string prefix){
  int location = ident->ident_info.pos;
  return prefix + to_string(location);
}



llvm::Type* getType(DeclarationSpecifiers* decl_specs, int ptr_depth){
  SymbolType ts= typespecs2stg(decl_specs->type_specs);
  llvm::Type* t;
  switch (ts)
  {
  case FP32:
    t = Type::getFloatTy(*llvm_ctx);
    break;
  case FP64:
    t = Type::getDoubleTy(*llvm_ctx);
    break;
  case I32:
    t = Type::getInt32Ty(*llvm_ctx);
    break;
  case I64:
    t = Type::getInt64Ty(*llvm_ctx);
    break;
  case U8:
    t = Type::getInt8Ty(*llvm_ctx);
    break;
  
  
  default:
    break;
  }

  while(ptr_depth>0){
    t = llvm::PointerType::get(t, 0);
    ptr_depth--;
  }
  return t;
}


AllocaInst *CreateEntryBlockAlloca(llvm::Function *func, DeclarationSpecifiers* decl_specs, int ptr_depth, Identifier* ident) {
  llvm::IRBuilder<> TmpB(&func->getEntryBlock(), func->getEntryBlock().begin());
  return TmpB.CreateAlloca(getType(decl_specs, ptr_depth), nullptr, getVarName(ident, "l"));
}

llvm::Value* DeclarationStatement::codegen(){
  return decl->codegen();
}

llvm::Value* Declaration::codegen(){

  llvm::Function *func = llvm_builder->GetInsertBlock()->getParent();
  for(auto init_decl: *decl_list){
    AllocaInst* A = CreateEntryBlockAlloca(func, decl_specs, init_decl->ptr_depth, init_decl->ident);
    if(init_decl->init_expr){
      Value* init_val = init_decl->init_expr->codegen();
      llvm_builder->CreateStore(init_val, A);
    }
    llvm_st[getVarName(init_decl->ident, "l") ] = A;
  }

  return nullptr;
}

void TranslationUnit::codegen() {
  llvm_ctx = std::make_unique<llvm::LLVMContext>();
  llvm_mod = std::make_unique<llvm::Module>("Code Generator", *llvm_ctx);

  // Create a new builder for the module.
  llvm_builder = std::make_unique<llvm::IRBuilder<>>(*llvm_ctx);

  Function* func_ptr;
  for (auto node_ptr: *nodes){
    if ((func_ptr = dynamic_cast<Function*>(node_ptr))) {
      // cout<<"READ FUNC DEF"<<endl;
      llvm::Function* func_ir = func_ptr->codegen();
      func_ir->print(errs());
    }
  }
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

Value* Literal::codegen() {
  int v;
  float f;
  type_info.is_ref = 0;
  type_info.ptr_depth = 0;
  switch(ltype) {
        case LT_INT_LIKE:
            v = intLiteralToInt(value);
            type_info.type = I32;
            return ConstantInt::get(*llvm_ctx, APInt(32, v));
            break;
            // break;
        case LT_INT64:
            v = intLiteralToInt(value);
            type_info.type = I64;
            return ConstantInt::get(*llvm_ctx, APInt(64, v));
        case LT_FLOAT:
            type_info.type = FP32;
            return ConstantFP::get(llvm::Type::getFloatTy(*llvm_ctx), APFloat(floatLiteralToFloat(value)));
        case LT_DOUBLE:
            type_info.type = FP64;
            return ConstantFP::get(llvm::Type::getDoubleTy(*llvm_ctx), APFloat(floatLiteralToFloat(value)));
          case LT_FLOAT_LIKE:
            type_info.type = FP32;
            return ConstantFP::get(llvm::Type::getFloatTy(*llvm_ctx), APFloat(floatLiteralToFloat(value)));
        default:
            cout<<"invalid literal"<<endl;
            return nullptr;
  }
}

Value* Expression::assign(Expression* rhs){
  cout<<"ASSIGNMENT TO NON REFERENCE TYPE"<<endl;
  return nullptr;
}

// write assign for unary expressions

Value* Identifier::assign(Expression* rhs){
  Value* R = rhs->codegen();
  type_info.type = ident_info.stype;
  type_info.ptr_depth = 0;   // REMOVE LATER, should resolve this during scopify
  type_info.is_ref = true;

  if((rhs->type_info.ptr_depth == type_info.ptr_depth) && (rhs->type_info.type == type_info.type)){
    return llvm_builder->CreateStore(R, llvm_st[getVarName(this, "l")]);   // should we return this or value* of rhs? for chain assignments
  }
  else{
    cout<<"ASSIGNMENT TYPE MISMATCH"<<endl;
    return nullptr;
  }

}

Value *BinaryExpression::codegen() {
  if (op == OP_ASSIGN){
    Value* a = lhs->assign(rhs);
    if(a){
      type_info.type = rhs->type_info.type;
      type_info.is_ref = false;
      type_info.ptr_depth = rhs->type_info.ptr_depth;
    }
    return a;
  }
  Value *L = lhs->codegen();
  Value *R = rhs->codegen();
  // TODO type check. error if types don't match

  if (!L || !R)
    return nullptr;

  if(lhs->type_info.type != rhs->type_info.type || lhs->type_info.ptr_depth != rhs->type_info.ptr_depth || lhs->type_info.ptr_depth != 0){
    cout<<"Type mismatch"<<endl;
    return nullptr;
  }

  type_info.is_ref = false;
  type_info.ptr_depth = 0;

  switch (op) {
    case OP_ADD:
      type_info.type = lhs->type_info.type;
      return handleAdd(L, R, lhs->type_info.type);
    case OP_SUB:
      type_info.type = lhs->type_info.type;
      return handleSub(L, R, lhs->type_info.type);
    case OP_MUL:
      type_info.type = lhs->type_info.type;
      return handleMul(L, R, lhs->type_info.type);
    case OP_DIV:
      type_info.type = lhs->type_info.type;
      return handleDiv(L, R, lhs->type_info.type);
    case OP_GE:
      type_info.type = I1;
      return handleGTE(L, R, lhs->type_info.type);
    case OP_GT:
      type_info.type = I1;
      return handleGT(L, R, lhs->type_info.type);
    case OP_LE:
      type_info.type = I1;
      return handleLTE(L, R, lhs->type_info.type);
    case OP_LT:
      type_info.type = I1;
      return handleLT(L, R, lhs->type_info.type);
    case OP_EQ:
      type_info.type = I1;
      return handleEQ(L, R, lhs->type_info.type);
    case OP_NE:
      type_info.type = I1;
      return handleNE(L, R, lhs->type_info.type);
    case OP_BOOL_AND:
      type_info.type = I1;
      return handleBAnd(L, R, lhs->type_info.type);
    case OP_BOOL_OR:
      type_info.type = I1;
      return handleBOr(L, R, lhs->type_info.type);
    case OP_AND:
      type_info.type = lhs->type_info.type;
      return handleAnd(L, R, lhs->type_info.type);
    case OP_OR:
      type_info.type = lhs->type_info.type;
      return handleOr(L, R, lhs->type_info.type);
    case OP_XOR:
      type_info.type = lhs->type_info.type;
      return handleXor(L, R, lhs->type_info.type);
    default:
      cout<<"OP NOT FOUND"<<endl;
  }
}

Value* ReturnStatement::codegen(){
  Value* ret_val = ret_expr->codegen();
  llvm_builder->CreateRet(ret_val);

}


Value* BlockStatement::codegen(){
  Value* v;
  for (auto stmt = begin(); stmt != end(); ++stmt) {
        v = (*stmt)->codegen();
        // if(!v){
        //   return nullptr;
        // }
    }
  return v;
}

llvm::Value* FunctionInvocationExpression::codegen(){
  // cout<<"num args don't match"<<endl;
  int num_params = 0;
  if(params){
    num_params = params->size();
  }

 // Type should be assigned during scopify
  Identifier* ident = (Identifier*) fn;
  llvm::Function* func = llvm_mod->getFunction(ident->name);

  if (func->arg_size() != num_params){
    cout<<"num args don't match"<<endl;
    return nullptr;
  }


  vector<Value*> argsV;
  for(int i = 0; i < num_params; i++){
    argsV.push_back((*params)[i]->codegen());
  }
  
  return llvm_builder->CreateCall(func, argsV, "calltmp");
}


llvm::Function *Function::codegen() {
  // Uncommenting this causes a segfault !?
  // cdebug << "Generating function code " << func_decl->ident->name << endl;
  
  int num_args = 0;
  if(params){
    num_args = params->params->size();
  }

  std::vector<llvm::Type*> argtypes(num_args);
  for(int i = 0; i < num_args; i++){
    argtypes[i] = getType((*(params->params))[i]->decl_specs, (*(params->params))[i]->ptr_depth);  // TODO
  }
  FunctionType *func_type = FunctionType::get(getType(func_decl->decl_specs, func_decl->ptr_depth), argtypes, false);
  std::string func_name = func_decl->ident->name;
  llvm::Function *func = llvm::Function::Create(func_type, llvm::Function::ExternalLinkage, func_name, llvm_mod.get());

  int i=0;
  for (auto &Arg : func->args()){
    Arg.setName(getVarName((*(params->params))[i]->ident, "v"));                // arg vals -> v, global vars -> g, local vars -> l 
    i++;
  }


  BasicBlock *block = BasicBlock::Create(*llvm_ctx, "entry", func);
  llvm_builder->SetInsertPoint(block);
  llvm_st.clear();
  i = 0;
  for (auto &Arg : func->args()) {

    AllocaInst *Alloca = CreateEntryBlockAlloca(func, (*(params->params))[i]->decl_specs,  (*(params->params))[i]->ptr_depth , (*(params->params))[i]->ident);

    llvm_builder->CreateStore(&Arg, Alloca);

  
    llvm_st[getVarName((*(params->params))[i]->ident, "l")] = Alloca;
    i++;
  }
  cdebug << "Generating function code " << func_decl->ident->name << endl;
  // for (auto &Arg : func->args())
  //   llvm_st[std::string(Arg.getName())] = &Arg;
  if (Value *ret_val = stmts->codegen()) {
    // llvm_builder->CreateRet(ret_val);
    // creating return statement in retrunstatement->codegen() instead.
    verifyFunction(*func);
    return func;
  }
  func->eraseFromParent();
  return nullptr;
}

Value* Identifier::codegen(){
  // assuming undeclared variables handled in scopify
  AllocaInst* A;
  int location = ident_info.pos;
  if(location < 0){
    A = global_st["g" + to_string(-location)];
  }
  else{
    A = llvm_st["l" + to_string(location)];
  }


 
  type_info.type = ident_info.stype;
  type_info.ptr_depth = 0;   // REMOVE LATER
  type_info.is_ref = true;
  return llvm_builder->CreateLoad(A->getAllocatedType(), A, name);
} 

}
