/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___oahAtomsCollection___
#define ___oahAtomsCollection___

#include "list"
#include "map"
#include "set"

#include "rational.h"

#include "msrBasicTypes.h"
#include "msrMidi.h"

#include "oahBasicTypes.h"


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
class EXP oahAtomAlias : public oahAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahAtomAlias> create (
                            const string& longName,
                            const string& shortName,
                            const string& description,
                            S_oahAtom     originalOahAtom);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          oahAtomAlias (
                            const string& longName,
                            const string& shortName,
                            const string& description,
                            S_oahAtom     originalOahAtom);

    virtual               ~oahAtomAlias ();

  public:

    // set and get
    // ------------------------------------------------------

     S_oahAtom            getOriginalOahAtom () const
                              { return fOriginalOahAtom; }

  public:

    // public services
    // ------------------------------------------------------

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

    void                  printAtomWithVariableOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const override;

  private:

    // private fields
    // ------------------------------------------------------

     S_oahAtom            fOriginalOahAtom;
};
typedef SMARTP<oahAtomAlias> S_oahAtomAlias;
EXP ostream& operator<< (ostream& os, const S_oahAtomAlias& elt);

//______________________________________________________________________________
class EXP oahMacroAtom : public oahAtom
{
/*
  a list of atoms
*/

  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahMacroAtom> create (
                            const string& longName,
                            const string& shortName,
                            const string& description);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          oahMacroAtom (
                            const string& longName,
                            const string& shortName,
                            const string& description);

    virtual               ~oahMacroAtom ();

  public:

    // set and get
    // ------------------------------------------------------

     const list<S_oahAtom>&
                          getMacroAtomsList () const
                              { return fMacroAtomsList; }

  public:

    // public services
    // ------------------------------------------------------

    void                  appendAtomToMacro (S_oahAtom atom);

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

    void                  printHelp (ostream& os) const override;

    void                  printAtomWithVariableOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const override;
  private:

    // private fields
    // ------------------------------------------------------

     list<S_oahAtom>      fMacroAtomsList;
};
typedef SMARTP<oahMacroAtom> S_oahMacroAtom;
EXP ostream& operator<< (ostream& os, const S_oahMacroAtom& elt);

//______________________________________________________________________________
class EXP oahOptionsUsageAtom : public oahPureHelpAtomWithoutAValue
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahOptionsUsageAtom> create (
                            const string& longName,
                            const string& shortName,
                            const string& description,
                            const string& serviceName);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          oahOptionsUsageAtom (
                            const string& longName,
                            const string& shortName,
                            const string& description,
                            const string& serviceName);

    virtual               ~oahOptionsUsageAtom ();

  public:

    // public services
    // ------------------------------------------------------

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

    void                  printOptionsUsage (ostream& os) const;

    void                  print (ostream& os) const override;
};
typedef SMARTP<oahOptionsUsageAtom> S_oahOptionsUsageAtom;
EXP ostream& operator<< (ostream& os, const S_oahOptionsUsageAtom& elt);

//______________________________________________________________________________
class EXP oahHelpAtom : public oahPureHelpAtomWithoutAValue
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahHelpAtom> create (
                            const string& longName,
                            const string& shortName,
                            const string& description,
                            const string& serviceName);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          oahHelpAtom (
                            const string& longName,
                            const string& shortName,
                            const string& description,
                            const string& serviceName);

    virtual               ~oahHelpAtom ();

  public:

    // public services
    // ------------------------------------------------------

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

    void                  printOptionsSummary (ostream& os) const;
};
typedef SMARTP<oahHelpAtom> S_oahHelpAtom;
EXP ostream& operator<< (ostream& os, const S_oahHelpAtom& elt);

//______________________________________________________________________________
class EXP oahHelpSummaryAtom : public oahPureHelpAtomWithoutAValue
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahHelpSummaryAtom> create (
                            const string& longName,
                            const string& shortName,
                            const string& description,
                            const string& serviceName);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          oahHelpSummaryAtom (
                            const string& longName,
                            const string& shortName,
                            const string& description,
                            const string& serviceName);

    virtual               ~oahHelpSummaryAtom ();

  public:

    // public services
    // ------------------------------------------------------

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

    void                  printOptionsSummary (ostream& os) const;
};
typedef SMARTP<oahHelpSummaryAtom> S_oahHelpSummaryAtom;
EXP ostream& operator<< (ostream& os, const S_oahHelpSummaryAtom& elt);

//______________________________________________________________________________
class EXP oahAboutAtom : public oahPureHelpAtomWithoutAValue
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahAboutAtom> create (
                            const string& longName,
                            const string& shortName,
                            const string& description,
                            const string& serviceName);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          oahAboutAtom (
                            const string& longName,
                            const string& shortName,
                            const string& description,
                            const string& serviceName);

    virtual               ~oahAboutAtom ();

  public:

    // public services
    // ------------------------------------------------------

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

    void                  printAbout (ostream& os) const;
};
typedef SMARTP<oahAboutAtom> S_oahAboutAtom;
EXP ostream& operator<< (ostream& os, const S_oahAboutAtom& elt);

//______________________________________________________________________________
enum class oahVersionKind {
  kOahVersionShort, kOahVersionFull
};

string oahVersionKindAsString (
  oahVersionKind versionKind);

ostream& operator<< (ostream& os, oahVersionKind& elt);

//______________________________________________________________________________
class EXP oahVersionAtom : public oahPureHelpAtomWithoutAValue
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahVersionAtom> create (
                            const string&  longName,
                            const string&  shortName,
                            const string&  description,
                            const string&  serviceName,
                            oahVersionKind versionKind);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          oahVersionAtom (
                            const string&  longName,
                            const string&  shortName,
                            const string&  description,
                            const string&  serviceName,
                            oahVersionKind versionKind);

    virtual               ~oahVersionAtom ();

  public:

    // public services
    // ------------------------------------------------------

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

    void                  printVersionShort (ostream& os) const;
    void                  printVersionFull (ostream& os) const;

    oahVersionKind        fVersionKind;
};
typedef SMARTP<oahVersionAtom> S_oahVersionAtom;
EXP ostream& operator<< (ostream& os, const S_oahVersionAtom& elt);

