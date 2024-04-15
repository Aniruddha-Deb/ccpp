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
  // cdebug<<"Entered stod "+ s<<endl;
  double temp = stod(s);
  // cdebug<<"temp "<<temp<<endl;
  return temp;
}

using namespace llvm;


namespace ast {

 std::unique_ptr<llvm::LLVMContext> llvm_ctx;
 std::unique_ptr<llvm::Module> llvm_mod;
 std::unique_ptr<llvm::IRBuilder<>> llvm_builder;
 std::map<std::string, AllocaInst*> llvm_st;
 std::map<std::string, GlobalVariable*> global_st;



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
  int idx = ident->ident_info.idx;
  return prefix + to_string(abs(idx));
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



llvm::Type* getTypeNew(SymbolInfo symbol_info){
  SymbolType ts= symbol_info.stype;
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

  while(symbol_info.ptr_depth>0){
    t = llvm::PointerType::get(t, 0);
    symbol_info.ptr_depth--;
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


llvm::Value* DeclarationStatement::globalgen(){
  return decl->globalgen();
}

llvm::Value* Declaration::globalgen(){
  cout<<"HI"<<endl;
  for(auto init_decl: *decl_list){
    GlobalVariable* A = new llvm::GlobalVariable(*llvm_mod, getType(decl_specs, init_decl->ptr_depth), false, llvm::GlobalValue::ExternalLinkage, 0, getVarName(init_decl->ident, "g"));
    if(init_decl->init_expr){
      Value* init_val = init_decl->init_expr->codegen();
      llvm_builder->CreateStore(init_val, A);
    }
    global_st[getVarName(init_decl->ident, "g") ] = A;
  }

  return nullptr;
}

void TranslationUnit::codegen() {
  llvm_ctx = std::make_unique<llvm::LLVMContext>();
  llvm_mod = std::make_unique<llvm::Module>("Code Generator", *llvm_ctx);

  // Create a new builder for the module.
  llvm_builder = std::make_unique<llvm::IRBuilder<>>(*llvm_ctx);

  DeclarationStatement* decl_ptr;
  for (auto node_ptr: *nodes){
    // cdebug<<"HI1"<<endl;
    if ((decl_ptr = dynamic_cast<DeclarationStatement*>(node_ptr))) {
      cout<<"READ FUNC DEF"<<endl;
      Value* v = decl_ptr->globalgen();
      // func_ir->print(errs());
    }
  }

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
  type_info.is_ref = false;
  type_info.st.ptr_depth = 0;
  switch(ltype) {
        case LT_INT_LIKE:
            v = intLiteralToInt(value);
            type_info.st.stype = I32;
            return ConstantInt::get(*llvm_ctx, APInt(32, v));
            break;
            // break;
        case LT_INT64:
            v = intLiteralToInt(value);
            type_info.st.stype = I64;
            return ConstantInt::get(*llvm_ctx, APInt(64, v));
        case LT_FLOAT:
            type_info.st.stype = FP32;
            return ConstantFP::get(llvm::Type::getFloatTy(*llvm_ctx), APFloat(floatLiteralToFloat(value)));
        case LT_DOUBLE:
            type_info.st.stype = FP64;
            return ConstantFP::get(llvm::Type::getDoubleTy(*llvm_ctx), APFloat(floatLiteralToFloat(value)));
          case LT_FLOAT_LIKE:
            type_info.st.stype = FP32;
            return ConstantFP::get(llvm::Type::getFloatTy(*llvm_ctx), APFloat(floatLiteralToFloat(value)));
        default:
            cout<<"invalid literal"<<endl;
            return nullptr;
  }
}



Value* Identifier::get_address(){
  type_info.st = ident_info;
  type_info.is_ref = true;

  return llvm_st[getVarName(this, "l")];
}


Value *BinaryExpression::codegen() {
  if (op == OP_ASSIGN){
    // cdebug<<"calling getaddr"<<endl;
    Value* A = lhs->get_address(); 
    // Value* a = lhs->assign(rhs);
    // cdebug<<"calling codegen"<<endl;
    Value* R = rhs->codegen();

    type_info.st = rhs->type_info.st;
    type_info.is_ref = false;                         // should this be rhs->is_ref?? Nope
    // cout<<lhs->type_info.type<<" "<<rhs->type_info.type<<endl<<lhs->type_info.ptr_depth<<" "<<rhs->type_info.ptr_depth<<endl<<lhs->type_info.is_ref<<endl;
    if((lhs->type_info.st.stype == rhs->type_info.st.stype) && (lhs->type_info.st.ptr_depth == rhs->type_info.st.ptr_depth) && (lhs->type_info.is_ref)){
      llvm_builder->CreateStore(R, A, "assigntemp");
      return R;
    }
    else{
      cout<<"Assignment type mismatch";
      return nullptr;
    }
  }
  Value *L = lhs->codegen();
  Value *R = rhs->codegen();
  // TODO type check. error if types don't match

  if (!L || !R)
    return nullptr;

  if(lhs->type_info.st.stype != rhs->type_info.st.stype || lhs->type_info.st.ptr_depth != rhs->type_info.st.ptr_depth || lhs->type_info.st.ptr_depth != 0){
    cout<<"Type mismatch"<<endl;
    return nullptr;
  }

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
    case OP_GE:
      type_info.st.stype = I1;
      return handleGTE(L, R, lhs->type_info.st.stype);
    case OP_GT:
      type_info.st.stype = I1;
      return handleGT(L, R, lhs->type_info.st.stype);
    case OP_LE:
      type_info.st.stype = I1;
      return handleLTE(L, R, lhs->type_info.st.stype);
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
  }
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
      return llvm_builder->CreateLoad(getTypeNew(type_info.st), R, "dereftemp");
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
  Identifier* ident = (Identifier*) fn;               // is it fine to call codegen func here? nope
  llvm::Function* func = llvm_mod->getFunction(ident->name);

  type_info.is_ref = false;
  type_info.st = ident->ident_info;          
  cdebug<<"Function invocation with type "<<fn->type_info.st.stype<<endl;
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

  if (Value *ret_val = stmts->codegen()) {

    verifyFunction(*func);
    return func;
  }
  func->eraseFromParent();
  return nullptr;
}

Value* Identifier::codegen(){
  // assuming undeclared variables handled in scopify
  AllocaInst* A;
  int idx = ident_info.idx;
  if(idx < 0){
    cout<<idx<<"GLOVAL"<<endl;
    GlobalVariable* A = global_st[ getVarName(this, "g")];
    type_info.st= ident_info;
    type_info.is_ref = true;
    return llvm_builder->CreateLoad(A->getType(), A, name);
  }
  else{
    A = llvm_st["l" + to_string(idx)];
  }


 
  type_info.st = ident_info;
  type_info.is_ref = true;
  return llvm_builder->CreateLoad(A->getAllocatedType(), A, name);
} 

}
