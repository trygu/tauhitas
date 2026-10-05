/*
* Argus Open Source
* Software to apply Statistical Disclosure Control techniques
* 
* Copyright 2014 Statistics Netherlands
* 
* This program is free software; you can redistribute it and/or 
* modify it under the terms of the European Union Public Licence 
* (EUPL) version 1.1, as published by the European Commission.
* 
* You can find the text of the EUPL v1.1 on
* https://joinup.ec.europa.eu/software/page/eupl/licence-eupl
* 
* This software is distributed on an "AS IS" basis without 
* warranties or conditions of any kind, either express or implied.
*/

// Makes choice of Solver-dll invisible to user
// All solver names are dispatched to the single open-source CSP engine
// (built against HiGHS), which exports its CSP* API under the "SCIP" name.
#include <string.h>
#include "WrapCSP.h"
#include "resource.h"

void CSPSetFileNames(const char* Solver, const char* dir)
{
    SCIPv::CSPSetFileNames(dir);
}

void CSPFreeFileNames(const char* Solver)
{
    SCIPv::CSPFreeFileNames();
}

void   CSPSetDoubleConstant(const char* Solver, const int VarNumber, double VarValue)
{
    SCIPv::CSPSetDoubleConstant(VarNumber, VarValue);
}

void   CSPSetIntegerConstant(const char* Solver, const int VarNumber, int VarValue)
{
    SCIPv::CSPSetIntegerConstant(VarNumber, VarValue);
}

double CSPGetDoubleConstant(const char* Solver, const int VarNumber)
{
    return SCIPv::CSPGetDoubleConstant(VarNumber);
}
int    CSPGetIntegerConstant(const char* Solver, const int VarNumber)
{
    return SCIPv::CSPGetIntegerConstant(VarNumber);
}

int    CSPoptimize(const char* Solver, IProgressListener* ProgressListener)
{
    return SCIPv::CSPoptimize(ProgressListener);
}

int    CSPloadprob(const char* Solver, int nsums_,double *rhs_,int ncells_,double *data_,int  *weight_,char *status_,double *lpl_,double *upl_,double *lb_,double *ub_,char **names_,int  *nlist_,int  *listcell_,signed char *listcoef_)
{
    int retval = SCIPv::CSPloadprob(nsums_,rhs_,ncells_,data_,weight_,status_,lpl_,upl_,lb_,ub_,names_,nlist_,listcell_,listcoef_);
    if (retval){
        throw(HITAS_JJNOTLOADED);
    }
    return retval;
}

int    CSPfreeprob(const char* Solver)
{
    return SCIPv::CSPfreeprob();
}

int    CSPsolution(const char* Solver, int *lowerb_,int *upperb_,char *status_)
{
    return SCIPv::CSPsolution(lowerb_,upperb_,status_);
}

int    CSPrelbounds(const char* Solver, int nlist,int *list,double *ub,double *lb,char type)
{
    return SCIPv::CSPrelbounds(nlist,list,ub,lb,type);
}
