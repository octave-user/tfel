/*!
 * \file   include/TFEL/Material/OrthotropicPlasticity.ixx
 * \brief This files implements the function declared in
 * `include/TFEL/Material/OrthotropicPlasticity.ixx`.
 * \author Thomas Helfer
 * \date   14/01/2018
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef LIB_TFEL_MATERIAL_ORTHOTROPICPLASTICITY_IXX
#define LIB_TFEL_MATERIAL_ORTHOTROPICPLASTICITY_IXX

namespace tfel{

  namespace material{

    template<typename StensorType>
    typename std::enable_if<
      tfel::meta::Implements<StensorType,tfel::math::StensorConcept>::cond,
      tfel::math::UnaryOperationResult<tfel::math::StensorNumType<StensorType>,
				       tfel::math::Power<2>>>::type
    computeJ2O(const StensorType& s,
	       const J2OCoefficients<StensorType>& a){
      return computeJ2O(s,a[0],a[1],a[2],a[3],a[4],a[5]);
    }

    template<typename StensorType>
    typename std::enable_if<
      tfel::meta::Implements<StensorType,tfel::math::StensorConcept>::cond&&
      tfel::math::StensorTraits<StensorType>::dime==1u,
      tfel::math::UnaryOperationResult<tfel::math::StensorNumType<StensorType>,
				       tfel::math::Power<2>>>::type
    computeJ2O(const StensorType& s,
	       const tfel::math::StensorBaseType<StensorType> a1,
	       const tfel::math::StensorBaseType<StensorType> a2,
	       const tfel::math::StensorBaseType<StensorType> a3,
	       const tfel::math::StensorBaseType<StensorType> a4,
	       const tfel::math::StensorBaseType<StensorType> a5,
	       const tfel::math::StensorBaseType<StensorType> a6)
    {
      return (a2*(s[1]-s[2])*(s[1]-s[2])+
	      a3*(s[0]-s[2])*(s[0]-s[2])+
	      a1*(s[0]-s[1])*(s[0]-s[1]))/6;
    } // end of computeJ2O

    template<typename StensorType>
    typename std::enable_if<
      tfel::meta::Implements<StensorType,tfel::math::StensorConcept>::cond&&
      tfel::math::StensorTraits<StensorType>::dime==2u,
      tfel::math::UnaryOperationResult<tfel::math::StensorNumType<StensorType>,
				       tfel::math::Power<2>>>::type
    computeJ2O(const StensorType& s,
	       const tfel::math::StensorBaseType<StensorType> a1,
	       const tfel::math::StensorBaseType<StensorType> a2,
	       const tfel::math::StensorBaseType<StensorType> a3,
	       const tfel::math::StensorBaseType<StensorType> a4,
	       const tfel::math::StensorBaseType<StensorType> a5,
	       const tfel::math::StensorBaseType<StensorType> a6)
    {
      return (a2*(s[1]-s[2])*(s[1]-s[2])+a3*(s[0]-s[2])*(s[0]-s[2])+
	      a1*(s[0]-s[1])*(s[0]-s[1]))/6+a4*s[3]*s[3]/2;
    } // end of computeJ2O

    template<typename StensorType>
    typename std::enable_if<
      tfel::meta::Implements<StensorType,tfel::math::StensorConcept>::cond&&
      tfel::math::StensorTraits<StensorType>::dime==3u,
      tfel::math::UnaryOperationResult<tfel::math::StensorNumType<StensorType>,
				       tfel::math::Power<2>>>::type
    computeJ2O(const StensorType& s,
	       const tfel::math::StensorBaseType<StensorType> a1,
	       const tfel::math::StensorBaseType<StensorType> a2,
	       const tfel::math::StensorBaseType<StensorType> a3,
	       const tfel::math::StensorBaseType<StensorType> a4,
	       const tfel::math::StensorBaseType<StensorType> a5,
	       const tfel::math::StensorBaseType<StensorType> a6)
    {
      return (a6*s[5]*s[5]+a5*s[4]*s[4]+a4*s[3]*s[3])/2+
	(a2*(s[1]-s[2])*(s[1]-s[2])+a3*(s[0]-s[2])*(s[0]-s[2])+a1*(s[0]-s[1])*(s[0]-s[1]))/6;
    } // end of computeJ2O

    template<typename StensorType>
    typename std::enable_if<
      tfel::meta::Implements<StensorType,tfel::math::StensorConcept>::cond,
      tfel::math::stensor<tfel::math::StensorTraits<StensorType>::dime,
			  tfel::math::StensorNumType<StensorType>>>::type
    computeJ2ODerivative(const StensorType& s,
			 const J2OCoefficients<StensorType>& a){
      return computeJ2ODerivative(s,a[0],a[1],a[2],a[3],a[4],a[5]);
    } // end of computeJ2ODerivative

    template<typename StensorType>
    typename std::enable_if<
      tfel::meta::Implements<StensorType,tfel::math::StensorConcept>::cond&&
      tfel::math::StensorTraits<StensorType>::dime==1u,
      tfel::math::stensor<tfel::math::StensorTraits<StensorType>::dime,
			  tfel::math::StensorNumType<StensorType>>>::type
    computeJ2ODerivative(const StensorType& s,
			 const tfel::math::StensorBaseType<StensorType> a1,
			 const tfel::math::StensorBaseType<StensorType> a2,
			 const tfel::math::StensorBaseType<StensorType> a3,
			 const tfel::math::StensorBaseType<StensorType>,
			 const tfel::math::StensorBaseType<StensorType>,
			 const tfel::math::StensorBaseType<StensorType>)
    {
      return {(a3*(s[0]-s[2]))/3+(a1*(s[0]-s[1]))/3,
	  (a2*(s[1]-s[2]))/3-(a1*(s[0]-s[1]))/3,
	  (-(a2*(s[1]-s[2]))/3)-(a3*(s[0]-s[2]))/3};
    } // end of computeJ2ODerivative

    template<typename StensorType>
    typename std::enable_if<
      tfel::meta::Implements<StensorType,tfel::math::StensorConcept>::cond&&
      tfel::math::StensorTraits<StensorType>::dime==2u,
      tfel::math::stensor<tfel::math::StensorTraits<StensorType>::dime,
			  tfel::math::StensorNumType<StensorType>>>::type
    computeJ2ODerivative(const StensorType& s,
			 const tfel::math::StensorBaseType<StensorType> a1,
			 const tfel::math::StensorBaseType<StensorType> a2,
			 const tfel::math::StensorBaseType<StensorType> a3,
			 const tfel::math::StensorBaseType<StensorType> a4,
			 const tfel::math::StensorBaseType<StensorType>,
			 const tfel::math::StensorBaseType<StensorType>)
    {
      return {(a3*(s[0]-s[2]))/3+(a1*(s[0]-s[1]))/3,
	  (a2*(s[1]-s[2]))/3-(a1*(s[0]-s[1]))/3,
	  (-(a2*(s[1]-s[2]))/3)-(a3*(s[0]-s[2]))/3,a4*s[3]};
    } // end of computeJ2ODerivative
  
    template<typename StensorType>
    typename std::enable_if<
      tfel::meta::Implements<StensorType,tfel::math::StensorConcept>::cond&&
      tfel::math::StensorTraits<StensorType>::dime==3u,
      tfel::math::stensor<tfel::math::StensorTraits<StensorType>::dime,
			  tfel::math::StensorNumType<StensorType>>>::type
    computeJ2ODerivative(const StensorType& s,
			 const tfel::math::StensorBaseType<StensorType> a1,
			 const tfel::math::StensorBaseType<StensorType> a2,
			 const tfel::math::StensorBaseType<StensorType> a3,
			 const tfel::math::StensorBaseType<StensorType> a4,
			 const tfel::math::StensorBaseType<StensorType> a5,
			 const tfel::math::StensorBaseType<StensorType> a6)
    {
      return {(a3*(s[0]-s[2]))/3+(a1*(s[0]-s[1]))/3,
	  (a2*(s[1]-s[2]))/3-(a1*(s[0]-s[1]))/3,
	  (-(a2*(s[1]-s[2]))/3)-(a3*(s[0]-s[2]))/3,
	  a4*s[3],a5*s[4],a6*s[5]};
    } // end of computeJ2ODerivative

    template<typename StensorType>
    typename std::enable_if<
      tfel::meta::Implements<StensorType,tfel::math::StensorConcept>::cond,
      tfel::math::st2tost2<tfel::math::StensorTraits<StensorType>::dime,
			   tfel::math::BinaryOperationResult<tfel::math::StensorBaseType<StensorType>,
							     tfel::math::StensorNumType<StensorType>,
							     tfel::math::OpDiv>>>::type
    computeJ2OSecondDerivative(const StensorType& s,
			       const J2OCoefficients<StensorType>& a){
      return computeJ2OSecondDerivative(s,a[0],a[1],a[2],a[3],a[4],a[5]);
    } // end of computeJ2OSecondDerivative
  
    template<typename StensorType>
    typename std::enable_if<
      tfel::meta::Implements<StensorType,tfel::math::StensorConcept>::cond&&
      tfel::math::StensorTraits<StensorType>::dime==1u,
      tfel::math::st2tost2<tfel::math::StensorTraits<StensorType>::dime,
			   tfel::math::BinaryOperationResult<tfel::math::StensorBaseType<StensorType>,
							     tfel::math::StensorNumType<StensorType>,
							     tfel::math::OpDiv>>>::type
    computeJ2OSecondDerivative(const StensorType&,
			       const tfel::math::StensorBaseType<StensorType> a1,
			       const tfel::math::StensorBaseType<StensorType> a2,
			       const tfel::math::StensorBaseType<StensorType> a3,
			       const tfel::math::StensorBaseType<StensorType>,
			       const tfel::math::StensorBaseType<StensorType>,
			       const tfel::math::StensorBaseType<StensorType>)
    {
      return {(a3+a1)/3,-a1/3,-a3/3,
	  -a1/3,(a2+a1)/3,-a2/3,
	  -a3/3,-a2/3,(a2+a3)/3};
    } // end of computeJ2OSecondDerivative
      
    template<typename StensorType>
    typename std::enable_if<
      tfel::meta::Implements<StensorType,tfel::math::StensorConcept>::cond&&
      tfel::math::StensorTraits<StensorType>::dime==2u,
      tfel::math::st2tost2<tfel::math::StensorTraits<StensorType>::dime,
			   tfel::math::BinaryOperationResult<tfel::math::StensorBaseType<StensorType>,
							     tfel::math::StensorNumType<StensorType>,
							     tfel::math::OpDiv>>>::type
    computeJ2OSecondDerivative(const StensorType&,
			       const tfel::math::StensorBaseType<StensorType> a1,
			       const tfel::math::StensorBaseType<StensorType> a2,
			       const tfel::math::StensorBaseType<StensorType> a3,
			       const tfel::math::StensorBaseType<StensorType> a4,
			       const tfel::math::StensorBaseType<StensorType> a5,
			       const tfel::math::StensorBaseType<StensorType> a6)
    {
      using real = tfel::math::StensorBaseType<StensorType>;
      constexpr const auto zero = real{0};
      return {(a3+a1)/3,-a1/3,-a3/3,zero,
	  -a1/3,(a2+a1)/3,-a2/3,zero,
	  -a3/3,-a2/3,(a2+a3)/3,zero,
	  zero,zero,zero,a4};
    } // end of computeJ2OSecondDerivative

    template<typename StensorType>
    typename std::enable_if<
      tfel::meta::Implements<StensorType,tfel::math::StensorConcept>::cond&&
      tfel::math::StensorTraits<StensorType>::dime==3u,
      tfel::math::st2tost2<tfel::math::StensorTraits<StensorType>::dime,
			   tfel::math::BinaryOperationResult<tfel::math::StensorBaseType<StensorType>,
							     tfel::math::StensorNumType<StensorType>,
							     tfel::math::OpDiv>>>::type
    computeJ2OSecondDerivative(const StensorType&,
			       const tfel::math::StensorBaseType<StensorType> a1,
			       const tfel::math::StensorBaseType<StensorType> a2,
			       const tfel::math::StensorBaseType<StensorType> a3,
			       const tfel::math::StensorBaseType<StensorType> a4,
			       const tfel::math::StensorBaseType<StensorType> a5,
			       const tfel::math::StensorBaseType<StensorType> a6)
    {
      using real = tfel::math::StensorBaseType<StensorType>;
      constexpr const auto zero = real{0};
      return {(a3+a1)/3,-a1/3,-a3/3,zero,zero,zero,
	  -a1/3,(a2+a1)/3,-a2/3,zero,zero,zero,
	  -a3/3,-a2/3,(a2+a3)/3,zero,zero,zero,
	  zero,zero,zero,a4,zero,zero,
	  zero,zero,zero,zero,a5,zero,
	  zero,zero,zero,zero,zero,a6};
    } // end of computeJ2OSecondDerivative

    template<typename StensorType>
    typename std::enable_if<
      tfel::meta::Implements<StensorType,tfel::math::StensorConcept>::cond,
      tfel::math::UnaryOperationResult<tfel::math::StensorNumType<StensorType>,
				       tfel::math::Power<3>>>::type
    computeJ3O(const StensorType& s,
	       const J3OCoefficients<StensorType>& b){
      return computeJ3O(s,b[0],b[1],b[2],b[3],b[4],b[5],
			b[6],b[7],b[8],b[9],b[10]);
    }

    template<typename StensorType>
    typename std::enable_if<
      tfel::meta::Implements<StensorType,tfel::math::StensorConcept>::cond&&
      tfel::math::StensorTraits<StensorType>::dime==1u,
      tfel::math::UnaryOperationResult<tfel::math::StensorNumType<StensorType>,
				       tfel::math::Power<3>>>::type
    computeJ3O(const StensorType& s,
	       const tfel::math::StensorBaseType<StensorType> b1,
	       const tfel::math::StensorBaseType<StensorType> b2,
	       const tfel::math::StensorBaseType<StensorType> b3,
	       const tfel::math::StensorBaseType<StensorType> b4,
	       const tfel::math::StensorBaseType<StensorType>,
	       const tfel::math::StensorBaseType<StensorType>,
	       const tfel::math::StensorBaseType<StensorType>,
	       const tfel::math::StensorBaseType<StensorType>,
	       const tfel::math::StensorBaseType<StensorType>,
	       const tfel::math::StensorBaseType<StensorType>,
	       const tfel::math::StensorBaseType<StensorType>){
      return ((2*(b4+b1)-b3-b2)*s[2]*s[2]*s[2])/27
	+((b4+b3)*s[1]*s[1]*s[1])/27
	+((b2+b1)*s[0]*s[0]*s[0])/27
	-(((b4+b1-b3)*s[1]+(b4-b2+b1)*s[0])*s[2]*s[2])/9
	-(s[1]*s[1]*(b3*s[2]+b4*s[0]))/9
	+(2*(b4+b1)*s[0]*s[1]*s[2])/9
	-(s[0]*s[0]*(b1*s[1]+b2*s[2]))/9;
    }

    template<typename StensorType>
    typename std::enable_if<
      tfel::meta::Implements<StensorType,tfel::math::StensorConcept>::cond&&
      tfel::math::StensorTraits<StensorType>::dime==2u,
      tfel::math::UnaryOperationResult<tfel::math::StensorNumType<StensorType>,
				       tfel::math::Power<3>>>::type
    computeJ3O(const StensorType& s,
	       const tfel::math::StensorBaseType<StensorType> b1,
	       const tfel::math::StensorBaseType<StensorType> b2,
	       const tfel::math::StensorBaseType<StensorType> b3,
	       const tfel::math::StensorBaseType<StensorType> b4,
	       const tfel::math::StensorBaseType<StensorType> b5,
	       const tfel::math::StensorBaseType<StensorType>,
	       const tfel::math::StensorBaseType<StensorType>,
	       const tfel::math::StensorBaseType<StensorType>,
	       const tfel::math::StensorBaseType<StensorType>,
	       const tfel::math::StensorBaseType<StensorType> b10,
	       const tfel::math::StensorBaseType<StensorType>){
      return ((2*(b4+b1)-b3-b2)*s[2]*s[2]*s[2])/27
	+((b4+b3)*s[1]*s[1]*s[1])/27
	+((b2+b1)*s[0]*s[0]*s[0])/27
	-((2*b10*s[2]-b5*s[1]+(b5-2*b10)*s[0])*s[3]*s[3])/6
	-(((b4+b1-b3)*s[1]+(b4-b2+b1)*s[0])*s[2]*s[2])/9
	-(s[1]*s[1]*(b3*s[2]+b4*s[0]))/9
	+(2*(b4+b1)*s[0]*s[1]*s[2])/9
	-(s[0]*s[0]*(b1*s[1]+b2*s[2]))/9;
    }
    
    template<typename StensorType>
    typename std::enable_if<
      tfel::meta::Implements<StensorType,tfel::math::StensorConcept>::cond&&
      tfel::math::StensorTraits<StensorType>::dime==3u,
      tfel::math::UnaryOperationResult<tfel::math::StensorNumType<StensorType>,
				       tfel::math::Power<3>>>::type
    computeJ3O(const StensorType& s,
	       const tfel::math::StensorBaseType<StensorType> b1,
	       const tfel::math::StensorBaseType<StensorType> b2,
	       const tfel::math::StensorBaseType<StensorType> b3,
	       const tfel::math::StensorBaseType<StensorType> b4,
	       const tfel::math::StensorBaseType<StensorType> b5,
	       const tfel::math::StensorBaseType<StensorType> b6,
	       const tfel::math::StensorBaseType<StensorType> b7,
	       const tfel::math::StensorBaseType<StensorType> b8,
	       const tfel::math::StensorBaseType<StensorType> b9,
	       const tfel::math::StensorBaseType<StensorType> b10,
	       const tfel::math::StensorBaseType<StensorType> b11){
      using real = tfel::math::StensorBaseType<StensorType>;
      TFEL_CONSTEXPR const auto icste = tfel::math::Cste<real>::isqrt2;
      return ((2*(b4+b1)-b3-b2)*s[2]*s[2]*s[2])/27
	+((b4+b3)*s[1]*s[1]*s[1])/27
	+((b2+b1)*s[0]*s[0]*s[0])/27
	+(-((-b7*s[2]-b6*s[1]+(b7+b6)*s[0])*s[5]*s[5])/6)
	-(((-b8*s[2])+2*b9*s[1]+(b8-2*b9)*s[0])*s[4]*s[4])/6
	-((2*b10*s[2]-b5*s[1]+(b5-2*b10)*s[0])*s[3]*s[3])/6
	-(((b4+b1-b3)*s[1]+(b4-b2+b1)*s[0])*s[2]*s[2])/9
	-(s[1]*s[1]*(b3*s[2]+b4*s[0]))/9
	+(2*(b4+b1)*s[0]*s[1]*s[2])/9
	-(s[0]*s[0]*(b1*s[1]+b2*s[2]))/9
	+(b11*s[3]*s[4]*s[5])*icste;
    }

    template<typename StensorType>
    typename std::enable_if<
      tfel::meta::Implements<StensorType,tfel::math::StensorConcept>::cond,
      tfel::math::stensor<tfel::math::StensorTraits<StensorType>::dime,
			  tfel::math::UnaryOperationResult<tfel::math::StensorNumType<StensorType>,
							   tfel::math::Power<2>>>>::type
    computeJ3ODerivative(const StensorType& s,
			 const J3OCoefficients<StensorType>& b){
      return computeJ3ODerivative(s,b[0],b[1],b[2],b[3],b[4],b[5],
				  b[6],b[7],b[8],b[9],b[10]);
    } // end of computeJ3ODerivative

    template<typename StensorType>
    typename std::enable_if<
      tfel::meta::Implements<StensorType,tfel::math::StensorConcept>::cond&&
      tfel::math::StensorTraits<StensorType>::dime==1u,
      tfel::math::stensor<tfel::math::StensorTraits<StensorType>::dime,
			  tfel::math::UnaryOperationResult<tfel::math::StensorNumType<StensorType>,
							   tfel::math::Power<2>>>>::type
    computeJ3ODerivative(const StensorType& s,
			 const tfel::math::StensorBaseType<StensorType> b1,
			 const tfel::math::StensorBaseType<StensorType> b2,
			 const tfel::math::StensorBaseType<StensorType> b3,
			 const tfel::math::StensorBaseType<StensorType> b4,
			 const tfel::math::StensorBaseType<StensorType>,
			 const tfel::math::StensorBaseType<StensorType>,
			 const tfel::math::StensorBaseType<StensorType>,
			 const tfel::math::StensorBaseType<StensorType>,
			 const tfel::math::StensorBaseType<StensorType>,
			 const tfel::math::StensorBaseType<StensorType>,
			 const tfel::math::StensorBaseType<StensorType>){
      using namespace tfel::math;
      auto dJ3O = stensor<1u,UnaryOperationResult<StensorNumType<StensorType>,
						  Power<2>>>{};
      dJ3O[0] = -((b4-b2+b1)*s[2]*s[2])/9+(2*(b4+b1)*s[1]*s[2])/9-
	(b4*s[1]*s[1])/9+(-2*s[0]*(b1*s[1]+b2*s[2]))/9+((b2+b1)*s[0]*s[0])/9;
      dJ3O[1] = -((b4+b1-b3)*s[2]*s[2])/9+(-2*s[1]*(b3*s[2]+b4*s[0]))/9+
	(2*(b4+b1)*s[0]*s[2])/9+((b4+b3)*s[1]*s[1])/9-(b1*s[0]*s[0])/9;
      dJ3O[2] = ((2*(b4+b1)-b3-b2)*s[2]*s[2])/9+
	(-2*((b4+b1-b3)*s[1]+(b4-b2+b1)*s[0])*s[2])/9-
	(b3*s[1]*s[1])/9+(2*(b4+b1)*s[0]*s[1])/9-(b2*s[0]*s[0])/9;
      return dJ3O;
    } // end of computeJ3ODerivative
    
    template<typename StensorType>
    typename std::enable_if<
      tfel::meta::Implements<StensorType,tfel::math::StensorConcept>::cond&&
      tfel::math::StensorTraits<StensorType>::dime==2u,
      tfel::math::stensor<tfel::math::StensorTraits<StensorType>::dime,
			  tfel::math::UnaryOperationResult<tfel::math::StensorNumType<StensorType>,
							   tfel::math::Power<2>>>>::type
    computeJ3ODerivative(const StensorType& s,
			 const tfel::math::StensorBaseType<StensorType> b1,
			 const tfel::math::StensorBaseType<StensorType> b2,
			 const tfel::math::StensorBaseType<StensorType> b3,
			 const tfel::math::StensorBaseType<StensorType> b4,
			 const tfel::math::StensorBaseType<StensorType> b5,
			 const tfel::math::StensorBaseType<StensorType>,
			 const tfel::math::StensorBaseType<StensorType>,
			 const tfel::math::StensorBaseType<StensorType>,
			 const tfel::math::StensorBaseType<StensorType>,
			 const tfel::math::StensorBaseType<StensorType> b10,
			 const tfel::math::StensorBaseType<StensorType>){
      using namespace tfel::math;
      auto dJ3O = stensor<2u,UnaryOperationResult<StensorNumType<StensorType>,
						 Power<2>>>{};
      dJ3O[0] = -((b5-2*b10)*s[3]*s[3])/6-((b4-b2+b1)*s[2]*s[2])/9+(2*(b4+b1)*s[1]*s[2])/9-
	(b4*s[1]*s[1])/9+(-2*s[0]*(b1*s[1]+b2*s[2]))/9+((b2+b1)*s[0]*s[0])/9;
      dJ3O[1] = (b5*s[3]*s[3])/6-((b4+b1-b3)*s[2]*s[2])/9+(-2*s[1]*(b3*s[2]+b4*s[0]))/9+
	(2*(b4+b1)*s[0]*s[2])/9+((b4+b3)*s[1]*s[1])/9-(b1*s[0]*s[0])/9;
      dJ3O[2] = -(b10*s[3]*s[3])/3+((2*(b4+b1)-b3-b2)*s[2]*s[2])/9+
	(-2*((b4+b1-b3)*s[1]+(b4-b2+b1)*s[0])*s[2])/9-
	(b3*s[1]*s[1])/9+(2*(b4+b1)*s[0]*s[1])/9-(b2*s[0]*s[0])/9;
      dJ3O[3] = -((2*b10*s[2]-b5*s[1]+(b5-2*b10)*s[0])*s[3])/3;
      return dJ3O;
    } // end of computeJ3ODerivative
    
    template<typename StensorType>
    typename std::enable_if<
      tfel::meta::Implements<StensorType,tfel::math::StensorConcept>::cond&&
      tfel::math::StensorTraits<StensorType>::dime==3u,
      tfel::math::stensor<tfel::math::StensorTraits<StensorType>::dime,
			  tfel::math::UnaryOperationResult<tfel::math::StensorNumType<StensorType>,
							   tfel::math::Power<2>>>>::type
    computeJ3ODerivative(const StensorType& s,
			 const tfel::math::StensorBaseType<StensorType> b1,
			 const tfel::math::StensorBaseType<StensorType> b2,
			 const tfel::math::StensorBaseType<StensorType> b3,
			 const tfel::math::StensorBaseType<StensorType> b4,
			 const tfel::math::StensorBaseType<StensorType> b5,
			 const tfel::math::StensorBaseType<StensorType> b6,
			 const tfel::math::StensorBaseType<StensorType> b7,
			 const tfel::math::StensorBaseType<StensorType> b8,
			 const tfel::math::StensorBaseType<StensorType> b9,
			 const tfel::math::StensorBaseType<StensorType> b10,
			 const tfel::math::StensorBaseType<StensorType> b11){
      using namespace tfel::math;
      using real = StensorBaseType<StensorType>;
      TFEL_CONSTEXPR const auto icste = Cste<real>::isqrt2;
      auto dJ3O = stensor<3u,UnaryOperationResult<StensorNumType<StensorType>,
						 Power<2>>>{};
      dJ3O[0] = (-((b7+b6)*s[5]*s[5])/6)-((b8-2*b9)*s[4]*s[4])/6-
	((b5-2*b10)*s[3]*s[3])/6-((b4-b2+b1)*s[2]*s[2])/9+(2*(b4+b1)*s[1]*s[2])/9-
	(b4*s[1]*s[1])/9+(-2*s[0]*(b1*s[1]+b2*s[2]))/9+((b2+b1)*s[0]*s[0])/9;
      dJ3O[1] = (b6*s[5]*s[5])/6-(b9*s[4]*s[4])/3+(b5*s[3]*s[3])/6-
	((b4+b1-b3)*s[2]*s[2])/9+(-2*s[1]*(b3*s[2]+b4*s[0]))/9+
	(2*(b4+b1)*s[0]*s[2])/9+((b4+b3)*s[1]*s[1])/9-(b1*s[0]*s[0])/9;
      dJ3O[2] = (b7*s[5]*s[5])/6+(b8*s[4]*s[4])/6-
	(b10*s[3]*s[3])/3+((2*(b4+b1)-b3-b2)*s[2]*s[2])/9+
	(-2*((b4+b1-b3)*s[1]+(b4-b2+b1)*s[0])*s[2])/9-
	(b3*s[1]*s[1])/9+(2*(b4+b1)*s[0]*s[1])/9-(b2*s[0]*s[0])/9;
      dJ3O[3] = (b11*s[4]*s[5])*icste-
	((2*b10*s[2]-b5*s[1]+(b5-2*b10)*s[0])*s[3])/3;
      dJ3O[4] = (b11*s[3]*s[5])*icste-
	(((-b8*s[2])+2*b9*s[1]+(b8-2*b9)*s[0])*s[4])/3;
      dJ3O[5] = (b11*s[3]*s[4])*icste-
	(((-b7*s[2])-b6*s[1]+(b7+b6)*s[0])*s[5])/3;
      return dJ3O;
    } // end of computeJ3ODerivative

    template<typename StensorType>
    typename std::enable_if<
      tfel::meta::Implements<StensorType,tfel::math::StensorConcept>::cond,
      tfel::math::st2tost2<tfel::math::StensorTraits<StensorType>::dime,
			   tfel::math::StensorNumType<StensorType>>>::type
    computeJ3OSecondDerivative(const StensorType& s,
			       const J3OCoefficients<StensorType>& b){
      return computeJ3OSecondDerivative(s,b[0],b[1],b[2],b[3],b[4],b[5],
					b[6],b[7],b[8],b[9],b[10]);
    } // end of computeJ3OSecondDerivative

    template<typename StensorType>
    typename std::enable_if<
      tfel::meta::Implements<StensorType,tfel::math::StensorConcept>::cond&&
      tfel::math::StensorTraits<StensorType>::dime==1u,
      tfel::math::st2tost2<tfel::math::StensorTraits<StensorType>::dime,
			   tfel::math::StensorNumType<StensorType>>>::type
    computeJ3OSecondDerivative(const StensorType& s,
			       const tfel::math::StensorBaseType<StensorType> b1,
			       const tfel::math::StensorBaseType<StensorType> b2,
			       const tfel::math::StensorBaseType<StensorType> b3,
			       const tfel::math::StensorBaseType<StensorType> b4,
			       const tfel::math::StensorBaseType<StensorType>,
			       const tfel::math::StensorBaseType<StensorType>,
			       const tfel::math::StensorBaseType<StensorType>,
			       const tfel::math::StensorBaseType<StensorType>,
			       const tfel::math::StensorBaseType<StensorType>,
			       const tfel::math::StensorBaseType<StensorType>,
			       const tfel::math::StensorBaseType<StensorType>){
      auto d2J3O = tfel::math::st2tost2<1u,tfel::math::StensorNumType<StensorType>>{};
      d2J3O(0,0) = (-2*(b2*s[2]+b1*s[1]))/9+(2*(b2+b1)*s[0])/9;
      d2J3O(0,1) = (2*(b4+b1)*s[2])/9+(-2*b4*s[1])/9+(-2*b1*s[0])/9;
      d2J3O(0,2) = (-2*(b4-b2+b1)*s[2])/9+(2*(b4+b1)*s[1])/9+(-2*b2*s[0])/9;
      d2J3O(1,0) = (2*(b4+b1)*s[2])/9+(-2*b4*s[1])/9+(-2*b1*s[0])/9;
      d2J3O(1,1) = (-2*(b3*s[2]+b4*s[0]))/9+(2*(b4+b3)*s[1])/9;
      d2J3O(1,2) = (-2*(b4+b1-b3)*s[2])/9+(-2*b3*s[1])/9+(2*(b4+b1)*s[0])/9;
      d2J3O(2,0) = (-2*(b4-b2+b1)*s[2])/9+(2*(b4+b1)*s[1])/9+(-2*b2*s[0])/9;
      d2J3O(2,1) = (-2*(b4+b1-b3)*s[2])/9+(-2*b3*s[1])/9+(2*(b4+b1)*s[0])/9;
      d2J3O(2,2) = (2*(2*(b4+b1)-b3-b2)*s[2])/9+(-2*((b4+b1-b3)*s[1]+(b4-b2+b1)*s[0]))/9;
      return d2J3O;
    } // end of computeJ3OSecondDerivative
    
    template<typename StensorType>
    typename std::enable_if<
      tfel::meta::Implements<StensorType,tfel::math::StensorConcept>::cond&&
      tfel::math::StensorTraits<StensorType>::dime==2u,
      tfel::math::st2tost2<tfel::math::StensorTraits<StensorType>::dime,
			   tfel::math::StensorNumType<StensorType>>>::type
    computeJ3OSecondDerivative(const StensorType& s,
			       const tfel::math::StensorBaseType<StensorType> b1,
			       const tfel::math::StensorBaseType<StensorType> b2,
			       const tfel::math::StensorBaseType<StensorType> b3,
			       const tfel::math::StensorBaseType<StensorType> b4,
			       const tfel::math::StensorBaseType<StensorType> b5,
			       const tfel::math::StensorBaseType<StensorType>,
			       const tfel::math::StensorBaseType<StensorType>,
			       const tfel::math::StensorBaseType<StensorType>,
			       const tfel::math::StensorBaseType<StensorType>,
			       const tfel::math::StensorBaseType<StensorType> b10,
			       const tfel::math::StensorBaseType<StensorType>){
      auto d2J3O = tfel::math::st2tost2<2u,tfel::math::StensorNumType<StensorType>>{};
      d2J3O(0,0) = (-2*(b2*s[2]+b1*s[1]))/9+(2*(b2+b1)*s[0])/9;
      d2J3O(0,1) = (2*(b4+b1)*s[2])/9+(-2*b4*s[1])/9+(-2*b1*s[0])/9;
      d2J3O(0,2) = (-2*(b4-b2+b1)*s[2])/9+(2*(b4+b1)*s[1])/9+(-2*b2*s[0])/9;
      d2J3O(0,3) = -((b5-2*b10)*s[3])/3;
      d2J3O(1,0) = (2*(b4+b1)*s[2])/9+(-2*b4*s[1])/9+(-2*b1*s[0])/9;
      d2J3O(1,1) = (-2*(b3*s[2]+b4*s[0]))/9+(2*(b4+b3)*s[1])/9;
      d2J3O(1,2) = (-2*(b4+b1-b3)*s[2])/9+(-2*b3*s[1])/9+(2*(b4+b1)*s[0])/9;
      d2J3O(1,3) = (b5*s[3])/3;
      d2J3O(2,0) = (-2*(b4-b2+b1)*s[2])/9+(2*(b4+b1)*s[1])/9+(-2*b2*s[0])/9;
      d2J3O(2,1) = (-2*(b4+b1-b3)*s[2])/9+(-2*b3*s[1])/9+(2*(b4+b1)*s[0])/9;
      d2J3O(2,2) = (2*(2*(b4+b1)-b3-b2)*s[2])/9+(-2*((b4+b1-b3)*s[1]+(b4-b2+b1)*s[0]))/9;
      d2J3O(2,3) = (-2*b10*s[3])/3;
      d2J3O(3,0) = -((b5-2*b10)*s[3])/3;
      d2J3O(3,1) = (b5*s[3])/3;
      d2J3O(3,2) = (-2*b10*s[3])/3;
      d2J3O(3,3) = -(2*b10*s[2]-b5*s[1]+(b5-2*b10)*s[0])/3;
      return d2J3O;
    } // end of computeJ3OSecondDerivative

    template<typename StensorType>
    typename std::enable_if<
      tfel::meta::Implements<StensorType,tfel::math::StensorConcept>::cond&&
      tfel::math::StensorTraits<StensorType>::dime==3u,
      tfel::math::st2tost2<tfel::math::StensorTraits<StensorType>::dime,
			   tfel::math::StensorNumType<StensorType>>>::type
    computeJ3OSecondDerivative(const StensorType& s,
			       const tfel::math::StensorBaseType<StensorType> b1,
			       const tfel::math::StensorBaseType<StensorType> b2,
			       const tfel::math::StensorBaseType<StensorType> b3,
			       const tfel::math::StensorBaseType<StensorType> b4,
			       const tfel::math::StensorBaseType<StensorType> b5,
			       const tfel::math::StensorBaseType<StensorType> b6,
			       const tfel::math::StensorBaseType<StensorType> b7,
			       const tfel::math::StensorBaseType<StensorType> b8,
			       const tfel::math::StensorBaseType<StensorType> b9,
			       const tfel::math::StensorBaseType<StensorType> b10,
			       const tfel::math::StensorBaseType<StensorType> b11){
      using namespace tfel::math;
      using real = StensorBaseType<StensorType>;
      TFEL_CONSTEXPR const auto icste = Cste<real>::isqrt2;
      auto d2J3O = st2tost2<3u,StensorNumType<StensorType>>{};
      d2J3O(0,0) = (-2*(b2*s[2]+b1*s[1]))/9+(2*(b2+b1)*s[0])/9;
      d2J3O(0,1) = (2*(b4+b1)*s[2])/9+(-2*b4*s[1])/9+(-2*b1*s[0])/9;
      d2J3O(0,2) = (-2*(b4-b2+b1)*s[2])/9+(2*(b4+b1)*s[1])/9+(-2*b2*s[0])/9;
      d2J3O(0,3) = -((b5-2*b10)*s[3])/3;
      d2J3O(0,4) = -((b8-2*b9)*s[4])/3;
      d2J3O(0,5) = -((b7+b6)*s[5])/3;
      d2J3O(1,0) = (2*(b4+b1)*s[2])/9+(-2*b4*s[1])/9+(-2*b1*s[0])/9;
      d2J3O(1,1) = (-2*(b3*s[2]+b4*s[0]))/9+(2*(b4+b3)*s[1])/9;
      d2J3O(1,2) = (-2*(b4+b1-b3)*s[2])/9+(-2*b3*s[1])/9+(2*(b4+b1)*s[0])/9;
      d2J3O(1,3) = (b5*s[3])/3;
      d2J3O(1,4) = (-2*b9*s[4])/3;
      d2J3O(1,5) = (b6*s[5])/3;
      d2J3O(2,0) = (-2*(b4-b2+b1)*s[2])/9+(2*(b4+b1)*s[1])/9+(-2*b2*s[0])/9;
      d2J3O(2,1) = (-2*(b4+b1-b3)*s[2])/9+(-2*b3*s[1])/9+(2*(b4+b1)*s[0])/9;
      d2J3O(2,2) = (2*(2*(b4+b1)-b3-b2)*s[2])/9+(-2*((b4+b1-b3)*s[1]+(b4-b2+b1)*s[0]))/9;
      d2J3O(2,3) = (-2*b10*s[3])/3;
      d2J3O(2,4) = (b8*s[4])/3;
      d2J3O(2,5) = (b7*s[5])/3;
      d2J3O(3,0) = -((b5-2*b10)*s[3])/3;
      d2J3O(3,1) = (b5*s[3])/3;
      d2J3O(3,2) = (-2*b10*s[3])/3;
      d2J3O(3,3) = -(2*b10*s[2]-b5*s[1]+(b5-2*b10)*s[0])/3;
      d2J3O(3,4) = (b11*s[5])*icste;
      d2J3O(3,5) = (b11*s[4])*icste;
      d2J3O(4,0) = -((b8-2*b9)*s[4])/3;
      d2J3O(4,1) = (-2*b9*s[4])/3;
      d2J3O(4,2) = (b8*s[4])/3;
      d2J3O(4,3) = (b11*s[5])*icste;
      d2J3O(4,4) = -((-b8*s[2])+2*b9*s[1]+(b8-2*b9)*s[0])/3;
      d2J3O(4,5) = (b11*s[3])*icste;
      d2J3O(5,0) = -((b7+b6)*s[5])/3;
      d2J3O(5,1) = (b6*s[5])/3;
      d2J3O(5,2) = (b7*s[5])/3;
      d2J3O(5,3) = (b11*s[4])*icste;
      d2J3O(5,4) = (b11*s[3])*icste;
      d2J3O(5,5) = -((-b7*s[2])-b6*s[1]+(b7+b6)*s[0])/3;
      return d2J3O;
    } // end of computeJ3OSecondDerivative

  } // end of namespace material

} // end of namespace tfel

#endif /* LIBTFEL_MATERIAL_ORTHOTROPICPLASTICITY_IXX */
