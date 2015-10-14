/*! 
 * \file  mtest/include/MTest/Types.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 05 avril 2013
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef LIB_MTEST_MTESTTYPES_H_
#define LIB_MTEST_MTESTTYPES_H_ 

#include<map>
#include<string>
#include<memory>

namespace mtest
{
  
  //! a simple alias
  typedef double real;
  
  //! forward declaration
  struct Evolution;
  // ! a simple alias
  using EvolutionPtr = std::shared_ptr<Evolution>;
  // ! a simple alias
  using EvolutionManager = std::map<std::string,EvolutionPtr>;

  /*!
   * \brief possible type of stiffness matrix
   */
  struct StiffnessMatrixType
  {
    //! list of possible matrix type
    enum mtype{
      NOSTIFFNESS,
      ELASTIC,
      SECANTOPERATOR,
      TANGENTOPERATOR,
      CONSISTENTTANGENTOPERATOR,
      // This one is mostly for the umat behaviour
      ELASTICSTIFNESSFROMMATERIALPROPERTIES,
      UNSPECIFIEDSTIFFNESSMATRIXTYPE
    }; // end of struct StiffnessMatrixType
  };

} // end of namespace mtest

#endif /* LIB_MTEST_MTESTTYPES_H_ */
