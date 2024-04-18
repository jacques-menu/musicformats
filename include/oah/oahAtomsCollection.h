/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2024

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___oahAtomsCollection___
#define ___oahAtomsCollection___

#include <list>
#include <map>
#include <set>

#include "mfRational.h"

#include "mfOnOff.h"

#include "msrColors.h"
#include "msrLengths.h"
#include "msrMidi.h"

#include "oahBasicTypes.h"


namespace MusicFormats
{

//______________________________________________________________________________
class EXP oahValueLessAtomAlias : public oahValueLessAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahValueLessAtomAlias> create (
                            const std::string&        shortName,
                            const std::string&        longName,
                            const std::string&        description,
                            const S_oahValueLessAtom& originalValueLessAtom);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          oahValueLessAtomAlias (
                            const std::string&        shortName,
                            const std::string&        longName,
                            const std::string&        description,
                            const S_oahValueLessAtom& originalValueLessAtom);

    virtual               ~oahValueLessAtomAlias ();

  public:

    // set and get
    // ------------------------------------------------------

     S_oahValueLessAtom   getOriginalValueLessAtom () const
                              { return fOriginalValueLessAtom; }

  public:

    // public services
    // ------------------------------------------------------

    void                  applyValueLessAtom (std::ostream& os) override;

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

  private:

    // private fields
    // ------------------------------------------------------

     S_oahValueLessAtom   fOriginalValueLessAtom;
};
typedef SMARTP<oahValueLessAtomAlias> S_oahValueLessAtomAlias;
EXP std::ostream& operator << (std::ostream& os, const S_oahValueLessAtomAlias& elt);

//______________________________________________________________________________
class EXP oahValueFittedAtomAlias : public oahValueFittedAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahValueFittedAtomAlias> create (
                            const std::string&          longName,
                            const std::string&          shortName,
                            const std::string&          description,
                            const S_oahValueFittedAtom& originalValueFittedAtom);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          oahValueFittedAtomAlias (
                            const std::string&          longName,
                            const std::string&          shortName,
                            const std::string&          description,
                            const S_oahValueFittedAtom& originalValueFittedAtom);

    virtual               ~oahValueFittedAtomAlias ();

  public:

    // set and get
    // ------------------------------------------------------

     S_oahValueFittedAtom getOriginalValueFittedAtom () const
                              { return fOriginalValueFittedAtom; }

  public:

    // public services
    // ------------------------------------------------------

    virtual void          applyAtomWithValue (
                            const std::string& theString,
                            std::ostream&      os) override;

//     virtual void          applyAtomWithDefaultValue (std::ostream& os) override;

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

  private:

    // private fields
    // ------------------------------------------------------

     S_oahValueFittedAtom fOriginalValueFittedAtom;
};
typedef SMARTP<oahValueFittedAtomAlias> S_oahValueFittedAtomAlias;
EXP std::ostream& operator << (std::ostream& os, const S_oahValueFittedAtomAlias& elt);

//______________________________________________________________________________
class EXP oahMacroAtom : public oahValueLessAtom
{
/*
  a list of value-less atoms to be applied as a unit
*/

  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahMacroAtom> create (
                            const std::string& longName,
                            const std::string& shortName,
                            const std::string& description);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          oahMacroAtom (
                            const std::string& longName,
                            const std::string& shortName,
                            const std::string& description);

    virtual               ~oahMacroAtom ();

  public:

    // set and get
    // ------------------------------------------------------

     const std::list<S_oahValueLessAtom>&
                          getMacroValueLessAtomsList () const
                              { return fMacroValueLessAtomsList; }

  public:

    // public services
    // ------------------------------------------------------

    void                  appendValueLessAtomToMacro (
                            S_oahValueLessAtom atomNotExpectingAValue);

    void                  applyValueLessAtom (std::ostream& os) override;

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

    void                  printHelp (std::ostream& os) const override;

    void                  displayAtomWithVariableOptionsValues (
                            std::ostream& os,
                            int           valueFieldWidth) const override;
  private:

    // private fields
    // ------------------------------------------------------

     std::list<S_oahValueLessAtom>
                          fMacroValueLessAtomsList;
};
typedef SMARTP<oahMacroAtom> S_oahMacroAtom;
EXP std::ostream& operator << (std::ostream& os, const S_oahMacroAtom& elt);

//______________________________________________________________________________
class EXP oahOptionsUsageAtom : public oahPureHelpValueLessAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahOptionsUsageAtom> create (
                            const std::string& longName,
                            const std::string& shortName,
                            const std::string& description,
                            const std::string& serviceName);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          oahOptionsUsageAtom (
                            const std::string& longName,
                            const std::string& shortName,
                            const std::string& description,
                            const std::string& serviceName);

    virtual               ~oahOptionsUsageAtom ();

  public:

    // public services
    // ------------------------------------------------------

    void                  applyValueLessAtom (std::ostream& os) override;

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    void                  printOptionsUsage (std::ostream& os) const;

    void                  print (std::ostream& os) const override;
};
typedef SMARTP<oahOptionsUsageAtom> S_oahOptionsUsageAtom;
EXP std::ostream& operator << (std::ostream& os, const S_oahOptionsUsageAtom& elt);

//______________________________________________________________________________
class EXP oahHelpAtom : public oahPureHelpValueLessAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahHelpAtom> create (
                            const std::string& longName,
                            const std::string& shortName,
                            const std::string& description,
                            const std::string& serviceName);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          oahHelpAtom (
                            const std::string& longName,
                            const std::string& shortName,
                            const std::string& description,
                            const std::string& serviceName);

    virtual               ~oahHelpAtom ();

  public:

    // public services
    // ------------------------------------------------------

    void                  applyValueLessAtom (std::ostream& os) override;

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

    void                  printOptionsSummary (std::ostream& os) const;
};
typedef SMARTP<oahHelpAtom> S_oahHelpAtom;
EXP std::ostream& operator << (std::ostream& os, const S_oahHelpAtom& elt);

//______________________________________________________________________________
class EXP oahHelpSummaryAtom : public oahPureHelpValueLessAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahHelpSummaryAtom> create (
                            const std::string& longName,
                            const std::string& shortName,
                            const std::string& description,
                            const std::string& serviceName);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          oahHelpSummaryAtom (
                            const std::string& longName,
                            const std::string& shortName,
                            const std::string& description,
                            const std::string& serviceName);

    virtual               ~oahHelpSummaryAtom ();

  public:

    // public services
    // ------------------------------------------------------

    void                  applyValueLessAtom (std::ostream& os) override;

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

    void                  printOptionsSummary (std::ostream& os) const;
};
typedef SMARTP<oahHelpSummaryAtom> S_oahHelpSummaryAtom;
EXP std::ostream& operator << (std::ostream& os, const S_oahHelpSummaryAtom& elt);

//______________________________________________________________________________
class EXP oahAboutAtom : public oahPureHelpValueLessAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahAboutAtom> create (
                            const std::string& longName,
                            const std::string& shortName,
                            const std::string& description,
                            const std::string& serviceName);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          oahAboutAtom (
                            const std::string& longName,
                            const std::string& shortName,
                            const std::string& description,
                            const std::string& serviceName);

    virtual               ~oahAboutAtom ();

  public:

    // public services
    // ------------------------------------------------------

    void                  applyValueLessAtom (std::ostream& os) override;

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

    void                  printAbout (std::ostream& os) const;
};
typedef SMARTP<oahAboutAtom> S_oahAboutAtom;
EXP std::ostream& operator << (std::ostream& os, const S_oahAboutAtom& elt);

//______________________________________________________________________________
enum class oahVersionKind {
  kOahVersion, kOahVersionFull
};

std::string oahVersionKindAsString (
  oahVersionKind versionKind);

std::ostream& operator << (std::ostream& os, const oahVersionKind& elt);

