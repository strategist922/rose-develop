// Copyright 2005,2006,2007 Markus Schordan, Gergo Barany
// $Id: set_pag_options.C,v 1.10 2008-11-04 10:04:22 gergo Exp $

// Author: Markus Schordan, 2006+

#include "set_pag_options.h"
#include "o_VariableId.h"
#include "o_ExpressionId.h"
#include <iostream>
#include <cstdlib>

#if HAVE_PAG
extern char* animation;
extern int cfg_ordering;
extern int sel_mapping;
extern int startbanks;
extern int low_perc;
extern int high_perc;
extern int share_min;
extern int share_num;
// extern int debug_stat;
extern int global_print_post_info;
extern int global_print_pre_info;
extern int global_proc_as_graphs;
extern int verbose;
// extern int global_retfunc;

extern "C" int pag_auto_configure_memsize(int quiet, int perc, int size);
#endif

// GB (2008-07-01): This global variable is accessed from deprecated support
// functions to determine whether they should emit a warning about their
// use; there is no other way to communicate this information to them.
bool satire_warn_deprecated;

void setPagOptions(AnalyzerOptions opt) {
#if HAVE_PAG
  cfg_ordering=opt.getCfgOrdering();
  // GB: debug_stat is set by the data flow analyzer object -- it is
  // prefixed by PAG, so we can't name it here because it might be called
  // something else at link time.
  // debug_stat=opt.statistics(); // bool->int

  // GB: memory options (low_perc is not documented, and probably need not be)
  low_perc=opt.getGcLow();
  high_perc=opt.getGcHigh();
  if (opt.memsizeMBSet()) {
    // memory size set as MB
    pag_auto_configure_memsize(opt.quiet(), 100, opt.getMemsizeMB());
  } else if (opt.memsizePercSet()) {
    // memory size set as percentage of autodetected system memory size
    pag_auto_configure_memsize(opt.quiet(), opt.getMemsizePerc(), 0);
  } else {
    // default: set memory size to 5 MB, which amounts to 10 half-MB banks
    startbanks = 10;
    // share_min and share_num appear to be set appropriately for a
    // startbanks value of 10, so there is nothing else to do
  }

  setOutputGdlAnimDirName(opt.outputGdlAnim()
                            ? (char *) opt.getOutputGdlAnimDirName().c_str()
                            : NULL);

  int csl=opt.getCallStringLength();
  if(csl==0 || csl==1) { 
    sel_mapping=csl; 
  }
  if(csl>=2) {
    sel_mapping=PAG_MAPPING_CALLSTRING_N;
    mapping_data.map_n = csl;
  }
  if(csl==-1) {
    sel_mapping=PAG_MAPPING_CALLSTRING_INF;
  }
  global_print_post_info=opt.postInfo();
  global_print_pre_info=opt.preInfo();
  global_proc_as_graphs=opt.gdlProcedureSubgraphs();

  //start_bank=opt.getStartBank();
  //share_min=opt.getShareMin();
  //share_num=opt.getShareNum();

  if(opt.vivu()) {
    mapping_data.map_l = opt.getVivuLoopUnrolling(); // [default 2]
    if(opt.getCallStringLength()==1) {
      sel_mapping=PAG_MAPPING_VIVU_1; // VIVU 1 (simple)
    }
    if(opt.getCallStringLength()==2) {
      sel_mapping=PAG_MAPPING_VIVU_2; // VIVU 2 (extended)
    }
    if(opt.getCallStringLength()>=3) {
      sel_mapping=PAG_MAPPING_VIVU_HT; // VIVU ht (extended), callstringlength defines chop size
    }
  }
  if(opt.getVivu4MaxUnrolling()!=-1) {
    sel_mapping=PAG_MAPPING_VIVU_4;
    mapping_data.map_global_max_cut=opt.getVivu4MaxUnrolling();
  }

  // GB (2009-02-23): This setting ensures that PAG does not garbage-collect
  // call string data if we want to access it. As we want to compute context
  // information by default, always set this.
  mapping_data.map_force = 0;

  if(opt.retFuncUsed()) {
    /* use retfunc for combining information from local and return edge */
    // GB: Set by the data flow analyzer object.
    // global_retfunc = 1;
  }
#endif

  satire_warn_deprecated = opt.deprecatedWarnings();

  int formatMode=0;
  {
    VariableId::PrintFormat printMode;

    if(opt.nodeFormatVarId()) {
      formatMode+=1;
    }
    if(opt.nodeFormatVarName()) {
      formatMode+=2;
    }
    switch(formatMode) {
    case 1: printMode=VariableId::F_Id;break;
    case 2: printMode=VariableId::F_Name;break;
    case 3: printMode=VariableId::F_IdAndName;break;
    default: std::cerr << "Wrong format mode or print mode for nodes (Variable) in gdl file"; std::abort();
    }
    VariableId::setPrintFormat(printMode);
  }
  formatMode=0;
  {
    ExpressionId::PrintFormat printMode;

    if(opt.nodeFormatExprId()) {
      formatMode+=1;
    }
    if(opt.nodeFormatExprSource()) {
      formatMode+=2;
    }
    switch(formatMode) {
    case 1: printMode=ExpressionId::F_Id;break;
    case 2: printMode=ExpressionId::F_Expression;break;
    case 3: printMode=ExpressionId::F_IdAndExpression;break;
    default: std::cerr << "Wrong format mode or print mode for nodes (Expression) in gdl file"; std::abort();    
    }
    ExpressionId::setPrintFormat(printMode);
  }
}

#include "satire_file_utils.h"

void setOutputGdlAnimDirName(char *animDirName)
{
#if HAVE_PAG
    animation = animDirName;
 // Make sure the animation directory exists.
    if (animDirName != NULL)
    {
        std::vector<std::string> dir = SATIrE::pathNameComponents(animDirName);
        SATIrE::createDirectoryHierarchy(dir);
    }
#endif
}
