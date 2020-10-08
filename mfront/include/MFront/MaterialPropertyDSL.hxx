/*!
 * \file   mfront/include/MFront/MaterialPropertyDSL.hxx
 * \brief
 *
 * \author Thomas Helfer
 * \date   06 jui 2007
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONTMATERIALLAWPARSER_HXX
#define LIB_MFRONTMATERIALLAWPARSER_HXX

#include <string>
#include <set>

#include "MFront/DSLBase.hxx"
#include "MFront/LawFunction.hxx"
#include "MFront/VariableBoundsDescription.hxx"
#include "MFront/MaterialPropertyDescription.hxx"

namespace mfront {

  // forward declaration
  struct AbstractMaterialPropertyInterface;

  /*!
   * \return if the given name is valid
   * \param[in] n: behaviour name
   */
  MFRONT_VISIBILITY_EXPORT bool isValidMaterialPropertyName(const std::string&);

  /*!
   * \brief domain specific language handling material properties
   */
  struct MFRONT_VISIBILITY_EXPORT MaterialPropertyDSL : public DSLBase {
    //! \return the name of the parser
    static std::string getName();
    //! \return a description of the parser
    static std::string getDescription();
    //! default constructor
    MaterialPropertyDSL();

    DSLTarget getTargetType() const override final;
    std::string getMaterialKnowledgeIdentifier() const override;
    std::string getMaterialName() const override;
    std::string getOverridableVariableNameByExternalName(
        const std::string&) const override;
    void overrideByAParameter(const std::string&, const double) override;
    void getKeywordsList(std::vector<std::string>&) const override;
    void setInterfaces(const std::set<std::string>&) override;
    void analyseFile(const std::string&,
                     const std::vector<std::string>& = {},
                     const std::map<std::string, std::string>& = {}) override;
    void analyseString(const std::string&) override;
    void importFile(const std::string&,
                    const std::vector<std::string>&,
                    const std::map<std::string, std::string>&) override;
    void endsInputFileProcessing() override;

    virtual const MaterialPropertyDescription& getMaterialPropertyDescription()
        const;
    //! destructor
    ~MaterialPropertyDSL() override;

   protected:
    typedef void (MaterialPropertyDSL::*MemberFuncPtr)();
    typedef std::map<std::string, MemberFuncPtr> CallBackContainer;
    void treatUnknownKeyword() override;
    void reserveName(const std::string&) override;
    bool isNameReserved(const std::string&) const override;
    std::string getClassName() const override;
    void addMaterialLaw(const std::string&) override;
    void appendToIncludes(const std::string&) override;
    void appendToMembers(const std::string&) override;
    void appendToPrivateCode(const std::string&) override;
    void appendToSources(const std::string&) override;
    void generateOutputFiles() override;
    void addStaticVariableDescription(
        const StaticVariableDescription&) override;
    int getIntegerConstant(const std::string&) const override;
    //! \brief method called by the analyseFile and analyseString method
    virtual void analyse();
    void setMaterial(const std::string&) override;
    void setMaterialKnowledgeIdentifier(const std::string&) override;
    /*!
     * \brief add a new interface
     * \param[in] n: interface name
     */
    virtual void addInterface(const std::string&);
    //! treat the `@Library` keyword
    virtual void treatLibrary();
    //! treat the `@Output` keyword
    virtual void treatOutput();
    //! treat the `@Input` keyword
    virtual void treatInput();
    //! treat the `@Law` keyword
    virtual void treatLaw();
    //! treat the `@Function` keyword
    virtual void treatFunction();
    /*!
     * treat the setGlossaryName and the setEntryName method for
     * inputs or output.
     */
    virtual void treatMethod();
    //! treat the `@Interface` keyword
    virtual void treatInterface();
    //! treat the `@Parameter` keyword
    virtual void treatParameter();
    //! treat the `@Constant` keyword
    virtual void treatConstant();
    //! treat the `@Bounds` keyword
    virtual void treatBounds();
    //! treat the `@PhysicalBounds` keyword
    virtual void treatPhysicalBounds();

    virtual void registerNewCallBack(const std::string&, const MemberFuncPtr);
    //! description of the material property
    MaterialPropertyDescription md;
    //! \brief overriding parameters
    std::map<std::string, double> overriding_parameters;
    /*!
     * list of registred interfaces
     */
    std::map<std::string, std::shared_ptr<AbstractMaterialPropertyInterface>>
        interfaces;
    //! \brief list of resgistred keywords
    std::set<std::string> registredKeyWords;
    //! \brief map of callbacks used during the file interpretation
    CallBackContainer callBacks;
    /*!
     * a temporary object used to store the name of the variable on
     * which a method is called
     */
    std::string currentVar;
  };  // end of class MaterialPropertyDSL

}  // end of namespace mfront

#endif /* LIB_MFRONTMATERIALLAWPARSER_HXX */