//______________________________________________________________________________
class EXP oahVersionAtom : public oahPureHelpValueLessAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahVersionAtom> create (
                            const std::string&  longName,
                            const std::string&  shortName,
                            const std::string&  description,
                            const std::string&  serviceName,
                            oahVersionKind      versionKind);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          oahVersionAtom (
                            const std::string&  longName,
                            const std::string&  shortName,
                            const std::string&  description,
                            const std::string&  serviceName,
                            oahVersionKind      versionKind);

    virtual               ~oahVersionAtom ();

  public:

    // public services
    // ------------------------------------------------------

    void                  applyValueLessAtom (std::ostream& os) override;

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

    void                  printVersionShort (std::ostream& os) const;
    void                  printVersionFull (std::ostream& os) const;

    oahVersionKind        fVersionKind;
};
typedef SMARTP<oahVersionAtom> S_oahVersionAtom;
EXP std::ostream& operator << (std::ostream& os, const S_oahVersionAtom& elt);

//______________________________________________________________________________
class EXP oahLibraryVersionAtom : public oahPureHelpValueLessAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahLibraryVersionAtom> create (
                            const std::string& longName,
                            const std::string& shortName,
                            const std::string& description,
                            const std::string& serviceName);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          oahLibraryVersionAtom (
                            const std::string& longName,
                            const std::string& shortName,
                            const std::string& description,
                            const std::string& serviceName);

    virtual               ~oahLibraryVersionAtom ();

  public:

    // public services
    // ------------------------------------------------------

    void                  applyValueLessAtom (std::ostream& os) override;

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

    void                  printVersion (std::ostream& os) const;
};
typedef SMARTP<oahLibraryVersionAtom> S_oahLibraryVersionAtom;
EXP std::ostream& operator << (std::ostream& os, const S_oahLibraryVersionAtom& elt);

//______________________________________________________________________________
class EXP oahHistoryAtom : public oahPureHelpValueLessAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahHistoryAtom> create (
                            const std::string& longName,
                            const std::string& shortName,
                            const std::string& description,
                            const std::string& serviceName);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          oahHistoryAtom (
                            const std::string& longName,
                            const std::string& shortName,
                            const std::string& description,
                            const std::string& serviceName);

    virtual               ~oahHistoryAtom ();

  public:

    // public services
    // ------------------------------------------------------

    void                  applyValueLessAtom (std::ostream& os) override;

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

    void                  printHistory (std::ostream& os) const;
};
typedef SMARTP<oahHistoryAtom> S_oahHistoryAtom;
EXP std::ostream& operator << (std::ostream& os, const S_oahHistoryAtom& elt);

//______________________________________________________________________________
class EXP oahLibraryHistoryAtom : public oahPureHelpValueLessAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahLibraryHistoryAtom> create (
                            const std::string& longName,
                            const std::string& shortName,
                            const std::string& description,
                            const std::string& serviceName);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          oahLibraryHistoryAtom (
                            const std::string& longName,
                            const std::string& shortName,
                            const std::string& description,
                            const std::string& serviceName);

    virtual               ~oahLibraryHistoryAtom ();

  public:

    // public services
    // ------------------------------------------------------

    void                  applyValueLessAtom (std::ostream& os) override;

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

    void                  printHistory (std::ostream& os) const;
};
typedef SMARTP<oahLibraryHistoryAtom> S_oahLibraryHistoryAtom;
EXP std::ostream& operator << (std::ostream& os, const S_oahLibraryHistoryAtom& elt);

//______________________________________________________________________________
class EXP oahContactAtom : public oahPureHelpValueLessAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahContactAtom> create (
                            const std::string& longName,
                            const std::string& shortName,
                            const std::string& description,
                            const std::string& serviceName);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          oahContactAtom (
                            const std::string& longName,
                            const std::string& shortName,
                            const std::string& description,
                            const std::string& serviceName);


    virtual               ~oahContactAtom ();

  public:

    // public services
    // ------------------------------------------------------

    void                  applyValueLessAtom (std::ostream& os) override;

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

    void                  printContact (std::ostream& os) const;
};
typedef SMARTP<oahContactAtom> S_oahContactAtom;
EXP std::ostream& operator << (std::ostream& os, const S_oahContactAtom& elt);

//______________________________________________________________________________
class EXP oahDisplayPrefixes : public oahPureHelpValueLessAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahDisplayPrefixes> create (
                            const std::string& longName,
                            const std::string& shortName,
                            const std::string& description,
                            const std::string& serviceName);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          oahDisplayPrefixes (
                            const std::string& longName,
                            const std::string& shortName,
                            const std::string& description,
                            const std::string& serviceName);


    virtual               ~oahDisplayPrefixes ();

  public:

    // public services
    // ------------------------------------------------------

    void                  applyValueLessAtom (std::ostream& os) override;

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

    void                  printPrefixes (std::ostream& os) const;
};
typedef SMARTP<oahDisplayPrefixes> S_oahDisplayPrefixes;
EXP std::ostream& operator << (std::ostream& os, const S_oahDisplayPrefixes& elt);

//______________________________________________________________________________
class EXP oahDisplaySingleCharacterOptions : public oahPureHelpValueLessAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahDisplaySingleCharacterOptions> create (
                            const std::string& longName,
                            const std::string& shortName,
                            const std::string& description,
                            const std::string& serviceName);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          oahDisplaySingleCharacterOptions (
                            const std::string& longName,
                            const std::string& shortName,
                            const std::string& description,
                            const std::string& serviceName);


    virtual               ~oahDisplaySingleCharacterOptions ();

  public:

    // public services
    // ------------------------------------------------------

    void                  applyValueLessAtom (std::ostream& os) override;

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

    void                  printSingleCharacterOptions (std::ostream& os) const;
};
typedef SMARTP<oahDisplaySingleCharacterOptions> S_oahDisplaySingleCharacterOptions;
EXP std::ostream& operator << (std::ostream& os, const S_oahDisplaySingleCharacterOptions& elt);

//______________________________________________________________________________
class EXP oahMusicFormatsTestAtom : public oahPureHelpValueLessAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahMusicFormatsTestAtom> create (
                            const std::string& longName,
                            const std::string& shortName,
                            const std::string& description,
                            const std::string& serviceName);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          oahMusicFormatsTestAtom (
                            const std::string& longName,
                            const std::string& shortName,
                            const std::string& description,
                            const std::string& serviceName);

    virtual               ~oahMusicFormatsTestAtom ();

  public:

    // public services
    // ------------------------------------------------------

    void                  applyValueLessAtom (std::ostream& os) override;

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    void                  printOptionsMfTest (std::ostream& os) const;

    void                  print (std::ostream& os) const override;
};
typedef SMARTP<oahMusicFormatsTestAtom> S_oahMusicFormatsTestAtom;
EXP std::ostream& operator << (std::ostream& os, const S_oahMusicFormatsTestAtom& elt);

//______________________________________________________________________________
class EXP oahOnOffAtom : public oahAtomStoringAValue
{
/*
  an atom controlling an on/off variableName,
  but expecting no value to be supplied:
  the variable is false initially,
  and is set to true by the mere occurrence of the atom
*/

  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahOnOffAtom> create (
                            const std::string& longName,
                            const std::string& shortName,
                            const std::string& description,
                            const std::string& valueSpecification,
                            const std::string& variableName,
                            mfOnOffKind&       onOffKindVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          oahOnOffAtom (
                            const std::string& longName,
                            const std::string& shortName,
                            const std::string& description,
                            const std::string& valueSpecification,
                            const std::string& variableName,
                            mfOnOffKind&       onOffKindVariable);

    virtual               ~oahOnOffAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setOnOffKindVariable (mfOnOffKind value);

    mfOnOffKind           getOnOffKindVariable () const
                              { return fOnOffKindVariable; }

  public:

    // public services
    // ------------------------------------------------------

    void                  applyAtomWithValue (
                            const std::string& theString,
                            std::ostream&      os) override;

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

    std::string           fVariableName;
    mfOnOffKind&          fOnOffKindVariable;
};
typedef SMARTP<oahOnOffAtom> S_oahOnOffAtom;
EXP std::ostream& operator << (std::ostream& os, const S_oahOnOffAtom& elt);

//______________________________________________________________________________
class EXP oahBooleanAtom : public oahValueLessAtom
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
                            const std::string& longName,
                            const std::string& shortName,
                            const std::string& description,
                            const std::string& variableName,
                            Bool&              booleanVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          oahBooleanAtom (
                            const std::string& longName,
                            const std::string& shortName,
                            const std::string& description,
                            const std::string& variableName,
                            Bool&              booleanVariable);