//______________________________________________________________________________
class EXP oahLibraryVersionAtom : public oahPureHelpAtomWithoutAValue
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahLibraryVersionAtom> create (
                            const string& longName,
                            const string& shortName,
                            const string& description,
                            const string& serviceName);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          oahLibraryVersionAtom (
                            const string& longName,
                            const string& shortName,
                            const string& description,
                            const string& serviceName);

    virtual               ~oahLibraryVersionAtom ();

  public:

    // public services
    // ------------------------------------------------------

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

    void                  printVersion (ostream& os) const;
};
typedef SMARTP<oahLibraryVersionAtom> S_oahLibraryVersionAtom;
EXP ostream& operator<< (ostream& os, const S_oahLibraryVersionAtom& elt);

//______________________________________________________________________________
class EXP oahHistoryAtom : public oahPureHelpAtomWithoutAValue
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahHistoryAtom> create (
                            const string& longName,
                            const string& shortName,
                            const string& description,
                            const string& serviceName);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          oahHistoryAtom (
                            const string& longName,
                            const string& shortName,
                            const string& description,
                            const string& serviceName);

    virtual               ~oahHistoryAtom ();

  public:

    // public services
    // ------------------------------------------------------

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

    void                  printHistory (ostream& os) const;
};
typedef SMARTP<oahHistoryAtom> S_oahHistoryAtom;
EXP ostream& operator<< (ostream& os, const S_oahHistoryAtom& elt);

//______________________________________________________________________________
class EXP oahLibraryHistoryAtom : public oahPureHelpAtomWithoutAValue
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahLibraryHistoryAtom> create (
                            const string& longName,
                            const string& shortName,
                            const string& description,
                            const string& serviceName);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          oahLibraryHistoryAtom (
                            const string& longName,
                            const string& shortName,
                            const string& description,
                            const string& serviceName);

    virtual               ~oahLibraryHistoryAtom ();

  public:

    // public services
    // ------------------------------------------------------

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

    void                  printHistory (ostream& os) const;
};
typedef SMARTP<oahLibraryHistoryAtom> S_oahLibraryHistoryAtom;
EXP ostream& operator<< (ostream& os, const S_oahLibraryHistoryAtom& elt);

//______________________________________________________________________________
class EXP oahContactAtom : public oahPureHelpAtomWithoutAValue
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahContactAtom> create (
                            const string& longName,
                            const string& shortName,
                            const string& description,
                            const string& serviceName);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          oahContactAtom (
                            const string& longName,
                            const string& shortName,
                            const string& description,
                            const string& serviceName);


    virtual               ~oahContactAtom ();

  public:

    // public services
    // ------------------------------------------------------

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

    void                  printContact (ostream& os) const;
};
typedef SMARTP<oahContactAtom> S_oahContactAtom;
EXP ostream& operator<< (ostream& os, const S_oahContactAtom& elt);

//______________________________________________________________________________
class EXP oahDisplayPrefixes : public oahPureHelpAtomWithoutAValue
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahDisplayPrefixes> create (
                            const string& longName,
                            const string& shortName,
                            const string& description,
                            const string& serviceName);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          oahDisplayPrefixes (
                            const string& longName,
                            const string& shortName,
                            const string& description,
                            const string& serviceName);


    virtual               ~oahDisplayPrefixes ();

  public:

    // public services
    // ------------------------------------------------------

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

    void                  printPrefixes (ostream& os) const;
};
typedef SMARTP<oahDisplayPrefixes> S_oahDisplayPrefixes;
EXP ostream& operator<< (ostream& os, const S_oahDisplayPrefixes& elt);

//______________________________________________________________________________
class EXP oahDisplaySingleCharacterOptions : public oahPureHelpAtomWithoutAValue
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahDisplaySingleCharacterOptions> create (
                            const string& longName,
                            const string& shortName,
                            const string& description,
                            const string& serviceName);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          oahDisplaySingleCharacterOptions (
                            const string& longName,
                            const string& shortName,
                            const string& description,
                            const string& serviceName);


    virtual               ~oahDisplaySingleCharacterOptions ();

  public:

    // public services
    // ------------------------------------------------------

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

    void                  printSingleCharacterOptions (ostream& os) const;
};
typedef SMARTP<oahDisplaySingleCharacterOptions> S_oahDisplaySingleCharacterOptions;
EXP ostream& operator<< (ostream& os, const S_oahDisplaySingleCharacterOptions& elt);

//______________________________________________________________________________
class EXP oahBooleanAtom : public oahAtom
{
/*
  an atom controlling a boolean variableName,
  but expecting no value to be supplied:
  the variable is false initially,
  and is set to true by the mere occurrence of the atom
*/

  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahBooleanAtom> create (
                            const string& longName,
                            const string& shortName,
                            const string& description,
                            const string& variableName,
                            Bool&         booleanVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          oahBooleanAtom (
                            const string& longName,
                            const string& shortName,
                            const string& description,
                            const string& variableName,
                            Bool&         booleanVariable);

    virtual               ~oahBooleanAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setBooleanVariable (Bool value);

    Bool                  getBooleanVariable () const
                              { return fBooleanVariable; }

    Bool                  getSetByUser () const
                              { return fSetByUser; }

  public:

    // public services
    // ------------------------------------------------------

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

    void                  printAtomWithVariableOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const override;

  protected:

    // protected fields
    // ------------------------------------------------------

    string                fVariableName;
    Bool&                 fBooleanVariable;
    Bool                  fSetByUser;
};
typedef SMARTP<oahBooleanAtom> S_oahBooleanAtom;
EXP ostream& operator<< (ostream& os, const S_oahBooleanAtom& elt);

//______________________________________________________________________________
class EXP oahTwoBooleansAtom : public oahBooleanAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahTwoBooleansAtom> create (
                            const string&    longName,
                            const string&    shortName,
                            const string&    description,
                            const string&    variableName,
                            Bool&            booleanVariable,
                            S_oahBooleanAtom secondBooleanAtom);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          oahTwoBooleansAtom (
                            const string&    longName,
                            const string&    shortName,
                            const string&    description,
                            const string&    variableName,
                            Bool&            booleanVariable,
                            S_oahBooleanAtom secondBooleanAtom);

    virtual               ~oahTwoBooleansAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setTwoBooleansVariables (Bool value);

  public:

    // public services
    // ------------------------------------------------------

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

    void                  printAtomWithVariableOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const override;

  private:

    // private fields
    // ------------------------------------------------------

