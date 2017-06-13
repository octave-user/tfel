/*!
 * \file   SlipSystemsDescription.cxx
 * \brief    
 * \author Thomas Helfer
 * \date   09 juin 2017
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include<string>
#include<stdexcept>
#include"NUMODIS/HCP.hxx"
#include"NUMODIS/FCC.hxx"
#include"NUMODIS/Cubic.hxx"
#include"NUMODIS/GSystem.hxx"
#include"NUMODIS/Hardening.hxx"
#include"TFEL/FSAlgorithm/equal.hxx"
#include"TFEL/Math/General/MathConstants.hxx"
#include"TFEL/Material/SlipSystemsDescription.hxx"

namespace tfel{

  namespace material{

    template<CrystalStructure cs>
    struct CristalStructureSlipSystems;

    template<>
    struct CristalStructureSlipSystems<CrystalStructure::HCP>
    {
      using numodis_handler = numodis::HCP;
      //! type of vector used to describe the slip systems
      using vector_type   = SlipSystemsDescription::vec4d;
      //! type used to describe the slip systems
      using system_type   = SlipSystemsDescription::system4d;
    }; // end of CristalStructureSlipSystems<CrystalStructure::HCP>

    template<>
    struct CristalStructureSlipSystems<CrystalStructure::Cubic>
    {
      using numodis_handler = numodis::Cubic;
      //! type of vector used to describe the slip systems
      using vector_type   = SlipSystemsDescription::vec3d;
      //! type used to describe the slip systems
      using system_type   = SlipSystemsDescription::system3d;
    }; // end of CristalStructureSlipSystems<CrystalStructure::Cubic>
    
    template<>
    struct CristalStructureSlipSystems<CrystalStructure::FCC>
    {
      using numodis_handler = numodis::FCC;
      //! type of vector used to describe the slip systems
      using vector_type   = SlipSystemsDescription::vec3d;
      //! type used to describe the slip systems
      using system_type   = SlipSystemsDescription::system3d;
    }; // end of CristalStructureSlipSystems<CrystalStructure::FCC>
    
    template<typename T,typename VectorType>
    static T to_numodis(const VectorType& v)
    {
      auto r = T{v.size()};
      for(decltype(v.size()) i=0;i!=v.size();++i){
	r[i] = v[i];
      }
      return r;
    }

    template<typename VectorType>
    static numodis::GSystem to_numodis(const VectorType& b,
				       const VectorType& n)
    {
      return numodis::GSystem{to_numodis<numodis::IBurgers>(b),
	                      to_numodis<numodis::IPlane>(n)};
    }
    
    template<typename VectorType,typename T>
    static VectorType from_numodis(const T& v)
    {
      auto r = VectorType{};
      if(r.size()!=v.getNindices()){
	throw(std::runtime_error("from_numodis: unmatched object size"));
      }
      for(decltype(v.getNindices()) i=0;i!=v.getNindices();++i){
	r[i] = v[i];
      }
      return r;
    }
    
    static SlipSystemsDescription::system
    from_numodis(const numodis::GSystem& g)
    {
      using vec3d    = SlipSystemsDescription::vec3d;
      using vec4d    = SlipSystemsDescription::vec4d;
      using system3d = SlipSystemsDescription::system3d;
      using system4d = SlipSystemsDescription::system4d;
      if(g.getIBurgers().getNindices()==3u){
	return system3d{from_numodis<vec3d>(g.getIBurgers()),
	                from_numodis<vec3d>(g.getIPlane())};
      }
      if(g.getIBurgers().getNindices()!=4u){
	throw(std::runtime_error("from_numodis: unexecpected Burgers' vector size"));
      }
      return system4d{from_numodis<vec4d>(g.getIBurgers()),
	              from_numodis<vec4d>(g.getIPlane())};
    }
    
    template<typename VectorType>
    std::string to_string(const VectorType& v,
			  const char b,const char e)
    {
      auto r = std::string{};
      r += b;
      for(typename VectorType::size_type i=0;i!=v.size();){
	r+= std::to_string(v[i]);
	if(++i!=v.size()){
	  r+=',';
	}
      }
      r += e;
      return r;	
    } // end of to_string

    template<CrystalStructure cs>
    std::vector<SlipSystemsDescription::system>
    generateSlipSystems(const typename CristalStructureSlipSystems<cs>::vector_type& b,
			const typename CristalStructureSlipSystems<cs>::vector_type& n)
    {
      using handler = typename CristalStructureSlipSystems<cs>::numodis_handler;
      const auto cd = handler{};
      const auto nb = to_numodis<numodis::IBurgers>(b);
      const auto nn = to_numodis<numodis::IPlane>(n);
      const auto ng = numodis::GSystem{cd.InitGSystem(nb,nn)};
      std::vector<numodis::GSystem> gs;
      cd.GenerateEquivalentGSystems(ng,gs);
      auto r = std::vector<SlipSystemsDescription::system>(gs.size());
      for(decltype(gs.size()) i=0;i!=gs.size();++i){
	r[i] = from_numodis(gs[i]);
      }
      return r;
    } //end of generateSlipSystems

    template<CrystalStructure cs>
    std::vector<SlipSystemsDescription::system>
    generateSlipSystems(const SlipSystemsDescription::system& g)
    {
      using system_type = typename CristalStructureSlipSystems<cs>::system_type;
      if(!g.is<system_type>()){
	throw(std::runtime_error("generateSlipSystems: unexpected system description"));
      }
      const auto& gd = g.get<system_type>();
      return generateSlipSystems<cs>(gd.burgers,gd.normal);
    } //end of generateSlipSystems
    
    template<CrystalStructure cs>
    static void
    addSlipSystemsFamily(std::vector<typename CristalStructureSlipSystems<cs>::system_type>& gs,
			 const typename CristalStructureSlipSystems<cs>::vector_type& b,
			 const typename CristalStructureSlipSystems<cs>::vector_type& n)
    {
      using system_type = typename CristalStructureSlipSystems<cs>::system_type;
      for(const auto& gn : gs){
	for(const auto& gg : generateSlipSystems<cs>(gn)){
	  const auto& g = gg.template get<system_type>();
	  if((std::equal(g.burgers.begin(),g.burgers.end(),b.begin()))||
	     (std::equal(g.normal.begin(),g.normal.end(),n.begin()))){
	    throw(std::runtime_error("SlipSystemsDescription::addSlipSystemsFamily: "
				     "slip system "+
				     to_string(b,'(',')')+to_string(n,'[',']')+
				     " already declared"));
	  }
	}
      }
      gs.push_back({b,n});
    } // end of addSlipSytem 
    
    SlipSystemsDescription::SlipSystemsDescription(const CrystalStructure s)
      : cs(s)
    {
      if((s==CrystalStructure::Cubic)||(s==CrystalStructure::FCC)){
	this->gs = std::vector<system3d>();
      } else if (s==CrystalStructure::HCP){
	this->gs = std::vector<system4d>();
      } else {
	throw(std::runtime_error("SlipSystemsDescription::SlipSystemsDescription: "
				 "unsupported crystal structure"));
      }
    } 
    
    SlipSystemsDescription::SlipSystemsDescription(const SlipSystemsDescription&) = default; 
    SlipSystemsDescription::SlipSystemsDescription(SlipSystemsDescription&&) = default;
    
    CrystalStructure SlipSystemsDescription::getCrystalStructure() const
    {
      return this->cs;
    } // end of SlipSystemsDescription::getCrystalStructure
    
    void SlipSystemsDescription::addSlipSystemsFamily(const vec3d& n,const vec3d& b)
    {
      auto throw_if = [](const bool c, const std::string& m){
	if(c){throw(std::runtime_error("SlipSystemsDescription::addSlipSystemsFamily: "+m));}
      };
      throw_if(!this->gs.is<std::vector<system3d>>(),"invalid size for Burgers' vector and normal");
      auto& gs3d = this->gs.get<std::vector<system3d>>();
      if(this->cs==CrystalStructure::Cubic){
	tfel::material::addSlipSystemsFamily<CrystalStructure::Cubic>(gs3d,b,n);
      }
      throw_if(cs!=CrystalStructure::FCC,"internal error (unknown cristal structure)");
      tfel::material::addSlipSystemsFamily<CrystalStructure::FCC>(gs3d,b,n);
    } // end of SlipSystemsDescription::addSlipSystemsFamily

    void SlipSystemsDescription::addSlipSystemsFamily(const vec4d& n,const vec4d& b)
    {
      auto throw_if = [](const bool c, const std::string& m){
	if(c){throw(std::runtime_error("SlipSystemsDescription::addSlipSystemsFamily: "+m));}
      };
      throw_if(!this->gs.is<std::vector<system4d>>(),"invalid size for Burgers' vector and normal");
      throw_if(cs!=CrystalStructure::HCP,"internal error (unknown cristal structure)");
      auto& gs4d = this->gs.get<std::vector<system4d>>();
      tfel::material::addSlipSystemsFamily<CrystalStructure::HCP>(gs4d,b,n);
    } // end of SlipSystemsDescription::addSlipSystemsFamily

    SlipSystemsDescription::size_type
    SlipSystemsDescription::getNumberOfSlipSystemsFamilies() const{
      if(this->gs.is<std::vector<system4d>>()){
	return this->gs.get<std::vector<system4d>>().size();
      } else if(this->gs.is<std::vector<system3d>>()){
	return this->gs.get<std::vector<system3d>>().size();
      }
      if(!this->gs.empty()){
	throw(std::runtime_error("SlipSystemsDescription::getNumberOfSlipSystemsFamilies: "
				 "internal error (unsupported slip system type)"));
      }
      return size_type{};
    } // end of SlipSystemsDescription::getNumberOfSlipSystemsFamilies()
    
    SlipSystemsDescription::system
    SlipSystemsDescription::getSlipSystemFamily(const size_type i) const
    {
      auto throw_if = [](const bool b, const std::string& m){
	if(b){throw(std::runtime_error("SlipSystemsDescription::getSlipSystem: "+m));}
      };
      if(this->gs.is<std::vector<system4d>>()){
	const auto& v = this->gs.get<std::vector<system4d>>();
	throw_if(i>=v.size(),"out of bounds index");
	return v[i];
      }
      throw_if(!this->gs.is<std::vector<system3d>>(),"no slip system family defined");
      const auto& v = this->gs.get<std::vector<system3d>>();
      throw_if(i>=v.size(),"out of bounds index");
      return v[i];
    }

    std::vector<SlipSystemsDescription::system>
    SlipSystemsDescription::getSlipSystems(const size_type i) const
    {
      const auto& g = this->getSlipSystemFamily(i);
      auto throw_if = [](const bool b, const std::string& m){
	if(b){throw(std::runtime_error("generateSlipSystems: "+m));}
      };
      if(this->cs==CrystalStructure::Cubic){
	return generateSlipSystems<CrystalStructure::Cubic>(g);
      } else if(this->cs==CrystalStructure::FCC){
	return generateSlipSystems<CrystalStructure::FCC>(g);
      }
      throw_if(cs!=CrystalStructure::HCP,"internal error (unknown cristal structure)");
      return generateSlipSystems<CrystalStructure::HCP>(g);
    }

    std::vector<std::vector<SlipSystemsDescription::system>>
    SlipSystemsDescription::getSlipSystems() const
    {
      const auto s = this->getNumberOfSlipSystemsFamilies();
      auto r = std::vector<std::vector<SlipSystemsDescription::system>>{s};
      for(size_type i=0;i!=this->getNumberOfSlipSystemsFamilies();++i){
	r[i] = this->getSlipSystems(i);
      }
      return r;
    }  // end of SlipSystemsDescription::getSlipSystems

    static SlipSystemsDescription::tensor
    getOrientationTensor(const SlipSystemsDescription::system3d& s)
    {
      const auto cm = tfel::math::Cste<long double>::isqrt2;
      const auto cn = tfel::math::Cste<long double>::isqrt3;
      const auto ns = cn*s.burgers;
      const auto ms = cm*s.normal; 
      auto mu = SlipSystemsDescription::tensor{};
      mu[0]=ns[0]*ms[0]; // XX
      mu[1]=ns[1]*ms[1]; // YY
      mu[2]=ns[2]*ms[2]; // ZZ
      mu[3]=ns[1]*ms[0]; // XY
      mu[4]=ns[0]*ms[1]; // YX
      mu[5]=ns[2]*ms[0]; // XZ
      mu[6]=ns[0]*ms[2]; // ZX
      mu[7]=ns[2]*ms[1]; // YZ
      mu[8]=ns[1]*ms[2]; // ZY
      return mu;
    } // end of getOrentiationTensor

    static SlipSystemsDescription::tensor
    getOrientationTensor(const SlipSystemsDescription::system4d&)
    {
      throw(std::runtime_error("getOrentiationTensor: "
			       "internal error (unsupported slip system type)"));
    } // end of getOrentiationTensor

    static SlipSystemsDescription::tensor
    getOrientationTensor(const SlipSystemsDescription::system& s)
    {
      if(s.is<SlipSystemsDescription::system3d>()){
	return getOrientationTensor(s.get<SlipSystemsDescription::system3d>());
      }
      if(!s.is<SlipSystemsDescription::system4d>()){
	throw(std::runtime_error("getOrentiationTensor: "
				 "internal error (unsupported slip system type)"));
      }
      return getOrientationTensor(s.get<SlipSystemsDescription::system4d>());
    } // end of getOrentiationTensor

    static SlipSystemsDescription::stensor
    getSymmetricOrientationTensor(const SlipSystemsDescription::system& s)
    {
      return tfel::math::syme(tfel::material::getOrientationTensor(s));
    } // end of getOrientationTensor
    
    std::vector<SlipSystemsDescription::stensor>
    SlipSystemsDescription::getSymmetricOrientationTensors(const size_type i) const
    {
      const auto ss = this->getSlipSystems(i);
      auto r = std::vector<stensor>{ss.size()};
      for(size_type idx=0;idx!=ss.size();++idx){
	r[idx] = tfel::material::getSymmetricOrientationTensor(ss[idx]);
      }
      return r;
    } // end of SlipSystemsDescription::getSymmetricOrientationTensors()
    
    std::vector<std::vector<SlipSystemsDescription::stensor>>
    SlipSystemsDescription::getSymmetricOrientationTensors() const
    {
      const auto s = this->getNumberOfSlipSystemsFamilies();
      auto r = std::vector<std::vector<stensor>>{s};
      for(size_type i=0;i!=this->getNumberOfSlipSystemsFamilies();++i){
	r[i] = this->getSymmetricOrientationTensors(i);
      }
      return r;
    } // end of SlipSystemsDescription::getSymmetricOrientationTensors()

    std::vector<SlipSystemsDescription::tensor>
    SlipSystemsDescription::getOrientationTensors(const size_type i) const
    {
      const auto ss = this->getSlipSystems(i);
      auto r = std::vector<tensor>{ss.size()};
      for(size_type idx=0;idx!=ss.size();++idx){
	r[idx] = tfel::material::getOrientationTensor(ss[idx]);
      }
      return r;
    } // end of SlipSystemsDescription::getOrientationTensors()
    
    std::vector<std::vector<SlipSystemsDescription::tensor>>
    SlipSystemsDescription::getOrientationTensors() const
    {
      const auto s = this->getNumberOfSlipSystemsFamilies();
      auto r = std::vector<std::vector<tensor>>{s};
      for(size_type i=0;i!=this->getNumberOfSlipSystemsFamilies();++i){
	r[i] = this->getOrientationTensors(i);
      }
      return r;
    } // end of SlipSystemsDescription::getOrientationTensors()
    
    SlipSystemsDescription::size_type
    SlipSystemsDescription::getNumberOfSlipSystems(const size_type i) const
    {
      return this->getSlipSystems(i).size();
    }  // end of SlipSystemsDescription::getNumberOfSlipSystems

    SlipSystemsDescription::size_type
    SlipSystemsDescription::getNumberOfSlipSystems() const
    {
      auto s = size_type{};
      for(size_type i=0;i!=this->getNumberOfSlipSystemsFamilies();++i){
	s += this->getNumberOfSlipSystems(i);
      }
      return s;
    }  // end of SlipSystemsDescription::getNumberOfSlipSystems

    template<CrystalStructure cs>
    static SlipSystemsDescription::InteractionMatrixStructure
    buildInteractionMatrix(const std::vector<std::vector<SlipSystemsDescription::system>>& gs){
      using SlidingSystemsInteraction =
	SlipSystemsDescription::InteractionMatrixStructure::SlidingSystemsInteraction;
      auto ngs = std::vector<numodis::GSystem>{};
      for(const auto& gs1: gs){
	for(const auto& g: gs1){
	  if(!g.template is<typename CristalStructureSlipSystems<cs>::system_type>()){
	    throw(std::runtime_error("buildInteractionMatrix: unexpected system description"));
	  }
	  const auto gg = g.template get<typename CristalStructureSlipSystems<cs>::system_type>();
	  ngs.push_back(to_numodis(gg.burgers,gg.normal));
	}
      }
      const auto s = typename CristalStructureSlipSystems<cs>::numodis_handler{};
      const auto h = numodis::Hardening(s,ngs);
      auto ranks = std::map<int,std::vector<std::pair<numodis::GSystem,
						      numodis::GSystem>>>{};
      for(const auto& g1: ngs){
	for(const auto& g2: ngs){
	  ranks[h.getRankInteraction(g1,g2)].push_back({g1,g2});
	}
      }
      const auto n = static_cast<size_t>(h.getNinteractions());
      auto ims = std::vector<std::vector<SlidingSystemsInteraction>>{n};
      for(decltype(ims.size()) i=0;i!=ims.size();++i){
	if(ranks.count(i)==0){
	  continue;
	}
	for(const auto& sp1 : ranks.at(i)){
	  const auto ssi = SlidingSystemsInteraction{from_numodis(sp1.first),
						     from_numodis(sp1.second)};
	  ims[i].push_back(std::move(ssi));
	}
      }
      return {ims};
    }

    SlipSystemsDescription::InteractionMatrixStructure::InteractionMatrixStructure(const std::vector<std::vector<SlidingSystemsInteraction>>& r)
      : ranks(r)
    {} // end of SlipSystemsDescription::SlipSystemsDescription::InteractionMatrixStructure

    SlipSystemsDescription::InteractionMatrixStructure::InteractionMatrixStructure(InteractionMatrixStructure&&) = default;

    SlipSystemsDescription::size_type
    SlipSystemsDescription::InteractionMatrixStructure::rank() const
    {
      return this->ranks.size();
    } // end of SlipSystemsDescription::InteractionMatrixStructure::rank()

    SlipSystemsDescription::size_type
    SlipSystemsDescription::InteractionMatrixStructure::getRank(const system& g1,
								const system& g2) const
    {
      auto throw_if = [](const bool b, const std::string& m){
	if(b){throw(std::runtime_error("SlipSystemsDescription::InteractionMatrixStructure"
				       "::getRank: "+m));}
      };
      auto equal = [throw_if](const system& ss1,
			      const system& ss2){
	if(ss1.is<system3d>()){
	  if(!ss2.is<system3d>()){
	    return false;
	  }
	  const auto& ss1_3d = ss1.get<system3d>();
	  const auto& ss2_3d = ss2.get<system3d>();
	  return ((tfel::fsalgo::equal<3u>::exe(ss1_3d.burgers.begin(),
						ss2_3d.burgers.begin()))&&
		  (tfel::fsalgo::equal<3u>::exe(ss1_3d.normal.begin(),
						ss2_3d.normal.begin())));
	}
	throw_if(!ss1.is<system3d>(),
		 "internal error (unsupported slip system type)");
	if(!ss2.is<system4d>()){
	  return false;
	}
	const auto& ss1_4d = ss1.get<system4d>();
	const auto& ss2_4d = ss2.get<system4d>();
	return ((tfel::fsalgo::equal<4u>::exe(ss1_4d.burgers.begin(),
					      ss2_4d.burgers.begin()))&&
		(tfel::fsalgo::equal<4u>::exe(ss1_4d.normal.begin(),
					      ss2_4d.normal.begin())));
      };
      auto i = size_type{};
      for(const auto r : this->ranks){
	for(const auto& ssi : r){
	  if((equal(ssi.g1,g1))&&(equal(ssi.g2,g2))){
	    return i;
	  }
	}
	++i;
      }
      throw_if(true,"systems pair not found");
      return i;
    } // end of SlipSystemsDescription::InteractionMatrixStructure::rank()
    
    SlipSystemsDescription::InteractionMatrixStructure::~InteractionMatrixStructure() = default;
    
    SlipSystemsDescription::InteractionMatrixStructure
    SlipSystemsDescription::getInteractionMatrixStructure() const
    {
      const auto ags = this->getSlipSystems();
      if(this->cs==CrystalStructure::Cubic){
	return buildInteractionMatrix<CrystalStructure::Cubic>(ags);
      } else if(this->cs==CrystalStructure::FCC){
	return buildInteractionMatrix<CrystalStructure::FCC>(ags);
      }
      if(cs!=CrystalStructure::HCP){
	throw(std::runtime_error("SlipSystemsDescription::getInteractionMatrixStructure: "
				 "internal error (unknown cristal structure)"));
      }
      return buildInteractionMatrix<CrystalStructure::HCP>(ags);
    } // end of SlipSystemsDescription::getInteractionMatrixStructure

    const SlipSystemsDescription::InteractionMatrixStructure::SSIContainer&
    SlipSystemsDescription::InteractionMatrixStructure::getSlidingSystemsInteraction() const{
      return this->ranks;
    } // end of SlipSystemsDescription::InteractionMatrixStructure::getSlidingSystemsInteraction
    
    SlipSystemsDescription::~SlipSystemsDescription() = default;
      
  } // end of namespace material

} // end of namespace tfel