    virtual               ~oahBooleanAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setBooleanVariable (Bool value);

    Bool                  getBooleanVariable () const
                              { return fBooleanVariable; }

  public:

    // public services
    // ------------------------------------------------------

    void                  applyValueLessAtom (std::ostream& os) override;

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

    std::string           fVariableName;
    Bool&                 fBooleanVariable;
};
typedef SMARTP<oahBooleanAtom> S_oahBooleanAtom;
EXP std::ostream& operator << (std::ostream& os, const S_oahBooleanAtom& elt);

//______________________________________________________________________________
class EXP oahBooleanAtomWithTracePasses : public oahBooleanAtom
{
/*
  an atom that activates passes trace implicitly
*/

  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahBooleanAtomWithTracePasses> create (
                            const std::string& longName,
                            const std::string& shortName,
                            const std::string& description,
                            const std::string& variableName,
                            Bool&              booleanVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          oahBooleanAtomWithTracePasses (
                            const std::string& longName,
                            const std::string& shortName,
                            const std::string& description,
                            const std::string& variableName,
                            Bool&              booleanVariable);

    virtual               ~oahBooleanAtomWithTracePasses ();

  public:

    // set and get
    // ------------------------------------------------------

  public:

    // public services
    // ------------------------------------------------------

    void                  applyValueLessAtom (std::ostream& os) override;

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
};
typedef SMARTP<oahBooleanAtomWithTracePasses> S_oahBooleanAtomWithTracePasses;
EXP std::ostream& operator << (std::ostream& os, const S_oahBooleanAtomWithTracePasses& elt);

//______________________________________________________________________________
class EXP oahTwoBooleansAtom : public oahBooleanAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahTwoBooleansAtom> create (
                            const std::string&      longName,
                            const std::string&      shortName,
                            const std::string&      description,
                            const std::string&      variableName,
                            Bool&                   booleanVariable,
                            const S_oahBooleanAtom& secondBooleanAtom);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          oahTwoBooleansAtom (
                            const std::string&      longName,
                            const std::string&      shortName,
                            const std::string&      description,
                            const std::string&      variableName,
                            Bool&                   booleanVariable,
                            const S_oahBooleanAtom& secondBooleanAtom);

    virtual               ~oahTwoBooleansAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setTwoBooleansVariables (Bool value);

  public:

    // public services
    // ------------------------------------------------------

    void                  applyValueLessAtom (std::ostream& os) override;

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

  private:

    // private fields
    // ------------------------------------------------------

    S_oahBooleanAtom      fSecondBooleanAtom;
};
typedef SMARTP<oahTwoBooleansAtom> S_oahTwoBooleansAtom;
EXP std::ostream& operator << (std::ostream& os, const S_oahTwoBooleansAtom& elt);

//______________________________________________________________________________
class EXP oahTwoBooleansAtomWithTracePasses : public oahTwoBooleansAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahTwoBooleansAtomWithTracePasses> create (
                            const std::string&      longName,
                            const std::string&      shortName,
                            const std::string&      description,
                            const std::string&      variableName,
                            Bool&                   booleanVariable,
                            const S_oahBooleanAtom& secondBooleanAtom);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          oahTwoBooleansAtomWithTracePasses (
                            const std::string&      longName,
                            const std::string&      shortName,
                            const std::string&      description,
                            const std::string&      variableName,
                            Bool&                   booleanVariable,
                            const S_oahBooleanAtom& secondBooleanAtom);

    virtual               ~oahTwoBooleansAtomWithTracePasses ();

  public:

    // set and get
    // ------------------------------------------------------

  public:

    // public services
    // ------------------------------------------------------

    void                  applyValueLessAtom (std::ostream& os) override;

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

  private:

    // private fields
    // ------------------------------------------------------

    S_oahBooleanAtom      fSecondBooleanAtom;
};
typedef SMARTP<oahTwoBooleansAtomWithTracePasses> S_oahTwoBooleansAtomWithTracePasses;
EXP std::ostream& operator << (std::ostream& os, const S_oahTwoBooleansAtomWithTracePasses& elt);

//______________________________________________________________________________
class EXP oahThreeBooleansAtom : public oahBooleanAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahThreeBooleansAtom> create (
                            const std::string&      longName,
                            const std::string&      shortName,
                            const std::string&      description,
                            const std::string&      variableName,
                            Bool&                   booleanVariable,
                            const S_oahBooleanAtom& secondBooleanAtom,
                            const S_oahBooleanAtom& thirdBooleanAtom);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          oahThreeBooleansAtom (
                            const std::string&      longName,
                            const std::string&      shortName,
                            const std::string&      description,
                            const std::string&      variableName,
                            Bool&                   booleanVariable,
                            const S_oahBooleanAtom& secondBooleanAtom,
                            const S_oahBooleanAtom& thirdBooleanAtom);

    virtual               ~oahThreeBooleansAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setThreeBooleansVariables (Bool value);

  public:

    // public services
    // ------------------------------------------------------

    void                  applyValueLessAtom (std::ostream& os) override;

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

  private:

    // private fields
    // ------------------------------------------------------

    S_oahBooleanAtom      fSecondBooleanAtom;
    S_oahBooleanAtom      fThirdBooleanAtom;
};
typedef SMARTP<oahThreeBooleansAtom> S_oahThreeBooleansAtom;
EXP std::ostream& operator << (std::ostream& os, const S_oahThreeBooleansAtom& elt);

//______________________________________________________________________________
class EXP oahThreeBooleansAtomWithTracePasses : public oahThreeBooleansAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahThreeBooleansAtomWithTracePasses> create (
                            const std::string&      longName,
                            const std::string&      shortName,
                            const std::string&      description,
                            const std::string&      variableName,
                            Bool&                   booleanVariable,
                            const S_oahBooleanAtom& secondBooleanAtom,
                            const S_oahBooleanAtom& thirdBooleanAtom);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          oahThreeBooleansAtomWithTracePasses (
                            const std::string&      longName,
                            const std::string&      shortName,
                            const std::string&      description,
                            const std::string&      variableName,
                            Bool&                   booleanVariable,
                            const S_oahBooleanAtom& secondBooleanAtom,
                            const S_oahBooleanAtom& thirdBooleanAtom);

    virtual               ~oahThreeBooleansAtomWithTracePasses ();

  public:

    // set and get
    // ------------------------------------------------------

  public:

    // public services
    // ------------------------------------------------------

    void                  applyValueLessAtom (std::ostream& os) override;

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

  private:

    // private fields
    // ------------------------------------------------------

    S_oahBooleanAtom      fSecondBooleanAtom;
    S_oahBooleanAtom      fThirdBooleanAtom;
};
typedef SMARTP<oahThreeBooleansAtomWithTracePasses> S_oahThreeBooleansAtomWithTracePasses;
EXP std::ostream& operator << (std::ostream& os, const S_oahThreeBooleansAtom& elt);

//______________________________________________________________________________
class EXP oahCombinedBooleansAtom : public oahValueLessAtom
{
/*
  an atom controlling a list of boolean atoms as in 'xml2ly -query minimal'
  without any controlled boolean variable of its own
*/

  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahCombinedBooleansAtom> create (
                            const std::string& longName,
                            const std::string& shortName,
                            const std::string& description);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          oahCombinedBooleansAtom (
                            const std::string& longName,
                            const std::string& shortName,
                            const std::string& description);

    virtual               ~oahCombinedBooleansAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    const std::list<S_oahBooleanAtom>&
                          getBooleanAtomsList () const
                              { return fBooleanAtomsList; }

  public:

    // public services
    // ------------------------------------------------------

    void                  addBooleanAtom (
                            const S_oahBooleanAtom& booleanAtom);

    void                  addBooleanAtomByName (
                            const std::string& name);

    void                  setCombinedBooleanVariables (Bool value);

