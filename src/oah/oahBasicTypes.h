/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___oahBasicTypes___
#define ___oahBasicTypes___

#include <list>
#include <map>
#include <vector>
#include <set>

#include "smartpointer.h"

#include "mfcBasicTypes.h"

#include "mfServices.h"
#include "mfIndentedTextOutput.h"

#include "oahElements.h"


using namespace MusicXML2;

namespace MusicFormats
{

//______________________________________________________________________________
// PRE-declarations for class mutual dependencies

class   oahAtom;
typedef SMARTP<oahAtom> S_oahAtom;

class   oahSubGroup;
typedef SMARTP<oahSubGroup> S_oahSubGroup;

class   oahGroup;
typedef SMARTP<oahGroup> S_oahGroup;

class   oahHandler;
typedef SMARTP<oahHandler> S_oahHandler;

//______________________________________________________________________________
enum class oahOptionOrArgumentKind {
  kOption,
  kArgument
};

std::string oahOptionOrArgumentKindAsString (
  oahOptionOrArgumentKind optionOrArgumentKind );

std::ostream& operator << (std::ostream& os, const oahOptionOrArgumentKind& elt);

//_______________________________________________________________________________
/*
  A type to hold options and their values if any or arguments

  In the pair:
    - first is the name of the option, such as '-x'
    - second is the value of the option
*/
class oahOptionOrArgument : public smartable
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahOptionOrArgument> create (
                            oahOptionOrArgumentKind optionOrArgumentKind,
                            const std::string&      first,
                            const std::string&      second);

  public:

    // constructors/destructor
    // ------------------------------------------------------

                          oahOptionOrArgument (
                            oahOptionOrArgumentKind optionOrArgumentKind,
                            const std::string&      first,
                            const std::string&      second);

    virtual               ~oahOptionOrArgument ();

  public:

    // set and get
    // ------------------------------------------------------

    const std::string&    getFirst () const
                              { return fFirst; }

    const std::string&    getSecond () const
                              { return fSecond; }

  public:

    // public services
    // ------------------------------------------------------

  public:

    // print
    // ------------------------------------------------------

    std::string   				asString () const;

    void          				print (std::ostream& os) const;

  protected:

    // protected fields
    // ------------------------------------------------------

    oahOptionOrArgumentKind
                          fOptionOrArgumentKind;

    std::string           fFirst;
    std::string           fSecond;
};
typedef SMARTP<oahOptionOrArgument> S_oahOptionOrArgument;
EXP std::ostream& operator << (std::ostream& os, const S_oahOptionOrArgument& elt);
EXP std::ostream& operator << (std::ostream& os, const oahOptionOrArgument& elt);

//_______________________________________________________________________________
class EXP oahOption : public smartable
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahOption> create (
                            const std::string& optionName,
                            const std::string& optionValue);

  public:

    // constructors/destructor
    // ------------------------------------------------------

                          oahOption (
                            const std::string& optionName,
                            const std::string& optionValue);

    virtual               ~oahOption ();

  public:

    // set and get
    // ------------------------------------------------------

    const std::string&    getOptionName () const
                              { return fOptionName; }

    const std::string&    getOptionValue () const
                              { return fOptionValue; }

  public:

    // public services
    // ------------------------------------------------------

  public:

    // print
    // ------------------------------------------------------

    std::string           asString () const;
    std::string           asStringForCommandLine () const;

    virtual void          print (std::ostream& os) const;

  protected:

    // protected fields
    // ------------------------------------------------------

    std::string           fOptionName;
    std::string           fOptionValue;
};
typedef SMARTP<oahOption> S_oahOption;
EXP std::ostream& operator << (std::ostream& os, const S_oahOption& elt);
EXP std::ostream& operator << (std::ostream& os, const oahOption& elt);

//______________________________________________________________________________
std::string oahOptionForCommandLine (
  const std::string& optionName,
  const std::string& optionValue);

void optionsNameAndValueVectorsPlusEquals (
  std::vector <S_oahOption>&       vector1,
  const std::vector <S_oahOption>& vector2);

// //_______________________________________________________________________________
// class EXP oahOptionsNamesAndValuesVector; JMI 0.9.66
// typedef SMARTP<oahOptionsNamesAndValuesVector> S_oahOptionsNamesAndValuesVector;
//
// class EXP oahOptionsNamesAndValuesVector : public smartable
// {
//   public:
//
//     // creation
//     // ------------------------------------------------------
//
//     static SMARTP<oahOptionsNamesAndValuesVector> create ();
//
//     static S_oahOptionsNamesAndValuesVector
//                           mergeOptionsNamesAndValues (
//                             const S_oahOptionsNamesAndValuesVector&
//                               optionsNamesAndValues1,
//                             const S_oahOptionsNamesAndValuesVector&
//                               optionsNamesAndValues2);
//
//   public:
//
//     // constructors/destructor
//     // ------------------------------------------------------
//
//                           oahOptionsNamesAndValuesVector ();
//
//     virtual               ~oahOptionsNamesAndValuesVector ();
//
//   public:
//
//     // set and get
//     // ------------------------------------------------------
//
//     const std::vector <S_oahOption>&
//                           getOptionsVector () const
//                               { return fOptionsVector; }
//
//     // public services
//     // ------------------------------------------------------
//
//     void                  appendOptionNameAndValue (
//                             const std::string& optionName,
//                             const std::string& optionValue);
//
//     void                  appendOptionNameAndValue (
//                             const S_oahOption&
//                               option);
//
//   public:
//
//     // print
//     // ------------------------------------------------------
//
//     void                  displayOptionsVector (std::ostream& os) const;
//
// //     std::string           asString () const;
//     std::string           asCommandLineOptionsString () const;
//
//     virtual void          print (std::ostream& os) const;
//
//   protected:
//
//     // protected fields
//     // ------------------------------------------------------
//
//     std::vector <S_oahOption>
//                           fOptionsVector;
// };
// typedef SMARTP<oahOptionsNamesAndValuesVector> S_oahOptionsNamesAndValuesVector;
// EXP std::ostream& operator << (std::ostream& os, const S_oahOptionsNamesAndValuesVector& elt);
// EXP std::ostream& operator << (std::ostream& os, const oahOptionsNamesAndValuesVector& elt);