    S_oahBooleanAtom      fSecondBooleanAtom;
};
typedef SMARTP<oahTwoBooleansAtom> S_oahTwoBooleansAtom;
EXP ostream& operator<< (ostream& os, const S_oahTwoBooleansAtom& elt);

//______________________________________________________________________________
class EXP oahThreeBooleansAtom : public oahBooleanAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahThreeBooleansAtom> create (
                            const string&    longName,
                            const string&    shortName,
                            const string&    description,
                            const string&    variableName,
                            Bool&            booleanVariable,
                            S_oahBooleanAtom secondBooleanAtom,
                            S_oahBooleanAtom thirdBooleanAtom);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          oahThreeBooleansAtom (
                            const string&    longName,
                            const string&    shortName,
                            const string&    description,
                            const string&    variableName,
                            Bool&            booleanVariable,
                            S_oahBooleanAtom secondBooleanAtom,
                            S_oahBooleanAtom thirdBooleanAtom);

    virtual               ~oahThreeBooleansAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setThreeBooleansVariables (Bool value);

  public:

    // public services
    // ------------------------------------------------------

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

    void                  printAtomWithVariableOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const override;

  private:

    // private fields
    // ------------------------------------------------------

    S_oahBooleanAtom      fSecondBooleanAtom;
    S_oahBooleanAtom      fThirdBooleanAtom;
};
typedef SMARTP<oahThreeBooleansAtom> S_oahThreeBooleansAtom;
EXP ostream& operator<< (ostream& os, const S_oahThreeBooleansAtom& elt);

//______________________________________________________________________________
class EXP oahCombinedBooleansAtom : public oahAtom
{
/*
  an atom controlling a list of boolean atoms as in 'xml2ly -query minimal'
  without any controlled boolean variable of its own
*/

  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahCombinedBooleansAtom> create (
                            const string& longName,
                            const string& shortName,
                            const string& description);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          oahCombinedBooleansAtom (
                            const string& longName,
                            const string& shortName,
                            const string& description);

    virtual               ~oahCombinedBooleansAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    const list<S_oahBooleanAtom>&
                          getBooleanAtomsList () const
                              { return fBooleanAtomsList; }

  public:

    // public services
    // ------------------------------------------------------

    void                  addBooleanAtom (
                            S_oahBooleanAtom booleanAtom);

    void                  addBooleanAtomByName (
                            const string& name);

    void                  setCombinedBooleanVariables (Bool value);

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

    virtual void          printCombinedBooleansEssentials (
                            ostream& os,
                            int fieldWidth) const;
    virtual void          printCombinedBooleansEssentialsShort (
                            ostream& os,
                            int fieldWidth) const;

    void                  print (ostream& os) const override;

    void                  printHelp (ostream& os) const override;

    void                  printAtomWithVariableOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const override;

  private:

    // private fields
    // ------------------------------------------------------

    list<S_oahBooleanAtom>
                          fBooleanAtomsList;
};
typedef SMARTP<oahCombinedBooleansAtom> S_oahCombinedBooleansAtom;
EXP ostream& operator<< (ostream& os, const S_oahCombinedBooleansAtom& elt);

//______________________________________________________________________________
class EXP oahCommonPrefixBooleansAtom : public oahAtom
{
/*
  an atom controlling a list of boolean atoms
  whose names share a common prefix, as in 'xml2ly -insider -query ticat'
  without any controlled boolean variable of its own
*/

  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahCommonPrefixBooleansAtom> create (
                            const string& longName,
                            const string& shortName,
                            const string& description,
                            const string& shortSuffixDescriptor,
                            const string& longSuffixDescriptor,
                            S_oahPrefix   shortNamesPrefix,
                            S_oahPrefix   longNamesPrefix);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          oahCommonPrefixBooleansAtom (
                            const string& longName,
                            const string& shortName,
                            const string& description,
                            const string& shortSuffixDescriptor,
                            const string& longSuffixDescriptor,
                            S_oahPrefix   shortNamesPrefix,
                            S_oahPrefix   longNamesPrefix);

    virtual               ~oahCommonPrefixBooleansAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    const list<S_oahBooleanAtom>&
                          getBooleanAtomsList () const
                              { return fBooleanAtomsList; }

  public:

    // public services
    // ------------------------------------------------------

    void                  addBooleanAtom (
                            S_oahBooleanAtom booleanAtom);