    void                  applyValueLessAtom (std::ostream& os) override;

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
                            std::ostream& os,
                            int fieldWidth) const;
    virtual void          printCombinedBooleansEssentialsFull (
                            std::ostream& os,
                            int fieldWidth) const;

    void                  print (std::ostream& os) const override;

    void                  printHelp (std::ostream& os) const override;

    void                  displayAtomWithVariableOptionsValues (
                            std::ostream& os,
                            int           valueFieldWidth) const override;

  private:

    // private fields
    // ------------------------------------------------------

    std::list<S_oahBooleanAtom>
                          fBooleanAtomsList;
};
typedef SMARTP<oahCombinedBooleansAtom> S_oahCombinedBooleansAtom;
EXP std::ostream& operator << (std::ostream& os, const S_oahCombinedBooleansAtom& elt);

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
                            const std::string& longName,
                            const std::string& shortName,
                            const std::string& description,
                            const std::string& valueSpecification,
                            const std::string& variableName,
                            int&               integerVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          oahIntegerAtom (
                            const std::string& longName,
                            const std::string& shortName,
                            const std::string& description,
                            const std::string& valueSpecification,
                            const std::string& variableName,
                            int&               integerVariable);

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
                            const std::string& theString,
                            std::ostream&      os) override;

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    std::string           asShortNamedOptionString () const override;
    std::string           asActualLongNamedOptionString () const override;

    void                  print (std::ostream& os) const override;

    void                  displayAtomWithVariableOptionsValues (
                            std::ostream& os,
                            int           valueFieldWidth) const override;
  protected:

    // protected fields
    // ------------------------------------------------------

    int&                  fIntegerVariable;
};
typedef SMARTP<oahIntegerAtom> S_oahIntegerAtom;
EXP std::ostream& operator << (std::ostream& os, const S_oahIntegerAtom& elt);

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
                            const std::string& longName,
                            const std::string& shortName,
                            const std::string& description,
                            const std::string& valueSpecification,
                            const std::string& variableName,
                            int&               integerVariable,
                            int&               integerSecondaryVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          oahTwoIntegersAtom (
                            const std::string& longName,
                            const std::string& shortName,
                            const std::string& description,
                            const std::string& valueSpecification,
                            const std::string& variableName,
                            int&               integerVariable,
                            int&               integerSecondaryVariable);

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
                            const std::string& theString,
                            std::ostream&      os) override;

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    std::string           asShortNamedOptionString () const override;
    std::string           asActualLongNamedOptionString () const override;

    void                  print (std::ostream& os) const override;

    void                  displayAtomWithVariableOptionsValues (
                            std::ostream& os,
                            int           valueFieldWidth) const override;
  private:

    // private fields
    // ------------------------------------------------------

    int&                  fIntegerSecondaryVariable;
};
typedef SMARTP<oahTwoIntegersAtom> S_oahTwoIntegersAtom;
EXP std::ostream& operator << (std::ostream& os, const S_oahTwoIntegersAtom& elt);

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
                            const std::string& longName,
                            const std::string& shortName,
                            const std::string& description,
                            const std::string& valueSpecification,
                            const std::string& variableName,
                            float&             floatVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          oahFloatAtom (
                            const std::string& longName,
                            const std::string& shortName,
                            const std::string& description,
                            const std::string& valueSpecification,
                            const std::string& variableName,
                            float&             floatVariable);

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
                            const std::string& theString,
                            std::ostream&      os) override;

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    std::string           asShortNamedOptionString () const override;
    std::string           asActualLongNamedOptionString () const override;

    void                  print (std::ostream& os) const override;

    void                  displayAtomWithVariableOptionsValues (
                            std::ostream& os,
                            int           valueFieldWidth) const override;

  private:

    // private fields
    // ------------------------------------------------------

    float&                fFloatVariable;
};
typedef SMARTP<oahFloatAtom> S_oahFloatAtom;
EXP std::ostream& operator << (std::ostream& os, const S_oahFloatAtom& elt);

//______________________________________________________________________________
class EXP oahStringAtom : public oahAtomStoringAValue
{
/*
  an atom controlling a std::string variable
*/

  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahStringAtom> create (
                            const std::string& longName,
                            const std::string& shortName,
                            const std::string& description,
                            const std::string& valueSpecification,
                            const std::string& variableName,
                            std::string&       stringVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          oahStringAtom (
                            const std::string& longName,
                            const std::string& shortName,
                            const std::string& description,
                            const std::string& valueSpecification,
                            const std::string& variableName,
                            std::string&       stringVariable);

    virtual               ~oahStringAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setStringVariable (const std::string& value);

    std::string           getStringVariable () const
                              { return fStringVariable; }

  public:

    // public services
    // ------------------------------------------------------

    void                  applyAtomWithValue (
                            const std::string& theString,
                            std::ostream&      os) override;

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    std::string           asShortNamedOptionString () const override;
    std::string           asActualLongNamedOptionString () const override;

    void                  print (std::ostream& os) const override;

    void                  displayAtomWithVariableOptionsValues (
                            std::ostream& os,
                            int           valueFieldWidth) const override;

  protected:

    // protected fields
    // ------------------------------------------------------

    std::string&          fStringVariable;
};
typedef SMARTP<oahStringAtom> S_oahStringAtom;
EXP std::ostream& operator << (std::ostream& os, const S_oahStringAtom& elt);

//______________________________________________________________________________
class EXP oahFactorizedStringAtom : public oahValueLessAtom
{
/*
  an atom factorizing a list of std::string atoms
  such as -set-variable-in-header in LilyPond generation
*/

  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahFactorizedStringAtom> create (
                            const std::string& longName,
                            const std::string& shortName,
                            const std::string& description,
                            const std::string& atomNameDescriptor,
                            const std::string& stringValueDescriptor);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          oahFactorizedStringAtom (
                            const std::string& longName,
                            const std::string& shortName,
                            const std::string& description,
                            const std::string& atomNameDescriptor,
                            const std::string& stringValueDescriptor);

    virtual               ~oahFactorizedStringAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    const std::list<S_oahStringAtom>&
                          getStringAtomsList () const
                              { return fStringAtomsList; }

  public:

    // public services
    // ------------------------------------------------------

    void                  addStringAtom (
                            const S_oahStringAtom& stringAtom);

    void                  addStringAtomByName (
                            const std::string& name);

    void                  applyValueLessAtom (std::ostream& os) override;

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

    void                  displayAtomWithVariableOptionsValues (
                            std::ostream& os,
                            int           valueFieldWidth) const override;

  private:

    // private fields
    // ------------------------------------------------------

    std::string           fAtomNameDescriptor;
    std::string           fStringValueDescriptor;

    std::list<S_oahStringAtom>
                          fStringAtomsList;

    std::list<std::string>
                          fAtomNamesList;
};
typedef SMARTP<oahFactorizedStringAtom> S_oahFactorizedStringAtom;
EXP std::ostream& operator << (std::ostream& os, const S_oahFactorizedStringAtom& elt);

//______________________________________________________________________________
class EXP oahDefaultedStringAtom : public oahValueDefaultedAtom
{
/*
  an atom controlling a std::string variable with a default value
*/

  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahDefaultedStringAtom> create (
                            const std::string& longName,
                            const std::string& shortName,
                            const std::string& description,
                            const std::string& valueSpecification,
                            const std::string& variableName,
                            std::string&       stringVariable,
                            const std::string& defaultStringValue);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          oahDefaultedStringAtom (
                            const std::string& longName,
                            const std::string& shortName,
                            const std::string& description,
                            const std::string& valueSpecification,
                            const std::string& variableName,
                            std::string&       stringVariable,
                            const std::string& defaultStringValue);

    virtual               ~oahDefaultedStringAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    std::string           getValueSpecification () const
                              { return fValueSpecification; }

    std::string           getVariableName () const
                              { return fVariableName; }

    void                  setStringVariable (const std::string& value);

    std::string           getStringVariable () const
                              { return fStringVariable; }

  public:

    // public services
    // ------------------------------------------------------

    void                  applyAtomWithValue (
                            const std::string& theString,
                            std::ostream&      os) override;

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    std::string           asShortNamedOptionString () const override;
    std::string           asActualLongNamedOptionString () const override;

    void                  print (std::ostream& os) const override;