//_______________________________________________________________________________
class EXP oahOptionsAndArguments : public smartable
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahOptionsAndArguments> create ();
  public:

    // constructors/destructor
    // ------------------------------------------------------

                          oahOptionsAndArguments ();

    virtual               ~oahOptionsAndArguments ();

  public:

    // set and get
    // ------------------------------------------------------

    const std::vector <oahOption>&
                          getOptionsVector () const
                              { return fOptionsVector; }

    const std::vector <std::string>&
                          getArgumentsVector () const
                              { return fArgumentsVector; }

    // public services
    // ------------------------------------------------------

    void                  appendOptionNameAndValue (
                              const oahOption& option);

    void                  appendOptionNameAndValue (
                            const std::string& optionName,
                            const std::string& optionValue);

    void                  appendArgument (const std::string& argument)
                              {
                              fArgumentsVector.push_back (argument);
                            }

  public:

    // print
    // ------------------------------------------------------

    void                  displayOptionsVector (std::ostream& os) const;
    void                  displayArgumentsVector (std::ostream& os) const;

    virtual void          print (std::ostream& os) const;

  protected:

    // protected fields
    // ------------------------------------------------------

    std::vector <oahOption>
                          fOptionsVector;
    std::vector <std::string>
                          fArgumentsVector;
};
typedef SMARTP<oahOptionsAndArguments> S_oahOptionsAndArguments;
EXP std::ostream& operator << (std::ostream& os, const S_oahOptionsAndArguments& elt);
EXP std::ostream& operator << (std::ostream& os, const oahOptionsAndArguments& elt);

//_______________________________________________________________________________
// early option?
enum class oahEarlyOptionKind {
  kEarlyOptionNo, // default value
  kEarlyOptionYes,
};

std::string oahEarlyOptionKindAsString (
  oahEarlyOptionKind earlyOptionKind);

std::ostream& operator << (std::ostream& os, const oahEarlyOptionKind& elt);

EXP mfIndentedStringStream& operator << (
  mfIndentedStringStream& iss, const oahEarlyOptionKind& theChar);

//_______________________________________________________________________________
// handler used thru...?
enum class oahHandlerUsedThruKind {
  kHandlerUsedThruUnknown, // default value
  kHandlerUsedThruArgcArgv,
  kHandlerUsedThruOptionsAndArguments
};

std::string oahHandlerUsedThruKindAsString (
  oahHandlerUsedThruKind handlerUsedThruKind);

std::ostream& operator << (std::ostream& os, const oahHandlerUsedThruKind& elt);

EXP mfIndentedStringStream& operator << (
  mfIndentedStringStream& iss, const oahHandlerUsedThruKind& theChar);

//______________________________________________________________________________
class EXP oahPrefix : public oahFindableElement //  smartable
/* JMI 0.9.71 OAH_OAH
An options prefix 'trace' --> 'trace-' allows:
  -trace=abc,def,gh
to be developped into :
  -trace-abc -trace-def -trace-gh
*/
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahPrefix> create (
                            const std::string& prefixName,
                            const std::string& prefixErsatz,
                            const std::string& prefixDescription);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          oahPrefix (
                            const std::string& prefixName,
                            const std::string& prefixErsatz,
                            const std::string& prefixDescription);

    virtual               ~oahPrefix ();

  public:

    // set and get
    // ------------------------------------------------------

    std::string           getPrefixName () const
                              { return fPrefixName; }

    std::string           getPrefixErsatz () const
                              { return fPrefixErsatz; }

    std::string           getPrefixDescription () const
                              { return fPrefixDescription; }

  public:

    // public services
    // ------------------------------------------------------

    std::string           fetchPrefixNames () const;

    Bool                  findStringInFindableElement (
                            const std::string&               lowerCaseString,
                            std::list <S_oahFindStringMatch>& foundMatchesList,
                            std::ostream&                    os) const override;

    Bool                  fetchElementsMatchingStringInPrefix (
                            const std::string&       lowerCaseString,
                            std::list <S_oahElement>& foundElementsList,
                            std::ostream&            os) const;

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    std::string           asString () const override;

    void                  printPrefixHeader (std::ostream& os) const;

    virtual void          printPrefixEssentials (
                            std::ostream& os) const;

    void                  print (std::ostream& os) const override;

    virtual void          printHelp (std::ostream& os) const;

//     const std::string     containingFindableElementAsString () const override;

  protected:

    // protected fields
    // ------------------------------------------------------

    std::string           fPrefixName;
    std::string           fPrefixErsatz;
    std::string           fPrefixDescription;
};
typedef SMARTP<oahPrefix> S_oahPrefix;
EXP std::ostream& operator << (std::ostream& os, const S_oahPrefix& elt);

//______________________________________________________________________________
/*
  a common ancestor for all atom classes,
  this class   contains  only an uplink to the containing subgroup
*/
class EXP oahAtom : public oahElement
{
  public:

    // creation
    // ------------------------------------------------------

/* this class is purely virtual
    static SMARTP<oahAtom> create (
                            const std::string&  longName,
                            const std::string&  shortName,
                            const std::string&  description,
                            oahElementValueKind atomValueExpectedKind);
*/

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          oahAtom (
                            const std::string&  longName,
                            const std::string&  shortName,
                            const std::string&  description,
                            oahElementValueKind atomValueExpectedKind);

    virtual               ~oahAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    // uplinks
    void                  setUpLinkToSubGroup (
                            const S_oahSubGroup& subGroup);

    S_oahSubGroup         getUpLinkToSubGroup () const
                              { return fUpLinkToSubGroup; }

    // early option?
    void                  setEarlyOptionKind (
                            oahEarlyOptionKind earlyOptionKind)
                              { fEarlyOptionKind = earlyOptionKind; }

    oahEarlyOptionKind    getEarlyOptionKind () const
                              { return fEarlyOptionKind; }

  public:

    // public services
    // ------------------------------------------------------

    // uplinks
    S_oahGroup            fetchAtomUpLinkToGroup () const;

    S_oahHandler          fetchAtomUpLinkToHandler () const;

    // atoms handling
    void                  appendAtomToElementsList (
                            const S_oahHandler& handler);

    void                  registerAtomAsBeingUsed ();

