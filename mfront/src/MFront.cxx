/*!
 * \file   mfront/src/MFront.cxx
 * \brief  
 * \author Helfer Thomas
 * \date   09 nov 2006
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include<stdexcept>
#include<iostream>
#include<iterator>
#include<cstdlib>
#include<cassert>
#include<fstream>
#include<sstream>
#include<cstring>
#include<string>
#include<cerrno>
#include<memory>

#include<sys/types.h>
#include<sys/stat.h>
#if defined _WIN32 || defined _WIN64
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <io.h>
#include <conio.h>
#include <windows.h>
#include <process.h>
#else
#include<dlfcn.h> 
#include<sys/wait.h>
#include<dirent.h>
#include<unistd.h>
#endif

#include"TFEL/Config/GetInstallPath.hxx"
#include"TFEL/Utilities/TerminalColors.hxx"
#include"TFEL/Utilities/StringAlgorithms.hxx"
#include"TFEL/System/System.hxx"
#include"TFEL/System/ExternalLibraryManager.hxx"

#include"MFront/MFrontHeader.hxx"
#include"MFront/MFrontLogStream.hxx"
#include"MFront/TargetsDescription.hxx"
#include"MFront/SearchFile.hxx"
#include"MFront/DSLFactory.hxx"
#include"MFront/MaterialPropertyInterfaceFactory.hxx"
#include"MFront/BehaviourInterfaceFactory.hxx"
#include"MFront/ModelInterfaceFactory.hxx"
#include"MFront/MFrontLock.hxx"
#include"MFront/MFrontDebugMode.hxx"
#include"MFront/MFrontUtilities.hxx"

#include"MFront/MFront.hxx"

#ifdef MFRONT_MAKE_SUPPORT  
#include"MFront/MakefileGenerator.hxx"
#endif /* MFRONT_MAKE_SUPPORT */

namespace mfront{

  /*!
   * \return the path to the documentation file if available.
   * If not, an empty string is returned
   * \param[in] pn : parser name
   * \param[in] k  : keyword
   */
  static std::string
  getDocumentationFilePath(const std::string& pn,
			   const std::string& k)
  {
    const auto root = tfel::getInstallPath();
    auto fn = root+"/share/doc/mfront/"+pn+"/"+k.substr(1)+".md";
    std::ifstream desc{fn};
    if(desc){
      return fn;
    }
    fn = root+"/share/doc/mfront/"+k.substr(1)+".md";
    desc.open(fn);
    if(desc){
      return fn;
    }
    return "";
  }

  std::string 
  MFront::getVersionDescription(void) const
  {
    return MFrontHeader::getHeader();
  }

  std::string 
  MFront::getUsageDescription(void) const
  {
    return "Usage: "+this->programName+" [options] [files]";
  }

  const tfel::utilities::Argument&
  MFront::getCurrentCommandLineArgument() const
  {
    return *(this->currentArgument);
  }

  void
  MFront::treatUnknownArgument(void)
  {
    if(!MFrontBase::treatUnknownArgumentBase()){
#if !(defined _WIN32 || defined _WIN64 ||defined __CYGWIN__)
      ArgumentParserBase<MFront>::treatUnknownArgument();
#else
      auto a = static_cast<const std::string&>(this->getCurrentCommandLineArgument());
      std::cerr << "mfront : unsupported option '" 
		<< a << '\'' << std::endl;
      exit(EXIT_FAILURE);
#endif /* __CYGWIN__ */
    }
  } // end of MFront::treatUnknownArgument()

#ifdef MFRONT_MAKE_SUPPORT  
  void
  MFront::treatMake(void)
  {
    this->genMake = true;
  } // end of MFront::treatMake

  void
  MFront::treatBuild(void)
  {
    this->genMake   = true;
    this->buildLibs = true;
  } // end of MFront::treatBuild

  void
  MFront::treatClean(void)
  {
    this->genMake   = true;
    this->cleanLibs = true;
  } // end of MFront::treatBuild

  void
  MFront::treatOMake(void)
  {
    this->genMake = true;
    const auto level = this->currentArgument->getOption();
    if(!level.empty()){
      if(level=="level2"){
	this->opts.oflags2   = true;
      } else if(level=="level0"){
	this->opts.oflags0   = true;
      } else if(level=="level1"){
	this->opts.oflags    = true;
      } else if(level!="level1"){
	throw(std::runtime_error("MFront::treatOMake: "
				 "unsupported value '"+level+
				 "' for the --omake option"));
      }
    } else {
      this->opts.oflags  = true;
    }
  } // end of MFront::treatOMake

