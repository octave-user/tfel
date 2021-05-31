/*!
 * \file   include/TFEL/Math/General/DerivativeType.hxx
 * \brief
 * \author Thomas Helfer
 * \date   24/06/2020
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_DERIVATIVETYPE_HXX
#define LIB_TFEL_MATH_DERIVATIVETYPE_HXX

#include <type_traits>
#include "TFEL/Metaprogramming/InvalidType.hxx"
#include "TFEL/Math/General/ComputeObjectTag.hxx"
#include "TFEL/Math/General/MathObjectTraits.hxx"
#include "TFEL/Math/Array/FixedSizeArrayDerivative.hxx"

namespace tfel::math {

  /*!
   * \brief an helper metafunction used to determine the type representing
   * the derivative of an object of type `FunctionType` with respect to an
   * object of type `VariableType`.
   *
   * \tparam FunctionTag: tag associated with the function type
   * \tparam VariableTag: tag associated with the variable type
   * \tparam FunctionType: function type
   * \tparam VariableType: variable type
   *
   * This helper metafunction must be specialised by every code.
   */
  template <typename FunctionTag,
            typename VariableTag,
            typename FunctionType,
            typename VariableType>
  struct DerivativeTypeDispatcher {
    //! \brief a simple alias
    using FunctionArrayPolicy = typename FunctionType::array_policy;
    //! \brief a simple alias
    using VariableArrayPolicy = typename VariableType::array_policy;
    //! \brief a boolean stating if both objects have fixed sizes
    static constexpr auto haveFixedSizes =
        (hasArrayPolicyFixedSizes<FunctionArrayPolicy>() &&
         hasArrayPolicyFixedSizes<VariableArrayPolicy>());
    //! \brief result
    using type =
        std::conditional_t<haveFixedSizes,
                           FixedSizeArrayDerivative<FunctionType, VariableType>,
                           tfel::meta::InvalidType>;
  };  // end of struct DerivativeTypeDispatcher

  /*!
   * \brief partial specialisation if the first object does not have a tag.
   * \tparam VariableTag: tag associated with the variable type
   * \tparam FunctionType: function type
   * \tparam VariableType: variable type
   */
  template <typename VariableTag, typename FunctionType, typename VariableType>
  struct DerivativeTypeDispatcher<tfel::meta::InvalidType,
                                  VariableTag,
                                  FunctionType,
                                  VariableType> {
    //! \brief result
    using type = tfel::meta::InvalidType;
  };  // end of struct DerivativeTypeDispatcher

  /*!
   * \brief partial specialisation if the second object does not have a tag.
   * \tparam FunctionTag: tag associated with the function type
   * \tparam FunctionType: function type
   * \tparam VariableType: variable type
   */
  template <typename FunctionTag, typename FunctionType, typename VariableType>
  struct DerivativeTypeDispatcher<FunctionTag,
                                  tfel::meta::InvalidType,
                                  FunctionType,
                                  VariableType> {
    //! \brief result
    using type = tfel::meta::InvalidType;
  };  // end of struct DerivativeTypeDispatcher

  /*!
   * \brief partial specialisation if none o the two objects have a tag.
   * \tparam FunctionType: function type
   * \tparam VariableType: variable type
   */
  template <typename FunctionType, typename VariableType>
  struct DerivativeTypeDispatcher<tfel::meta::InvalidType,
                                  tfel::meta::InvalidType,
                                  FunctionType,
                                  VariableType> {
    //! \brief result
    using type = tfel::meta::InvalidType;
  };  // end of struct DerivativeTypeDispatcher

  /*!
   * \brief a meta function returning the type of the derivative of
   * a variable of type `FunctionType` with respect to a variable of type
   * `VariableType`.
   * \tparam is_function_type_scalar: boolean stating if `FunctionType` is a
   * scalar
   * \tparam is_variable_type_scalar: boolean stating if `VariableType` is a
   * scalar
   * \tparam FunctionType: function type
   * \tparam VariableType: variable type
   */
  template <bool is_function_type_scalar,
            bool is_variable_type_scalar,
            typename FunctionType,
            typename VariableType>
  struct DerivativeTypeImplementation {
    //! \brief in none is scalar, use the derivative type dispatcher
    using type = typename tfel::math::DerivativeTypeDispatcher<
        typename ComputeObjectTag<FunctionType>::type,
        typename ComputeObjectTag<VariableType>::type,
        FunctionType,
        VariableType>::type;
  };  // end of DerivativeTypeImplementation

  /*!
   * \brief partial specialisation of both types are scalar
   * \tparam FunctionType: function type
   * \tparam VariableType: variable type
   */
  template <typename FunctionType, typename VariableType>
  struct DerivativeTypeImplementation<true, true, FunctionType, VariableType> {
    //! \brief the result
    using type = result_type<FunctionType, VariableType, OpDiv>;
  };

  /*!
   * \brief a meta function returning the type of the derivative of
   * a variable of type `FunctionType` with respect to a variable of type
   * `VariableType`.
   * \tparam FunctionType: function type
   * \tparam VariableType: variable type
   */
  template <typename FunctionType, typename VariableType>
  struct DerivativeType {
    //! \brief the result
    using type = typename DerivativeTypeImplementation<isScalar<FunctionType>(),
                                                       isScalar<VariableType>(),
                                                       FunctionType,
                                                       VariableType>::type;
  };  // end of DerivativeType

  /*!
   * \brief a simple alias giving the type representing the derivative of an
   * object of type `FunctionType` with respect to an object of type
   * `VariableType`.
   * \param[in] FunctionType: function type
   * \param[in] VariableType: variable type
   */
  template <typename FunctionType, typename VariableType>
  using derivative_type =
      typename DerivativeType<FunctionType, VariableType>::type;

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_DERIVATIVETYPE_HXX */