    Bool                  fetchElementsMatchingStringInHandler (
                            const std::string&       lowerCaseString,
                            std::list <S_oahElement>& foundElementsList,
                            std::ostream&            os) const;

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

//     virtual std::string   asString () const override;
//     virtual std::string   asShortString () const;

    void                  print (std::ostream& os) const override;
    void                  printFull (std::ostream& os) const override;

    void                  printSummary (std::ostream& os) const;

    virtual Bool          findStringInAtom (
                            const std::string&               lowerCaseString,
                            std::list <S_oahFindStringMatch>& foundMatchesList,
                            std::ostream&                    os) const;

    virtual Bool          atomMatchesString (
                            const std::string& lowerCaseString,
                            std::ostream&      os) const;

    virtual void          displayAtomWithVariableOptionsValues (
                            std::ostream& os,
                            int           valueFieldWidth) const;

//     const std::string     containingFindableElementAsString () const override;

  protected:

    // protected services
    // ------------------------------------------------------

  protected:

    // protected fields
    // ------------------------------------------------------

    // uplink
    S_oahSubGroup         fUpLinkToSubGroup;

    oahEarlyOptionKind    fEarlyOptionKind;
};
typedef SMARTP<oahAtom> S_oahAtom;
EXP std::ostream& operator << (std::ostream& os, const S_oahAtom& elt);

//______________________________________________________________________________
class EXP oahValueLessAtom : public oahAtom
/*
  a purely virtual common ancestor for all atom classes
  that don't take a value from argv or an oahOptionsVector
*/
{
  public:

    // creation
    // ------------------------------------------------------
/* this class is purely virtual
    static SMARTP<oahValueLessAtom> create (
                            const std::string& longName,
                            const std::string& shortName,
                            const std::string& description);
*/
  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          oahValueLessAtom (
                            const std::string& longName,
                            const std::string& shortName,
                            const std::string& description);

    virtual               ~oahValueLessAtom ();

  public:

    // set and get
    // ------------------------------------------------------

  public:

    // public services
    // ------------------------------------------------------

    virtual void          applyValueLessAtom (std::ostream& os) = 0;

    Bool                  atomMatchesString (
                            const std::string& lowerCaseString,
                            std::ostream&      os) const override;

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    void                  print (std::ostream& os) const override;
    void                  printFull (std::ostream& os) const override;

  protected:

    // protected fields
    // ------------------------------------------------------
};
typedef SMARTP<oahValueLessAtom> S_oahValueLessAtom;
EXP std::ostream& operator << (std::ostream& os, const S_oahValueLessAtom& elt);

//______________________________________________________________________________
class EXP oahValueFittedAtom : public oahAtom
/*
  a purely virtual common ancestor for all atom classes
  that take a value from argv or an oahOptionsVector
*/
{
  public:

    // creation
    // ------------------------------------------------------
/* this class is purely virtual
    static SMARTP<oahValueFittedAtom> create (
                            const std::string& longName,
                            const std::string& shortName,
                            const std::string& description);
*/
  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          oahValueFittedAtom (
                            const std::string& longName,
                            const std::string& shortName,
                            const std::string& description);

    virtual               ~oahValueFittedAtom ();

  public:

    // set and get
    // ------------------------------------------------------

  public:

    // public services
    // ------------------------------------------------------

    virtual void          applyAtomWithValue (
                            const std::string& theString,
                            std::ostream&      os) = 0;

//     virtual void          applyAtomWithDefaultValue (std::ostream& os); // JMI ??? 0.9.66

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    void                  print (std::ostream& os) const override;
    void                  printFull (std::ostream& os) const override;

  protected:

    // protected fields
    // ------------------------------------------------------
};
typedef SMARTP<oahValueFittedAtom> S_oahValueFittedAtom;
EXP std::ostream& operator << (std::ostream& os, const S_oahValueFittedAtom& elt);

//______________________________________________________________________________
class EXP oahAtomStoringAValue : public oahValueFittedAtom
/*
  a purely virtual common ancestor for all atom classes
  that take a value from argv or an oahOptionsVector
  and store it internally in a variable
*/
{
  public:

    // creation
    // ------------------------------------------------------
/* this class is purely virtual
    static SMARTP<oahAtomStoringAValue> create (
                            const std::string& longName,
                            const std::string& shortName,
                            const std::string& description,
                            const std::string& valueSpecification,
                            const std::string& variableName);
*/

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          oahAtomStoringAValue (
                            const std::string& longName,
                            const std::string& shortName,
                            const std::string& description,
                            const std::string& valueSpecification,
                            const std::string& variableName);

    virtual               ~oahAtomStoringAValue ();

  public:

    // set and get
    // ------------------------------------------------------

    std::string           getValueSpecification () const
                              { return fValueSpecification; }

  public:

    // public services
    // ------------------------------------------------------

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    virtual void          printAtomWithVariableEssentials (
                            std::ostream& os,
                            int fieldWidth) const;
    virtual void          printAtomWithVariableEssentialsFull (
                            std::ostream& os,
                            int fieldWidth) const;

    void                  print (std::ostream& os) const override;

    void                  printHelp (std::ostream& os) const override;

    virtual void          displayAtomWithVariableOptionsValues (
                            std::ostream& os,
                            int           valueFieldWidth) const override;


    // protected fields
    // ------------------------------------------------------

    std::string           fValueSpecification;

    std::string           fVariableName;
};
typedef SMARTP<oahAtomStoringAValue> S_oahAtomStoringAValue;
EXP std::ostream& operator << (std::ostream& os, const S_oahAtomStoringAValue& elt);