  void
  MFront::treatOBuild(void)
  {
    this->genMake   = true;
    this->buildLibs = true;
    const auto level = this->currentArgument->getOption();
    if(!level.empty()){
      if(level=="level2"){
	this->opts.oflags2   = true;
      } else if(level=="level0"){
	this->opts.oflags0   = true;
      } else if(level=="level1"){
	this->opts.oflags    = true;
      } else if(level!="level1"){
	throw(std::runtime_error("MFront::treatOBuild: "
				 "unsupported value '"+level+
				 "' for the --obuild option"));
      }
    } else {
      this->opts.oflags    = true;
    }
  } // end of MFront::treatOBuild

#endif /* MFRONT_MAKE_SUPPORT */
  
  void
  MFront::treatListParsers(void)
  {
    std::cout << "available dsl : \n";
    auto& parserFactory = DSLFactory::getDSLFactory();
    const auto& parsers = parserFactory.getRegistredParsers();
    auto p = parsers.begin();
    while(p!=parsers.end()){
      auto tmp = "- " + *p;
      if(tmp.size()<=32){
	tmp.insert(tmp.size(),32-tmp.size(),' ');
      }
      std::cout << tmp << " : " << parserFactory.getParserDescription(*p) << ".\n";
      ++p;
    }
    exit(EXIT_SUCCESS);
  } // end of MFront::treatListParsers

  void
  MFront::treatSilentBuild(void)
  {
    const auto& o = this->currentArgument->getOption();
    if(o.empty()){
      throw(std::runtime_error("MFront::treatSilentBuild : "
			       "no argument given to the "
			       "--silentBuild option"));
    }
    if(o=="on"){
      this->opts.silentBuild=true;
    } else if(o=="off"){
      this->opts.silentBuild=false;
    } else {
      throw(std::runtime_error("MFront::treatSilentBuild : "
			       "unsupported argument '"+o+
			       "' given to the --silentBuild option"));
    }
  } // end of MFront::treatSilentBuild

  
#ifdef MFRONT_MAKE_SUPPORT
  
  void
  MFront::treatTarget(void)
  {
    using tfel::utilities::tokenize;
    const auto& t = tokenize(this->currentArgument->getOption(),',');
    if(t.empty()){
      throw(std::runtime_error("MFront::treatTarget: "
			       "no argument given to the "
			       "--target option"));
    }
    this->specifiedTargets.insert(t.begin(),t.end());
    this->genMake   = true;
    this->buildLibs = true;
  } // end of MFront::treatTarget

  void
  MFront::treatOTarget(void)
  {
    if(this->currentArgument==this->args.end()){
      throw(std::runtime_error("MFront::treatTarget: "
			       "no argument given to the "
			       "--otarget option"));
    }
    this->opts.oflags    = true;
    this->treatTarget();
  } // end of MFront::treatTarget

#endif /* MFRONT_MAKE_SUPPORT */
  
#if !(defined _WIN32 || defined _WIN64 ||defined __CYGWIN__)
  void
  MFront::treatWin32(void)
  {
    this->opts.sys = "win32";
  } // end of MFront::treatWin32
#endif /* __CYGWIN__ */

