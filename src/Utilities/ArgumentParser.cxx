/*!
 * \file   ArgumentParser.cxx
 * \brief    
 * \author THOMAS HELFER
 * \date   09 juin 2016
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include<algorithm>
#include<stdexcept>
#include<iostream>
#include<utility>
#include<cstdlib>
#include"TFEL/Utilities/ArgumentParser.hxx"

namespace tfel{

  namespace utilities{

    ArgumentParser::CallBack::CallBack(const std::string& d_,
				       const std::function<void(void)>& c_,
				       const bool b)
      : d(d_),c(c_),hasOption(b)
    {} // end of ArgumentParser::CallBack::CallBack

    ArgumentParser::CallBack::CallBack(CallBack&&) = default;

    ArgumentParser::CallBack::CallBack(const CallBack&) = default;
    
    ArgumentParser::ArgumentParser()
    {
      this->registerDefaultCallBacks();
    } // end of ArgumentParser::ArgumentParser

    ArgumentParser::ArgumentParser(const int argc,
				   const char * const * const argv)
      : ArgumentParser()
    {
      this->setArguments(argc,argv);
    }

    void ArgumentParser::treatHelp(void)
    {
      std::cout << this->getUsageDescription() << "\n\n";
      std::cout << "Available options are : \n";
      for(const auto& c : this->callBacksContainer){ 
	auto aliasFound = false;
	auto aliasName = std::string{};
	for(const auto& a : this->alias){
	  if(a.second==c.first){
	    aliasName = a.first;
	    aliasFound = true;
	    break;
	  }
	}
	auto tmp = c.first;
	if(aliasFound){
	  tmp +=  ", " + aliasName;
	}
	if(tmp.size()<=32){
	  tmp.insert(tmp.size(),32-tmp.size(),' ');
	}
	std::cout << tmp << " : " <<  c.second.d << '\n';
      }
      std::exit(EXIT_SUCCESS);
    }

    void ArgumentParser::treatVersion(void)
    {
      std::cout << this->getVersionDescription() << std::endl;
      std::exit(EXIT_SUCCESS);
    }

    void ArgumentParser::registerDefaultCallBacks(void)
    {
      this->registerCallBack("--help","-h",{"Display this message",
	    std::bind(std::mem_fn(&ArgumentParser::treatHelp),this),false});
      this->registerCallBack("--version","-v",{"Display version information",
	    std::bind(std::mem_fn(&ArgumentParser::treatVersion),this),false});
    }

    void ArgumentParser::registerCallBack(const std::string& k,
					  const CallBack& c)
    {
      if(k.empty()){
	throw(std::runtime_error("ArgumentParser::registerNewCallBack: "
				 "invalid key"));	
      }
#ifdef _WIN32
      if(k.size()>2){
	if((k[0]=='-')&&(k[1]=='-')){
	  this->callBacksContainer.insert({'/'+k.substr(2),c});
	}
      }
#endif
      this->callBacksContainer.insert({k,c});
    }

    void ArgumentParser::registerCallBack(const std::string& k,
					  const std::string& a,
					  const CallBack& c)
    {
      if(a.empty()){
	throw(std::runtime_error("ArgumentParser::registerNewCallBack: "
				 "invalid alias"));	
      }
      this->alias.insert({a,k});
#ifdef _WIN32
      if(a[0]=='-'){
	this->alias.insert({'/'+a.substr(1),k});
      }
#endif
      this->registerCallBack(k,c);
    }
    
    void
    ArgumentParser::setArguments(const int argc,
					    const char * const * const argv)
    {
      if(argc<1){
	throw(std::runtime_error("ArgumentParser::setArguments: "
				 "argc value not valid"));
      }
      this->args.clear();
      this->programName = argv[0];
      std::copy(argv+1,argv+argc,back_inserter(this->args));
    } // end of ArgumentParser::setArguments

    void ArgumentParser::replaceAliases(void)
    {
      for(auto p=args.begin();p!=args.end();){
	const auto& pn = p->as_string();
	const auto pa=alias.find(pn);
	if(pa!=alias.end()){
	  const auto pf = this->callBacksContainer.find(pa->second);
	  if(pf==this->callBacksContainer.end()){
	    throw(std::runtime_error("ArgumentParser::replaceAliases: '"
				     +pn+"' is not a known argument"));
	  }
	  if(pf->second.hasOption){
	    const auto p2 = p+1;
	    if(p2==args.end()){
	      throw(std::runtime_error("ArgumentParser::replaceAliases : '"
				       "no argument given to option '"+pn+"'"));
	    }
	    const auto& p2n = p2->as_string();
	    if(p2n[0]=='-'){
	      throw(std::runtime_error("ArgumentParser::replaceAliases : '"
				       "no argument given to option '"+pn+"'"));
	    }
	    *p = pa->second;
	    p->setOption(p2n);
	    p = this->args.erase(p2);
	  } else {
	    *p = pa->second;
	    ++p;
	  }
	} else {
	  ++p;
	}
      }
    }

    void
    ArgumentParser::stripArguments(void)
    {
      for(auto& a : this->args){
	auto& an = a.as_string();
	const auto pos = an.find("=");
	if(pos!=std::string::npos){
	  if(pos!=std::string::npos){
	    const auto option = an.substr(pos+1,std::string::npos);
	    an.erase(pos,std::string::npos);
	    const auto pf = this->callBacksContainer.find(an);
	    if(pf!=this->callBacksContainer.end()){
	      if(!(pf->second.hasOption)){
		throw(std::runtime_error("ArgumentParser::stripArguments: "
					 "argument '"+an+"' does not have any option"));
	      }
	    }
	    a.setOption(option);
	  }
	}
      }
    } // end of ArgumentParser::stripArguments

    void
    ArgumentParser::treatUnknownArgument(void)
    {
      throw(std::runtime_error("ArgumentParser::treatUnknownArg: '"+
			       this->currentArgument->as_string()+
			       "' is not a valid argument"));
    } // end of ArgumentParser::treatUnknownArgument
  
    void ArgumentParser::parseArguments(void)
    {
      auto comp = [](const Argument& a,
		     const char* const s){
	return a.as_string()==s;
      };
      this->stripArguments();
      this->replaceAliases();
      for(auto pa = args.begin();pa!=args.end();){
	if(comp(*pa,"--help")){
	  this->treatHelp();
	  pa = this->args.erase(pa);
	} else {
	  ++pa;
	}
      }
      for(auto pa = args.begin();pa!=args.end();){
	if(comp(*pa,"--version")){
	  this->treatVersion();
	  pa = this->args.erase(pa);
	} else {
	  ++pa;
	}
      }
      this->currentArgument=this->args.begin();
      while(this->currentArgument!=this->args.end()){
	const auto a  = this->currentArgument->as_string();
	const auto pf = this->callBacksContainer.find(a);
	if(pf!=this->callBacksContainer.end()){
	  pf->second.c();
	} else {
	  this->treatUnknownArgument();
	}
	++(this->currentArgument);
      }
    }

    ArgumentParser::~ArgumentParser() = default;
    
  } // end of namespace utilities

} // end of namespace tfel