//______________________________________________________________________________
class EXP oahAtomImplicitlySettingAVariable : public oahValueLessAtom
/*
  a purely virtual common ancestor for all atom classes
  that store a value in a variable
  withoud taking it from argv or an oahOptionsVector
*/
{
  public:

    // creation
    // ------------------------------------------------------
/* this class is purely virtual
    static SMARTP<oahAtomImplicitlySettingAVariable> create (
                            const std::string& longName,
                            const std::string& shortName,
                            const std::string& description,
                            const std::string& variableName);
*/

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          oahAtomImplicitlySettingAVariable (
                            const std::string& longName,
                            const std::string& shortName,
                            const std::string& description,
                            const std::string& variableName);

    virtual               ~oahAtomImplicitlySettingAVariable ();

  public:

    // set and get
    // ------------------------------------------------------

  public:

    // public services
    // ------------------------------------------------------

    virtual void          setImplicitVariable (std::ostream& os) = 0;

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    virtual void          printAtomWithVariableEssentials (
                            std::ostream& os,
                            int fieldWidth) const;
    virtual void          printAtomWithVariableEssentialsFull (
                            std::ostream& os,
                            int fieldWidth) const;

    void                  print (std::ostream& os) const override;

    void                  printHelp (std::ostream& os) const override;

    virtual void          displayAtomWithVariableOptionsValues (
                            std::ostream& os,
                            int           valueFieldWidth) const override;

  protected:

    // protected fields
    // ------------------------------------------------------

    std::string           fVariableName;
};
typedef SMARTP<oahAtomImplicitlySettingAVariable> S_oahAtomImplicitlySettingAVariable;
EXP std::ostream& operator << (std::ostream& os, const S_oahAtomImplicitlySettingAVariable& elt);

//______________________________________________________________________________
class EXP oahValueDefaultedAtom : public oahAtom
{
/*
  an atom controlling a std::string variable with a default value
  used in case there is no '=...' attached to the atom name
*/

/* this class is purely virtual
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahValueDefaultedAtom> create (
                            const std::string& longName,
                            const std::string& shortName,
                            const std::string& description,
                            const std::string& defaultStringValue);
*/

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          oahValueDefaultedAtom (
                            const std::string& longName,
                            const std::string& shortName,
                            const std::string& description,
                            const std::string& defaultStringValue);

    virtual               ~oahValueDefaultedAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    std::string           getDefaultStringValue () const
                              { return fDefaultStringValue; }

  public:

    // public services
    // ------------------------------------------------------

    virtual void          applyAtomWithDefaultValue (std::ostream& os) = 0;

    virtual void          applyAtomWithValue (
                            const std::string& theString,
                            std::ostream&      os) = 0;

  public:

    // visitors
    // ------------------------------------------------------

//     void                  acceptIn  (basevisitor* v) override;
//     void                  acceptOut (basevisitor* v) override;
//
//     void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

//     std::string           asShortNamedOptionString () const override;
//     std::string           asActualLongNamedOptionString () const override;
//
//     void                  print (std::ostream& os) const override;
//
//     void                  displayAtomWithVariableOptionsValues (
//                             std::ostream& os,
//                             int           valueFieldWidth) const override;

  protected:

    // protected fields
    // ------------------------------------------------------

    std::string           fDefaultStringValue;
};
typedef SMARTP<oahValueDefaultedAtom> S_oahValueDefaultedAtom;
EXP std::ostream& operator << (std::ostream& os, const S_oahValueDefaultedAtom& elt);

//______________________________________________________________________________
class EXP oahPureHelpValueLessAtom : public oahValueLessAtom
{
  public:

    // creation
    // ------------------------------------------------------

/* this class is purely virtual
    static SMARTP<oahPureHelpValueLessAtom> create (
                            const std::string& longName,
                            const std::string& shortName,
                            const std::string& description,
                            const std::string& serviceName);
*/

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          oahPureHelpValueLessAtom (
                            const std::string& longName,
                            const std::string& shortName,
                            const std::string& description,
                            const std::string& serviceName);

    virtual               ~oahPureHelpValueLessAtom ();

  public:

    // set and get
    // ------------------------------------------------------

  public:

    // public services
    // ------------------------------------------------------

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    void                  print (std::ostream& os) const override;

    void                  displayAtomWithVariableOptionsValues (
                            std::ostream& os,
                            int           valueFieldWidth) const override;

  protected:

    // protected fields
    // ------------------------------------------------------

    std::string           fHelpValueLessAtomServiceName;
};
typedef SMARTP<oahPureHelpValueLessAtom> S_oahPureHelpValueLessAtom;
EXP std::ostream& operator << (std::ostream& os, const S_oahPureHelpValueLessAtom& elt);

//______________________________________________________________________________
class EXP oahPureHelpValueFittedAtom : public oahValueFittedAtom
{
  public:

    // creation
    // ------------------------------------------------------

/* this class is purely virtual
    static SMARTP<oahPureHelpValueFittedAtom> create (
                            const std::string& longName,
                            const std::string& shortName,
                            const std::string& description,
                            const std::string& valueSpecification,
                            const std::string& serviceName);
*/

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          oahPureHelpValueFittedAtom (
                            const std::string& longName,
                            const std::string& shortName,
                            const std::string& description,
                            const std::string& valueSpecification,
                            const std::string& serviceName);

    virtual               ~oahPureHelpValueFittedAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    std::string           getValueSpecification () const
                              { return fValueSpecification; }

  public:

    // public services
    // ------------------------------------------------------

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    void                  print (std::ostream& os) const override;

    void                  printHelp (std::ostream& os) const override;

  protected:

    // protected fields
    // ------------------------------------------------------

    std::string           fHelpValueFittedAtomServiceName; // JMI ???

    std::string           fValueSpecification;
};
typedef SMARTP<oahPureHelpValueFittedAtom> S_oahPureHelpValueFittedAtom;
EXP std::ostream& operator << (std::ostream& os, const S_oahPureHelpValueFittedAtom& elt);