    void                  addBooleanAtomByName (
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

    void                  printHelp (ostream& os) const override;

    void                  printAtomWithVariableOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const override;

  private:

    // private fields
    // ------------------------------------------------------

    S_oahPrefix           fShortNamesPrefix;
    S_oahPrefix           fLongNamesPrefix;

    string                fShortSuffixDescriptor;
    string                fLongSuffixDescriptor;

    list<S_oahBooleanAtom>
                          fBooleanAtomsList;

    string                fShortNamesPrefixName;
    string                fLongNamesPrefixName;

    list<string>          fShortNamesSuffixes;
    list<string>          fLongNamesSuffixes;
};
typedef SMARTP<oahCommonPrefixBooleansAtom> S_oahCommonPrefixBooleansAtom;
EXP ostream& operator<< (ostream& os, const S_oahCommonPrefixBooleansAtom& elt);

//______________________________________________________________________________
class EXP oahIntegerAtom : public oahAtomStoringAValue
{
/*
  an atom controlling an integer variable
*/

  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahIntegerAtom> create (
                            const string& longName,
                            const string& shortName,
                            const string& description,
                            const string& valueSpecification,
                            const string& variableName,
                            int&          integerVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          oahIntegerAtom (
                            const string& longName,
                            const string& shortName,
                            const string& description,
                            const string& valueSpecification,
                            const string& variableName,
                            int&          integerVariable);

  protected:


    virtual               ~oahIntegerAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setIntegerVariable (int value);

    int                   getIntegerVariable () const
                              { return fIntegerVariable; }

  public:

    // public services
    // ------------------------------------------------------

    void                  applyAtomWithValue (
                            const string& theString,
                            ostream&      os) override;

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    string                asShortNamedOptionString () const override;
    string                asActualLongNamedOptionString () const override;

    void                  print (ostream& os) const override;

    void                  printAtomWithVariableOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const override;
  protected:

    // protected fields
    // ------------------------------------------------------

    int&                  fIntegerVariable;
};
typedef SMARTP<oahIntegerAtom> S_oahIntegerAtom;
EXP ostream& operator<< (ostream& os, const S_oahIntegerAtom& elt);

//______________________________________________________________________________
class EXP oahTwoIntegersAtom : public oahIntegerAtom
{
/*
  an atom controlling two integer variables
*/

  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahTwoIntegersAtom> create (
                            const string& longName,
                            const string& shortName,
                            const string& description,
                            const string& valueSpecification,
                            const string& variableName,
                            int&          integerVariable,
                            int&          integerSecondaryVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          oahTwoIntegersAtom (
                            const string& longName,
                            const string& shortName,
                            const string& description,
                            const string& valueSpecification,
                            const string& variableName,
                            int&          integerVariable,
                            int&          integerSecondaryVariable);

  protected:


    virtual               ~oahTwoIntegersAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setIntegerVariable (int value);

  public:

    // public services
    // ------------------------------------------------------

    void                  applyAtomWithValue (
                            const string& theString,
                            ostream&      os) override;

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    string                asShortNamedOptionString () const override;
    string                asActualLongNamedOptionString () const override;

    void                  print (ostream& os) const override;

    void                  printAtomWithVariableOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const override;
  private:

    // private fields
    // ------------------------------------------------------

    int&                  fIntegerSecondaryVariable;
};
typedef SMARTP<oahTwoIntegersAtom> S_oahTwoIntegersAtom;
EXP ostream& operator<< (ostream& os, const S_oahTwoIntegersAtom& elt);

//______________________________________________________________________________
class EXP oahFloatAtom : public oahAtomStoringAValue
{
/*
  an atom controlling a float variable
*/

  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahFloatAtom> create (
                            const string& longName,
                            const string& shortName,
                            const string& description,
                            const string& valueSpecification,
                            const string& variableName,
                            float&        floatVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          oahFloatAtom (
                            const string& longName,
                            const string& shortName,
                            const string& description,
                            const string& valueSpecification,
                            const string& variableName,
                            float&        floatVariable);

    virtual               ~oahFloatAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setFloatVariable (float value);

    float                 getFloatVariable () const
                              { return fFloatVariable; }

  public:

    // public services
    // ------------------------------------------------------

    void                  applyAtomWithValue (
                            const string& theString,
                            ostream&      os) override;

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    string                asShortNamedOptionString () const override;
    string                asActualLongNamedOptionString () const override;

    void                  print (ostream& os) const override;

    void                  printAtomWithVariableOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const override;

  private:

    // private fields
    // ------------------------------------------------------

    float&                fFloatVariable;
};
typedef SMARTP<oahFloatAtom> S_oahFloatAtom;
EXP ostream& operator<< (ostream& os, const S_oahFloatAtom& elt);

//______________________________________________________________________________
class EXP oahStringAtom : public oahAtomStoringAValue
{
/*
  an atom controlling a string variable
*/

  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahStringAtom> create (
                            const string& longName,
                            const string& shortName,
                            const string& description,
                            const string& valueSpecification,
                            const string& variableName,
                            string&       stringVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          oahStringAtom (
                            const string& longName,
                            const string& shortName,
                            const string& description,
                            const string& valueSpecification,
                            const string& variableName,
                            string&       stringVariable);

    virtual               ~oahStringAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setStringVariable (const string& value);

    string                getStringVariable () const
                              { return fStringVariable; }

  public:

    // public services
    // ------------------------------------------------------

    void                  applyAtomWithValue (
                            const string& theString,
                            ostream&      os) override;

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    string                asShortNamedOptionString () const override;
    string                asActualLongNamedOptionString () const override;

    void                  print (ostream& os) const override;

    void                  printAtomWithVariableOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const override;

  protected:

    // protected fields
    // ------------------------------------------------------

    string&               fStringVariable;
};
typedef SMARTP<oahStringAtom> S_oahStringAtom;
EXP ostream& operator<< (ostream& os, const S_oahStringAtom& elt);

//______________________________________________________________________________
class EXP oahFactorizedStringAtom : public oahAtom // JMI UNUSED !!!
{
/*
  an atom factorizing a list of string atoms
  such as -set-variable-in-header in LilyPond generation
*/

  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahFactorizedStringAtom> create (
                            const string& longName,
                            const string& shortName,
                            const string& description,
                            const string& atomNameDescriptor,
                            const string& stringValueDescriptor);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          oahFactorizedStringAtom (
                            const string& longName,
                            const string& shortName,
                            const string& description,
                            const string& atomNameDescriptor,
                            const string& stringValueDescriptor);

    virtual               ~oahFactorizedStringAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    const list<S_oahStringAtom>&
                          getStringAtomsList () const
                              { return fStringAtomsList; }

  public:

    // public services
    // ------------------------------------------------------

    void                  addStringAtom (
                            S_oahStringAtom stringAtom);

    void                  addStringAtomByName (
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

    void                  printHelp (ostream& os) const override;

    void                  printAtomWithVariableOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const override;

  private:

    // private fields
    // ------------------------------------------------------

    string                fAtomNameDescriptor;
    string                fStringValueDescriptor;

    list<S_oahStringAtom>
                          fStringAtomsList;

    list<string>          fAtomNamesList;
};
typedef SMARTP<oahFactorizedStringAtom> S_oahFactorizedStringAtom;
EXP ostream& operator<< (ostream& os, const S_oahFactorizedStringAtom& elt);

//______________________________________________________________________________
class EXP oahStringWithDefaultValueAtom : public oahStringAtom
{
/*
  an atom controlling a string variable with a default value
*/

  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahStringWithDefaultValueAtom> create (
                            const string& longName,
                            const string& shortName,
                            const string& description,
                            const string& valueSpecification,
                            const string& variableName,
                            string&       stringVariable,
                            const string& defaultStringValue);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          oahStringWithDefaultValueAtom (
                            const string& longName,
                            const string& shortName,
                            const string& description,
                            const string& valueSpecification,
                            const string& variableName,
                            string&       stringVariable,
                            const string& defaultStringValue);

    virtual               ~oahStringWithDefaultValueAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setStringVariable (const string& value);

    string                getStringVariable () const
                              { return fStringVariable; }

  public:

    // public services
    // ------------------------------------------------------

    void                  applyAtomWithValue (
                            const string& theString,
                            ostream&      os) override;

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    string                asShortNamedOptionString () const override;
    string                asActualLongNamedOptionString () const override;

