/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

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

#include "oahBasicTypes.h"

#include "mfServiceRunData.h"
#include "mfIndentedTextOutput.h"

#include "oahElements.h"


using namespace std;
using namespace MusicXML2;

namespace MusicFormats
{

//______________________________________________________________________________
// PRE-declarations for class mutual dependencies

class oahAtom;
typedef SMARTP<oahAtom> S_oahAtom;

class oahSubGroup;
typedef SMARTP<oahSubGroup> S_oahSubGroup;

class oahGroup;
typedef SMARTP<oahGroup> S_oahGroup;

class oahHandler;
typedef SMARTP<oahHandler> S_oahHandler;

//______________________________________________________________________________
enum class oahOptionOrArgumentKind {
  kOption,
  kArgument
};

string optionOrArgumentKindAsString (
  oahOptionOrArgumentKind optionOrArgumentKind );

ostream& operator<< (ostream& os, oahOptionOrArgumentKind& elt);

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
                            const string&           first,
                            const string&           second);

  public:

    // constructors/destructor
    // ------------------------------------------------------

                          oahOptionOrArgument (
                            oahOptionOrArgumentKind optionOrArgumentKind,
                            const string&           first,
                            const string&           second);

    virtual               ~oahOptionOrArgument ();

  public:

    // set and get
    // ------------------------------------------------------

    const string&         getFirst () const
                              { return fFirst; }

    const string&         getSecond () const
                              { return fSecond; }

  public:

    // public services
    // ------------------------------------------------------

    void                  appendOptionNameAndValue (
                            const string& optionName,
                            const string& optionValue);

    void                  appendArgument (const string& argument);

  public:

    // print
    // ------------------------------------------------------

    virtual void          print (ostream& os) const;

  protected:

    // protected fields
    // ------------------------------------------------------

    oahOptionOrArgumentKind
                          fOptionOrArgumentKind;

    string                fFirst;
    string                fSecond;
};
typedef SMARTP<oahOptionOrArgument> S_oahOptionOrArgument;
EXP ostream& operator<< (ostream& os, const S_oahOptionOrArgument& elt);
EXP ostream& operator<< (ostream& os, const oahOptionOrArgument& elt);

//_______________________________________________________________________________
class EXP oahOptionNameAndValue : public smartable
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahOptionNameAndValue> create (
                            const string& optionName,
                            const string& optionValue);

  public:

    // constructors/destructor
    // ------------------------------------------------------

                          oahOptionNameAndValue (
                            const string& optionName,
                            const string& optionValue);

    virtual               ~oahOptionNameAndValue ();

  public:

    // set and get
    // ------------------------------------------------------

    const string&         getOptionName () const
                              { return fOptionName; }

    const string&         getOptionValue () const
                              { return fOptionValue; }

  public:

    // public services
    // ------------------------------------------------------

  public:

    // print
    // ------------------------------------------------------

    virtual void          print (ostream& os) const;

  protected:

    // protected fields
    // ------------------------------------------------------

    string                fOptionName;
    string                fOptionValue;
};
typedef SMARTP<oahOptionNameAndValue> S_oahOptionNameAndValue;
EXP ostream& operator<< (ostream& os, const S_oahOptionNameAndValue& elt);
EXP ostream& operator<< (ostream& os, const oahOptionNameAndValue& elt);

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

    const vector<oahOptionNameAndValue>&
                          getOptionNameAndValuesVector () const
                              { return fOptionNameAndValuesVector; }

    const vector<string>& getArgumentsVector () const
                              { return fArgumentsVector; }

    // public services
    // ------------------------------------------------------

    void                  appendOptionNameAndValue (
                            const string& optionName,
                            const string& optionValue);

    void                  appendArgument (const string& argument);

  public:

    // print
    // ------------------------------------------------------

    void                  displayOptionNameAndValuesVector (ostream& os) const;
    void                  displayArgumentsVector (ostream& os) const;

    virtual void          print (ostream& os) const;

  protected:

    // protected fields
    // ------------------------------------------------------

    vector<oahOptionNameAndValue>
                          fOptionNameAndValuesVector;
    vector<string>        fArgumentsVector;
};
typedef SMARTP<oahOptionsAndArguments> S_oahOptionsAndArguments;
EXP ostream& operator<< (ostream& os, const S_oahOptionsAndArguments& elt);
EXP ostream& operator<< (ostream& os, const oahOptionsAndArguments& elt);

//_______________________________________________________________________________
// early option?
enum class oahEarlyOptionKind {
  kEarlyOptionNo, // default value
  kEarlyOptionYes,
};

