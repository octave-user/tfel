/*! 
 * \file  mfront/include/MFront/BehaviourDescription.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 07 mars 2014
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef LIB_MFRONT_BEHAVIOURDESCRIPTION_H_
#define LIB_MFRONT_BEHAVIOURDESCRIPTION_H_ 

#include<set>
#include<map>
#include<memory>

#include"TFEL/Material/MechanicalBehaviour.hxx"
#include"TFEL/Material/ModellingHypothesis.hxx"
#include"TFEL/Material/OrthotropicAxesConvention.hxx"

#include"MFront/MFrontConfig.hxx"
#include"MFront/CodeBlock.hxx"
#include"MFront/SupportedTypes.hxx"
#include"MFront/BehaviourAttribute.hxx"
#include"MFront/BehaviourData.hxx"
#include"MFront/BehaviourSymmetryType.hxx"

namespace mfront
{

  /*!
   * This structure describes a mechanical behaviour
   *
   * This class handles the fact that the mechanical behaviour
   * data may depend on the modelling hypothesis considered,
   * altough most of the time they won't.
   *
   * This class thus stores a default mechanical behaviour data
   * which is intendend to be valid for most hypotheses, and specific
   * mechanical behaviour datas when required.
   */
  struct MFRONT_VISIBILITY_EXPORT BehaviourDescription
    : public tfel::material::MechanicalBehaviourBase,
      public SupportedTypes  
  {
    /*!
     * \brief Available integration schemes.
     * One of the first thing a parser shall do is to set the
     * integration scheme it uses.
     */
    enum IntegrationScheme{
      IMPLICITSCHEME, //!< value set by the `Implicit` Parser family
      EXPLICITSCHEME, //!< value set by the `Runge-Kutta` Parser
      SPECIFICSCHEME, /*!< value set by the `	IsotropicMisesCreepDSL`,
		       * `IsotropicMisesPlasticFlowDSL`,
		       * `IsotropicStrainHardeningMisesCreepDSL`
		       * `MultipleIsotropicMisesFlowsDSL` parsers. */
      UNDEFINEDINTEGRATIONSCHEME //! default value.
    };
    //! a simple alias
    typedef tfel::material::ModellingHypothesis ModellingHypothesis;
    //! a simple alias
    typedef tfel::material::ModellingHypothesis::Hypothesis Hypothesis;
    //! a simple alias
    typedef BehaviourData::Mode Mode;
    //! a simple alias
    typedef BehaviourData::Position Position;
    //! attribute name
    static const std::string requiresStiffnessTensor;
    //! attribute name
    static const std::string requiresUnAlteredStiffnessTensor;
    //! attribute name
    static const std::string requiresThermalExpansionCoefficientTensor;
    //! attribute name
    static const std::string setRequireThermalExpansionCoefficientTensor;
    /*!
     * constructor
     */
    BehaviourDescription();
    /*!
     * \brief set the behaviour name
     * \param[in] b : behaviour name
     */
    void setBehaviourName(const std::string&);
    /*!
     * \return the behaviour name
     */
    const std::string&
    getBehaviourName(void) const;
    /*!
     * \brief set the parser name
     * \param[in] b : parser name
     */
    void setParserName(const std::string&);
    /*!
     * \return the parser name
     */
    const std::string&
    getParserName(void) const;
    /*!
     * \brief set the material name
     * \param[in] m : material name
     */
    void setMaterialName(const std::string&);
    /*!
     * \return the material name
     */
    const std::string&
    getMaterialName(void) const;
    /*!
     * \brief set the library name
     * \param[in] l : library name
     */
    void setLibrary(const std::string&);
    /*!
     * \return the library name
     */
    const std::string&
    getLibrary(void) const;
    /*!
     * \brief set the class name
     * \param[in] n : class name
     */
    void setClassName(const std::string&);
    /*!
     * \return the class name
     */
    const std::string&
    getClassName(void) const;
    /*!
     * \brief append the given code to the sources
     * \param[in] s : sources
     */
    void appendToSources(const std::string&);
    /*!
     * \return the material
     */
    const std::string&
    getSources(void) const;
    /*!
     * \brief append the given code to the members
     * \param[in] s : members
     */
    void appendToMembers(const std::string&);
    /*!
     * \return the material
     */
    const std::string&
    getMembers(void) const;
    /*!
     * \brief append the given code to the members
     * \param[in] s : members
     */
    void appendToPrivateCode(const std::string&);
    /*!
     * \return the material
     */
    const std::string&
    getPrivateCode(void) const;
    /*!
     * \brief append the given code to the includes
     * \param[in] s : includes
     */
    void appendToIncludes(const std::string&);
    /*!
     * \return the material
     */
    const std::string&
    getIncludes(void) const;
    /*!
     * add a material law
     */
    void addMaterialLaw(const std::string&);
    /*!
     * \return the list of material laws
     */
    const std::vector<std::string>&
    getMaterialLaws(void) const;
    /*!
     * \return the size of the main variables
     */
    std::pair<SupportedTypes::TypeSize,
	      SupportedTypes::TypeSize>
    getMainVariablesSize(void) const;
    /*!
     * \brief add a new main variable
     * \note using this method means that the behaviour type is always
     * 'GENERALBEHAVIOUR'. This can't be changed afterwards.
     */
    void
    addMainVariable(const DrivingVariable&,
		    const ThermodynamicForce&);
    /*!
     * \return the main variables of the behaviour
     */
    const std::map<DrivingVariable,
		   ThermodynamicForce>&
    getMainVariables(void) const;
    /*!
     * \brief set the behaviour to be a small strain standard
     * behaviour
     */
    void declareAsASmallStrainStandardBehaviour(void);
    /*!
     * \brief set the behaviour to be a finite strain standard
     * behaviour
     */
    void declareAsAFiniteStrainStandardBehaviour(void);
    /*!
     * \brief set the behaviour to be a cohesive zone model
     */
    void declareAsACohesiveZoneModel(void);
    /*!
     * \return the type of the stiffness operator
     */
    std::string
    getTangentOperatorType(void) const;
    /*!
     * \return a type suitable for storing stress-free expansion
     */
    std::string
    getStressFreeExpansionType(void) const;

    bool useQt(void) const;

    void setUseQt(const bool);

    bool
    isDrivingVariableName(const std::string&) const;

    bool
    isDrivingVariableIncrementName(const std::string&) const;
    /*!
     * \return the behaviour type
     */
    BehaviourType
    getBehaviourType(void) const;
    /*!
     * \brief set the integration scheme
     * \param[in] s: integration scheme
     */
    void
    setIntegrationScheme(const IntegrationScheme);
    /*!
     * \return the integration scheme used
     */
    IntegrationScheme
    getIntegrationScheme(void) const;
    /*!
     * \return a string describing the behaviour type
     */
    std::string
    getBehaviourTypeFlag(void) const;
    /*!
     * \return the symmetry type of the behaviour
     */
    BehaviourSymmetryType
    getSymmetryType() const;

    void
    setSymmetryType(const BehaviourSymmetryType);
    /*!
     * \brief set the orthotropic axes convention.
     * \param[in] c : new value for the orthotropic axes convention.
     * \note this method can be only call once.
     */
    void
    setOrthotropicAxesConvention(const tfel::material::OrthotropicAxesConvention);
    /*!
     * \return the orthotropic axes convention.
     * \note if the orthotropic axes convention has not been defined,
     * the UNDEFINED convention is assumed and oacIsDefined is set to
     * true.
     */
    tfel::material::OrthotropicAxesConvention
    getOrthotropicAxesConvention(void) const;
    
    bool
    isSymmetryTypeDefined() const;

    BehaviourSymmetryType
    getElasticSymmetryType() const;

    void
    setElasticSymmetryType(const BehaviourSymmetryType);
    
    bool
    isElasticSymmetryTypeDefined() const;
    /*!
     * \return a mechanical behaviour data associated with the
     * given modelling hypothesis
     * \param[in] h : modelling hypothesis
     */
    const BehaviourData&
    getBehaviourData(const ModellingHypothesis::Hypothesis&) const;
    /*!
     * \return true if modelling hypotheses are defined.
     */
    bool
    areModellingHypothesesDefined(void) const;
    /*!
     * \return all the modelling hypotheses supported by the
     * behaviour.
     */
    const std::set<Hypothesis>&
    getModellingHypotheses(void) const;
    /*!
     * \return all the modelling hypotheses which are distinct.
     *
     * This method returns, among all the supported modelling
     * hypotheses (see getModellingHypotheses), the ones which have
     * specialised mechanical data and, if at least one modelling
     * hypothesis has no specialisation, UNDEFINEDHYPOTHESIS.
     *
     * This method has been introduced for iteration purpose in
     * behaviour parsers.
     */
    std::set<Hypothesis>
    getDistinctModellingHypotheses(void) const;
    /*!
     * \return true if the given modelling hypothesis is supported
     * \param[in] h : modelling hypothesis
     * \see hasSpecialisedMechanicalData
     */
    bool
    isModellingHypothesisSupported(const Hypothesis) const;
    /*!
     * \brief set the list of supported modelling hypotheses
     * \param[in] h : supported modelling hypotheses
     * \param[in] b : parameter used if some modelling hypotheses are
     * already defined. In this case:
     * - if b is true, the resulting modelling hypotheses
     *   will be the intersection of the given modelling hypotheses and
     *   the already declared ones. If such intersection is empty, an
     *   exception is thrown.
     * - if b is false, an exception is thrown.
     */
    void
    setModellingHypotheses(const std::set<Hypothesis>&,
			   const bool b = false);
    /*!
     * \brief add material properties
     * \param[in] h : modelling hypothesis
     * \param[in] v : material properties added
     * \param[in] s : registration status
     *
     * \note if h is UNDEFINEDHYPOTHESIS, add the material properties
     * to the default data and to all the specialisations
     */
    void
    addMaterialProperties(const Hypothesis,
			  const VariableDescriptionContainer&,
			  const BehaviourData::RegistrationStatus = BehaviourData::UNREGISTRED);
    /*!
     * \brief add a material property
     * \param[in] h : modelling hypothesis
     * \param[in] v : material property added
     * \param[in] s : registration status
     *
     * \note if h is UNDEFINEDHYPOTHESIS, add the material properties
     * to the default data and to all the specialisations
     */
    void
    addMaterialProperty(const Hypothesis,
			const VariableDescription&,
			const BehaviourData::RegistrationStatus = BehaviourData::UNREGISTRED);
    /*!
     * \brief add a state variables
     * \param[in] h : modelling hypothesis
     * \param[in] v : state variable added
     * \param[in] s : registration status
     *
     * \note if h is UNDEFINEDHYPOTHESIS, add the state variables
     * to the default data and to all the specialisations
     */
    void
    addIntegrationVariable(const Hypothesis,
			   const VariableDescription&,
			   const BehaviourData::RegistrationStatus = BehaviourData::UNREGISTRED);
    /*!
     * \brief add state variables
     * \param[in] h : modelling hypothesis
     * \param[in] v : state variables added
     * \param[in] s : registration status
     *
     * \note if h is UNDEFINEDHYPOTHESIS, add the state variables
     * to the default data and to all the specialisations
     */
    void
    addIntegrationVariables(const Hypothesis,
			    const VariableDescriptionContainer&,
			    const BehaviourData::RegistrationStatus = BehaviourData::UNREGISTRED);
    /*!
     * \brief add a state variables
     * \param[in] h : modelling hypothesis
     * \param[in] v : state variable added
     * \param[in] s : registration status
     *
     * \note if h is UNDEFINEDHYPOTHESIS, add the state variables
     * to the default data and to all the specialisations
     */
    void
    addStateVariable(const Hypothesis,
		     const VariableDescription&,
		     const BehaviourData::RegistrationStatus = BehaviourData::UNREGISTRED);
    /*!
     * \brief add state variables
     * \param[in] h : modelling hypothesis
     * \param[in] v : state variables added
     * \param[in] s : registration status
     *
     * \note if h is UNDEFINEDHYPOTHESIS, add the state variables
     * to the default data and to all the specialisations
     */
    void
    addStateVariables(const Hypothesis,
		      const VariableDescriptionContainer&,
		      const BehaviourData::RegistrationStatus = BehaviourData::UNREGISTRED);
    /*!
     * \brief add auxiliary state variables
     * \param[in] h : modelling hypothesis
     * \param[in] v : auxiliary state variables added
     * \param[in] s : registration status
     *
     * \note if h is UNDEFINEDHYPOTHESIS, add the auxiliary state
     * variables to the default data and to all the specialisations
     */
    void
    addAuxiliaryStateVariables(const Hypothesis,
			       const VariableDescriptionContainer&,
			       const BehaviourData::RegistrationStatus = BehaviourData::UNREGISTRED);
    /*!
     * \brief add an auxiliary state variable
     * \param[in] h : modelling hypothesis
     * \param[in] v : auxiliary state variable added
     * \param[in] s : registration status
     *
     * \note if h is UNDEFINEDHYPOTHESIS, add the auxiliary state
     * variables to the default data and to all the specialisations
     */
    void
    addAuxiliaryStateVariable(const Hypothesis,
			      const VariableDescription&,
			      const BehaviourData::RegistrationStatus = BehaviourData::UNREGISTRED);
    /*!
     * \brief add external state variables
     * \param[in] h : modelling hypothesis
     * \param[in] v : external state variables added
     * \param[in] s : registration status
     *
     * \note if h is UNDEFINEDHYPOTHESIS, add the external state
     * variables to the default data and to all the specialisations
     */
    void
    addExternalStateVariables(const Hypothesis,
			      const VariableDescriptionContainer&,
			      const BehaviourData::RegistrationStatus = BehaviourData::UNREGISTRED);
    /*!
     * \brief add external state variable
     * \param[in] h : modelling hypothesis
     * \param[in] v : external state variable added
     * \param[in] s : registration status
     *
     * \note if h is UNDEFINEDHYPOTHESIS, add the external state
     * variables to the default data and to all the specialisations
     */
    void
    addExternalStateVariable(const Hypothesis,
			     const VariableDescription&,
			     const BehaviourData::RegistrationStatus = BehaviourData::UNREGISTRED);
    /*!
     * \brief add a local variable
     * \param[in] h : modelling hypothesis
     * \param[in] v : local variable added
     * \param[in] s : registration status
     *
     * \note if h is UNDEFINEDHYPOTHESIS, add the local variables to
     * the default data and to all the specialisations
     */
    void
    addLocalVariable(const Hypothesis,
		     const VariableDescription&,
		     const BehaviourData::RegistrationStatus = BehaviourData::UNREGISTRED);
    /*!
     * \brief add local variables
     * \param[in] h : modelling hypothesis
     * \param[in] v : local variables added
     * \param[in] s : registration status
     *
     * \note if h is UNDEFINEDHYPOTHESIS, add the local variables to
     * the default data and to all the specialisations
     */
    void
    addLocalVariables(const Hypothesis,
		      const VariableDescriptionContainer&,
		      const BehaviourData::RegistrationStatus = BehaviourData::UNREGISTRED);
    /*!
     * \brief add a static variable
     * \param[in] h : modelling hypothesis
     * \param[in] v : static variable
     * \param[in] s : registration status
     *
     * \note if h is UNDEFINEDHYPOTHESIS, add the local variable to
     * the default data and to all the specialisations
     */
    void
    addStaticVariable(const Hypothesis,
		      const StaticVariableDescription&,
		      const BehaviourData::RegistrationStatus = BehaviourData::UNREGISTRED);
    /*!
     * \brief add a material property
     * \param[in] h : modelling hypothesis
     * \param[in] v : parameter
     * \param[in] s : registration status
     * \note if h is UNDEFINEDHYPOTHESIS, add the material properties
     * to the default data and to all the specialisations
     */
    void
    addParameter(const Hypothesis,
		 const VariableDescription&,
		 const BehaviourData::RegistrationStatus = BehaviourData::UNREGISTRED);
    /*!
     * \return true if the given name is the one of a material
     * property.
     * \param[in] h : modelling hypothesis
     * \param[in] n : name
     */
    bool
    isMaterialPropertyName(const Hypothesis,
			   const std::string&) const;
    /*!
     * \return true if the given name is the one of a local variable.
     * \param[in] h : modelling hypothesis
     * \param[in] n : name
     */
    bool
    isLocalVariableName(const Hypothesis,
			const std::string&) const;
    /*!
     * \return true if the given name is the one of an persistent
     * variable.
     * \param[in] h : modelling hypothesis
     * \param[in] n : name
     */
    bool
    isPersistentVariableName(const Hypothesis,
			      const std::string&) const;
    /*!
     * \return true if the given name is the one of an integration
     * variable.
     * \param[in] h : modelling hypothesis
     * \param[in] n : name
     */
    bool
    isIntegrationVariableName(const Hypothesis,
			      const std::string&) const;
    /*!
     * \return true if the given name is the one of an integration
     * variable increment.
     * \param[in] h : modelling hypothesis
     * \param[in] n : name
     */
    bool
    isIntegrationVariableIncrementName(const Hypothesis,
				       const std::string&) const;
    /*!
     * \return true if the given name is the one of an state
     * variable.
     * \param[in] h : modelling hypothesis
     * \param[in] n : name
     */
    bool
    isStateVariableName(const Hypothesis,
			const std::string&) const;
    /*!
     * \return true if the given name is the one of an state
     * variable increment.
     * \param[in] h : modelling hypothesis
     * \param[in] n : name
     */
    bool
    isStateVariableIncrementName(const Hypothesis,
				 const std::string&) const;
    /*!
     * \return true if the given name is the one of an auxiliary
     * internal state variable.
     * \param[in] h : modelling hypothesis
     * \param[in] n : name
     */
    bool
    isAuxiliaryStateVariableName(const Hypothesis,
				 const std::string&) const;
    /*!
     * \return true if the given name is the one of an external state
     * variable.
     * \param[in] h : modelling hypothesis
     * \param[in] n : name
     */
    bool
    isExternalStateVariableName(const Hypothesis,
				const std::string&) const;
    /*!
     * \return true if the given name is the one of an external state
     * variable increment
     * \param[in] h : modelling hypothesis
     * \param[in] n : name
     */
    bool
    isExternalStateVariableIncrementName(const Hypothesis,
					 const std::string&) const;
    /*!
     * \return true if the given name is the one of a static variable
     * \param[in] h : modelling hypothesis
     * \param[in] n : name
     */
    bool
    isStaticVariableName(const Hypothesis,
			 const std::string&) const;
    /*!
     * \brief check if one has to include tvector.hxx or vector.hxx
     * \param[in] b1 : requires true if one has to include tvector.hxx
     * \param[in] b2 : requires true if one has to include vector.hxx
     */
    void
    requiresTVectorOrVectorIncludes(bool&,bool&);   
    /*!
     * \return true if all mechanical data are specialised
     * This means that the default mechanical data is useless
     */
    bool areAllMechanicalDataSpecialised(void) const;
    /*!
     * \return true if all mechanical data associated with the given
     * modelling hypothesis are specialised.
     * \param[in] h : modelling hypotheses considered
     */
    bool areAllMechanicalDataSpecialised(const std::set<Hypothesis>&) const;
    /*!
     * \return true if the mechanical data associated with the given
     * modelling hypothesis is specialised.
     * \param[in] h : modelling hypothesis considered
     */
    bool hasSpecialisedMechanicalData(const Hypothesis) const;
    /*!
     * \return true if thermal expansion coefficient were defined
     */
    bool
    areThermalExpansionCoefficientsDefined(void) const;
    /*!
     * \return the thermal expansion coefficients
     */
    const std::vector<std::shared_ptr<MaterialPropertyDescription> >&
    getThermalExpansionCoefficients(void) const;
    /*!
     * set the behaviour thermal expansion coefficient (isotropic behaviour)
     * \param[in] a : thermal expansion
     */
    void setThermalExpansionCoefficient(const std::shared_ptr<MaterialPropertyDescription>&);
    /*!
     * set the behaviour thermal expansions coefficient (orthotropic behaviour)
     * \param[in] a1 : thermal expansion in the first direction
     * \param[in] a2 : thermal expansion in the second direction
     * \param[in] a3 : thermal expansion in the third  direction
     */
    void setThermalExpansionCoefficients(const std::shared_ptr<MaterialPropertyDescription>&,
					 const std::shared_ptr<MaterialPropertyDescription>&,
					 const std::shared_ptr<MaterialPropertyDescription>&);
    /*!
     * \return the external names associated with the variables
     * contained in the given container
     * \param[in] h : modelling Hypothesis
     * \param[in] v : variables for which external names are requested
     */
    std::vector<std::string>
    getExternalNames(const Hypothesis,
		     const VarContainer&) const;
    /*!
     * get the external names associated with the variables
     * contained in the given container
     * \param[out] n : names
     * \param[in]  h : modelling Hypothesis
     * \param[in]  v : variables for which external names are requested
     */
    void
    getExternalNames(std::vector<std::string>&,
		     const Hypothesis,
		     const VarContainer&) const;
    /*!
     * get the external names associated with the variables
     * contained in the given container
     * \param[out] n : names
     * \param[in]  h : modelling Hypothesis
     * \param[in]  v : variables for which external names are requested
     */
    void
    appendExternalNames(std::vector<std::string>&,
			const Hypothesis,
			const VarContainer&) const;
    /*!
     * \return true if the given variable is used in a code block
     * \param[in] h : modelling hypothesis
     * \param[in] n : name
     */
    bool
    isVariableUsedInCodeBlocks(const Hypothesis,
			       const std::string&) const;
    /*!
     * \return true if the given name is a parameter name
     * \param[in] h : modelling hypothesis
     * \param[in] n : name
     */
    bool
    isParameterName(const Hypothesis,
		    const std::string&) const;
    /*!
     * \param[in] h : modelling hypothesis
     * \param[in] n : parameter name
     * \param[in] v : parameter default value
     */
    void
    setParameterDefaultValue(const Hypothesis,
			     const std::string&,
			     const double);
    /*!
     * \param[in] h : modelling hypothesis
     * \param[in] n : parameter name
     * \param[in] v : parameter default value
     */
    void
    setParameterDefaultValue(const Hypothesis,
			     const std::string&,
			     const int);
    /*!
     * \param[in] h : modelling hypothesis
     * \param[in] n : parameter name
     * \param[in] v : parameter default value
     */
    void
    setParameterDefaultValue(const Hypothesis,
			     const std::string&,
			     const unsigned short);
    /*!
     * \return the default value of a paramater
     * \param[in] h : modelling hypothesis
     * \param[in] v : parameter default value
     */
    double
    getFloattingPointParameterDefaultValue(const Hypothesis,
					   const std::string&) const;
    /*!
     * \return the default value of a paramater
     * \param[in] h : modelling hypothesis
     * \param[in] v : parameter default value
     */
    int
    getIntegerParameterDefaultValue(const Hypothesis,
				    const std::string&) const;
    /*!
     * \return the default value of a paramater
     * \param[in] h : modelling hypothesis
     * \param[in] v : parameter default value
     */
    unsigned short
    getUnsignedShortParameterDefaultValue(const Hypothesis,
					  const std::string&) const;
    /*!
     * associate a glossary name to a variable
     * \param[in] h : modelling Hypothesis
     * \param[in] n : variable name
     * \param[in] g : glossary name
     */
    void
    setGlossaryName(const Hypothesis,
		    const std::string&,
		    const std::string&);
    /*!
     * \return true if the given glossary name is used
     * \param[in] h : modelling Hypothesis
     * \param[in] n : name
     */
    bool
    isGlossaryNameUsed(const Hypothesis,
		       const std::string&) const;
    /*!
     * \return true if the given variable name is associated with a
     * glossary name
     * \param[in] h : modelling hypothesis
     * \param[in] n : name
     */
    bool
    hasGlossaryName(const Hypothesis,
		    const std::string&) const;
    /*!
     * \return true if the given variable name is associated with a
     * entry name
     * \param[in] h : modelling hypothesis
     * \param[in] n : name
     */
    bool
    hasEntryName(const Hypothesis,
		 const std::string&) const;
    /*!
     * associate an entry name to a variable
     * \param[in] h : modelling Hypothesis
     * \param[in] n : variable name
     * \param[in] e : entry name
     */
    void
    setEntryName(const Hypothesis,
		 const std::string&,
		 const std::string&);
    /*!
     * get the glossary name or the entry name of a variable
     * \param[in] h : modelling Hypothesis
     * \param[in] n : variable name
     */
    std::string
    getExternalName(const Hypothesis h,
		    const std::string& n) const;
    /*!
     * \return the name of the variable associated with the given
     * glossary or entry name
     * \param[in] h : modelling Hypothesis
     * \param[in] n : name
     */
    std::string
    getVariableNameFromGlossaryNameOrEntryName(const Hypothesis,
					       const std::string&) const;
    /*!
     * \return true if the given name is an entry name
     * \param[in] h : modelling Hypothesis
     * \param[in] n : name
     */
    bool
    isUsedAsEntryName(const Hypothesis,
		      const std::string&) const;
    /*!
     * \brief declares an external state variable to be probably
     * unusable in a purely implicit resolution. This means that its
     * increment is used somewhere.
     *
     * The term "probably" refers to the fact that it may happens that
     * the increment is only used to get the value of the external
     * state variable at a given date. For example, in a standard
     * resolution (non implicit), the value of \(T+\theta\,\Delta\,T\)
     * gives the temperature value at
     * \(t+\theta\,\Delta\,t\). However, this expression is still
     * valid in a purely implicit resolution.
     *
     * \param[in] h : modelling hypothesis
     * \param[in] n : variable name
     */
    void
    declareExternalStateVariableProbablyUnusableInPurelyImplicitResolution(const Hypothesis,
									   const std::string&);
    /*!
     * \brief set if this behaviour can be used in a purely implicit
     * resolution.
     * \param[in] h : modelling hypothesis
     * \param[in] b : new value of the
     * usableInPurelyImplicitResolution member
     * \see isUsableInPurelyImplicitResolution for details about
     * purely implicit resolution.
     */
    void
    setUsableInPurelyImplicitResolution(const Hypothesis,
					const bool);
    /*!
     * \brief declare or update a code block
     * \param[in] h : modelling hypothesis
     * \param[in] n : name
     * \param[in] c : code
     * \param[in] m : mode
     * \param[in] p : position
     * \param[in] b : if true, the code can be overriden or completed
     */
    void setCode(const Hypothesis,
		 const std::string&,
		 const CodeBlock&,
		 const Mode,
		 const Position,
		 const bool = true);
    /*!
     * \brief return the name of the code blocks defined so far
     * \param[in] h : modelling hypothesis
     */
    std::vector<std::string>
    getCodeBlockNames(const Hypothesis) const;
    /*!
     * \return the code block with the given name
     * \param[in] h : modelling hypothesis
     * \param[in] n : name
     */
    const CodeBlock&
    getCodeBlock(const Hypothesis,
		 const std::string&) const;
    /*!
     * \return the code with the given name
     * \param[in] h : modelling hypothesis
     * \param[in] n : name
     */
    std::string
    getCode(const Hypothesis,
	    const std::string&) const;
    /*!
     * \return the code with the given name
     * \param[in] h : modelling hypothesis
     * \param[in] n : name
     */
    bool
    hasCode(const Hypothesis,
	    const std::string&) const;
    /*!
     * \brief set the bound description for a given variable
     * \param[in] h : modelling hypothesis
     * \param[in] b : bounds description
     */
    void
    setBounds(const Hypothesis,
	      const BoundsDescription&);
    /*!
     * \brief set a mechanical attribute
     * \param[in] h : modelling hypothesis
     * \param[in] n : name
     * \param[in] a : attribute
     * \param[in] b : don't throw if the the
     *                attribute already exists.
     *                The attribute is left unchanged.
     *                However the type of the attribute is checked.
     */
    void setAttribute(const Hypothesis,
		      const std::string&,
		      const BehaviourAttribute&,
		      const bool = false);
    /*!
     * \return a mechanical attribute
     * \param[in] h : modelling hypothesis
     * \param[in] n : name
     */
    template<typename T>
    const T&
    getAttribute(const Hypothesis,
		 const std::string&) const;
    /*!
     * \return a mechanical attribute if it exists or the default
     * value
     * \param[in] h : modelling hypothesis
     * \param[in] n : name
     * \param[in] v : default value
     */
    template<typename T>
    const T
    getAttribute(const Hypothesis,
		 const std::string&,
		 const T&) const;
    /*!
     * \return true an attribute with the given name has been declared
     * \param[in] h : modelling hypothesis
     * \param[in] n : name
     */
    bool
    hasAttribute(const Hypothesis,
		 const std::string&) const;
    /*!
     * \return true a parameter with the given name has been declared
     * \param[in] h : modelling hypothesis
     * \param[in] n : name
     */
    bool
    hasParameter(const Hypothesis,
		 const std::string&) const;
    /*!
     * \return true the given modelling hypothesis has a
     * parameter.
     * \param[in] h : modelling hypothesis \param[in] n :
     * name
     */
    bool
    hasParameters(const Hypothesis) const;
    /*!
     * \return true if at least one modelling hypothesis has a
     * parameter.
     */
    bool
    hasParameters(void) const;
    /*!
     * \brief insert a new attribute
     * \param[in] n : name
     * \param[in] a : attribute
     * \param[in] b : don't throw if the the
     *                attribute already exists.
     *                The attribute is left unchanged.
     *                However the type of the attribute is checked.
     */
    void
    setAttribute(const std::string&,
		 const BehaviourAttribute&,
		 const bool);
    /*!
     * \return true if an attribute with the given name as been registred
     * \param[in] n : name
     */
    bool
    hasAttribute(const std::string&) const;
    /*!
     * \return the attribute with the given name
     * \param[in] n : name
     */
    template<typename T>
    typename std::enable_if<
      tfel::meta::TLCountNbrOfT<T,BehaviourAttributeTypes>::value==1, 
      T&>::type
    getAttribute(const std::string&);
    /*!
     * \return the attribute with the given name
     * \param[in] n : name
     */
    template<typename T>
    typename std::enable_if<
      tfel::meta::TLCountNbrOfT<T,BehaviourAttributeTypes>::value==1, 
      const T&>::type
    getAttribute(const std::string&) const;
    /*!
     * \return the attribute with the given name
     * \param[in] n : name
     */
    template<typename T>
    typename std::enable_if<
      tfel::meta::TLCountNbrOfT<T,BehaviourAttributeTypes>::value==1, 
      T>::type
    getAttribute(const std::string&,
		 const T&) const;
    /*!
     * \return all the attribute registred
     * \param[in] n : name
     */
    const std::map<std::string,BehaviourAttribute>&
    getAttributes(void) const;
    /*!
     * reserve the given name
     * \param[in] h : hypothesis
     * \param[in] n : variable name
     */
    void reserveName(const Hypothesis,
		     const std::string&);
    /*!
     * register the given member name
     * \param[in] h : hypothesis
     * \param[in] n : variable name
     */
    void registerMemberName(const Hypothesis,
			    const std::string&);
    /*!
     * register the given static member name
     * \param[in] h : hypothesis
     * \param[in] n : variable name
     */
    void registerStaticMemberName(const Hypothesis,
				  const std::string&);
    /*!
     * \brief check variable existence
     * \return a pair of iterator. The first part tells if the
     * variable was found in a least one specialisation of the
     * behaviour and the second telles if the variable is available
     * for all distinct modelling hypothesis.
     * The variable could be any sort of variable (local variable,
     * integration variable, static variable, ...)
     * \param[in] v  : variable name
     */
    std::pair<bool,bool>
    checkVariableExistence(const std::string&) const;
    /*!
     * \brief check variable existence for a particular sort of variable.
     * \return a pair of iterator. The first part tells if the
     * variable was found in a least one specialisation of the
     * behaviour and the second telles if the variable is available
     * for all distinct modelling hypothesis.
     * \param[in] v: variable name
     * \param[in] c: variable category
     * \param[in] b: if true, an exception if thrown if the variable is not found
     */
    std::pair<bool,bool>
    checkVariableExistence(const std::string&,
			   const std::string&,
			   const bool = true) const;
    /*!
     * \brief check variable glossary name.
     * The variable must exists for all modelling hypothesis and have
     * the given glossary name. If not, an exception is thrown. If
     * another variable has the given glossary name, an exception if
     * thrown.
     * \param[in] v: variable name
     * \param[in] g: glossary name
     */
    void
    checkVariableGlossaryName(const std::string&,
			      const std::string&) const;
    /*!
     * \brief check a variable position. Throw an exception if this
     * check is not satisfied.
     * \param[in] v: variable name
     * \param[in] c: variable category
     * \param[in] p: variable position
     * \pre The number of variables of the given type must be greater
     * than the position given, otherwise an exception is thrown
     */
    void
    checkVariablePosition(const std::string&,
			  const std::string&,
			  const size_t);
    /*!
     * \brief : add a new library dependency
     * \param[in] l : library dependency
     */
    void
    addLibraryDependency(const std::string&);
    //! external library dependencies
    const std::vector<std::string>&
    getLibrariesDependencies(void) const;
    //! destructor
    ~BehaviourDescription();
  private:
    /*!
     * \brief throw an exception saying that no attribute with the
     * given name exists
     */
    TFEL_NORETURN static void
    throwUndefinedAttribute(const std::string&);
    /*!
     * update the class name
     */
    void updateClassName();
    /*!
     * \brief create the mechanical behaviour data associated with the
     * given modelling hypothesis if necessary, and returns it.
     * \param[in] h : modelling hypothesis
     */
    BehaviourData&
    getBehaviourData2(const ModellingHypothesis::Hypothesis&);
    /*!
     * \call the behaviour data associated with the given hypothesis
     * \param[in] h : modelling hypothesis
     * \param[in] m : behaviour data method
     * \param[in] a : argument given to the behaviour data's method
     */
    template<typename Res,
	     typename Arg1>
    Res
    getData(const Hypothesis,
	    Res (BehaviourData:: *)(const Arg1&) const,
	    const Arg1&) const;
    /*!
     * \brief call the behaviour data associated with the given hypothesis
     * \param[in] h : modelling hypothesis
     * \param[in] m : behaviour data method
     * \param[in] a : argument given to the behaviour data's method
     * \note if h is Hypothesis::UNDEFINEDHYPOTHESIS, the default data
     * and all the specialisations are called.
     */
    template<typename Arg1>
    void
    callBehaviourData(const Hypothesis,
    				void (BehaviourData:: *)(const Arg1&),
    				const Arg1&);
    /*!
     * \brief call the behaviour data associated with the given hypothesis
     * \param[in] h : modelling hypothesis
     * \param[in] m : behaviour data method
     * \param[in] a : first  argument given to the behaviour data's method
     * \param[in] b : second argument given to the behaviour data's method
     * \note if h is Hypothesis::UNDEFINEDHYPOTHESIS, the default data
     * and all the specialisations are called.
     */
    template<typename Arg1,
	     typename Arg2>
    void
    callBehaviourData(const Hypothesis,
    				void (BehaviourData:: *)(const Arg1&,
							 const Arg2),
    				const Arg1&,const Arg2);
    /*!
     * \brief call the behaviour data associated with the given hypothesis
     * \param[in] h : modelling hypothesis
     * \param[in] m : behaviour data method
     * \param[in] a : first  argument given to the behaviour data's method
     * \param[in] b : second argument given to the behaviour data's method
     * \note if h is Hypothesis::UNDEFINEDHYPOTHESIS, the default data
     * and all the specialisations are called.
     */
    template<typename Arg1,
	     typename Arg2>
    void
    callBehaviourData(const Hypothesis,
		      void (BehaviourData:: *)(const Arg1&,
					       const Arg2&),
		      const Arg1&,const Arg2&);
    /*!
     * \brief call the behaviour data associated with the given hypothesis
     * \param[in] h : modelling hypothesis
     * \param[in] m : behaviour data method
     * \param[in] a : argument given to the behaviour data's method
     * \note if h is Hypothesis::UNDEFINEDHYPOTHESIS, the default data
     * and all the specialisations are called.
     */
    template<typename Arg1>
    void
    callBehaviourData(const Hypothesis,
		      void (BehaviourData:: *)(const Arg1),
		      const Arg1);
    /*!
     * \brief add variables to the behaviour data
     * \param[in] h : modelling hypothesis
     * \param[in] v : variables to be added
     * \param[in] s : registration status
     * \param[in] m : method retrieving the variable container     
     *
     * \note if h is UNDEFINEDHYPOTHESIS, add the material properties
     * to the default data and to all the specialisations
     */
    void
    addVariables(const Hypothesis,
		 const VariableDescriptionContainer&,
		 const BehaviourData::RegistrationStatus,
		 void (BehaviourData::*)(const VariableDescription&,
					 const BehaviourData::RegistrationStatus));
    /*!
     * \brief add a variable to the behaviour data
     * \param[in] h : modelling hypothesis
     * \param[in] v : variable to be added
     * \param[in] s : registration status
     * \param[in] m : method retrieving the variable container     
     *
     * \note if h is UNDEFINEDHYPOTHESIS, add the material properties
     * to the default data and to all the specialisations
     */
    void
    addVariable(const Hypothesis,
		const VariableDescription&,
		const BehaviourData::RegistrationStatus,
		void (BehaviourData::*)(const VariableDescription&,
					const BehaviourData::RegistrationStatus));
    /*!
     * \brief add variables to the behaviour data
     * \param[out] b : behaviour data
     * \param[in]  v : variables to be added
     * \param[in]  s : registration status
     * \param[in]  m : method retrieving the variable container     
     */
    void
    addVariables(BehaviourData&,
		 const VariableDescriptionContainer&,
		 const BehaviourData::RegistrationStatus,
		 void (BehaviourData::*)(const VariableDescription&,
					 const BehaviourData::RegistrationStatus));
    /*!
     * \brief add a variable to the behaviour data
     * \param[out] b : behaviour data
     * \param[in]  v : variable to be added
     * \param[in]  s : registration status
     * \param[in]  m : method retrieving the variable container     
     */
    void
    addVariable(BehaviourData&,
		const VariableDescription&,
		const BehaviourData::RegistrationStatus,
		void (BehaviourData::*)(const VariableDescription&,
					const BehaviourData::RegistrationStatus));
    /*!
     * \brief check that the given hypothesis is supported
     * \param[in] h : modelling hypothesis
     */
    void
    checkModellingHypothesis(const Hypothesis&) const;
    //! a simple alias
    typedef std::shared_ptr<BehaviourData> MBDPtr;
    //! external library dependencies
    std::vector<std::string> librariesDependencies;
    /*!
     * behaviour attributes
     */
    std::map<std::string,
	     BehaviourAttribute> attributes;
    /*!
     * behaviour name
     */
    std::string behaviour;
    /*!
     * parser name
     */
    std::string parser;
    /*!
     * library name
     */
    std::string library;
    /*!
     * material name
     */
    std::string material;
    /*!
     * name of the generated class
     */
    std::string className;
    /*!
     * included header files
     */
    std::string includes;
    /*!
     * specific sources
     */
    std::string sources;
    /*!
     * private code
     */
    std::string privateCode;
    /*!
     * class member
     */
    std::string members;
    /*!
     * list of modelling hypotheses for
     * which the behaviour is defined
     */
    mutable std::set<Hypothesis> hypotheses;
    /*!
     * list of modelling hypotheses for
     * which this class returned a mechanical data
     */
    mutable std::set<Hypothesis> requestedHypotheses;
    /*!
     * default behaviour data
     */
    BehaviourData d;
    /*!
     * specialisations
     */
    std::map<Hypothesis,MBDPtr> sd;
    /*!
     * main variables, association of a driving variable and a
     * thermodynamicforce
     */
    std::map<DrivingVariable,
	     ThermodynamicForce> mvariables;
    /*!
     * average thermal coefficient
     * For isotropic   behaviours, only one thermal expansion coefficient must be defined.
     * For orthotropic behaviours, three thermal expansions coefficient must be defined.
     */
    std::vector<std::shared_ptr<MaterialPropertyDescription> > thermalExpansionCoefficients;
    //! use units
    bool use_qt;
    //! type of behaviour
    BehaviourType type;
    //! symmetry of behaviour (isotropic or orthotropic)
    mutable BehaviourSymmetryType stype;
    //! orthotropic axes convention
    tfel::material::OrthotropicAxesConvention oac =
      tfel::material::OrthotropicAxesConvention::DEFAULT;
    //!flag telling if the orthotropic axes convention has been defined
    mutable bool oacIsDefined = false;
    //! flag telling the behaviour symmetry has been defined
    mutable bool stypeIsDefined;
    /*!
     * symmetry of elastic behaviour (isotropic or orthotropic)
     */
    mutable BehaviourSymmetryType estype;
    //! flag telling the elastic symmetry has been defined
    mutable bool estypeIsDefined;
    /*!
     * integration schemes
     */
    IntegrationScheme ischeme;
    /*!
     * list of material laws used
     */
    std::vector<std::string> materialLaws;
  }; // end of struct BehaviourDescription

} // end of namespace mfront

#include"MFront/BehaviourDescription.ixx"

#endif /* LIB_MFRONT_BEHAVIOURDESCRIPTION_H_ */