    void                  displayAtomWithVariableOptionsValues (
                            std::ostream& os,
                            int           valueFieldWidth) const override;

  protected:

    // protected fields
    // ------------------------------------------------------

    std::string           fValueSpecification;

    std::string           fVariableName;
    std::string&          fStringVariable;
};
typedef SMARTP<oahDefaultedStringAtom> S_oahDefaultedStringAtom;
EXP std::ostream& operator << (std::ostream& os, const S_oahDefaultedStringAtom& elt);

//______________________________________________________________________________
class EXP oahCommonPrefixBooleansAtom : public oahValueLessAtom
// class EXP oahCommonPrefixBooleansAtom : public oahDefaultedStringAtom JMI ERROR
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
                            const std::string& longName,
                            const std::string& shortName,
                            const std::string& description,
                            const std::string& shortSuffixDescriptor,
                            const std::string& longSuffixDescriptor,
                            const S_oahPrefix& shortNamesPrefix,
                            const S_oahPrefix& longNamesPrefix);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          oahCommonPrefixBooleansAtom (
                            const std::string& longName,
                            const std::string& shortName,
                            const std::string& description,
                            const std::string& shortSuffixDescriptor,
                            const std::string& longSuffixDescriptor,
                            const S_oahPrefix& shortNamesPrefix,
                            const S_oahPrefix& longNamesPrefix);

    virtual               ~oahCommonPrefixBooleansAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    const std::list<S_oahBooleanAtom>&
                          getBooleanAtomsList () const
                              { return fBooleanAtomsList; }

  public:

    // public services
    // ------------------------------------------------------

    void                  addBooleanAtom (
                            const S_oahBooleanAtom& booleanAtom);

    void                  addBooleanAtomByName (
                            const std::string& name);

    void                  applyValueLessAtom (std::ostream& os) override;

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

    void                  displayAtomWithVariableOptionsValues (
                            std::ostream& os,
                            int           valueFieldWidth) const override;

  private:

    // private fields
    // ------------------------------------------------------

    S_oahPrefix           fShortNamesPrefix;
    S_oahPrefix           fLongNamesPrefix;

    std::string           fShortSuffixDescriptor;
    std::string           fLongSuffixDescriptor;

    std::list<S_oahBooleanAtom>
                          fBooleanAtomsList;

    std::string           fShortNamesPrefixName;
    std::string           fLongNamesPrefixName;

    std::list<std::string>

                          fShortNamesSuffixes;
    std::list<std::string>
                          fLongNamesSuffixes;
};
typedef SMARTP<oahCommonPrefixBooleansAtom> S_oahCommonPrefixBooleansAtom;
EXP std::ostream& operator << (std::ostream& os, const S_oahCommonPrefixBooleansAtom& elt);

//______________________________________________________________________________
class EXP oahRegexAtom : public oahStringAtom
{
/*
  an atom controlling a std::string variable with a default value
*/

  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahRegexAtom> create (
                            const std::string& longName,
                            const std::string& shortName,
                            const std::string& description,
                            const std::string& valueSpecification,
                            const std::string& variableName,
                            std::string&       stringVariable,
                            const std::string& regexString);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          oahRegexAtom (
                            const std::string& longName,
                            const std::string& shortName,
                            const std::string& description,
                            const std::string& valueSpecification,
                            const std::string& variableName,
                            std::string&       stringVariable,
                            const std::string& regexString);

    virtual               ~oahRegexAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setStringVariable (const std::string& value);

    std::string           getStringVariable () const
                              { return fStringVariable; }

  public:

    // public services
    // ------------------------------------------------------

    void                  applyAtomWithValue (
                            const std::string& theString,
                            std::ostream&      os) override;

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    std::string           asShortNamedOptionString () const override;
    std::string           asActualLongNamedOptionString () const override;

    void                  print (std::ostream& os) const override;

    void                  displayAtomWithVariableOptionsValues (
                            std::ostream& os,
                            int           valueFieldWidth) const override;

  protected:

    // protected fields
    // ------------------------------------------------------

    std::string           fRegexString;
};
typedef SMARTP<oahRegexAtom> S_oahRegexAtom;
EXP std::ostream& operator << (std::ostream& os, const S_oahRegexAtom& elt);

//______________________________________________________________________________
class EXP oahRationalAtom : public oahAtomStoringAValue
{
/*
  an atom controlling a mfRational variable
*/

  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahRationalAtom> create (
                            const std::string& longName,
                            const std::string& shortName,
                            const std::string& description,
                            const std::string& valueSpecification,
                            const std::string& variableName,
                            mfRational&          rationalVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          oahRationalAtom (
                            const std::string& longName,
                            const std::string& shortName,
                            const std::string& description,
                            const std::string& valueSpecification,
                            const std::string& variableName,
                            mfRational&          rationalVariable);

    virtual               ~oahRationalAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setRationalVariable (
                            const mfRational& value)
                              {
                                fRationalVariable = value;
                              }

    mfRational              getRationalVariable () const
                              { return fRationalVariable; }

  public:

    // public services
    // ------------------------------------------------------

    void                  applyAtomWithValue (
                            const std::string& theString,
                            std::ostream&      os) override;

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    std::string           asShortNamedOptionString () const override;
    std::string           asActualLongNamedOptionString () const override;

    void                  print (std::ostream& os) const override;

    void                  displayAtomWithVariableOptionsValues (
                            std::ostream& os,
                            int           valueFieldWidth) const override;

  private:

    // private fields
    // ------------------------------------------------------

    mfRational&             fRationalVariable;
};
typedef SMARTP<oahRationalAtom> S_oahRationalAtom;
EXP std::ostream& operator << (std::ostream& os, const S_oahRationalAtom& elt);

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
                            const std::string& longName,
                            const std::string& shortName,
                            const std::string& description,
                            const std::string& valueSpecification,
                            const std::string& variableName,
                            std::set<int>&     naturalNumbersSetVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          oahNaturalNumbersSetElementAtom (
                            const std::string& longName,
                            const std::string& shortName,
                            const std::string& description,
                            const std::string& valueSpecification,
                            const std::string& variableName,
                            std::set<int>&     naturalNumbersSetVariable);

    virtual               ~oahNaturalNumbersSetElementAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setNaturalNumbersSetVariable (
                            std::set<int>& value)
                              {
                                fNaturalNumbersSetVariable = value;
                              }

    const std::set<int>&       getNaturalNumbersSetVariable () const
                              { return fNaturalNumbersSetVariable; }

  public:

    // public services
    // ------------------------------------------------------

    void                  applyAtomWithValue (
                            const std::string& theString,
                            std::ostream&      os) override;

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    std::string           asShortNamedOptionString () const override;
    std::string           asActualLongNamedOptionString () const override;

    void                  print (std::ostream& os) const override;

    void                  displayAtomWithVariableOptionsValues (
                            std::ostream& os,
                            int           valueFieldWidth) const override;

  private:

    // private fields
    // ------------------------------------------------------

    std::set<int>&             fNaturalNumbersSetVariable;
};
typedef SMARTP<oahNaturalNumbersSetElementAtom> S_oahNaturalNumbersSetElementAtom;
EXP std::ostream& operator << (std::ostream& os, const S_oahNaturalNumbersSetElementAtom& elt);