string oahEarlyOptionKindAsString (
  oahEarlyOptionKind earlyOptionKind);

ostream& operator<< (ostream& os, oahEarlyOptionKind& elt);

//_______________________________________________________________________________
// handler used thru...?
enum class oahHandlerUsedThruKind {
  kHandlerUsedThruUnknown, // default value
  kHandlerUsedThruArgcArgv,
  kHandlerUsedThruOptionsAndArguments
};

string oahHandlerUsedThruKindAsString (
  oahHandlerUsedThruKind handlerUsedThruKind);

ostream& operator<< (ostream& os, oahHandlerUsedThruKind& elt);

//______________________________________________________________________________
class oahPrefix;
typedef SMARTP<oahPrefix> S_oahPrefix;

class EXP oahPrefix : public smartable
/*
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
                            const string& prefixName,
                            const string& prefixErsatz,
                            const string& prefixDescription);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          oahPrefix (
                            const string& prefixName,
                            const string& prefixErsatz,
                            const string& prefixDescription);

    virtual               ~oahPrefix ();

  public:

    // set and get
    // ------------------------------------------------------

    string                getPrefixName () const
                              { return fPrefixName; }

    string                getPrefixErsatz () const
                              { return fPrefixErsatz; }

    string                getPrefixDescription () const
                              { return fPrefixDescription; }

  public:

    // public services
    // ------------------------------------------------------

    string                fetchPrefixNames () const;

    void                  findStringInPrefix (
                            const string& lowerCaseString,
                            list<string>& foundStringsList,
                            ostream&      os) const;

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    string                asString () const;

    void                  printPrefixHeader (ostream& os) const;

    virtual void          printPrefixEssentials (
                            ostream& os,
                            int fieldWidth) const;

    virtual void          print (ostream& os) const;
    virtual void          printShort (ostream& os) const;

    virtual void          printHelp (ostream& os) const;

  protected:

    // protected fields
    // ------------------------------------------------------

    string                fPrefixName;
    string                fPrefixErsatz;
    string                fPrefixDescription;
};
EXP ostream& operator<< (ostream& os, const S_oahPrefix& elt);

//______________________________________________________________________________
/*
  a common ancestor for all atom classes,
  this class contains  only an uplink to the containing subgroup
*/
class EXP oahAtom : public oahElement
{
  public:

    // creation
    // ------------------------------------------------------

/* this class is purely virtual
    static SMARTP<oahAtom> create (
                            const string&       longName,
                            const string&       shortName,
                            const string&       description,
                            oahElementValueKind atomValueExpectedKind);
*/

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          oahAtom (
                            const string&       longName,
                            const string&       shortName,
                            const string&       description,
                            oahElementValueKind atomValueExpectedKind);

    virtual               ~oahAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    // uplinks
    void                  setSubGroupUpLink (
                            S_oahSubGroup subGroup);

    S_oahSubGroup         getSubGroupUpLink () const
                              { return fSubGroupUpLink; }

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
    S_oahGroup            fetchAtomGroupUpLink () const;

    S_oahHandler          fetchAtomHandlerUpLink () const;

    // atoms handling
    void                  appendAtomToElementsList (
                            S_oahHandler handler);

    void                  registerAtomAsBeingUsed ();

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const override;
    void                  printShort (ostream& os) const override;
    void                  printSummary (ostream& os) const;

    virtual void          findStringInAtom (
                            const string& lowerCaseString,
                            list<string>& foundStringsList,
                            ostream&      os) const;

    virtual void          printAtomWithVariableOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const;

  protected:

    // protected services
    // ------------------------------------------------------

  protected:

    // protected fields
    // ------------------------------------------------------

    // uplink
    S_oahSubGroup         fSubGroupUpLink;

    oahEarlyOptionKind    fEarlyOptionKind;
};
typedef SMARTP<oahAtom> S_oahAtom;
EXP ostream& operator<< (ostream& os, const S_oahAtom& elt);