  void 
  MFront::registerArgumentCallBacks(void)
  {
    this->registerNewCallBack("--verbose",&MFront::treatVerbose,
			      "set verbose output",true);
    this->registerNewCallBack("--list-parsers",&MFront::treatListParsers,"list all available domain specific languages (deprecated)");
    this->registerNewCallBack("--list-dsl",&MFront::treatListParsers,"list all available domain specific languages");
    this->registerNewCallBack("--help-commands",&MFront::treatHelpCommands,
			      "display the help associated with all the keywords for the given domain specific language and exits",true);
    this->registerNewCallBack("--help-keywords",&MFront::treatHelpCommands,
			      "display the help associated with all the keywords for the given domain specific language and exits",true);
    this->registerNewCallBack("--help-commands-list",&MFront::treatHelpCommandsList,
			      "list all keywords for the given domain specific language and exits",true);
    this->registerNewCallBack("--help-keywords-list",&MFront::treatHelpCommandsList,
			      "list all keywords for the given domain specific language and exits",true);
    this->registerNewCallBack("--include","-I",&MFront::treatSearchPath,
			      "add a new path at the beginning of the search paths",true);
    this->registerNewCallBack("--search-path",&MFront::treatSearchPath,
			      "add a new path at the beginning of the search paths",true);
    this->registerNewCallBack("--help-keyword",&MFront::treatHelpCommand,
			      "display the help associated for the given domain specific language and exits",true);
    this->registerNewCallBack("--help-command",&MFront::treatHelpCommand,
			      "display the help associated for the given domain specific language and exits",true);
    this->registerNewCallBack("--debug",&MFront::treatDebug,
			      "set debug mode (remove references to initial file)");
    this->registerNewCallBack("--warning","-W",&MFront::treatWarning,"print warnings");
    this->registerNewCallBack("--pedantic",&MFront::treatPedantic,
			      "print pedantic warning message");
    this->registerNewCallBack("--interface","-i",&MFront::treatInterface,
			      "specify which interface to use",true);
#if (defined _WIN32 || defined _WIN64 ||defined __CYGWIN__)
    this->registerNewCallBack("--def-file",&MFront::treatDefFile,
			      "outputs def file associated with the libraries given in arguments (separated by commas)",true);
#endif    
    this->registerNewCallBack("--silent-build",&MFront::treatSilentBuild,
			      "active or desactivate silent build",true);
#ifdef MFRONT_MAKE_SUPPORT
    this->registerNewCallBack("--make",&MFront::treatMake,
			      "generate MakeFile (see also --build)");
    this->registerNewCallBack("--build",&MFront::treatBuild,
			      "generate MakeFile and build libraries");
    this->registerNewCallBack("--omake","-m",&MFront::treatOMake,
			      "generate MakeFile with optimized compilations flags (see also --obuild)",true);
    this->registerNewCallBack("--obuild","-b",&MFront::treatOBuild,
			      "generate MakeFile with optimized compilations flags and build libraries",true);
    this->registerNewCallBack("--target","-t",&MFront::treatTarget,
			      "generate MakeFile and build the specified target",true);
    this->registerNewCallBack("--otarget",&MFront::treatOTarget,
			      "generate MakeFile with optimized compilations flags and build the specified target",true);
    this->registerNewCallBack("--clean",&MFront::treatClean,
			      "generate MakeFile and clean libraries");
#if (defined _WIN32 || defined _WIN64 ||defined __CYGWIN__)
    this->registerNewCallBack("--nodeps",&MFront::treatNoDeps,
			      "don't generate compilation dependencies");
#endif /* __CYGWIN__ */
#endif /* MFRONT_MAKE_SUPPORT */    
    this->registerNewCallBack("--nomelt",&MFront::treatNoMelt,
			      "don't melt librairies sources");
#if !(defined _WIN32 || defined _WIN64 ||defined __CYGWIN__)
    this->registerNewCallBack("--win32",&MFront::treatWin32,
			      "specify that the target system is win32");
#endif /* __CYGWIN__ */
  } // end of MFront::registerArgumentCallBacks

  MFront::MFront()
    : tfel::utilities::ArgumentParserBase<MFront>()
  {
#if defined _WIN32 || defined _WIN64 ||defined __CYGWIN__
    this->opts.sys = "win32";
#elif defined __APPLE__
    this->opts.sys = "apple";
#else
    this->opts.sys = "default";
#endif /* __CYGWIN__ */
  } // end of MFront::MFront

  MFront::MFront(const int argc, const char *const *const argv)
    : MFront()
  {
    this->setArguments(argc,argv);
    this->registerArgumentCallBacks();
    this->parseArguments();
  } // end of MFront::MFront

  void
  MFront::treatHelpCommandsList(void)
  {
    using namespace std;
    using namespace tfel::utilities;
    auto& f = DSLFactory::getDSLFactory();
    const auto& o = this->currentArgument->getOption();
    if(o.empty()){
      throw(runtime_error("MFront::treatHelpCommandsList: "
			  "no parser name given"));
    }
    shared_ptr<AbstractDSL> p{f.createNewParser(o)};
    vector<string> k;
    p->getKeywordsList(k);
    string::size_type msize = 0;
    for(auto pk=k.begin();pk!=k.end();++pk){
      msize = max(msize,pk->size());
    }
    for(auto pk=k.begin();pk!=k.end();++pk){
      const auto fp = getDocumentationFilePath(o,*pk);
      auto key = *pk;
      key.resize(msize,' ');
      cout << key << "  ";
      if(!fp.empty()){
	cout.write(TerminalColors::Green,sizeof(TerminalColors::Green));
	cout << "(documented)";
      } else {
	cout.write(TerminalColors::Red,sizeof(TerminalColors::Red));
	cout << "(undocumented)";
      }
      cout.write(TerminalColors::Reset,sizeof(TerminalColors::Reset));
      cout << endl;
    }
    exit(EXIT_SUCCESS);
  } // end of MFront::treatHelpCommandsList