//______________________________________________________________________________
class EXP oahColorRGBAtom : public oahAtomStoringAValue
{
/*
  an atom controlling an RGB color variable
*/

  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahColorRGBAtom> create (
                            const std::string& shortName,
                            const std::string& longName,
                            const std::string& description,
                            const std::string& valueSpecification,
                            const std::string& variableName,
                            msrColorRGB&       colorRGBVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          oahColorRGBAtom (
                            const std::string& shortName,
                            const std::string& longName,
                            const std::string& description,
                            const std::string& valueSpecification,
                            const std::string& variableName,
                            msrColorRGB&       colorRGBVariable);

    virtual               ~oahColorRGBAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setColorRGBVariable (
                            msrColorRGB& value)
                              {
                                fColorRGBVariable = value;
                              }

    const msrColorRGB&    getColorRGBVariable () const
                              { return fColorRGBVariable; }

  public:

    // public services
    // ------------------------------------------------------

    void                  applyAtomWithValue (
                            const std::string& theString,
                            std::ostream&      os) override;

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    std::string           asShortNamedOptionString () const override;
    std::string           asActualLongNamedOptionString () const override;

    void                  print (std::ostream& os) const override;

    void                  displayAtomWithVariableOptionsValues (
                            std::ostream& os,
                            int           valueFieldWidth) const override;

  private:

    // private fields
    // ------------------------------------------------------

    msrColorRGB&          fColorRGBVariable;
};
typedef SMARTP<oahColorRGBAtom> S_oahColorRGBAtom;
EXP std::ostream& operator << (std::ostream& os, const S_oahColorRGBAtom& elt);

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
                            const std::string& longName,
                            const std::string& shortName,
                            const std::string& description,
                            const std::string& valueSpecification,
                            const std::string& variableName,
                            std::set<int>&     intSetVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          oahIntSetElementAtom (
                            const std::string& longName,
                            const std::string& shortName,
                            const std::string& description,
                            const std::string& valueSpecification,
                            const std::string& variableName,
                            std::set<int>&     intSetVariable);

    virtual               ~oahIntSetElementAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    const std::set<int>&       getIntSetVariable () const
                              { return fIntSetVariable; }

  public:

    // public services
    // ------------------------------------------------------

    void                  applyAtomWithValue (
                            const std::string& theString,
                            std::ostream&      os) override;

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    std::string           asShortNamedOptionString () const override;
    std::string           asActualLongNamedOptionString () const override;

    void                  print (std::ostream& os) const override;

    void                  displayAtomWithVariableOptionsValues (
                            std::ostream& os,
                            int           valueFieldWidth) const override;

  private:

    // private fields
    // ------------------------------------------------------

    std::set<int>&        fIntSetVariable;
};
typedef SMARTP<oahIntSetElementAtom> S_oahIntSetElementAtom;
EXP std::ostream& operator << (std::ostream& os, const S_oahIntSetElementAtom& elt);

//______________________________________________________________________________
class EXP oahStringSetElementAtom : public oahAtomStoringAValue
{
/*
  an atom controlling a set of std::string variables
*/

  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahStringSetElementAtom> create (
                            const std::string&     longName,
                            const std::string&     shortName,
                            const std::string&     description,
                            const std::string&     valueSpecification,
                            const std::string&     variableName,
                            std::set<std::string>& stringSetVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          oahStringSetElementAtom (
                            const std::string&     longName,
                            const std::string&     shortName,
                            const std::string&     description,
                            const std::string&     valueSpecification,
                            const std::string&     variableName,
                            std::set<std::string>& stringSetVariable);

    virtual               ~oahStringSetElementAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setStringSetVariable (
                            const std::string& partName)
                              {
                                fStringSetVariable.insert (partName);
                              }

    const std::set<std::string>&
                          getStringSetVariable () const
                              { return fStringSetVariable; }

  public:

    // public services
    // ------------------------------------------------------

    void                  applyAtomWithValue (
                            const std::string& theString,
                            std::ostream&      os) override;

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    std::string           asShortNamedOptionString () const override;
    std::string           asActualLongNamedOptionString () const override;

    void                  print (std::ostream& os) const override;

    void                  displayAtomWithVariableOptionsValues (
                            std::ostream& os,
                            int           valueFieldWidth) const override;

  private:

    // private fields
    // ------------------------------------------------------

    std::set<std::string>&
                          fStringSetVariable;
};
typedef SMARTP<oahStringSetElementAtom> S_oahStringSetElementAtom;
EXP std::ostream& operator << (std::ostream& os, const S_oahStringSetElementAtom& elt);

//______________________________________________________________________________
class EXP oahStringToIntMapElementAtom : public oahAtomStoringAValue
{
/*
  an atom controlling a std::string to integer mapping variable
*/

  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahStringToIntMapElementAtom> create (
                            const std::string&          longName,
                            const std::string&          shortName,
                            const std::string&          description,
                            const std::string&          valueSpecification,
                            const std::string&          variableName,
                            std::map<std::string, int>& stringToIntMapVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          oahStringToIntMapElementAtom (
                            const std::string&          longName,
                            const std::string&          shortName,
                            const std::string&          description,
                            const std::string&          valueSpecification,
                            const std::string&          variableName,
                            std::map<std::string, int>& stringToIntMapVariable);

    virtual               ~oahStringToIntMapElementAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    const std::map<std::string, int>&
                          getStringToIntMapVariable () const
                              { return fStringToIntMapVariable; }

  public:

    // public services
    // ------------------------------------------------------

    void                  applyAtomWithValue (
                            const std::string& theString,
                            std::ostream&      os) override;

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    std::string           asShortNamedOptionString () const override;
    std::string           asActualLongNamedOptionString () const override;

    void                  print (std::ostream& os) const override;

    void                  displayAtomWithVariableOptionsValues (
                            std::ostream& os,
                            int           valueFieldWidth) const override;

  private:

    // private fields
    // ------------------------------------------------------

    std::map<std::string, int>&
                          fStringToIntMapVariable;
};
typedef SMARTP<oahStringToIntMapElementAtom> S_oahStringToIntMapElementAtom;
EXP std::ostream& operator << (std::ostream& os, const S_oahStringToIntMapElementAtom& elt);

//______________________________________________________________________________
class EXP oahStringToStringMapElementAtom : public oahAtomStoringAValue
{
/*
  an atom controlling a std::string to std::string mapping variable
*/

  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahStringToStringMapElementAtom> create (
                            const std::string&                  longName,
                            const std::string&                  shortName,
                            const std::string&                  description,
                            const std::string&                  valueSpecification,
                            const std::string&                  variableName,
                            std::map<std::string, std::string>& stringToStringMapVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          oahStringToStringMapElementAtom (
                            const std::string&                  longName,
                            const std::string&                  shortName,
                            const std::string&                  description,
                            const std::string&                  valueSpecification,
                            const std::string&                  variableName,
                            std::map<std::string, std::string>& stringToStringMapVariable);

    virtual               ~oahStringToStringMapElementAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    const std::map<std::string, std::string>&
                          getStringToStringMapVariable () const
                              { return fStringToStringMapVariable; }

  public:

    // public services
    // ------------------------------------------------------

    void                  applyAtomWithValue (
                            const std::string& theString,
                            std::ostream&      os) override;

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    std::string           asShortNamedOptionString () const override;
    std::string           asActualLongNamedOptionString () const override;

    void                  print (std::ostream& os) const override;

    void                  displayAtomWithVariableOptionsValues (
                            std::ostream& os,
                            int           valueFieldWidth) const override;

  private:

    // private fields
    // ------------------------------------------------------

    std::map<std::string, std::string>&
                          fStringToStringMapVariable;
};
typedef SMARTP<oahStringToStringMapElementAtom> S_oahStringToStringMapElementAtom;
EXP std::ostream& operator << (std::ostream& os, const S_oahStringToStringMapElementAtom& elt);

//______________________________________________________________________________
class EXP oahStringToStringMultiMapElementAtom : public oahAtomStoringAValue
{
/*
  an atom controlling a std::string to std::string multiple mapping variable
*/

  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahStringToStringMultiMapElementAtom> create (
                            const std::string&                       longName,
                            const std::string&                       shortName,
                            const std::string&                       description,
                            const std::string&                       valueSpecification,
                            const std::string&                       variableName,
                            std::multimap<std::string, std::string>& stringToStringMultiMapVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          oahStringToStringMultiMapElementAtom (
                            const std::string&                       longName,
                            const std::string&                       shortName,
                            const std::string&                       description,
                            const std::string&                       valueSpecification,
                            const std::string&                       variableName,
                            std::multimap<std::string, std::string>& stringToStringMultiMapVariable);

    virtual               ~oahStringToStringMultiMapElementAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    const std::multimap<std::string, std::string>&
                          getStringToStringMultiMapVariable () const
                              { return fStringToStringMultiMapVariable; }

  public:

    // public services
    // ------------------------------------------------------

    void                  applyAtomWithValue (
                            const std::string& theString,
                            std::ostream&      os) override;

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    std::string           asShortNamedOptionString () const override;
    std::string           asActualLongNamedOptionString () const override;

    void                  print (std::ostream& os) const override;

    void                  displayAtomWithVariableOptionsValues (
                            std::ostream& os,
                            int           valueFieldWidth) const override;

  private:

    // private fields
    // ------------------------------------------------------

    std::multimap<std::string, std::string>&
                          fStringToStringMultiMapVariable;
};
typedef SMARTP<oahStringToStringMultiMapElementAtom> S_oahStringToStringMultiMapElementAtom;
EXP std::ostream& operator << (std::ostream& os, const S_oahStringToStringMultiMapElementAtom& elt);

//______________________________________________________________________________
class EXP oahStringAndIntegerAtom : public oahAtomStoringAValue
{
/*
  an atom controlling a std::string and an integer variables
*/

  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahStringAndIntegerAtom> create (
                            const std::string& longName,
                            const std::string& shortName,
                            const std::string& description,
                            const std::string& valueSpecification,
                            const std::string& stringVariableName,
                            std::string&       stringVariable,
                            const std::string& integerVariableName,
                            int&               integerVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          oahStringAndIntegerAtom (
                            const std::string& longName,
                            const std::string& shortName,
                            const std::string& description,
                            const std::string& valueSpecification,
                            const std::string& stringVariableName,
                            std::string&       stringVariable,
                            const std::string& integerVariableName,
                            int&               integerVariable);

    virtual               ~oahStringAndIntegerAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setStringVariable (const std::string& value);

    std::string           getStringVariable () const
                              { return fStringVariable; }

    void                  setIntegerVariable (int value);

    int                   getIntegerVariable () const
                              { return fIntegerVariable; }

  public:

    // public services
    // ------------------------------------------------------

    void                  applyAtomWithValue (
                            const std::string& theString,
                            std::ostream&      os) override;

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    std::string           asShortNamedOptionString () const override;
    std::string           asActualLongNamedOptionString () const override;

    void                  print (std::ostream& os) const override;

    void                  displayAtomWithVariableOptionsValues (
                            std::ostream& os,
                            int           valueFieldWidth) const override;

  private:

    // private fields
    // ------------------------------------------------------

    std::string           fStringVariableName;
    std::string&          fStringVariable;

    std::string           fIntegerVariableName;
    int&                  fIntegerVariable;
};
typedef SMARTP<oahStringAndIntegerAtom> S_oahStringAndIntegerAtom;
EXP std::ostream& operator << (std::ostream& os, const S_oahStringAndIntegerAtom& elt);

//______________________________________________________________________________
class EXP oahStringAndTwoIntegersAtom : public oahAtomStoringAValue
{
/*
  an atom controlling a std::string and two integer variables
*/

  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahStringAndTwoIntegersAtom> create (
                            const std::string&  shortName,
                            const std::string&  longName,
                            const std::string&  description,
                            const std::string&  valueSpecification,
                            const std::string&  variableName,
                            std::string&        stringVariable,
                            int&                primaryIntegerVariable,
                            int&                secondaryIntegerVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          oahStringAndTwoIntegersAtom (
                            const std::string&  shortName,
                            const std::string&  longName,
                            const std::string&  description,
                            const std::string&  valueSpecification,
                            const std::string&  variableName,
                            std::string&        stringVariable,
                            int&                primaryIntegerVariable,
                            int&                secondaryIntegerVariable);

    virtual               ~oahStringAndTwoIntegersAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setStringVariable (const std::string& value);

    std::string           getStringVariable () const
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
                            const std::string& theString,
                            std::ostream&      os) override;

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    std::string           asShortNamedOptionString () const override;
    std::string           asActualLongNamedOptionString () const override;