//______________________________________________________________________________
class EXP oahAtomExpectingAValue : public oahAtom
/*
  a purely virtual common ancestor for all atom classes
  that take a value from argv or an oahOptionsVector
*/
{
  public:

    // creation
    // ------------------------------------------------------
/* this class is purely virtual
    static SMARTP<oahAtomExpectingAValue> create (
                            const string& longName,
                            const string& shortName,
                            const string& description);
*/
  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          oahAtomExpectingAValue (
                            const string& longName,
                            const string& shortName,
                            const string& description);

    virtual               ~oahAtomExpectingAValue ();

  public:

    // set and get
    // ------------------------------------------------------

  public:

    // public services
    // ------------------------------------------------------

    void                  applyElement (ostream& os) override;
                            // reports an error

    virtual void          applyAtomWithValue (
                            const string& theString,
                            ostream&      os) = 0;

    virtual void          applyAtomWithDefaultValue (ostream& os);
                            // used only if fElementValueKind
                            // is oahElementValueKind::kElementValueImplicit
                            // or oahElementValueKind::kElementValueOptional

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const override;
    void                  printShort (ostream& os) const override;

  protected:

    // protected fields
    // ------------------------------------------------------
};
typedef SMARTP<oahAtomExpectingAValue> S_oahAtomExpectingAValue;
EXP ostream& operator<< (ostream& os, const S_oahAtomExpectingAValue& elt);

//______________________________________________________________________________
class EXP oahAtomImplicitlyStoringAValue : public oahAtom
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
    static SMARTP<oahAtomImplicitlyStoringAValue> create (
                            const string& longName,
                            const string& shortName,
                            const string& description,
                            const string& variableName);
*/

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          oahAtomImplicitlyStoringAValue (
                            const string& longName,
                            const string& shortName,
                            const string& description,
                            const string& variableName);

    virtual               ~oahAtomImplicitlyStoringAValue ();

  public:

    // set and get
    // ------------------------------------------------------

    Bool                  getSetByUser () const
                              { return fSetByUser; }

  public:

    // public services
    // ------------------------------------------------------

    void                  applyElement (ostream& os) override;
                            // reports an error

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
                            ostream& os,
                            int fieldWidth) const;
    virtual void          printAtomWithVariableEssentialsShort (
                            ostream& os,
                            int fieldWidth) const;

    void                  print (ostream& os) const override;
    void                  printShort (ostream& os) const override;

    void                  printHelp (ostream& os) const override;

    virtual void          printAtomWithVariableOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const override;

  protected:

    // protected fields
    // ------------------------------------------------------

    string                fVariableName;
    Bool                  fSetByUser;
};
typedef SMARTP<oahAtomImplicitlyStoringAValue> S_oahAtomImplicitlyStoringAValue;
EXP ostream& operator<< (ostream& os, const S_oahAtomImplicitlyStoringAValue& elt);

//______________________________________________________________________________
class EXP oahAtomStoringAValue : public oahAtomExpectingAValue
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
                            const string& longName,
                            const string& shortName,
                            const string& description,
                            const string& valueSpecification,
                            const string& variableName);
*/

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          oahAtomStoringAValue (
                            const string& longName,
                            const string& shortName,
                            const string& description,
                            const string& valueSpecification,
                            const string& variableName);

    virtual               ~oahAtomStoringAValue ();

  public:

    // set and get
    // ------------------------------------------------------

    string                getValueSpecification () const
                              { return fValueSpecification; }

    Bool                  getSetByUser () const
                              { return fSetByUser; }

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
                            ostream& os,
                            int fieldWidth) const;
    virtual void          printAtomWithVariableEssentialsShort (
                            ostream& os,
                            int fieldWidth) const;

    void                  print (ostream& os) const override;
    void                  printShort (ostream& os) const override;

    void                  printHelp (ostream& os) const override;

    virtual void          printAtomWithVariableOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const override;

    void                  findStringInAtom (
                            const string& lowerCaseString,
                            list<string>& foundStringsList,
                            ostream&      os) const override;

    // protected fields
    // ------------------------------------------------------

    string                fValueSpecification;

    string                fVariableName;
    Bool                  fSetByUser;
};
typedef SMARTP<oahAtomStoringAValue> S_oahAtomStoringAValue;
EXP ostream& operator<< (ostream& os, const S_oahAtomStoringAValue& elt);

//______________________________________________________________________________
class EXP oahPureHelpAtomWithoutAValue : public oahAtom
{
  public:

    // creation
    // ------------------------------------------------------

/* this class is purely virtual
    static SMARTP<oahPureHelpAtomWithoutAValue> create (
                            const string& longName,
                            const string& shortName,
                            const string& description,
                            const string& serviceName);
*/

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          oahPureHelpAtomWithoutAValue (
                            const string& longName,
                            const string& shortName,
                            const string& description,
                            const string& serviceName);

    virtual               ~oahPureHelpAtomWithoutAValue ();

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

    void                  print (ostream& os) const override;

    void                  printAtomWithVariableOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const override;

  protected:

    // protected fields
    // ------------------------------------------------------

    string                fHelpAtomWithoutAValueServiceName;
};
typedef SMARTP<oahPureHelpAtomWithoutAValue> S_oahPureHelpAtomWithoutAValue;
EXP ostream& operator<< (ostream& os, const S_oahPureHelpAtomWithoutAValue& elt);

//______________________________________________________________________________
class EXP oahPureHelpAtomExpectingAValue : public oahAtomExpectingAValue
{
  public:

    // creation
    // ------------------------------------------------------

/* this class is purely virtual
    static SMARTP<oahPureHelpAtomExpectingAValue> create (
                            const string& longName,
                            const string& shortName,
                            const string& description,
                            const string& valueSpecification,
                            const string& serviceName);
*/

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          oahPureHelpAtomExpectingAValue (
                            const string& longName,
                            const string& shortName,
                            const string& description,
                            const string& valueSpecification,
                            const string& serviceName);

    virtual               ~oahPureHelpAtomExpectingAValue ();

  public:

    // set and get
    // ------------------------------------------------------

    string                getValueSpecification () const
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

    void                  print (ostream& os) const override;

    void                  printHelp (ostream& os) const override;

  protected:

    // protected fields
    // ------------------------------------------------------

    string                fHelpAtomExpectingAValueServiceName; // JMI ???

    string                fValueSpecification;
};
typedef SMARTP<oahPureHelpAtomExpectingAValue> S_oahPureHelpAtomExpectingAValue;
EXP ostream& operator<< (ostream& os, const S_oahPureHelpAtomExpectingAValue& elt);

//_______________________________________________________________________________
class EXP oahSubGroup : public oahElement
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahSubGroup> create (
                            const string&            subGroupHeader,
                            const string&            longName,
                            const string&            shortName,
                            const string&            description,
                            oahElementVisibilityKind optionVisibilityKind,
                            S_oahGroup               groupUpLink);

    static SMARTP<oahSubGroup> createWithNames (
                            const string&            subGroupHeader,
                            const string&            longName,
                            const string&            shortName,
                            const string&            description,
                            oahElementVisibilityKind optionVisibilityKind,
                            S_oahGroup               groupUpLink);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          oahSubGroup (
                            const string&            subGroupHeader,
                            const string&            longName,
                            const string&            shortName,
                            const string&            description,
                            oahElementVisibilityKind optionVisibilityKind,
                            S_oahGroup               groupUpLink);

    virtual               ~oahSubGroup ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setGroupUpLink (
                            S_oahGroup group)
                              { fGroupUpLink = group; }

    S_oahGroup            getGroupUpLink () const
                              { return fGroupUpLink; }

    string                getSubGroupHeader () const
                              { return fSubGroupHeader; }

    void                  incrementNumberOfUserChoseAtomsInThisSubGroup ()
                              { fNumberOfUserChoseAtomsInThisSubGroup += 1; }

    int                   getNumberOfUserChoseAtomsInThisSubGroup () const
                              { return fNumberOfUserChoseAtomsInThisSubGroup; }

    const list<S_oahAtom>&
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
    S_oahHandler          fetchSubGroupHandlerUpLink () const;

    void                  appendSubGroupToElementsList (
                            S_oahHandler handler);

    void                  registerNamesInSubGroupToTheNamesToElementsMap (
                            S_oahHandler handler);

    void                  appendAtomToSubGroup (
                            S_oahAtom oahAtom);

    S_oahElement          fetchOptionByNameInSubGroup (
                            const string& name);

    void                  applyElement (ostream& os) override;

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const override;
    void                  printShort (ostream& os) const override;
    void                  printSummary (ostream& os) const;

    void                  printHelp (ostream& os) const override;

    void                  printHelpWithHeaderWidth (
                            ostream& os,
                            int      subGroupHeaderWidth) const;

    void                  underlineSubGroupHeader (ostream& os) const;

    void                  printSubGroupHeader (ostream& os) const;
    void                  printSubGroupHeaderWithHeaderWidth (
                            ostream& os,
                            int      subGroupHeaderWidth) const;

    void                  printSubGroupHelp (
                            ostream& os) const;

    void                  printSubGroupAndAtomHelp (
                            ostream&  os,
                            S_oahAtom targetAtom) const;

    void                  printOptionsSummary (ostream& os) const;

    void                  printSubGroupSpecificHelpOrOptionsSummary (
                            ostream&      os,
                            S_oahSubGroup subGroup) const;

    void                  findStringInSubGroup (
                            const string& lowerCaseString,
                            list<string>& foundStringsList,
                            ostream&      os) const;

    void                  printSubGroupOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const;
    void                  printSubGroupOptionsValuesAll (
                            ostream& os,
                            int      valueFieldWidth) const;

  private:

    // private fields
    // ------------------------------------------------------

    S_oahGroup            fGroupUpLink;

    string                fSubGroupHeader;

    list<S_oahAtom>       fSubGroupAtomsList;

    // protected work fields
    // ------------------------------------------------------

    int                   fNumberOfUserChoseAtomsInThisSubGroup;
};
typedef SMARTP<oahSubGroup> S_oahSubGroup;
EXP ostream& operator<< (ostream& os, const S_oahSubGroup& elt);