    void                  print (ostream& os) const override;

    void                  printAtomWithVariableOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const override;

  protected:

    // protected fields
    // ------------------------------------------------------

    string                fDefaultStringValue;
};
typedef SMARTP<oahStringWithDefaultValueAtom> S_oahStringWithDefaultValueAtom;
EXP ostream& operator<< (ostream& os, const S_oahStringWithDefaultValueAtom& elt);

//______________________________________________________________________________
class EXP oahStringWithRegexAtom : public oahStringAtom
{
/*
  an atom controlling a string variable with a default value
*/

  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahStringWithRegexAtom> create (
                            const string& longName,
                            const string& shortName,
                            const string& description,
                            const string& valueSpecification,
                            const string& variableName,
                            string&       stringVariable,
                            const string& regexString);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          oahStringWithRegexAtom (
                            const string& longName,
                            const string& shortName,
                            const string& description,
                            const string& valueSpecification,
                            const string& variableName,
                            string&       stringVariable,
                            const string& regexString);

    virtual               ~oahStringWithRegexAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setStringVariable (const string& value);

    string                getStringVariable () const
                              { return fStringVariable; }

  public:

    // public services
    // ------------------------------------------------------

    void                  applyAtomWithValue (
                            const string& theString,
                            ostream&      os) override;

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    string                asShortNamedOptionString () const override;
    string                asActualLongNamedOptionString () const override;

    void                  print (ostream& os) const override;

    void                  printAtomWithVariableOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const override;

  protected:

    // protected fields
    // ------------------------------------------------------