    void                  print (std::ostream& os) const override;

    void                  displayAtomWithVariableOptionsValues (
                            std::ostream& os,
                            int           valueFieldWidth) const override;

  private:

    // private fields
    // ------------------------------------------------------

    std::string&          fStringVariable;

    int&                  fPrimaryIntegerVariable;
    int&                  fSecondaryIntegerVariable;
};
typedef SMARTP<oahStringAndTwoIntegersAtom> S_oahStringAndTwoIntegersAtom;
EXP std::ostream& operator << (std::ostream& os, const S_oahStringAndTwoIntegersAtom& elt);

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
                            const std::string& longName,
                            const std::string& shortName,
                            const std::string& description,
                            const std::string& valueSpecification,
                            const std::string& variableName,
                            msrLengthUnitKind& lengthUnitKindVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          oahLengthUnitKindAtom (
                            const std::string& longName,
                            const std::string& shortName,
                            const std::string& description,
                            const std::string& valueSpecification,
                            const std::string& variableName,
                            msrLengthUnitKind& lengthUnitKindVariable);

    virtual               ~oahLengthUnitKindAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setLengthUnitKindVariable (
                            msrLengthUnitKind value)
                              {
                                fLengthUnitKindVariable = value;
                              }

    msrLengthUnitKind     getLengthUnitKindVariable () const
                              { return fLengthUnitKindVariable; }

  public:

    // public services
    // ------------------------------------------------------

    void                  applyAtomWithValue (
                            const std::string& theString,
                            std::ostream&      os) override;

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    std::string           asShortNamedOptionString () const override;
    std::string           asActualLongNamedOptionString () const override;

    void                  print (std::ostream& os) const override;

    void                  displayAtomWithVariableOptionsValues (
                            std::ostream& os,
                            int           valueFieldWidth) const override;

  private:

    // private fields
    // ------------------------------------------------------

    msrLengthUnitKind&    fLengthUnitKindVariable;
};
typedef SMARTP<oahLengthUnitKindAtom> S_oahLengthUnitKindAtom;
EXP std::ostream& operator << (std::ostream& os, const S_oahLengthUnitKindAtom& elt);

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
                            const std::string& longName,
                            const std::string& shortName,
                            const std::string& description,
                            const std::string& valueSpecification,
                            const std::string& variableName,
                            msrLength&         lengthVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          oahLengthAtom (
                            const std::string& longName,
                            const std::string& shortName,
                            const std::string& description,
                            const std::string& valueSpecification,
                            const std::string& variableName,
                            msrLength&         lengthVariable);

    virtual               ~oahLengthAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setLengthVariable (
                            msrLength value)
                              {
                                fLengthVariable = value;
                              }

    msrLength             getLengthVariable () const
                              { return fLengthVariable; }

  public:

    // public services
    // ------------------------------------------------------

    void                  applyAtomWithValue (
                            const std::string& theString,
                            std::ostream&      os) override;

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    std::string           asShortNamedOptionString () const override;
    std::string           asActualLongNamedOptionString () const override;

    void                  print (std::ostream& os) const override;

    void                  displayAtomWithVariableOptionsValues (
                            std::ostream& os,
                            int           valueFieldWidth) const override;

  private:

    // private fields
    // ------------------------------------------------------

    msrLength&            fLengthVariable;
};
typedef SMARTP<oahLengthAtom> S_oahLengthAtom;
EXP std::ostream& operator << (std::ostream& os, const S_oahLengthAtom& elt);

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
                            const std::string& longName,
                            const std::string& shortName,
                            const std::string& description,
                            const std::string& valueSpecification,
                            const std::string& variableName,
                            msrMidiTempo&      midiTempoVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          oahMidiTempoAtom (
                            const std::string& longName,
                            const std::string& shortName,
                            const std::string& description,
                            const std::string& valueSpecification,
                            const std::string& variableName,
                            msrMidiTempo&      midiTempoVariable);

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
                            const std::string& theString,
                            std::ostream&      os) override;

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    std::string           asShortNamedOptionString () const override;
    std::string           asActualLongNamedOptionString () const override;

    void                  print (std::ostream& os) const override;

    void                  displayAtomWithVariableOptionsValues (
                            std::ostream& os,
                            int           valueFieldWidth) const override;

  private:

    // private fields
    // ------------------------------------------------------