//_______________________________________________________________________________
class EXP oahGroup : public oahElement
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahGroup> create (
                            const string&            header,
                            const string&            longName,
                            const string&            shortName,
                            const string&            description,
                            oahElementVisibilityKind optionVisibilityKind);

    static SMARTP<oahGroup> create (
                            const string&            header,
                            const string&            longName,
                            const string&            shortName,
                            const string&            description,
                            oahElementVisibilityKind optionVisibilityKind,
                            S_oahHandler             groupHandlerUpLink);

    static SMARTP<oahGroup> createWithNames (
                            const string&            header,
                            const string&            longName,
                            const string&            shortName,
                            const string&            description,
                            oahElementVisibilityKind optionVisibilityKind);

    static SMARTP<oahGroup> createWithNames (
                            const string&            header,
                            const string&            longName,
                            const string&            shortName,
                            const string&            description,
                            oahElementVisibilityKind optionVisibilityKind,
                            S_oahHandler             groupHandlerUpLink);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          oahGroup (
                            const string&            header,
                            const string&            longName,
                            const string&            shortName,
                            const string&            description,
                            oahElementVisibilityKind optionVisibilityKind);

                          oahGroup (
                            const string&            header,
                            const string&            longName,
                            const string&            shortName,
                            const string&            description,
                            oahElementVisibilityKind optionVisibilityKind,
                            S_oahHandler             groupHandlerUpLink);

    virtual               ~oahGroup ();

  public:

    // set and get
    // ------------------------------------------------------

    // uplink
    void                  setHandlerUpLink (
                            S_oahHandler handlerUpLink);

    S_oahHandler          getHandlerUpLink () const
                              { return fHandlerUpLink; }

    // header
    string                getGroupHeader () const
                              { return fGroupHeader; }

    // groups list
    const list<S_oahSubGroup>&
                          getGroupSubGroupsList () const
                              { return fGroupSubGroupsList; }

    // is the header to be written?
    void                  setGroupHeaderIsToBeWritten (Bool value)
                              { fGroupHeaderIsToBeWritten = value; }

    Bool                  getGroupHeaderIsToBeWritten () const
                              { return fGroupHeaderIsToBeWritten; }

    // number of chosen atoms
    void                  incrementNumberOfUserChoseAtomsInThisGroup ()
                              { fNumberOfUserChoseAtomsInThisGroup += 1; }

    // number of chosen atoms
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
                            S_oahHandler handler);

    void                  registerNamesInGroupToTheNamesToElementsMap (
                            S_oahHandler handler);

    void                  appendSubGroupToGroup (
                            S_oahSubGroup subGroup);

    S_oahElement          fetchOptionByNameInGroup (
                            const string& name);

    void                  applyElement (ostream& os) override;

    virtual void          handleAtomValue (
                            ostream&      os,
                            S_oahAtom     atom,
                            const string& theString);

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

    void                  print (ostream& os) const override;
    void                  printShort (ostream& os) const override;
    void                  printSummary (ostream& os) const;

    void                  printGroupHeader (ostream& os) const;

    void                  printHelp (ostream& os) const override;

    void                  underlineGroupHeader (ostream& os) const;

    void                  printGroupAndSubGroupHelp (
                            ostream&      os,
                            S_oahSubGroup targetSubGroup) const;

    void                  printGroupAndSubGroupAndAtomHelp (
                            ostream&      os,
                            S_oahSubGroup targetSubGroup,
                            S_oahAtom     targetAtom) const;

    void                  printOptionsSummary (ostream& os) const;

    void                  printGroupAndSubGroupSpecificHelp (
                            ostream&      os,
                            S_oahSubGroup subGroup) const;

    void                  findStringInGroup (
                            const string& lowerCaseString,
                            list<string>& foundStringsList,
                            ostream&      os) const;

    void                  printGroupOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const;
    void                  printGroupOptionsValuesAll (
                            ostream& os,
                            int      valueFieldWidth) const;

  private:

    // private services
    // ------------------------------------------------------