    string                fRegexString;
};
typedef SMARTP<oahStringWithRegexAtom> S_oahStringWithRegexAtom;
EXP ostream& operator<< (ostream& os, const S_oahStringWithRegexAtom& elt);

//______________________________________________________________________________
class EXP oahRationalAtom : public oahAtomStoringAValue
{
/*
  an atom controlling a rational variable
*/

  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahRationalAtom> create (
                            const string& longName,
                            const string& shortName,
                            const string& description,
                            const string& valueSpecification,
                            const string& variableName,
                            rational&     rationalVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          oahRationalAtom (
                            const string& longName,
                            const string& shortName,
                            const string& description,
                            const string& valueSpecification,
                            const string& variableName,
                            rational&     rationalVariable);

    virtual               ~oahRationalAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setRationalVariable (
                            const rational& value)
                              {
                                fRationalVariable = value;
                                fSetByUser = true;
                              }

    rational              getRationalVariable () const
                              { return fRationalVariable; }

  public:

    // public services
    // ------------------------------------------------------

    void                  applyAtomWithValue (
                            const string& theString,
                            ostream&      os) override;

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    string                asShortNamedOptionString () const override;
    string                asActualLongNamedOptionString () const override;

    void                  print (ostream& os) const override;

    void                  printAtomWithVariableOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const override;

  private:

    // private fields
    // ------------------------------------------------------

    rational&             fRationalVariable;
};
typedef SMARTP<oahRationalAtom> S_oahRationalAtom;
EXP ostream& operator<< (ostream& os, const S_oahRationalAtom& elt);

//______________________________________________________________________________
class EXP oahNaturalNumbersSetElementAtom : public oahAtomStoringAValue
{
/*
  an atom controlling a set of natural number variables
*/

  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahNaturalNumbersSetElementAtom> create (
                            const string& longName,
                            const string& shortName,
                            const string& description,
                            const string& valueSpecification,
                            const string& variableName,
                            set<int>&     naturalNumbersSetVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          oahNaturalNumbersSetElementAtom (
                            const string& longName,
                            const string& shortName,
                            const string& description,
                            const string& valueSpecification,
                            const string& variableName,
                            set<int>&     naturalNumbersSetVariable);

    virtual               ~oahNaturalNumbersSetElementAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setNaturalNumbersSetVariable (
                            set<int>& value)
                              {
                                fNaturalNumbersSetVariable = value;
                                fSetByUser = true;
                              }

    const set<int>&       getNaturalNumbersSetVariable () const
                              { return fNaturalNumbersSetVariable; }

  public:

    // public services
    // ------------------------------------------------------

    void                  applyAtomWithValue (
                            const string& theString,
                            ostream&      os) override;

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    string                asShortNamedOptionString () const override;
    string                asActualLongNamedOptionString () const override;

    void                  print (ostream& os) const override;

    void                  printAtomWithVariableOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const override;

  private:

    // private fields
    // ------------------------------------------------------

    set<int>&             fNaturalNumbersSetVariable;
};
typedef SMARTP<oahNaturalNumbersSetElementAtom> S_oahNaturalNumbersSetElementAtom;
EXP ostream& operator<< (ostream& os, const S_oahNaturalNumbersSetElementAtom& elt);

//______________________________________________________________________________
class EXP oahRGBColorAtom : public oahAtomStoringAValue
{
/*
  an atom controlling an RGB color variable
*/

  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahRGBColorAtom> create (
                            const string&       longName,
                            const string&       shortName,
                            const string&       description,
                            const string&       valueSpecification,
                            const string&       variableName,
                            msrRGBColor& RGBColorVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          oahRGBColorAtom (
                            const string&       longName,
                            const string&       shortName,
                            const string&       description,
                            const string&       valueSpecification,
                            const string&       variableName,
                            msrRGBColor& RGBColorVariable);

    virtual               ~oahRGBColorAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setRGBColorVariable (
                            msrRGBColor& value)
                              {
                                fRGBColorVariable = value;
                                fSetByUser = true;
                              }

    const msrRGBColor&    getRGBColorVariable () const
                              { return fRGBColorVariable; }

  public:

    // public services
    // ------------------------------------------------------

    void                  applyAtomWithValue (
                            const string& theString,
                            ostream&      os) override;

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    string                asShortNamedOptionString () const override;
    string                asActualLongNamedOptionString () const override;

    void                  print (ostream& os) const override;

    void                  printAtomWithVariableOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const override;

  private:

    // private fields
    // ------------------------------------------------------

    msrRGBColor&          fRGBColorVariable;
};
typedef SMARTP<oahRGBColorAtom> S_oahRGBColorAtom;
EXP ostream& operator<< (ostream& os, const S_oahRGBColorAtom& elt);

//______________________________________________________________________________
class EXP oahIntSetElementAtom : public oahAtomStoringAValue
{
/*
  an atom controlling a set of integer variables
*/

  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahIntSetElementAtom> create (
                            const string& longName,
                            const string& shortName,
                            const string& description,
                            const string& valueSpecification,
                            const string& variableName,
                            set<int>&     intSetVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          oahIntSetElementAtom (
                            const string& longName,
                            const string& shortName,
                            const string& description,
                            const string& valueSpecification,
                            const string& variableName,
                            set<int>&     intSetVariable);

    virtual               ~oahIntSetElementAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    const set<int>&       getIntSetVariable () const
                              { return fIntSetVariable; }

  public:

    // public services
    // ------------------------------------------------------

    void                  applyAtomWithValue (
                            const string& theString,
                            ostream&      os) override;

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    string                asShortNamedOptionString () const override;
    string                asActualLongNamedOptionString () const override;

    void                  print (ostream& os) const override;

    void                  printAtomWithVariableOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const override;

  private:

    // private fields
    // ------------------------------------------------------

    set<int>&             fIntSetVariable;
};
typedef SMARTP<oahIntSetElementAtom> S_oahIntSetElementAtom;
EXP ostream& operator<< (ostream& os, const S_oahIntSetElementAtom& elt);

//______________________________________________________________________________
class EXP oahStringSetElementAtom : public oahAtomStoringAValue
{
/*
  an atom controlling a set of string variables
*/

  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahStringSetElementAtom> create (
                            const string& longName,
                            const string& shortName,
                            const string& description,
                            const string& valueSpecification,
                            const string& variableName,
                            set<string>&  stringSetVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          oahStringSetElementAtom (
                            const string& longName,
                            const string& shortName,
                            const string& description,
                            const string& valueSpecification,
                            const string& variableName,
                            set<string>&  stringSetVariable);

    virtual               ~oahStringSetElementAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setStringSetVariable (
                            const string& partName)
                              {
                                fStringSetVariable.insert (partName);
                                fSetByUser = true;
                              }

    const set<string>&    getStringSetVariable () const
                              { return fStringSetVariable; }

  public:

    // public services
    // ------------------------------------------------------

    void                  applyAtomWithValue (
                            const string& theString,
                            ostream&      os) override;

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    string                asShortNamedOptionString () const override;
    string                asActualLongNamedOptionString () const override;

    void                  print (ostream& os) const override;

    void                  printAtomWithVariableOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const override;

  private:

    // private fields
    // ------------------------------------------------------

    set<string>&          fStringSetVariable;
};
typedef SMARTP<oahStringSetElementAtom> S_oahStringSetElementAtom;
EXP ostream& operator<< (ostream& os, const S_oahStringSetElementAtom& elt);

//______________________________________________________________________________
class EXP oahStringToIntMapElementAtom : public oahAtomStoringAValue
{
/*
  an atom controlling a string to integer mapping variable
*/

  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahStringToIntMapElementAtom> create (
                            const string&     longName,
                            const string&     shortName,
                            const string&     description,
                            const string&     valueSpecification,
                            const string&     variableName,
                            map<string, int>& stringToIntMapVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          oahStringToIntMapElementAtom (
                            const string&     longName,
                            const string&     shortName,
                            const string&     description,
                            const string&     valueSpecification,
                            const string&     variableName,
                            map<string, int>& stringToIntMapVariable);

    virtual               ~oahStringToIntMapElementAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    const map<string, int>&
                          getStringToIntMapVariable () const
                              { return fStringToIntMapVariable; }

  public:

    // public services
    // ------------------------------------------------------

    void                  applyAtomWithValue (
                            const string& theString,
                            ostream&      os) override;

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    string                asShortNamedOptionString () const override;
    string                asActualLongNamedOptionString () const override;

    void                  print (ostream& os) const override;

    void                  printAtomWithVariableOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const override;

  private:

    // private fields
    // ------------------------------------------------------

    map<string, int>&     fStringToIntMapVariable;
};
typedef SMARTP<oahStringToIntMapElementAtom> S_oahStringToIntMapElementAtom;
EXP ostream& operator<< (ostream& os, const S_oahStringToIntMapElementAtom& elt);

//______________________________________________________________________________
class EXP oahStringToStringMapElementAtom : public oahAtomStoringAValue
{
/*
  an atom controlling a string to integer mapping variable
*/

  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahStringToStringMapElementAtom> create (
                            const string&         longName,
                            const string&         shortName,
                            const string&         description,
                            const string&         valueSpecification,
                            const string&         variableName,
                            map<string, string>&  stringToStringMapVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          oahStringToStringMapElementAtom (
                            const string&         longName,
                            const string&         shortName,
                            const string&         description,
                            const string&         valueSpecification,
                            const string&         variableName,
                            map<string, string>&  stringToStringMapVariable);

    virtual               ~oahStringToStringMapElementAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    const map<string, string>&
                          getStringToStringMapVariable () const
                              { return fStringToStringMapVariable; }

  public:

    // public services
    // ------------------------------------------------------

    void                  applyAtomWithValue (
                            const string& theString,
                            ostream&      os) override;

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    string                asShortNamedOptionString () const override;
    string                asActualLongNamedOptionString () const override;

    void                  print (ostream& os) const override;

    void                  printAtomWithVariableOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const override;

  private:

    // private fields
    // ------------------------------------------------------

    map<string, string>&     fStringToStringMapVariable;
};
typedef SMARTP<oahStringToStringMapElementAtom> S_oahStringToStringMapElementAtom;
EXP ostream& operator<< (ostream& os, const S_oahStringToStringMapElementAtom& elt);

//______________________________________________________________________________
class EXP oahStringAndIntegerAtom : public oahAtomStoringAValue
{
/*
  an atom controlling a string and an integer variables
*/

  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahStringAndIntegerAtom> create (
                            const string& longName,
                            const string& shortName,
                            const string& description,
                            const string& valueSpecification,
                            const string& stringVariableName,
                            string&       stringVariable,
                            const string& integerVariableName,
                            int&          integerVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          oahStringAndIntegerAtom (
                            const string& longName,
                            const string& shortName,
                            const string& description,
                            const string& valueSpecification,
                            const string& stringVariableName,
                            string&       stringVariable,
                            const string& integerVariableName,
                            int&          integerVariable);

    virtual               ~oahStringAndIntegerAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setStringVariable (const string& value);

    string                getStringVariable () const
                              { return fStringVariable; }

    void                  setIntegerVariable (int value);

    int                   getIntegerVariable () const
                              { return fIntegerVariable; }

  public:

    // public services
    // ------------------------------------------------------

    void                  applyAtomWithValue (
                            const string& theString,
                            ostream&      os) override;

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    string                asShortNamedOptionString () const override;
    string                asActualLongNamedOptionString () const override;

    void                  print (ostream& os) const override;

    void                  printAtomWithVariableOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const override;

  private:

    // private fields
    // ------------------------------------------------------

    string                fStringVariableName;
    string&               fStringVariable;

    string                fIntegerVariableName;
    int&                  fIntegerVariable;
};
typedef SMARTP<oahStringAndIntegerAtom> S_oahStringAndIntegerAtom;
EXP ostream& operator<< (ostream& os, const S_oahStringAndIntegerAtom& elt);

//______________________________________________________________________________
class EXP oahStringAndTwoIntegersAtom : public oahAtomStoringAValue
{
/*
  an atom controlling a string and two integer variables
*/

  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahStringAndTwoIntegersAtom> create (
                            const string&  shortName,
                            const string&  longName,
                            const string&  description,
                            const string&  valueSpecification,
                            const string&  variableName,
                            string&        stringVariable,
                            int&           primaryIntegerVariable,
                            int&           secondaryIntegerVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          oahStringAndTwoIntegersAtom (
                            const string&  shortName,
                            const string&  longName,
                            const string&  description,
                            const string&  valueSpecification,
                            const string&  variableName,
                            string&        stringVariable,
                            int&           primaryIntegerVariable,
                            int&           secondaryIntegerVariable);

    virtual               ~oahStringAndTwoIntegersAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setStringVariable (const string& value);

    string                getStringVariable () const
                              { return fStringVariable; }

    void                  setPrimaryIntegerVariable (int value);

    int                   getPrimaryIntegerVariable () const
                              { return fPrimaryIntegerVariable; }

    void                  setSecondaryIntegerVariable (int value);

    int                   getSecondaryIntegerVariable () const
                              { return fSecondaryIntegerVariable; }

  public:

    // public services
    // ------------------------------------------------------

    void                  applyAtomWithValue (
                            const string& theString,
                            ostream&      os) override;

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    string                asShortNamedOptionString () const override;
    string                asActualLongNamedOptionString () const override;

    void                  print (ostream& os) const override;

    void                  printAtomWithVariableOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const override;

  private:

    // private fields
    // ------------------------------------------------------

    string&               fStringVariable;

    int&                  fPrimaryIntegerVariable;
    int&                  fSecondaryIntegerVariable;
};
typedef SMARTP<oahStringAndTwoIntegersAtom> S_oahStringAndTwoIntegersAtom;
EXP ostream& operator<< (ostream& os, const S_oahStringAndTwoIntegersAtom& elt);

//______________________________________________________________________________
class EXP oahLengthUnitKindAtom : public oahAtomStoringAValue
{
/*
  an atom controlling a length unit kind variable
*/

  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahLengthUnitKindAtom> create (
                            const string&      longName,
                            const string&      shortName,
                            const string&      description,
                            const string&      valueSpecification,
                            const string&      variableName,
                            msrLengthUnitKind& lengthUnitKindVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          oahLengthUnitKindAtom (
                            const string&      longName,
                            const string&      shortName,
                            const string&      description,
                            const string&      valueSpecification,
                            const string&      variableName,
                            msrLengthUnitKind& lengthUnitKindVariable);

    virtual               ~oahLengthUnitKindAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setLengthUnitKindVariable (
                            msrLengthUnitKind value)
                              {
                                fLengthUnitKindVariable = value;
                                fSetByUser = true;
                              }

    msrLengthUnitKind     getLengthUnitKindVariable () const
                              { return fLengthUnitKindVariable; }

  public:

    // public services
    // ------------------------------------------------------

    void                  applyAtomWithValue (
                            const string& theString,
                            ostream&      os) override;

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    string                asShortNamedOptionString () const override;
    string                asActualLongNamedOptionString () const override;

    void                  print (ostream& os) const override;

    void                  printAtomWithVariableOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const override;

  private:

    // private fields
    // ------------------------------------------------------

    msrLengthUnitKind&    fLengthUnitKindVariable;
};
typedef SMARTP<oahLengthUnitKindAtom> S_oahLengthUnitKindAtom;
EXP ostream& operator<< (ostream& os, const S_oahLengthUnitKindAtom& elt);

//______________________________________________________________________________
class EXP oahLengthAtom : public oahAtomStoringAValue
{
/*
  an atom controlling a length variable
*/

  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahLengthAtom> create (
                            const string& longName,
                            const string& shortName,
                            const string& description,
                            const string& valueSpecification,
                            const string& variableName,
                            msrLength&    lengthVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          oahLengthAtom (
                            const string& longName,
                            const string& shortName,
                            const string& description,
                            const string& valueSpecification,
                            const string& variableName,
                            msrLength&    lengthVariable);

    virtual               ~oahLengthAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setLengthVariable (
                            msrLength value)
                              {
                                fLengthVariable = value;
                                fSetByUser = true;
                              }

    msrLength             getLengthVariable () const
                              { return fLengthVariable; }

  public:

    // public services
    // ------------------------------------------------------

    void                  applyAtomWithValue (
                            const string& theString,
                            ostream&      os) override;

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    string                asShortNamedOptionString () const override;
    string                asActualLongNamedOptionString () const override;

    void                  print (ostream& os) const override;

    void                  printAtomWithVariableOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const override;

  private:

    // private fields
    // ------------------------------------------------------

    msrLength&            fLengthVariable;
};
typedef SMARTP<oahLengthAtom> S_oahLengthAtom;
EXP ostream& operator<< (ostream& os, const S_oahLengthAtom& elt);

//______________________________________________________________________________
class EXP oahMidiTempoAtom : public oahAtomStoringAValue
{
/*
  an atom controlling a MIDI tempo variable
*/

  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahMidiTempoAtom> create (
                            const string& longName,
                            const string& shortName,
                            const string& description,
                            const string& valueSpecification,
                            const string& variableName,
                            msrMidiTempo& midiTempoVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          oahMidiTempoAtom (
                            const string& longName,
                            const string& shortName,
                            const string& description,
                            const string& valueSpecification,
                            const string& variableName,
                            msrMidiTempo& midiTempoVariable);

    virtual               ~oahMidiTempoAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setMidiTempoVariable (
                            const msrMidiTempo& value);

    const msrMidiTempo&   getMidiTempoVariable () const
                              { return fMidiTempoVariable; }

  public:

    // public services
    // ------------------------------------------------------

    void                  applyAtomWithValue (
                            const string& theString,
                            ostream&      os) override;

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    string                asShortNamedOptionString () const override;
    string                asActualLongNamedOptionString () const override;

    void                  print (ostream& os) const override;

    void                  printAtomWithVariableOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const override;

  private:

    // private fields
    // ------------------------------------------------------

    msrMidiTempo&         fMidiTempoVariable;
};
typedef SMARTP<oahMidiTempoAtom> S_oahMidiTempoAtom;
EXP ostream& operator<< (ostream& os, const S_oahMidiTempoAtom& elt);

//______________________________________________________________________________
class EXP oahOptionNameHelpAtom : public oahStringWithDefaultValueAtom
{
/*
  This is where OAH is introspective:
    OAH provides a way to obtain help on any option name,
    be it one to print help about a group or subgroup,
    or the name of a 'regular' atom.
  This option is reflexive, since it can provide help about itself.
*/

  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahOptionNameHelpAtom> create (
                            const string& longName,
                            const string& shortName,
                            const string& description,
                            const string& valueSpecification,
                            const string& variableName,
                            string&       stringVariable,
                            const string& defaultOptionName);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          oahOptionNameHelpAtom (
                            const string& longName,
                            const string& shortName,
                            const string& description,
                            const string& valueSpecification,
                            const string& variableName,
                            string&       stringVariable,
                            const string& defaultOptionName);

