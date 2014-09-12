/*! 
 * \file  include/TFEL/Math/T2toST2/RightCauchyGreenTensorDerivativeExpr.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 04 juin 2014
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef _LIB_TFEL_MATH_RIGHTCAUCHYGREENTENSORDERIVATIVEEXPR_H_
#define _LIB_TFEL_MATH_RIGHTCAUCHYGREENTENSORDERIVATIVEEXPR_H_ 

#include<cmath>

#include"TFEL/Config/TFELConfig.hxx"
#include"TFEL/Metaprogramming/StaticAssert.hxx"
#include"TFEL/Math/General/EmptyRunTimeProperties.hxx"
#include"TFEL/Math/T2toST2/T2toST2Expr.hxx"

namespace tfel{

  namespace math{

    /*!
     * Empty structure allowing partial specialisation
     */
    template<unsigned short N>
    struct RightCauchyGreenTensorDerivativeExpr
    {}; // end of struct RightCauchyGreenTensorDerivativeExpr

    /*!
     * Partial specialisation for 1D tensor
     */
    template<typename T2toST2Type>
    struct T2toST2Expr<T2toST2Type,RightCauchyGreenTensorDerivativeExpr<1u> >
      : public T2toST2Concept<T2toST2Expr<T2toST2Type,RightCauchyGreenTensorDerivativeExpr<1u> > >,
	public fsarray<9u,typename T2toST2Traits<T2toST2Type>::NumType>
    {
      //! a simple check
      TFEL_STATIC_ASSERT(T2toST2Traits<T2toST2Type>::dime==1u);
      //! a simple alias
      typedef EmptyRunTimeProperties RunTimeProperties;
      //! a simple alias
      typedef typename T2toST2Traits<T2toST2Type>::NumType value_type;
      /*!
       * \param[in] B : second tensor of the product
       */
      template<typename TensorType>
      TFEL_MATH_INLINE2
      T2toST2Expr(const TensorType& F)
      {
	//! a simple check
	TFEL_STATIC_ASSERT((tfel::meta::Implements<TensorType,TensorConcept>::cond));
	//! a simple check
	TFEL_STATIC_ASSERT((T2toST2Traits<T2toST2Type>::dime==TensorTraits<TensorType>::dime));
	//! a simple check
	TFEL_STATIC_ASSERT((tfel::typetraits::IsAssignableTo<typename TensorTraits<TensorType>::NumType,
							     typename T2toST2Traits<T2toST2Type>::NumType>::cond));
	const value_type zero(0);
	this->v[0] = 2*F[0];
	this->v[1] = this->v[2] = this->v[3] = zero;
	this->v[4] = 2*F[1];
	this->v[5] = this->v[6] = this->v[7] = zero;
	this->v[8] = 2*F[2];
      } // end of T2toST2Expr
      /*!
       * \brief access operator
       * \param[in] i : line   index
       * \param[in] j : column index
       */
      TFEL_MATH_INLINE const value_type&
      operator()(const unsigned short i,
		 const unsigned short j) const
      {
	return this->v[i*3+j];
      } // end of operator()
      /*!
       * \return the runtime properties
       */
      TFEL_MATH_INLINE RunTimeProperties
      getRunTimeProperties() const
      {
	return RunTimeProperties();
      }
    }; // end of struct T2toST2Expr<T2toST2Type,RightCauchyGreenTensorDerivativeExpr<1u> >

    /*!
     * Partial specialisation for 2D tensor
     */
    template<typename T2toST2Type>
    struct T2toST2Expr<T2toST2Type,RightCauchyGreenTensorDerivativeExpr<2u> >
      : public T2toST2Concept<T2toST2Expr<T2toST2Type,RightCauchyGreenTensorDerivativeExpr<2u> > >,
	public fsarray<20u,typename T2toST2Traits<T2toST2Type>::NumType>
    {
      //! a simple check
      TFEL_STATIC_ASSERT(T2toST2Traits<T2toST2Type>::dime==2u);
      //! a simple alias
      typedef EmptyRunTimeProperties RunTimeProperties;
      //! a simple alias
      typedef typename T2toST2Traits<T2toST2Type>::NumType value_type;
      /*!
       * \param[in] B : second tensor of the product
       */
      template<typename TensorType>
      TFEL_MATH_INLINE2
      T2toST2Expr(const TensorType& F)
      {
	//! a simple check
	TFEL_STATIC_ASSERT((tfel::meta::Implements<TensorType,TensorConcept>::cond));
	//! a simple check
	TFEL_STATIC_ASSERT((T2toST2Traits<T2toST2Type>::dime==TensorTraits<TensorType>::dime));
	//! a simple check
	TFEL_STATIC_ASSERT((tfel::typetraits::IsAssignableTo<typename TensorTraits<TensorType>::NumType,
							     typename T2toST2Traits<T2toST2Type>::NumType>::cond));
	static const value_type sqrt2 = std::sqrt(value_type(2));
	const value_type zero(0);
	this->v[0]  = 2*F[0];
	this->v[1]  = this->v[2] = this->v[3] = zero;
	this->v[4]  = 2*F[4];
	this->v[5]  = zero;
	this->v[6]  = 2*F[1];
	this->v[7]  = zero;
	this->v[8]  = 2*F[3];
	this->v[9]  = this->v[10] = this->v[11] = zero;
	this->v[12] = 2*F[2];
	this->v[13] = this->v[14] = zero;
	this->v[15] = sqrt2*F[3];
	this->v[16] = sqrt2*F[4];
	this->v[17] = zero;
	this->v[18] = sqrt2*F[0];
	this->v[19] = sqrt2*F[1];
      } // end of T2toST2Expr
      /*!
       * \brief access operator
       * \param[in] i : line   index
       * \param[in] j : column index
       */
      TFEL_MATH_INLINE const value_type&
      operator()(const unsigned short i,
		 const unsigned short j) const
      {
	return this->v[i*5+j];
      } // end of operator()
      /*!
       * \return the runtime properties
       */
      TFEL_MATH_INLINE RunTimeProperties
      getRunTimeProperties() const
      {
	return RunTimeProperties();
      }
    }; // end of struct T2toST2Expr<T2toST2Type,RightCauchyGreenTensorDerivativeExpr<1u> >

    /*!
     * Partial specialisation for 3D tensor
     */
    template<typename T2toST2Type>
    struct T2toST2Expr<T2toST2Type,RightCauchyGreenTensorDerivativeExpr<3u> >
      : public T2toST2Concept<T2toST2Expr<T2toST2Type,RightCauchyGreenTensorDerivativeExpr<3u> > >,
	public fsarray<54u,typename T2toST2Traits<T2toST2Type>::NumType>
    {
      //! a simple check
      TFEL_STATIC_ASSERT(T2toST2Traits<T2toST2Type>::dime==3u);
      //! a simple alias
      typedef EmptyRunTimeProperties RunTimeProperties;
      //! a simple alias
      typedef typename T2toST2Traits<T2toST2Type>::NumType value_type;
      /*!
       * \param[in] B : second tensor of the product
       */
      template<typename TensorType>
      TFEL_MATH_INLINE2
      T2toST2Expr(const TensorType& F)
      {
	//! a simple check
	TFEL_STATIC_ASSERT((tfel::meta::Implements<TensorType,TensorConcept>::cond));
	//! a simple check
	TFEL_STATIC_ASSERT((T2toST2Traits<T2toST2Type>::dime==TensorTraits<TensorType>::dime));
	//! a simple check
	TFEL_STATIC_ASSERT((tfel::typetraits::IsAssignableTo<typename TensorTraits<TensorType>::NumType,
							     typename T2toST2Traits<T2toST2Type>::NumType>::cond));
	static const value_type sqrt2 = std::sqrt(value_type(2));
	const value_type zero(0);
	this->v[0]  = 2*F[0];
	this->v[1]  = this->v[2] = this->v[3] = zero;
	this->v[4]  = 2*F[4];
	this->v[5]  = zero;
	this->v[6]  = 2*F[6];
	this->v[7]  = this->v[8]  = this->v[9]  = zero;
	this->v[10] = 2*F[1];	
	this->v[11] = zero;
	this->v[12] = 2*F[3];
	this->v[13] = this->v[14] = this->v[15] = this->v[16] = zero;
	this->v[17] = 2*F[8];
	this->v[18] = this->v[19] = zero;
	this->v[20] = 2*F[2];
	this->v[21] = this->v[22] = zero;
	this->v[23] = 2*F[5];
	this->v[24] = zero;
	this->v[25] = 2*F[7];
	this->v[26] = zero;
	this->v[27] = sqrt2*F[3];
	this->v[28] = sqrt2*F[4];
	this->v[29] = zero;
	this->v[30] = sqrt2*F[0];
	this->v[31] = sqrt2*F[1];
	this->v[32] = zero;
	this->v[33] = sqrt2*F[8];
	this->v[34] = zero;
	this->v[35] = sqrt2*F[6];
	this->v[36] = sqrt2*F[5];
	this->v[37] = zero;
	this->v[38] = sqrt2*F[6];
	this->v[39] = zero;
	this->v[40] = sqrt2*F[7];
	this->v[41] = sqrt2*F[0];
	this->v[42] = sqrt2*F[2];
	this->v[43] = sqrt2*F[4];
	this->v[44] = zero;
	this->v[45] = zero;
	this->v[46] = sqrt2*F[7];
	this->v[47] = sqrt2*F[8];
	this->v[48] = sqrt2*F[5];
	this->v[49] = zero;
	this->v[50] = sqrt2*F[3];
	this->v[51] = zero;
	this->v[52] = sqrt2*F[1];
	this->v[53] = sqrt2*F[2];
      } // end of T2toST2Expr
      /*!
       * \brief access operator
       * \param[in] i : line   index
       * \param[in] j : column index
       */
      TFEL_MATH_INLINE const value_type&
      operator()(const unsigned short i,
		 const unsigned short j) const
      {
	return this->v[i*9+j];
      } // end of operator()
      /*!
       * \return the runtime properties
       */
      TFEL_MATH_INLINE RunTimeProperties
      getRunTimeProperties() const
      {
	return RunTimeProperties();
      }
    }; // end of struct T2toST2Expr<T2toST2Type,RightCauchyGreenTensorDerivativeExpr<1u> >

  } // end of namespace tfel

} // end of namespace math

#endif /* _LIB_TFEL_MATH_RIGHTCAUCHYGREENTENSORDERIVATIVEEXPR_H */