/* JMI STRANGE
    // prefixes
    string                prefixNamesInColumns (
                            int subGroupsShortNameFieldWidth) const;

    string                prefixNamesBetweenParentheses () const;
    string                prefixNamesInColumnsBetweenParentheses (
                            int subGroupsShortNameFieldWidth) const;

    string                operator () () const
                              { return fPrefixErsatz; }
*/

  protected:

    // protected fields
    // ------------------------------------------------------

    // uplink
    S_oahHandler          fHandlerUpLink;

    string                fGroupHeader;

    list<S_oahSubGroup>   fGroupSubGroupsList;

    Bool                  fGroupHeaderIsToBeWritten;

  protected:

    // protected work fields
    // ------------------------------------------------------

    int                   fNumberOfUserChoseAtomsInThisGroup;
};
typedef SMARTP<oahGroup> S_oahGroup;
EXP ostream& operator<< (ostream& os, const S_oahGroup& elt);

//_______________________________________________________________________________
class EXP oahHandler : public smartable
{
  public:

    // creation
    // ------------------------------------------------------

 /* this class is purely virtual
    static SMARTP<oahHandler> create (
                            const string&           serviceName,
                            const string&           handlerHeader,
                            const string&           handlerDescription,
                            const string&           handlerUsage);
*/

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          oahHandler (
                            const string&           serviceName,
                            const string&           handlerHeader,
                            const string&           handlerDescription,
                            const string&           handlerUsage);

    virtual               ~oahHandler ();

  protected:

    // protected initialization
    // ------------------------------------------------------

    virtual void          initializeHandlerMultiComponent () = 0;

  public:

    // set and get
    // ------------------------------------------------------

    // OAH handler
    static S_oahHandler   getCurrentOahHandler ()
                              { return pCurrentOahHandler; }

  public:

    // set and get
    // ------------------------------------------------------

    string                getHandlerServiceName () const
                              { return fHandlerServiceName; }

    void                  setHandlerMultiComponent (
                            S_mfcMultiComponent multiComponent)
                              { fHandlerMultiComponent = multiComponent; }

    S_mfcMultiComponent   getHandlerMultiComponent () const
                              { return fHandlerMultiComponent; }

    string                getHandlerHeader () const
                              { return fHandlerHeader; }

    string                getHandlerDescription () const
                              { return fHandlerDescription; }

    string                getHandlerUsage () const
                              { return fHandlerUsage; }

    // launch command
    string                getLaunchCommandAsSupplied () const
                              { return fLaunchCommandAsSupplied; }

    string                getLaunchCommandWithLongOptionsNames () const
                              { return fLaunchCommandWithLongOptionsNames; }
    string                getLaunchCommandWithShortOptionsNames () const
                              { return fLaunchCommandWithShortOptionsNames; }

    // prefixes
    const map<string, S_oahPrefix>&
                          getHandlerPrefixesMap () const
                              { return fHandlerPrefixesMap; }

    // groups lists
    const list<S_oahGroup>&
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
    const list<S_oahElement>&
                          getHandlerElementsList () const
                              { return fHandlerElementsList; }

    const list<S_oahElement>&
                          getHandlerCommandLineElementsList () const
                              { return fHandlerCommandLineElementsList; }

    const multiset<S_oahElement>&
                          getHandlerCommandLineElementsMultiset () const
                              { return fHandlerCommandLineElementsMultiset; }

    // widths and sizes
    int                   getMaximumShortNameWidth () const
                              { return fMaximumShortNameWidth; }

