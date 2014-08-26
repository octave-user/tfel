/*! 
 * \file  MapConverter.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 11 mars 2013
 */

#ifndef _LIB_TFEL_PYTHON_MAPCONVERTER_H_
#define _LIB_TFEL_PYTHON_MAPCONVERTER_H_ 

#include<map>

#include<boost/python.hpp>
#include<boost/python/stl_iterator.hpp>

namespace tfel
{

  namespace python
  {

    /*!
     * convert map converter to python dict
     */
    template<typename K,
	     typename V>
    struct map_to_python_dict
    {
      static PyObject* convert(const std::map<K,V>& v)
      {
	using namespace std;
	using namespace boost::python;
	using boost::python::iterator;
	typename map<K,V>::const_iterator p;
	dict d;
	for(p=v.begin();p!=v.end();++p){
	  d[p->first] = p->second;
	}
	return incref(d.ptr());
      }
    };

    template<typename K,
	     typename V>
    struct map_from_python_dict
    {

      map_from_python_dict()
      {
	using boost::python::type_id;
	using namespace boost::python::converter;
	registry::push_back(&convertible,&construct,
			    type_id<std::map<K,V> > ());
      }

      static void* convertible(PyObject * ptr)
      {
	using namespace boost::python;
	if (!PyDict_Check(ptr)){
	  return 0;
	}
	handle<> h(borrowed(ptr));
	dict d(h);
	list keys = d.keys();
	list values = d.values();
	stl_input_iterator<object> pk(keys);
	stl_input_iterator<object> pv(values);
	stl_input_iterator<object> pe;
	while(pk!=pe){
	  extract<K> k(*pk);
	  extract<V> v(*pv);
	  if(!k.check()){
	    return 0;
	  }
	  if(!v.check()){
	    return 0;
	  }
	  ++pk;
	  ++pv;
	}
	return ptr;
      }

      static void construct(PyObject* ptr,
			    boost::python::converter::rvalue_from_python_stage1_data * data)
      {
	using namespace boost::python;
	using namespace boost::python::converter;
	typedef rvalue_from_python_storage<std::map<K,V> > py_storage;
	using std::map;
	assert(PyDict_Check(ptr));
	handle<> h(borrowed(ptr));
	dict d(h);
	list keys = d.keys();
	list values = d.values();
	stl_input_iterator<object> pk(keys);
	stl_input_iterator<object> pv(values);
	stl_input_iterator<object> pe;
	// Grab pointer to memory into which to construct the new map<T>
	void* storage = reinterpret_cast<py_storage *>(data)->storage.bytes;
	// in-place construct the new map<T> using the character data
	// extraced from the python object
	new (storage) map<K,V>();
	map<K,V>& m = *(static_cast<map<K,V> *>(storage));
	while(pk!=pe){
	  extract<K> k(*pk);
	  extract<V> v(*pv);
	  assert(k.check());
	  assert(v.check());
	  m[k()] = v();
	  ++pk;
	  ++pv;
	}    
	// Stash the memory chunk pointer for later use by boost.python
	data->convertible = storage;
      }

    };

    template<typename K,
	     typename V> 
    static void initializeMapConverter()
    {
      using namespace boost::python;
      using std::map;
      // register the to-python converter
      to_python_converter<map<K,V>,map_to_python_dict<K,V> >();
      // register the from-python converter
      map_from_python_dict<K,V>();
    }

  } // end of namespace python

} // end of namespace tfel

#endif /* _LIB_TFEL_PYTHON_MAPCONVERTER_H */