//_______________________________________________________________________________
class EXP oahSubGroup : public oahElement
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahSubGroup> create (
                            const std::string&       subGroupHeader,
                            const std::string&       longName,
                            const std::string&       shortName,
                            const std::string&       description,
                            oahElementVisibilityKind optionVisibilityKind,
                            const S_oahGroup&        upLinkToGroup);

    static SMARTP<oahSubGroup> createWithNames (
                            const std::string&       subGroupHeader,
                            const std::string&       longName,
                            const std::string&       shortName,
                            const std::string&       description,
                            oahElementVisibilityKind optionVisibilityKind,
                            const S_oahGroup&        upLinkToGroup);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          oahSubGroup (
                            const std::string&       subGroupHeader,
                            const std::string&       longName,
                            const std::string&       shortName,
                            const std::string&       description,
                            oahElementVisibilityKind optionVisibilityKind,
                            const S_oahGroup&        upLinkToGroup);

    virtual               ~oahSubGroup ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setUpLinkToGroup (
                            const S_oahGroup& group)
                              { fUpLinkToGroup = group; }

    S_oahGroup            getUpLinkToGroup () const
                              { return fUpLinkToGroup; }

    std::string           getSubGroupHeader () const
                              { return fSubGroupHeader; }

    void                  incrementNumberOfUserChoseAtomsInThisSubGroup ()
                              { fNumberOfUserChoseAtomsInThisSubGroup += 1; }

    int                   getNumberOfUserChoseAtomsInThisSubGroup () const
                              { return fNumberOfUserChoseAtomsInThisSubGroup; }

    const std::list <S_oahAtom>&
                          getSubGroupAtomsList () const
                              { return fSubGroupAtomsList; }

  public:

    // consistency check
    // ------------------------------------------------------

    virtual void          checkSubGroupOptionsConsistency ();

  public:

    // public services
    // ------------------------------------------------------

    // uplink
    S_oahHandler          fetchSubGroupUpLinkToHandler () const;

    void                  appendSubGroupToElementsList (
                            const S_oahHandler& handler);

    void                  registerNamesInSubGroupToTheNamesToElementsMap (
                            const S_oahHandler& handler);

    void                  appendAtomToSubGroup (
                            const S_oahAtom& oahAtom);

    S_oahElement          fetchOptionByNameInSubGroup (
                            const std::string& name);

    void                  applySubGroup (std::ostream& os);

    Bool                  fetchElementsMatchingStringInSubGroup (
                            const std::string&       lowerCaseString,
                            std::list <S_oahElement>& foundElementsList,
                            std::ostream&            os) const;

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    void                  print (std::ostream& os) const override;

    void                  printSummary (std::ostream& os) const;

    void                  printHelp (std::ostream& os) const override;

    void                  printHelpWithHeaderWidth (
                            std::ostream& os,
                            int           subGroupHeaderWidth) const;

    void                  underlineSubGroupHeader (std::ostream& os) const;

    void                  printSubGroupHeader (std::ostream& os) const;
    void                  printSubGroupHeaderWithHeaderWidth (
                            std::ostream& os,
                            int           subGroupHeaderWidth) const;

    void                  printSubGroupHelp (
                            std::ostream& os) const;

    void                  printSubGroupAndAtomHelp (
                            std::ostream&  os,
                            const S_oahAtom& targetAtom) const;

    void                  printOptionsSummary (std::ostream& os) const;

    void                  printSubGroupSpecificHelpOrOptionsSummary (
                            std::ostream&      os,
                            const S_oahSubGroup& subGroup) const;

    Bool                  findStringInSubGroup (
                            const std::string&               lowerCaseString,
                            std::list <S_oahFindStringMatch>& foundMatchesList,
                            std::ostream&                    os) const;

    void                  printSubGroupOptionsValues (
                            std::ostream& os,
                            int           valueFieldWidth) const;
    void                  printSubGroupOptionsValuesAll (
                            std::ostream& os,
                            int           valueFieldWidth) const;

//     const std::string     containingFindableElementAsString () const override;

  private:

    // private fields
    // ------------------------------------------------------

    S_oahGroup            fUpLinkToGroup;

    std::string           fSubGroupHeader;

    std::list <S_oahAtom>  fSubGroupAtomsList;

    // protected work fields
    // ------------------------------------------------------

    int                   fNumberOfUserChoseAtomsInThisSubGroup;
};
typedef SMARTP<oahSubGroup> S_oahSubGroup;
EXP std::ostream& operator << (std::ostream& os, const S_oahSubGroup& elt);

//_______________________________________________________________________________
class EXP oahGroup : public oahElement
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahGroup> create (
                            const std::string&       header,
                            const std::string&       longName,
                            const std::string&       shortName,
                            const std::string&       description,
                            oahElementVisibilityKind optionVisibilityKind);

    static SMARTP<oahGroup> create (
                            const std::string&       header,
                            const std::string&       longName,
                            const std::string&       shortName,
                            const std::string&       description,
                            oahElementVisibilityKind optionVisibilityKind,
                            const S_oahHandler&      groupUpLinkToHandler);

    static SMARTP<oahGroup> createWithNames (
                            const std::string&       header,
                            const std::string&       longName,
                            const std::string&       shortName,
                            const std::string&       description,
                            oahElementVisibilityKind optionVisibilityKind);

    static SMARTP<oahGroup> createWithNames (
                            const std::string&       header,
                            const std::string&       longName,
                            const std::string&       shortName,
                            const std::string&       description,
                            oahElementVisibilityKind optionVisibilityKind,
                            const S_oahHandler&      groupUpLinkToHandler);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          oahGroup (
                            const std::string&       header,
                            const std::string&       longName,
                            const std::string&       shortName,
                            const std::string&       description,
                            oahElementVisibilityKind optionVisibilityKind);

                          oahGroup (
                            const std::string&       header,
                            const std::string&       longName,
                            const std::string&       shortName,
                            const std::string&       description,
                            oahElementVisibilityKind optionVisibilityKind,
                            const S_oahHandler&      groupUpLinkToHandler);

    virtual               ~oahGroup ();

  public:

    // set and get
    // ------------------------------------------------------

    // uplink
    void                  setUpLinkToHandler (
                            const S_oahHandler& upLinkToHandler);

    S_oahHandler          getUpLinkToHandler () const
                              { return fUpLinkToHandler; }

    // header
    std::string           getGroupHeader () const
                              { return fGroupHeader; }

    // groups list
    const std::list <S_oahSubGroup>&
                          getGroupSubGroupsList () const
                              { return fGroupSubGroupsList; }

    // is the header to be written?
    void                  setGroupHeaderIsToBeWritten (Bool value)
                              { fGroupHeaderIsToBeWritten = value; }

    Bool                  getGroupHeaderIsToBeWritten () const
                              { return fGroupHeaderIsToBeWritten; }

    // number of selected atoms
    void                  incrementNumberOfUserChoseAtomsInThisGroup ()
                              { fNumberOfUserChoseAtomsInThisGroup += 1; }

    // number of selected atoms
    int                   getNumberOfUserChoseAtomsInThisGroup () const
                              { return fNumberOfUserChoseAtomsInThisGroup; }

  public:

    // quiet mode
    // ------------------------------------------------------

    virtual void          enforceGroupQuietness ();

  public:

    // consistency check
    // ------------------------------------------------------

    void                  checkGroupSubGroupsOptionsConsistency ();

    virtual void          checkGroupOptionsConsistency ();

  public:

    // public services
    // ------------------------------------------------------

    void                  appendGroupToElementsList (
                            const S_oahHandler& handler);

    void                  registerNamesInGroupToTheNamesToElementsMap (
                            const S_oahHandler& handler);

    void                  appendSubGroupToGroup (
                            const S_oahSubGroup& subGroup);

    S_oahElement          fetchOptionByNameInGroup (
                            const std::string& name);

    void                  applyGroup (std::ostream& os);

    virtual void          handleAtomValue (
                            std::ostream&      os,
                            const S_oahAtom&   atom,
                            const std::string& theString);

    // multiComponent
    S_mfcMultiComponent   fetchGroupHandlerMultiComponent () const;

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    void                  print (std::ostream& os) const override;

    void                  printSummary (std::ostream& os) const;

    void                  printGroupHeader (std::ostream& os) const;

    void                  printHelp (std::ostream& os) const override;

    void                  underlineGroupHeader (std::ostream& os) const;

    void                  printGroupAndSubGroupHelp (
                            std::ostream&      os,
                            const S_oahSubGroup& targetSubGroup) const;

    void                  printGroupAndSubGroupAndAtomHelp (
                            std::ostream&        os,
                            const S_oahSubGroup& targetSubGroup,
                            const S_oahAtom&     targetAtom) const;

    void                  printOptionsSummary (std::ostream& os) const;

    void                  printGroupAndSubGroupSpecificHelp (
                            std::ostream&        os,
                            const S_oahSubGroup& subGroup) const;

    void                  findStringInGroup (
                            const std::string&               lowerCaseString,
                            std::list <S_oahFindStringMatch>& foundMatchesList,
                            std::ostream&                    os) const;

    Bool                  fetchElementsMatchingStringInGroup (
                            const std::string&       lowerCaseString,
                            std::list <S_oahElement>& foundElementsList,
                            std::ostream&            os) const;

    void                  printGroupOptionsValues (
                            std::ostream& os,
                            int           valueFieldWidth) const;
    void                  printGroupOptionsValuesAll (
                            std::ostream& os,
                            int           valueFieldWidth) const;