    msrMidiTempo&         fMidiTempoVariable;
};
typedef SMARTP<oahMidiTempoAtom> S_oahMidiTempoAtom;
EXP std::ostream& operator << (std::ostream& os, const S_oahMidiTempoAtom& elt);

//______________________________________________________________________________
class EXP oahOptionNameHelpAtom : public oahDefaultedStringAtom
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
                            const std::string& longName,
                            const std::string& shortName,
                            const std::string& description,
                            const std::string& valueSpecification,
                            const std::string& variableName,
                            std::string&       stringVariable,
                            const std::string& defaultOptionName);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          oahOptionNameHelpAtom (
                            const std::string& longName,
                            const std::string& shortName,
                            const std::string& description,
                            const std::string& valueSpecification,
                            const std::string& variableName,
                            std::string&       stringVariable,
                            const std::string& defaultOptionName);

  protected:

    virtual               ~oahOptionNameHelpAtom ();

  public:

    // set and get
    // ------------------------------------------------------

  public:

    // public services
    // ------------------------------------------------------

    void                  applyAtomWithValue (
                            const std::string& theString,
                            std::ostream&      os) override;

    void                  applyAtomWithDefaultValue (std::ostream& os) override;

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    std::string           asShortNamedOptionString () const override;
    std::string           asActualLongNamedOptionString () const override;

    void                  print (std::ostream& os) const override;

    virtual void          displayAtomWithVariableOptionsValues (
                            std::ostream& os,
                            int           valueFieldWidth) const override;
  private:

    // private fields
    // ------------------------------------------------------

//    std::string           fValueSpecification;
};
typedef SMARTP<oahOptionNameHelpAtom> S_oahOptionNameHelpAtom;
EXP std::ostream& operator << (std::ostream& os, const S_oahOptionNameHelpAtom& elt);

//______________________________________________________________________________
class EXP oahQueryOptionNameAtom : public oahPureHelpValueFittedAtom
{
/*
  This is where OAH is introspective too
*/

  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahQueryOptionNameAtom> create (
                            const std::string& longName,
                            const std::string& shortName,
                            const std::string& description,
                            const std::string& valueSpecification,
                            const std::string& serviceName);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          oahQueryOptionNameAtom (
                            const std::string& longName,
                            const std::string& shortName,
                            const std::string& description,
                            const std::string& valueSpecification,
                            const std::string& serviceName);

    virtual               ~oahQueryOptionNameAtom ();

  public:

    // set and get
    // ------------------------------------------------------

  public:

    // public services
    // ------------------------------------------------------

    void                  applyAtomWithValue (
                            const std::string& theString,
                            std::ostream&      os) override;

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    std::string           asShortNamedOptionString () const override;
    std::string           asActualLongNamedOptionString () const override;

    void                  print (std::ostream& os) const override;

    virtual void          displayAtomWithVariableOptionsValues (
                            std::ostream& os,
                            int           valueFieldWidth) const override;
  private:

    // private fields
    // ------------------------------------------------------
};
typedef SMARTP<oahQueryOptionNameAtom> S_oahQueryOptionNameAtom;
EXP std::ostream& operator << (std::ostream& os, const S_oahQueryOptionNameAtom& elt);

//______________________________________________________________________________
class EXP oahFindStringResult : public smartable // JMI ???
{
/*
  This is where OAH is introspective too
*/

  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahFindStringResult> create (
                            const std::string&  theString,
                            const S_oahElement& theElement);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          oahFindStringResult (
                            const std::string&  theString,
                            const S_oahElement& theElement);

    virtual               ~oahFindStringResult ();

  public:

    // set and get
    // ------------------------------------------------------

  public:

    // public services
    // ------------------------------------------------------

  public:

    // print
    // ------------------------------------------------------

    std::string           asString () const;

    void                  print (std::ostream& os) const;

  private:

    // private fields
    // ------------------------------------------------------

    std::string           fTheString;
    S_oahElement          fTheElement;
};
typedef SMARTP<oahFindStringResult> S_oahFindStringResult;
EXP std::ostream& operator << (std::ostream& os, const oahFindStringResult& elt);
EXP std::ostream& operator << (std::ostream& os, const S_oahFindStringResult& elt);

//______________________________________________________________________________
class EXP oahFindStringAtom : public oahPureHelpValueFittedAtom
{
/*
  This is where OAH is introspective too
*/

  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahFindStringAtom> create (
                            const std::string& longName,
                            const std::string& shortName,
                            const std::string& description,
                            const std::string& valueSpecification,
                            const std::string& serviceName);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          oahFindStringAtom (
                            const std::string& longName,
                            const std::string& shortName,
                            const std::string& description,
                            const std::string& valueSpecification,
                            const std::string& serviceName);

    virtual               ~oahFindStringAtom ();

  public:

    // set and get
    // ------------------------------------------------------

  public:

    // public services
    // ------------------------------------------------------

    void                  applyAtomWithValue (
                            const std::string& theString,
                            std::ostream&      os) override;

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    std::string           asShortNamedOptionString () const override;
    std::string           asActualLongNamedOptionString () const override;

    void                  print (std::ostream& os) const override;

    virtual void          displayAtomWithVariableOptionsValues (
                            std::ostream& os,
                            int           valueFieldWidth) const override;
  private:

    // private fields
    // ------------------------------------------------------
};
typedef SMARTP<oahFindStringAtom> S_oahFindStringAtom;
EXP std::ostream& operator << (std::ostream& os, const S_oahFindStringAtom& elt);

//______________________________________________________________________________
class EXP oahFilterStringAtom : public oahPureHelpValueFittedAtom
{
/*
  This is where OAH is introspective too
*/

  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahFilterStringAtom> create (
                            const std::string& longName,
                            const std::string& shortName,
                            const std::string& description,
                            const std::string& valueSpecification,
                            const std::string& serviceName);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          oahFilterStringAtom (
                            const std::string& longName,
                            const std::string& shortName,
                            const std::string& description,
                            const std::string& valueSpecification,
                            const std::string& serviceName);

    virtual               ~oahFilterStringAtom ();

  public:

    // set and get
    // ------------------------------------------------------

  public:

    // public services
    // ------------------------------------------------------

    void                  applyAtomWithValue (
                            const std::string& theString,
                            std::ostream&      os) override;

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    std::string           asShortNamedOptionString () const override;
    std::string           asActualLongNamedOptionString () const override;

    void                  print (std::ostream& os) const override;

    virtual void          displayAtomWithVariableOptionsValues (
                            std::ostream& os,
                            int           valueFieldWidth) const override;
  private:

    // private fields
    // ------------------------------------------------------
};
typedef SMARTP<oahFilterStringAtom> S_oahFilterStringAtom;
EXP std::ostream& operator << (std::ostream& os, const S_oahFilterStringAtom& elt);

//______________________________________________________________________________
class EXP oahIncludeOptionsAndArgumentsFileAtom : public oahValueFittedAtom
{
/*
  This is where OAH is introspective too
*/

  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahIncludeOptionsAndArgumentsFileAtom> create (
                            const std::string& longName,
                            const std::string& shortName,
                            const std::string& description,
                            const std::string& valueSpecification,
                            const std::string& serviceName);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          oahIncludeOptionsAndArgumentsFileAtom (
                            const std::string& longName,
                            const std::string& shortName,
                            const std::string& description,
                            const std::string& valueSpecification,
                            const std::string& serviceName);

    virtual               ~oahIncludeOptionsAndArgumentsFileAtom ();

  public:

    // set and get
    // ------------------------------------------------------

  public:

    // public services
    // ------------------------------------------------------

    void                  applyAtomWithValue (
                            const std::string& theString,
                            std::ostream&      os) override;

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    std::string           asShortNamedOptionString () const override;
    std::string           asActualLongNamedOptionString () const override;

    void                  print (std::ostream& os) const override;

    virtual void          displayAtomWithVariableOptionsValues (
                            std::ostream& os,
                            int           valueFieldWidth) const override;
  private:

    // private fields
    // ------------------------------------------------------
};
typedef SMARTP<oahIncludeOptionsAndArgumentsFileAtom> S_oahIncludeOptionsAndArgumentsFileAtom;
EXP std::ostream& operator << (std::ostream& os, const S_oahIncludeOptionsAndArgumentsFileAtom& elt);


}


#endif // ___oahAtomsCollection___
