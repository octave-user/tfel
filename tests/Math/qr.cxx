/*! 
 * \file  qr.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 14 avr 2009
 */

#ifdef NDEBUG
#undef NDEBUG
#endif /* NDEBUG */

#include<cmath>
#include<cassert>
#include<cstdlib>
#include<limits>

#include"TFEL/Tests/Test.hxx"
#include"TFEL/Tests/TestManager.hxx"
#include"TFEL/Tests/TestFunctionWrapper.hxx"

#include"TFEL/Math/vector.hxx"
#include"TFEL/Math/matrix.hxx"
#include"TFEL/Math/QR/QRDecomp.hxx"

template<typename T>
bool QRDecompTest()
{
  using namespace std;
  using namespace tfel::math;
  using tfel::math::vector;
  typedef typename matrix<T>::size_type IndexType;
  matrix<T> m(3,3);
  vector<T> rdiag(3);
  vector<T> beta(3);
  vector<T> b(3);
  m(0,0)=0.;
  m(0,1)=2.;
  m(0,2)=1.;
  m(1,0)=1.;
  m(1,1)=0.;
  m(1,2)=0.;
  m(2,0)=3.;
  m(2,1)=0.;
  m(2,2)=1.;
  b(0) = 5;
  b(1) = -1;
  b(2) = -2;
  QRDecomp::exe(m,rdiag,beta);
  QRDecomp::tq_product(b,m,beta);
  QRDecomp::back_substitute(b,m,rdiag);
  return ((abs(b(0)+1)<10*numeric_limits<T>::epsilon())&&
	  (abs(b(1)-2)<10*numeric_limits<T>::epsilon())&&
	  (abs(b(2)-1)<10*numeric_limits<T>::epsilon()));
} // end of QRDecompTest

int main(void)
{
  using namespace std;
  using namespace tfel::tests;
  using namespace tfel::utilities;
  typedef TestFunctionWrapper<QRDecompTest<long double> >  Wrapper1;
  typedef TestFunctionWrapper<QRDecompTest<double> >       Wrapper2;
  typedef TestFunctionWrapper<QRDecompTest<float> >        Wrapper3;
  TestManager& manager = TestManager::getTestManager();
  manager.addTestOutput(cout);
  manager.addXMLTestOutput("qr.xml");
  manager.addTest("QRDecomp",shared_ptr<Test>(new Wrapper1("QRDecompTest<long double>")));
  manager.addTest("QRDecomp",shared_ptr<Test>(new Wrapper2("QRDecompTest<double>")));
  manager.addTest("QRDecomp",shared_ptr<Test>(new Wrapper3("QRDecompTest<float>")));
  TestResult r = manager.execute();
  if(!r.success()){
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
} // end of main