  protected:

    virtual               ~oahOptionNameHelpAtom ();

  public:

    // set and get
    // ------------------------------------------------------

  public:

    // public services
    // ------------------------------------------------------

    void                  applyAtomWithValue (
                            const string& theString,
                            ostream&      os) override;

    void                  applyAtomWithDefaultValue (ostream& os) override;

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    string                asShortNamedOptionString () const override;
    string                asActualLongNamedOptionString () const override;

    void                  print (ostream& os) const override;

    virtual void          printAtomWithVariableOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const override;
  private:

    // private fields
    // ------------------------------------------------------

//    string                fValueSpecification;
};
typedef SMARTP<oahOptionNameHelpAtom> S_oahOptionNameHelpAtom;
EXP ostream& operator<< (ostream& os, const S_oahOptionNameHelpAtom& elt);

//______________________________________________________________________________
class EXP oahQueryOptionNameAtom : public oahPureHelpAtomExpectingAValue
{
/*
  This is where OAH is introspective too
*/

  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahQueryOptionNameAtom> create (
                            const string& longName,
                            const string& shortName,
                            const string& description,
                            const string& valueSpecification,
                            const string& serviceName);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          oahQueryOptionNameAtom (
                            const string& longName,
                            const string& shortName,
                            const string& description,
                            const string& valueSpecification,
                            const string& serviceName);