  void
  MFront::treatHelpCommands(void)
  {
    using namespace std;
    auto& f = DSLFactory::getDSLFactory();
    const auto& o = this->currentArgument->getOption();
    if(o.empty()){
      throw(runtime_error("MFront::treatHelpCommandsList: "
			  "no parser name given"));
    }
    auto keys = vector<string>{};
    f.createNewParser(o)->getKeywordsList(keys);
    cout << "% `" << o << "` keywords\n\n";
    for(const auto& k : keys){
      const auto fp = getDocumentationFilePath(o,k);
      cout << "\n# The `" <<  k << "` keyword\n\n";
      if(!fp.empty()){
	std::ifstream desc{fp};
	if(!desc){
	  // note, this shall never append...
	  cout << "Internal error : can't access to the description of keyword '"
	       << k << '\n';
	} else {
	  cout << desc.rdbuf();
	}
      } else {
	cout << "The keyword `" <<  k << "` is not documented yet\n";
      }
    }
    exit(EXIT_SUCCESS);
  } // end of MFront::treatHelpCommands
  
  void
  MFront::treatHelpCommand(void)
  {
    using namespace std;
    auto& f = DSLFactory::getDSLFactory();
    const auto& o = this->currentArgument->getOption();
    if(o.empty()){
      throw(runtime_error("MFront::treatHelpCommand: "
			  "no argument given"));
    }
    const auto pos = o.rfind(':');
    if((pos==string::npos)||(pos+1==o.size())){
      throw(runtime_error("MFront::treatHelpCommand: "
			  "ill-formed argument, expected "
			  "'parser:@keyword'"));
    }
    const auto pn = o.substr(0,pos); // parser name
    const auto k  = o.substr(pos+1); // key
    if((pn.empty())||(k.empty())){
      throw(runtime_error("MFront::treatHelpCommand: "
			  "ill-formed argument, expected "
			  "'parser:@keyword'"));
    }
    if(k[0]!='@'){
      throw(runtime_error("MFront::treatHelpCommand: "
			  "ill-formed argument, expected "
			  "'parser:@keyword'"));
    }
    auto p = f.createNewParser(pn);
    vector<string> keys;
    p->getKeywordsList(keys);
    if(find(keys.begin(),keys.end(),k)==keys.end()){
      string msg("MFront::treatHelpCommand : ");
      msg += "keyword '"+k+"' is not declared ";
      throw(runtime_error(msg));
    }
    const auto fp = getDocumentationFilePath(pn,k);
    if(fp.empty()){
      cout << "no description available for keyword '"
	   << k << "'" << endl;
    } else {
      ifstream desc(fp);
      if(!desc){
	// note, this shall never append...
	cout << "can't access to the description of keyword '"
	     << k << "'" << endl;
      } else {
	cout << desc.rdbuf();
      }
    }
    exit(EXIT_SUCCESS);
  } // end of MFront::treatHelpCommand

#ifdef MFRONT_MAKE_SUPPORT
  void
  MFront::treatNoDeps(void)
  {
    this->opts.nodeps = true;
  } // end of MFront::treatNoDeps
#endif /* MFRONT_MAKE_SUPPORT */

  void MFront::treatNoMelt(void)
  {
    this->opts.melt = false;
  } // end of MFront::treatNoMelt

#if (defined _WIN32 || defined _WIN64 ||defined __CYGWIN__)
  void MFront::treatDefFile(void){
    const auto& o = this->getCurrentCommandLineArgument().getOption();
    if(o.empty()){
      throw(std::runtime_error("MFrontBase::treatDefFile : "
			       "no option given to the "
			       "'--def-file' argument"));
    }
    for(const auto& l : tfel::utilities::tokenize(o,',')){
      if(l.empty()){
	throw(std::runtime_error("MFrontBase::treatDefFile : "
				 "empty library specified."));
      }
      this->defs.insert(l);
    }    
  } // end of void MFront::treatDefFile
#endif /* (defined _WIN32 || defined _WIN64 ||defined __CYGWIN__) */
  