//     const std::string     containingFindableElementAsString () const override;

  private:

    // private services
    // ------------------------------------------------------

/* JMI STRANGE
    // prefixes
    std::string           prefixNamesInColumns (
                            int subGroupsShortNameFieldWidth) const;

    std::string           prefixNamesBetweenParentheses () const;
    std::string           prefixNamesInColumnsBetweenParentheses (
                            int subGroupsShortNameFieldWidth) const;

    std::string           operator () () const
                              { return fPrefixErsatz; }
*/

  protected:

    // protected fields
    // ------------------------------------------------------

    // uplink
    S_oahHandler          fUpLinkToHandler;

    std::string           fGroupHeader;

    std::list <S_oahSubGroup>
                          fGroupSubGroupsList;

    Bool                  fGroupHeaderIsToBeWritten;

  protected:

    // protected work fields
    // ------------------------------------------------------

    int                   fNumberOfUserChoseAtomsInThisGroup;
};
typedef SMARTP<oahGroup> S_oahGroup;
EXP std::ostream& operator << (std::ostream& os, const S_oahGroup& elt);

//_______________________________________________________________________________
class EXP oahHandler : public oahFindableElement //  smartable
{
  public:

    // creation
    // ------------------------------------------------------

/* this class is purely virtual
    static SMARTP<oahHandler> create (
                            const std::string& serviceName,
                            const std::string& handlerHeader,
                            const std::string& handlerDescription,
                            const std::string& handlerUsage);
*/

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          oahHandler (
                            const std::string& serviceName,
                            const std::string& handlerHeader,
                            const std::string& handlerDescription,
                            const std::string& handlerUsage);

   virtual                ~oahHandler ();

  protected:

    // protected initialization
    // ------------------------------------------------------

    virtual void          initializeHandlerMultiComponent () = 0;

  public:

    // set and get
    // ------------------------------------------------------

    // OAH handler
    static S_oahHandler   getCurrentOahHandler ()
                              { return sCurrentOahHandler; }

  public:

    // set and get
    // ------------------------------------------------------

    std::string           getHandlerServiceName () const
                              { return fHandlerServiceName; }

    void                  setHandlerMultiComponent (
                            const S_mfcMultiComponent& multiComponent)
                              { fHandlerMultiComponent = multiComponent; }

    S_mfcMultiComponent   getHandlerMultiComponent () const
                              { return fHandlerMultiComponent; }

    std::string           getHandlerHeader () const
                              { return fHandlerHeader; }

    std::string           getHandlerDescription () const
                              { return fHandlerDescription; }

    std::string           getHandlerUsage () const
                              { return fHandlerUsage; }

    // launch command
    std::string           getLaunchCommandAsSupplied () const
                              { return fLaunchCommandAsSupplied; }

    std::string           getLaunchCommandWithLongOptionsNames () const
                              { return fLaunchCommandWithLongOptionsNames; }
    std::string           getLaunchCommandWithShortOptionsNames () const
                              { return fLaunchCommandWithShortOptionsNames; }

    // prefixes
    const std::map <std::string, S_oahPrefix>&
                          getHandlerPrefixesMap () const
                              { return fHandlerPrefixesMap; }

    // groups lists
    const std::list <S_oahGroup>&
                          getHandlerGroupsList () const
                              { return fHandlerGroupsList; }

/* JMI
    // optional values style
    oahOptionalValuesStyleKind
                          getHandlerOptionalValuesStyleKind () const
                              { return fHandlerOptionalValuesStyleKind; }
*/

    // handler used thru...?
    oahHandlerUsedThruKind
                          getHandlerUsedThruKind () const
                              { return fHandlerUsedThruKind; }

    // options and arguments
    const oahOptionsAndArguments&
                          getOptionsAndArguments () const
                              { return fOptionsAndArguments; }

    // elements
    const std::list <S_oahElement>&
                          getHandlerElementsList () const
                              { return fHandlerElementsList; }