    virtual               ~oahQueryOptionNameAtom ();

  public:

    // set and get
    // ------------------------------------------------------

  public:

    // public services
    // ------------------------------------------------------

    void                  applyAtomWithValue (
                            const string& theString,
                            ostream&      os) override;

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    string                asShortNamedOptionString () const override;
    string                asActualLongNamedOptionString () const override;

    void                  print (ostream& os) const override;

    virtual void          printAtomWithVariableOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const override;
  private:

    // private fields
    // ------------------------------------------------------
};
typedef SMARTP<oahQueryOptionNameAtom> S_oahQueryOptionNameAtom;
EXP ostream& operator<< (ostream& os, const S_oahQueryOptionNameAtom& elt);

//______________________________________________________________________________
class EXP oahFindStringAtom : public oahPureHelpAtomExpectingAValue
{
/*
  This is where OAH is introspective too
*/

  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahFindStringAtom> create (
                            const string& longName,
                            const string& shortName,
                            const string& description,
                            const string& valueSpecification,
                            const string& serviceName);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          oahFindStringAtom (
                            const string& longName,
                            const string& shortName,
                            const string& description,
                            const string& valueSpecification,
                            const string& serviceName);

    virtual               ~oahFindStringAtom ();

  public:

    // set and get
    // ------------------------------------------------------

  public:

    // public services
    // ------------------------------------------------------

    void                  applyAtomWithValue (
                            const string& theString,
                            ostream&      os) override;

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    string                asShortNamedOptionString () const override;
    string                asActualLongNamedOptionString () const override;

    void                  print (ostream& os) const override;

    virtual void          printAtomWithVariableOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const override;
  private:

    // private fields
    // ------------------------------------------------------
};
typedef SMARTP<oahFindStringAtom> S_oahFindStringAtom;
EXP ostream& operator<< (ostream& os, const S_oahFindStringAtom& elt);

//______________________________________________________________________________
class EXP oahIncludeOptionsAndArgumentsFileAtom : public oahPureHelpAtomExpectingAValue
{
/*
  This is where OAH is introspective too
*/

  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahIncludeOptionsAndArgumentsFileAtom> create (
                            const string& longName,
                            const string& shortName,
                            const string& description,
                            const string& valueSpecification,
                            const string& serviceName);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          oahIncludeOptionsAndArgumentsFileAtom (
                            const string& longName,
                            const string& shortName,
                            const string& description,
                            const string& valueSpecification,
                            const string& serviceName);

    virtual               ~oahIncludeOptionsAndArgumentsFileAtom ();

  public:

    // set and get
    // ------------------------------------------------------

  public:

    // public services
    // ------------------------------------------------------

    void                  applyAtomWithValue (
                            const string& theString,
                            ostream&      os) override;

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    string                asShortNamedOptionString () const override;
    string                asActualLongNamedOptionString () const override;

    void                  print (ostream& os) const override;

    virtual void          printAtomWithVariableOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const override;
  private:

    // private fields
    // ------------------------------------------------------
};
typedef SMARTP<oahIncludeOptionsAndArgumentsFileAtom> S_oahIncludeOptionsAndArgumentsFileAtom;
EXP ostream& operator<< (ostream& os, const S_oahIncludeOptionsAndArgumentsFileAtom& elt);


}


#endif