    int                   getMaximumLongNameWidth () const
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
                            string name,
                            string context);

    void                  unknownOptionNameError (
                            string name,
                            string context);

    virtual string        handlerServiceAboutInformation () const = 0;

    S_oahElement          fetchElementByNameInHandler (
                            const string& name);

    void                  registerPrefixInHandler (
                            S_oahPrefix prefix);

    void                  appendGroupToHandler (
                            S_oahGroup group);
    void                  prependGroupToHandler (
                            S_oahGroup group);

    void                  appendElementToElementsList (
                            S_oahElement element);

    S_oahPrefix           fetchNameInPrefixesMap (
                            const string& prefixName) const;

    S_oahElement          fetchNameInNamesToElementsMap (
                            const string& name) const;

    void                  registerElementNamesInHandler (
                            S_oahElement element);

    // options and arguments handling
    oahElementHelpOnlyKind
                          handleOptionsFromOptionsAndArguments (
                            const string& serviceName);

    oahElementHelpOnlyKind
                          handleOptionsAndArgumentsFromArgcArgv (
                            int   argc,
                            char* argv[]);

    virtual void          checkOptionsAndArgumentsConsistency ();

    virtual void          checkOptionsAndArguments () const = 0;

    void                  setOahHandlerFoundAHelpOption (
                            const string& context);

    virtual string        fetchOutputFileNameFromTheOptions () const;
                            // JMI the MFSL interpreter has no output file... v0.9.62

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
                            ostream& os,
                            int fieldWidth) const;

    void                  printKnownPrefixes (ostream& os) const;
    void                  printKnownSingleCharacterOptions (ostream& os) const;
//     void                  printOptionsDefaultValuesInformation (ostream& os) const; JMI

    void                  displayNamesToElementsMap (ostream& os) const;
    void                  displayNamesToElementsMapShort (ostream& os) const;

    void                  displayNamesToElementsMap ();

    void                  displayElementUsesList (ostream& os) const;

    void                  displayOptionsAndArguments (ostream& os) const;