    const std::list <S_oahElement>&
                          getHandlerCommandLineElementsList () const
                              { return fHandlerCommandLineElementsList; }

    const std::multiset <S_oahElement>&
                          getHandlerCommandLineElementsMultiset () const
                              { return fHandlerCommandLineElementsMultiset; }

    // widths and sizes
    size_t                getMaximumShortNameWidth () const
                              { return fMaximumShortNameWidth; }

    size_t                getMaximumLongNameWidth () const
                              { return fMaximumLongNameWidth; }

    int                   getMaximumSubGroupsHeadersSize () const
                              { return fMaximumSubGroupsHeadersSize; }

    Bool                  getOahHandlerFoundAHelpOption () const
                              { return fOahHandlerFoundAHelpOption; }

  public:

    // consistency check
    // ------------------------------------------------------

    virtual void          checkHandlerOptionsConsistency ();

  public:

    // public services
    // ------------------------------------------------------

    // OAH errors
    void                  unknownOptionNameWarning (
                            const std::string& name,
                            const std::string& context);

    void                  unknownOptionNameError (
                            const std::string& name,
                            const std::string& context);

    void                  unknownOptionValueKindError (const std::string& name);

    virtual std::string   handlerServiceAboutInformation () const = 0;

    S_oahElement          fetchElementByNameInHandler (
                            const std::string& name);

    void                  registerPrefixInHandler (
                            const S_oahPrefix& prefix);

    void                  appendGroupToHandler (
                            const S_oahGroup& group);
    void                  prependGroupToHandler (
                            const S_oahGroup& group);

    void                  appendElementToElementsList (
                            const S_oahElement& element);

    S_oahPrefix           fetchNameInPrefixesMap (
                            const std::string& prefixName) const;

    S_oahElement          fetchNameInNamesToElementsMap (
                            const std::string& name) const;

    void                  registerElementNamesInHandler (
                            const S_oahElement& element);

    // options and arguments handling
    oahElementHelpOnlyKind
                          handleOptionsFromOptionsAndArguments (
                            const std::string& serviceName);

    oahElementHelpOnlyKind
                          handleOptionsAndArgumentsFromArgcArgv (
                            int   argc,
                            char* argv[]);

    virtual void          checkOptionsAndArgumentsConsistency ();

    virtual void          checkOptionsAndArguments () const = 0;

    void                  setOahHandlerFoundAHelpOption (
                            const std::string& context);

    virtual std::string   fetchOutputFileNameFromTheOptions () const;
                            // JMI the MFSL interpreter has no output file... 0.9.62

    // quiet mode
    virtual void          enforceHandlerQuietness ();

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    virtual void          printHandlerEssentials (
                            std::ostream& os,
                            int fieldWidth) const;

    void                  printKnownPrefixes (std::ostream& os) const;
    void                  printKnownSingleCharacterOptions (std::ostream& os) const;
//     void                  printOptionsDefaultValuesInformation (std::ostream& os) const; JMI 0.9.66

    void                  displayNamesToElementsMap (std::ostream& os) const;
    void                  displayNamesToElementsMapShort (std::ostream& os) const;

    void                  displayNamesToElementsMap ();

    void                  displayElementUsesList (std::ostream& os) const;

    void                  displayOptionsAndArguments (std::ostream& os) const;

//     void                  displayArgumentsVector (std::ostream& os) const; JMI 0.9.65

    virtual std::string   asString () const override;

    virtual void          print (std::ostream& os) const override;
    virtual void          printFull (std::ostream& os) const override;

    void                  printSummary (std::ostream& os) const;

    void                  printHelp (std::ostream& os) const;

    void                  printOptionsSummary (std::ostream& os) const;
    void                  printOptionsSummary () const
                              { printOptionsSummary (gLog); }

    void                  printHandlerAndGroupAndSubGroupSpecificHelp (
                            std::ostream&      os,
                            const S_oahSubGroup& subGroup) const;

    void                  printNameIntrospectiveHelp (
                            std::ostream&      os,
                            const std::string& name);

    Bool                  findStringInFindableElement (
                            const std::string&               lowerCaseString,
                            std::list <S_oahFindStringMatch>& foundMatchesList,
                            std::ostream&                    os) const override;

    Bool                  fetchElementsMatchingStringInHandler (
                            const std::string&       lowerCaseString,
                            std::list <S_oahElement>& foundElementsList,
                            std::ostream&            os) const;

    void                  includeOptionsFileInHandler (
                            const std::string& optionsFileName,
                            std::ostream&      os);

    static int            getIncludeFilesMaximumLevel ()
                              { return sIncludeFilesMaximumLevel; }

    void                  printHandlerOptionsAndArguments (
                            std::ostream& os) const;

    void                  printHandlerOptionsValues (
                            std::ostream& os) const;
    void                  printHandlerOptionsValuesAll (
                            std::ostream& os) const;

    void                  printIncludeFileNamesStack (
                            std::ostream& os) const;

//     const std::string     containingFindableElementAsString () const override;

  protected:

    // protected services
    // ------------------------------------------------------

    void                  createTheCommonPrefixes ();

    void                  checkNoInputSourceInArgumentsVector () const;

    void                  checkNoOrOneInputSourceInArgumentsVector () const;

    void                  checkSingleInputSourceInArgumentsVector () const;

    void                  checkOneArgumentInArgumentsVector () const;

    void                  checkOneArgumentAndNoOrOneInputSourceInArgumentsVector () const;

  private:

    // private services
    // ------------------------------------------------------

    // initialization
    void                  initializeHandler ();

    // names handling
    S_oahPrefix           fetchNameInHandlerPrefixesMap (
                            const std::string& name) const;

    void                  registerNamesInHandlerToTheNamesToElementsMap ();

    void                  registerOahElementUse (
                            const S_oahElement& elementUsed,
                            const std::string&  nameUsed,
                            const std::string&  valueUsed);

    void                  registerGroupUse (
                            const S_oahGroup&  groupUsed,
                            const std::string& nameUsed,
                            const std::string& valueUsed)
                              {
                                registerOahElementUse (
                                  groupUsed,
                                  nameUsed,
                                  valueUsed);
                              }

