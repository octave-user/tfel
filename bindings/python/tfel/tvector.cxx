/*! 
 * \file  tvector.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 30 ao� 2010
 */

#include<boost/python.hpp>

#include<TFEL/Math/tvector.hxx>

template<unsigned short N>
static double&
tvector_getitem(tfel::math::tvector<N,double>& v,
		const unsigned short i)
{
  return v[i];
}

template<unsigned short N>
static void
tvector_setitem(tfel::math::tvector<N,double>& v,
		const unsigned short i,
		const double vi)
{
  v[i] = vi;
}

template<unsigned short N>
static std::string
tvector_str(const tfel::math::tvector<N,double>& v)
{
  using namespace std;
  ostringstream os;
  os << v;
  return os.str();
}


template<unsigned short N>
static void declaretvector(const char * const n)
{
  using namespace std;
  using namespace boost;
  using namespace boost::python;
  using namespace tfel::math;
  using boost::python::iterator;

  class_<tvector<N,double> >(n,init<>())
    .def(init<double>())
    .def("__getitem__",&tvector_getitem<N>,
	 return_value_policy<copy_non_const_reference>())
    .def("__setitem__",&tvector_setitem<N>,
	 with_custodian_and_ward<1,2>())
    .def("__iter__",iterator<tvector<N,double> >())
    .def("__str__",tvector_str<N>)
    ;

}

void
declaretvector()
{
  declaretvector<1u>("TVector1D");
  declaretvector<2u>("TVector2D");
  declaretvector<3u>("TVector3D");
} // end of declaretvector