//     void                  displayArgumentsVector (ostream& os) const;

    void                  print (ostream& os) const;
    void                  printShort (ostream& os) const;
    void                  printSummary (ostream& os) const;

    void                  printHelp (ostream& os) const;

    void                  printOptionsSummary (ostream& os) const;
    void                  printOptionsSummary () const
                              { printOptionsSummary (gLogStream); }

    void                  printHandlerAndGroupAndSubGroupSpecificHelp (
                            ostream&      os,
                            S_oahSubGroup subGroup) const;

    void                  printNameIntrospectiveHelp (
                            ostream&      os,
                            const string& name);

    void                  findStringInHandler (
                            const string& lowerCaseString,
                            list<string>& foundStringsList,
                            ostream&      os) const;

    void                  includeOptionsFileInHandler (
                            const string& optionsFileName,
                            ostream&      os);

    static int            getIncludeFilesMaximumLevel ()
                              { return pIncludeFilesMaximumLevel; }

    void                  printHandlerOptionsAndArguments (
                            ostream& os) const;

    void                  printHandlerOptionsValues (
                            ostream& os) const;
    void                  printHandlerOptionsValuesAll (
                            ostream& os) const;

    void                  printIncludeFileNamesStack (
                            ostream& os) const;

  protected:

    // protected services
    // ------------------------------------------------------

    void                  createTheCommonPrefixes ();

    void                  checkNoInputSourceInArgumentsVector () const;

    void                  checkNoOrOneInputSourceInArgumentsVector () const;

    void                  checkSingleInputSourceInArgumentsVector () const;

  private:

    // private services
    // ------------------------------------------------------

    // initialization
    void                  initializeHandler ();

    // names handling
    S_oahPrefix           fetchNameInHandlerPrefixesMap (
                            const string& name) const;

    void                  registerNamesInHandlerToTheNamesToElementsMap ();

    void                  registerOahElementUse (
                            S_oahElement  elementUsed,
                            const string& nameUsed,
                            const string& valueUsed);

    void                  registerGroupUse (
                            S_oahGroup    groupUsed,
                            const string& nameUsed,
                            const string& valueUsed)
                              {
                                registerOahElementUse (
                                  groupUsed,
                                  nameUsed,
                                  valueUsed);
                              }

    void                  registerSubGroupUse (
                            S_oahSubGroup subGroupUsed,
                            const string& nameUsed,
                            const string& valueUsed)
                              {
                                registerOahElementUse (
                                  subGroupUsed,
                                  nameUsed,
                                  valueUsed);
                              }

    void                  registerAtomUse (
                            S_oahAtom     atomUsed,
                            const string& nameUsed,
                            const string& valueUsed);

    void                  registerOahElementUseInLaunchCommand (
                            S_oahElement  elementUsed,
                            const string& nameUsed,
                            const string& valueUsed);

    void                  registerArgumentUseInLaunchCommand (
                            const string& argumentUsed);

    void                  handleOptionPrefixName (
                            const string& prefixName,
                            size_t        equalsSignPosition,
                            const string& stringAfterEqualsSign);

    Bool                  isNameASingleCharacterOptionsCluster (
                            const string& optionName);

    void                  handleUnknownOptionName (
                            const string& optionName);

    // options and arguments handling
    // pass 1
    void                  createElementUsesListFromOptionsAndArguments (
                            const string& serviceName);

    void                  createElementUsesListFromArgcArgv (
                            int   argc,
                            char* argv[]);

    // pass 2
    oahElementHelpOnlyKind
                          applyOptionsFromElementUsesList ();

    // oahOptionsVector elements handling
    void                  handleOptionNameAndValueAtTopOahLevel (
                            const string& optionName,
                            const string& optionValue);

    void                  handleOptionsVectorOptionName (
                            const string& theString,
                            const string& valueUsed);

    void                  handleKnownOptionsVectorElement (
                            S_oahElement  element,
                            const string& optionNameUsed,
                            const string& valueUsed);

    void                  handleKnownOptionsVectorAtom (
                            S_oahAtom     atom,
                            const string& optionNameUsed,
                            const string& valueUsed);

    // argc/argv handling JMIJMIJMI
    void                  handleArgvOptionValueOrArgument (
                            const string& theString);

    string                handleArgvOptionName (
                            const string& optionName);

    void                  handleKnownArgvAtom (
                            S_oahAtom     atom,
                            const string& optionNameUsed);

    void                  handleKnownArgvElement (
                            S_oahElement  element,
                            const string& optionNameUsed);

    void                  checkMissingPendingArgvAtomExpectingAValueValue ( // JMIJMIJMI
                            const string& atomName,
                            const string& context);

    // common methods
    void                  handleNameContainingEqualSign (
                            const string& optionName,
                            size_t        equalsSignPosition);

    void                  handleOptionNameCommon (
                            const string& optionName);

    // options concistency
    void                  checkHandlerOwnOptionsConsistency ();

  protected:

    // protected fields
    // ------------------------------------------------------

    // current OAH handler
    static S_oahHandler   pCurrentOahHandler;

    // service
    string                fHandlerServiceName;

    // multi-components
    S_mfcMultiComponent   fHandlerMultiComponent;

    // header
    string                fHandlerHeader;

    // description
    string                fHandlerDescription;

    // usage
    string                fHandlerUsage;

    // prefixes map
    map<string, S_oahPrefix>
                          fHandlerPrefixesMap;

    // single character short names
    set<string>           fSingleCharacterShortNamesSet;

    // groups list
    list<S_oahGroup>      fHandlerGroupsList;

    // elements map
    map<string, S_oahElement>
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


    void                  includeOptionsAndArgumentsFile ( // JMI ???
                            const string&           includeFileName,
                            ostream&                os);

    void                  includeOptionsFromFile (
                            istream&                inputStream,
                            oahOptionsAndArguments& optionsAndArguments);

    void                  analyzeOptionsAndArguments (
                            string                        serviceName,
                            const oahOptionsAndArguments& optionsAndArguments);

    int                   getMaximumVariableNameWidth () const
                              { return fMaximumVariableNameWidth; }

  private:

    // work fields
    // ------------------------------------------------------

    // all OAH elements are registered in the handler upon initialization
    list<S_oahElement>    fHandlerElementsList;

    // those ones have be used in the command line
    list<S_oahElement>    fHandlerCommandLineElementsList;
    multiset<S_oahElement>
                          fHandlerCommandLineElementsMultiset;

    // launch command
    string                fLaunchCommandAsSupplied;

    string                fLaunchCommandWithLongOptionsNames;
    string                fLaunchCommandWithShortOptionsNames;

    Bool                  fLaunchOptionsAndOrArgumentsAlreadyMet;

//     Bool                  fNowEverythingIsAnArgument; // JMI ???

    // sizes and widths
    int                   fMaximumSubGroupsHeadersSize;

    int                   fMaximumShortNameWidth;
    int                   fMaximumLongNameWidth;

    int                   fMaximumVariableNameWidth;

    // elements uses
    list<S_oahElementUse> fElementUsesList;

    // options and arguments
    oahOptionsAndArguments
                          fOptionsAndArguments;

    // atoms waiting for a value
    S_oahAtomExpectingAValue
                          fPendingArgvAtomExpectingAValue;
    string                fNameUsedForPendingArgvAtomExpectingAValue;

    list <string>         fIncludeFileNamesStack;
    int                   fIncludeFilesCurrentLevel;
    static int            pIncludeFilesMaximumLevel;
};
typedef SMARTP<oahHandler> S_oahHandler;
EXP ostream& operator<< (ostream& os, const S_oahHandler& elt);

//______________________________________________________________________________
extern EXP S_oahHandler getCurrentOahHandler (); // UNUSED JMI


}


#endif

