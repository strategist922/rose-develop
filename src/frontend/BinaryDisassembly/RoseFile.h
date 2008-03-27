/****************************************************
 * RoseBin :: Binary Analysis for ROSE
 * Author : tps
 * Date : 3Apr07
 * Decription : Interface to user
 ****************************************************/

#ifndef __RoseFile__
#define __RoseFile__


#include <stdio.h>
#include <iostream>
// #include "rose.h"
#include "RoseBin_file.h"

class RoseFile : public AstSimpleProcessing {
 private:
  // the DB connection
  RoseBin_FILE* idaDB; 
  RoseBin_unparse* unparser;
  SgAsmNode* globalNode;
  std::list<std::string> functionNames;



  int trav_inst;
  int trav_blocks;
  int trav_funcs;
  int nodes;

  int num_func;
  int num_inst;


  void loadAST(std::string filename);
  void saveAST(std::string filename);



 public:

  RoseFile(std::string file) {
    //RoseBin_support::setDebugMode(true);    
        RoseBin_support::setDebugMode(false);    
        RoseBin_support::setDataBaseSupport(true);    
    idaDB = new RoseBin_FILE(file);
    unparser = new RoseBin_unparse();
    RoseBin_support::setUnparseVisitor(unparser->getVisitor());
    globalNode = 0;
    functionNames.clear();
    num_inst=0;
    num_func=0;
    RoseBin_Def::RoseAssemblyLanguage = RoseBin_Def::none;
    //RoseBin_support::setAssemblyLanguage(RoseBin::none);
  }
  
  ~RoseFile() {
    delete idaDB;
    delete unparser;

    idaDB = NULL;
    unparser = NULL;

    if (globalNode)
      delete globalNode;
  }

  int getNumberOfFunctions() {return (num_func-1);}
  int getNumberOfInstructions() {return (num_inst-1);}

  // allow filtering of functions
  void setFunctionFilter(std::list<std::string> functionName);

  void visit(SgNode* node);

  // query the DB to retrieve all data
  SgAsmNode* retrieve_DB();

  // unparse the AST to assembly
  void unparse(char* fileName);

  void test();
};

#endif