  TargetsDescription
  MFront::treatFile(const std::string& f) const
  {
    if(getVerboseMode()>=VERBOSE_LEVEL2){
      auto& log = getLogStream();
      log << "Treating file : '" << f << "'" <<  std::endl;
    }
    auto dsl = MFrontBase::getDSL(f);
    if(!this->interfaces.empty()){
      dsl->setInterfaces(this->interfaces);
    }
    dsl->analyseFile(f,this->ecmds);
    dsl->generateOutputFiles();
    return dsl->getTargetsDescription();
  } // end of MFront::treatFile(void)

  void
  MFront::analyseTargetsFile(void){
    using tfel::utilities::CxxTokenizer;
    using tfel::system::dirStringSeparator;
    MFrontLockGuard lock;
    const auto file = "src"+dirStringSeparator()+"targets.lst";
    const std::ifstream test{file};
    if(!test){
      return;
    }
    CxxTokenizer tokenizer{file};
    auto c = tokenizer.begin();
    const auto t = read<TargetsDescription>(c,tokenizer.end());
    if(getVerboseMode()>=VERBOSE_LEVEL2){
      auto& log = getLogStream();
      log << t << std::endl;
    }
    mergeTargetsDescription(this->targets,t,false);
  }
  
#ifdef MFRONT_MAKE_SUPPORT
  static const char*
  getMakeCommand(void){
    const char * emake = ::getenv("MAKE");
    if(emake!=nullptr){
      return emake;
    }
#if defined _WIN32 || defined _WIN64
    return "make.exe";
#else
    return "make";
#endif
  }

  static void callMake(const std::string& target){
    const char * make = getMakeCommand();
    const char * silent = getDebugMode() ? nullptr : "-s";
    const char *const argv[] = {make,"-C","src","-f",
				"Makefile.mfront",
				target.c_str(),
				silent,nullptr};
    auto error = [&argv,&target](const std::string& e){
      auto msg = "callMake: can't build target '"+target+"'\n";
      if(!e.empty()){
	msg += e+'\n';
      }
      msg += "Command was: ";
      for(const char * const * a = argv;*a!=nullptr;++a){
	msg += *a;msg += ' ';
      }
      throw(std::runtime_error(msg));
    };
    if(::strlen(make)==0u){
      throw(std::runtime_error("callMake: empty make command"));
    }
#if (defined _WIN32 || defined _WIN64) && (!defined __CYGWIN__)
    if(_spawnvp(_P_WAIT,make,argv)!=0){
      error("");
    }
#else
    const auto child_pid = fork();
    if(child_pid!=0){
      int status;
      if(wait(&status)==-1){
	error("something went wrong while "
	      "waiting end of make process");
      }
      if(status!=0){
	error("libraries building went wrong");
      }
    } else {
      execvp(make,const_cast<char* const*>(argv));
      ::exit(EXIT_FAILURE);
    }
#endif
  }
  
  void
  MFront::buildLibraries(const std::string& target)
  {
    callMake(target.c_str());
  } // end of MFront::buildLibraries

  void
  MFront::cleanLibraries(void)
  {
    callMake("clean");
  } // end of MFront::cleanLibraries
#endif /* MFRONT_MAKE_SUPPORT */

#if (defined _WIN32 || defined _WIN64 ||defined __CYGWIN__)
  void MFront::generateDefsFiles(void){
    MFrontLockGuard lock;
    for(const auto& d:this->defs){
      if(!describes(this->targets,d)){
	throw(std::runtime_error("MFront::generateDefsFile : "
				 "libray '"+d+"' is not handled"));
      }
      const auto f = "src"+tfel::system::dirStringSeparator()+d+".def";
      std::ofstream def{f};
      def.exceptions(std::ios::badbit|std::ios::failbit);
      if(!def){
	throw(std::runtime_error("MFront::generateDefsFile : "
				 "can't open file '"+f+"'"));
      }
      def << "LIBRARY " << d << "\n"
	  << "EXPORTS\n";
      std::copy(this->targets[d].epts.begin(),this->targets[d].epts.end(),
		std::ostream_iterator<std::string>(def,"\n"));
    }
  } // end of MFront::generateDefsFile
#endif /* (defined _WIN32 || defined _WIN64 ||defined __CYGWIN__) */

  void
  MFront::writeTargetsDescription(void) const
  {
    using namespace tfel::system;
    MFrontLockGuard lock;
    std::ofstream file{"src"+dirStringSeparator()+"targets.lst"};
    file.exceptions(std::ios::badbit|std::ios::failbit);
    file << this->targets;
  } // end of MFront::writeTargetDescription
  
