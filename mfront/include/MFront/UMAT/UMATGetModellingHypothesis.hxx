/*! 
 * \file  UMATGetModellingHypothesis.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 16 mar 2011
 */

#ifndef _LIB_MFRONT_UMAT_UMATGETMODELLINGHYPOTHESIS_H_
#define _LIB_MFRONT_UMAT_UMATGETMODELLINGHYPOTHESIS_H_ 

#include"TFEL/Config/TFELConfig.hxx"
#include"TFEL/Material/ModellingHypothesis.hxx"
#include"MFront/UMAT/UMAT.hxx"

namespace umat
{

  TFEL_VISIBILITY_EXPORT
  tfel::material::ModellingHypothesis::Hypothesis
  getModellingHypothesis(const UMATInt);
  
} // end of namespace umat

#endif /* _LIB_MFRONT_UMAT_UMATGETMODELLINGHYPOTHESIS_H */