    void                  registerSubGroupUse (
                            const S_oahSubGroup& subGroupUsed,
                            const std::string&   nameUsed,
                            const std::string&   valueUsed)
                              {
                                registerOahElementUse (
                                  subGroupUsed,
                                  nameUsed,
                                  valueUsed);
                              }

    void                  registerAtomUse (
                            const S_oahAtom&   atomUsed,
                            const std::string& nameUsed,
                            const std::string& valueUsed);

    void                  registerOahElementUseInLaunchCommand (
                            const S_oahElement& elementUsed,
                            const std::string&  nameUsed,
                            const std::string&  valueUsed);

    void                  registerArgumentUseInLaunchCommand (
                            const std::string& argumentUsed);

    void                  handleOptionPrefixName (
                            const std::string& prefixName,
                            size_t             equalsSignPosition,
                            const std::string& stringAfterEqualsSign);

    Bool                  isNameASingleCharacterOptionsCluster (
                            const std::string& optionName);

    void                  handleUnknownOptionName (
                            const std::string& optionName);

    // options and arguments handling
    // pass 1
    void                  createElementUsesListFromOptionsAndArguments (
                            const std::string& serviceName);

    void                  createElementUsesListFromArgcArgv (
                            int   argc,
                            char* argv[]);

    // pass 2
    oahElementHelpOnlyKind
                          applyOptionsFromElementUsesList ();

    // oahOptionsVector elements handling
    void                  handleOptionNameAndValueAtTopOahLevel (
                            const std::string& optionName,
                            const std::string& optionValue);

    void                  handleOptionsVectorOptionName (
                            const std::string& theString,
                            const std::string& valueUsed);

    void                  handleKnownOptionsVectorElement (
                            const S_oahElement& element,
                            const std::string&  optionNameUsed,
                            const std::string&  valueUsed);

    void                  handleKnownOptionsVectorAtom (
                            const S_oahAtom&   atom,
                            const std::string& optionNameUsed,
                            const std::string& valueUsed);

    // argc/argv handling
    void                  handleArgvOptionValueOrArgument (
                            const std::string& theString);

    std::string           handleArgvOptionName (
                            const std::string& optionName);

    void                  handleKnownArgvAtom (
                            const S_oahAtom&   atom,
                            const std::string& optionNameUsed);

    void                  handleKnownArgvElement (
                            const S_oahElement& element,
                            const std::string&  optionNameUsed);

    void                  checkMissingPendingArgvValueFittedAtomValue (
                            const std::string& atomName,
                            const std::string& context);

    // common methods
    void                  handleNameContainingEqualSign (
                            const std::string& optionName,
                            size_t        equalsSignPosition);

    void                  handleOptionNameCommon (
                            const std::string& optionName);

    // options concistency
    void                  checkHandlerOwnOptionsConsistency ();

  protected:

    // protected fields
    // ------------------------------------------------------

    // current OAH handler
    static S_oahHandler   sCurrentOahHandler;

    // service
    std::string           fHandlerServiceName;

    // multi-components
    S_mfcMultiComponent   fHandlerMultiComponent;

    // header
    std::string           fHandlerHeader;

    // description
    std::string           fHandlerDescription;

    // usage
    std::string           fHandlerUsage;

    // prefixes map
    std::map <std::string, S_oahPrefix>
                          fHandlerPrefixesMap;

    // single character names
    std::set <std::string> fSingleCharacterNamesSet;

    // groups list
    std::list <S_oahGroup> fHandlerGroupsList;

    // elements map
    std::map <std::string, S_oahElement>
                          fHandlerNamesToElementsMap;

/* JMI
    // optional values style
    oahOptionalValuesStyleKind
                          fHandlerOptionalValuesStyleKind;
*/

    // handler used thru...?
    oahHandlerUsedThruKind
                          fHandlerUsedThruKind;

    // has help been requested?
    // this is needed to quit if the API function or service
    // is launched with one or more help options,
    // i.e. options that are only used to display help to the user
    Bool                  fOahHandlerFoundAHelpOption;

  private:

    // private services
    // ------------------------------------------------------


    void                  includeOptionsAndArgumentsFile (
                            const std::string& includeFileName,
                            std::ostream&      os);

    void                  includeOptionsFromFile (
                            std::istream&           inputStream,
                            oahOptionsAndArguments& optionsAndArguments);

//     void                  analyzeOptionsAndArguments ( // UNUSED JMI 0.9.66
//                             std::string              serviceName,
//                             const oahOptionsAndArguments& optionsAndArguments);

    size_t                getMaximumVariableNameWidth () const
                              { return fMaximumVariableNameWidth; }

  private:

    // work fields
    // ------------------------------------------------------

    // all OAH elements are registered in the handler upon initialization
    std::list <S_oahElement>
                          fHandlerElementsList;

    // those ones have be used in the command line
    std::list <S_oahElement>
                          fHandlerCommandLineElementsList;
    std::multiset <S_oahElement>
                          fHandlerCommandLineElementsMultiset;

    // launch command
    std::string           fLaunchCommandAsSupplied;

    std::string           fLaunchCommandWithLongOptionsNames;
    std::string           fLaunchCommandWithShortOptionsNames;

    Bool                  fLaunchOptionsAndOrArgumentsAlreadyMet;

//     Bool                  fNowEverythingIsAnArgument; // JMI ???

    // sizes and widths
    size_t                fMaximumSubGroupsHeadersSize;

    size_t                fMaximumShortNameWidth;
    size_t                fMaximumLongNameWidth;

    size_t                fMaximumVariableNameWidth;

    // elements uses
    std::list <S_oahElementUse>
                          fElementUsesList;

    // options and arguments
    oahOptionsAndArguments
                          fOptionsAndArguments;

    // atoms waiting for a value
    S_oahValueFittedAtom  fPendingArgvValueFittedAtom;
    std::string           fNameUsedForPendingArgvValueFittedAtom;

    std::list <std::string>
                          fIncludeFileNamesStack;
    int                   fIncludeFilesCurrentLevel;
    static int            sIncludeFilesMaximumLevel;
};
typedef SMARTP<oahHandler> S_oahHandler;
EXP std::ostream& operator << (std::ostream& os, const S_oahHandler& elt);

//______________________________________________________________________________
extern EXP S_oahHandler getCurrentOahHandler ();


}


#endif // ___oahBasicTypes___