  void
  MFront::exe(void)
  {
    using namespace std;
    using namespace tfel::system;
    systemCall::mkdir("src");
    systemCall::mkdir("include");
    if(this->specifiedTargets.empty()){
      this->specifiedTargets.insert("all");
    }
    bool w = !this->inputs.empty(); //< something to be done 
#ifdef MFRONT_MAKE_SUPPORT
    w = w || this->genMake;
#endif /* MFRONT_MAKE_SUPPORT */
#if (defined _WIN32 || defined _WIN64 ||defined __CYGWIN__)
    w = w || (!this->defs.empty());
#endif /* (defined _WIN32 || defined _WIN64 ||defined __CYGWIN__) */
    if(!w){
      throw(runtime_error("MFront::exe : no file name specified and nothing to be done\n"+
			  this->getUsageDescription()));
    }
    // get file generated by previous sessions
    this->analyseTargetsFile();
    auto errors = vector<pair<string,string>>{};
    if(!this->inputs.empty()){
      for(const auto& i : this->inputs){
	try{
	  const auto td = this->treatFile(i);
	  mergeTargetsDescription(this->targets,td,true);
	} catch(exception& e){
	  errors.push_back({i,e.what()});
	}
      }
      for(auto& t : this->targets.specific_targets){
	auto tmp = vector<string>{};
	for(auto p2=t.second.first.cbegin();p2!=t.second.first.cend();++p2){
	  const auto p4 = p2+1;
	  if(find(p4,t.second.first.cend(),*p2)==t.second.first.cend()){
	    tmp.push_back(*p2);
	  }
	}
	t.second.first.swap(tmp);
	tmp.clear();
	for(auto p2=t.second.second.cbegin();
	    p2!=t.second.second.cend();++p2){
	  const auto p4 = p2+1;
	  if(find(p4,t.second.second.cend(),*p2)==t.second.second.cend()){
	    tmp.push_back(*p2);
	  }
	}
	t.second.second.swap(tmp);
      }
      // get file generated by previous sessions
      this->analyseTargetsFile();
      // save all
      this->writeTargetsDescription();
    }
    if(!errors.empty()){
      auto msg = string{};
      auto p6=errors.begin();
      while(p6!=errors.end()){
	msg += "Error while treating file '"+p6->first+"'\n";
	msg += p6->second;
	if(++p6!=errors.end()){
	  msg += "\n\n";
	}
      }
      throw(runtime_error(msg));
    }
#if (defined _WIN32 || defined _WIN64 ||defined __CYGWIN__)
    this->generateDefsFiles();
#endif /* (defined _WIN32 || defined _WIN64 ||defined __CYGWIN__) */
#ifdef MFRONT_MAKE_SUPPORT
    const auto has_libs = this->targets.begin()!=this->targets.end();
    if((this->genMake)&&((has_libs)||(!this->targets.specific_targets.empty()))){
      generateMakeFile(this->targets,this->opts);
    }
    if(this->cleanLibs){
      this->cleanLibraries();
    }
    if((this->buildLibs)&&((has_libs)||(!this->targets.specific_targets.empty()))){
      auto& log = getLogStream();
      for(const auto t : this->specifiedTargets){
	if(getVerboseMode()>=VERBOSE_LEVEL0){
	  log << "Treating target : " << t << endl;
	}
	this->buildLibraries(t);
      }
      if(getVerboseMode()>=VERBOSE_LEVEL0){
	if(has_libs){
	  if(this->targets.end()-this->targets.begin()==1){
	    log << "The following library has been built :\n";
	  } else {
	    log << "The following libraries have been built :\n";
	  }
	  for(const auto& l:this->targets){
	    log << "- " << l.prefix << l.name << "." << l.suffix << " : ";
	    for(const auto& pts:l.epts){
	      log << " " << pts;
	    }
	    log << endl;
	  }
	}
	if(!this->targets.specific_targets.empty()){
	  auto pt2=this->targets.specific_targets.find("all");
	  if(pt2!=this->targets.specific_targets.end()){	      
	    if(pt2->second.first.size()==1){
	      log << "The following main target has been build :\n";
	    } else {
	      log << "The following main targets have been build :\n";
	    }
	    for(const auto& t : pt2->second.first){
	      log << "- " << t << endl;
	    }
	  }
	}
      }
    }
#endif /* MFRONT_MAKE_SUPPORT */
  } // end of MFront::exe

  MFront::~MFront()
  {} // end of MFront::~MFront

} // end of namespace mfront
