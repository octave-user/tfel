/*! 
 * \file  tests/Math/t2tot2/TensorProductDerivative.cxx
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

#ifdef NDEBUG
#undef NDEBUG
#endif /* NDEBUG */

#include<cmath>
#include<cstdlib>
#include<cassert>

#include"TFEL/Math/tensor.hxx"
#include"TFEL/Math/t2tot2.hxx"
#include"TFEL/Utilities/ToString.hxx"

#include"TFEL/Tests/TestCase.hxx"
#include"TFEL/Tests/TestProxy.hxx"
#include"TFEL/Tests/TestManager.hxx"

template<unsigned short N>
struct TensorProductDerivative
  : public tfel::tests::TestCase
{
  TensorProductDerivative()
    : tfel::tests::TestCase("TFEL/Math",
			    "TensorProductDerivative"+
			    tfel::utilities::ToString(N))
  {} // end of TensorProductDerivative
  tfel::tests::TestResult
  execute()
  {
    using namespace std;
    using namespace tfel::math;
    const double eps  = 1.e-3;
    const double prec = 1.e-10;
    tensor<N> t1;
    for(typename tensor<N>::size_type i=0;i!=t1.size();++i){
      t1[i] = 0.3+i;
    }
    tensor<N> t2;
    for(typename tensor<N>::size_type i=0;i!=t2.size();++i){
      t2[i] = -0.45+i;
    }
    tensor<N> t3(t1*t2);
    t2tot2<N> ld = t2tot2<N>::tpld(t2);
    t2tot2<N> rd = t2tot2<N>::tprd(t1);
    t2tot2<N> nld;
    t2tot2<N> nrd;
    for(typename tensor<N>::size_type i=0;i!=t1.size();++i){
      tensor<N> t1e(t1);
      tensor<N> t2e(t2);
      t1e[i] += eps;
      t2e[i] += eps;
      tensor<N> t3l(t1e*t2);
      tensor<N> t3r(t1*t2e);
      for(typename tensor<N>::size_type j=0;j!=t1.size();++j){
	nld(j,i) = (t3l(j)-t3(j))/eps;
	nrd(j,i) = (t3r(j)-t3(j))/eps;
      }
    }
    for(typename tensor<N>::size_type i=0;i!=t1.size();++i){
      for(typename tensor<N>::size_type j=0;j!=t1.size();++j){
	TFEL_TESTS_ASSERT(abs(nld(i,j)-ld(i,j))<prec);
	// if(abs(nld(i,j)-ld(i,j))>1.e-7){
	//   cout << "Error nld (" << N << "): " << i << " " << j << " "
	//        << nld(i,j) << " vs " << ld(i,j) << " " << abs(nld(i,j)-ld(i,j)) << endl;
	// }
      }
    }
    for(typename tensor<N>::size_type i=0;i!=t1.size();++i){
      for(typename tensor<N>::size_type j=0;j!=t1.size();++j){
	TFEL_TESTS_ASSERT(abs(nrd(i,j)-rd(i,j))<prec);
	// if(abs(nrd(i,j)-rd(i,j))>1.e-7){
	// {
	//   cout << "Error nrd (" << N << "): " << i << " " << j << " "
	//        << nrd(i,j) << " vs " << rd(i,j) << " " << abs(nrd(i,j)-rd(i,j)) << endl;
	// }
      }
    }
    return this->result;
  } // end of execute
};

typedef TensorProductDerivative<1u> TensorProductDerivative_1D;
typedef TensorProductDerivative<2u> TensorProductDerivative_2D;
typedef TensorProductDerivative<3u> TensorProductDerivative_3D;
TFEL_TESTS_GENERATE_PROXY(TensorProductDerivative_1D,"TensorProductDerivative-1D");
TFEL_TESTS_GENERATE_PROXY(TensorProductDerivative_2D,"TensorProductDerivative-2D");
TFEL_TESTS_GENERATE_PROXY(TensorProductDerivative_3D,"TensorProductDerivative-3D");

int main(void){
  using namespace std;
  using namespace std;
  using namespace tfel::tests;
  using namespace tfel::utilities;
  TestManager& manager = TestManager::getTestManager();
  manager.addTestOutput(cout);
  manager.addXMLTestOutput("TensorProductDerivative.xml");
  TestResult r = manager.execute();
  if(!r.success()){
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
