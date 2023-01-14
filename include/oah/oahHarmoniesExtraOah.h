/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___harmoniesExtraOah___
#define ___harmoniesExtraOah___


#include "mfEnableHarmoniesExtraSetting.h"

#ifdef MF_HARMONIES_EXTRA_IS_ENABLED


#include "msrPitches.h"

#include "oahBasicTypes.h"


namespace MusicFormats
{

//______________________________________________________________________________
class EXP extraShowAllHarmoniesStructuresAtom : public oahAtomStoringAValue
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<extraShowAllHarmoniesStructuresAtom> create (
                            const std::string& shortName,
                            const std::string& longName,
                            const std::string& description);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    extraShowAllHarmoniesStructuresAtom (
                            const std::string& shortName,
                            const std::string& longName,
                            const std::string& description);

    virtual               ~extraShowAllHarmoniesStructuresAtom ();

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

    void                  print (std::ostream& os) const override;

    void                  printAllHarmoniesStructures (std::ostream& os) const;

    void                  printAtomWithVariableOptionsValues (
                            std::ostream& os,
                            int           valueFieldWidth) const override;

  private:

    // private fields
    // ------------------------------------------------------

    //  JMI ???
};
typedef SMARTP<extraShowAllHarmoniesStructuresAtom> S_extraShowAllHarmoniesStructuresAtom;
EXP std::ostream& operator << (std::ostream& os, const S_extraShowAllHarmoniesStructuresAtom& elt);

//______________________________________________________________________________
class EXP extraShowAllHarmoniesContentsAtom : public oahAtomStoringAValue
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<extraShowAllHarmoniesContentsAtom> create (
                            const std::string& shortName,
                            const std::string& longName,
                            const std::string& description,
                            const std::string& valueSpecification,
                            const std::string& variableName,
                            std::string&       stringVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    extraShowAllHarmoniesContentsAtom (
                            const std::string& shortName,
                            const std::string& longName,
                            const std::string& description,
                            const std::string& valueSpecification,
                            const std::string& variableName,
                            std::string&       stringVariable);

    virtual               ~extraShowAllHarmoniesContentsAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setStringVariable (const std::string& value)
                              { fStringVariable = value; }

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

    void                  printAllHarmoniesContents (
                            std::ostream&              os,
                            msrSemiTonesPitchKind semiTonesPitchKind) const;

    void                  printAtomWithVariableOptionsValues (
                            std::ostream& os,
                            int           valueFieldWidth) const override;

  private:

    // private fields
    // ------------------------------------------------------

    std::string&               fStringVariable;
};
typedef SMARTP<extraShowAllHarmoniesContentsAtom> S_extraShowAllHarmoniesContentsAtom;
EXP std::ostream& operator << (std::ostream& os, const S_extraShowAllHarmoniesContentsAtom& elt);

//______________________________________________________________________________
class EXP extraShowHarmonyDetailsAtom : public oahAtomStoringAValue
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<extraShowHarmonyDetailsAtom> create (
                            const std::string& shortName,
                            const std::string& longName,
                            const std::string& description,
                            const std::string& valueSpecification,
                            const std::string& variableName,
                            std::string&       stringVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    extraShowHarmonyDetailsAtom (
                            const std::string& shortName,
                            const std::string& longName,
                            const std::string& description,
                            const std::string& valueSpecification,
                            const std::string& variableName,
                            std::string&       stringVariable);

    virtual               ~extraShowHarmonyDetailsAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setShowHarmonyDetailsVariableValue (
                            const std::string& value)
                              {  fStringVariable = value; }

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

    void                  printAtomWithVariableOptionsValues (
                            std::ostream& os,
                            int           valueFieldWidth) const override;

  private:

    // private fields
    // ------------------------------------------------------

    std::string&               fStringVariable;
};
typedef SMARTP<extraShowHarmonyDetailsAtom> S_extraShowHarmonyDetailsAtom;
EXP std::ostream& operator << (std::ostream& os, const S_extraShowHarmonyDetailsAtom& elt);

//______________________________________________________________________________
class EXP extraShowHarmonyAnalysisAtom : public oahAtomStoringAValue
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<extraShowHarmonyAnalysisAtom> create (
                            const std::string& shortName,
                            const std::string& longName,
                            const std::string& description,
                            const std::string& valueSpecification,
                            const std::string& variableName,
                            std::string&       stringVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    extraShowHarmonyAnalysisAtom (
                            const std::string& shortName,
                            const std::string& longName,
                            const std::string& description,
                            const std::string& valueSpecification,
                            const std::string& variableName,
                            std::string&       stringVariable);

    virtual               ~extraShowHarmonyAnalysisAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setShowHarmonyAnalysisVariableValue (
                            const std::string& value)
                              { fStringVariable = value; }

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

    void                  printAtomWithVariableOptionsValues (
                            std::ostream& os,
                            int           valueFieldWidth) const override;

  private:

    // private fields
    // ------------------------------------------------------

    std::string&               fStringVariable;
};
typedef SMARTP<extraShowHarmonyAnalysisAtom> S_extraShowHarmonyAnalysisAtom;
EXP std::ostream& operator << (std::ostream& os, const S_extraShowHarmonyAnalysisAtom& elt);

//______________________________________________________________________________
class EXP harmoniesExtraOahGroup : public oahGroup
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<harmoniesExtraOahGroup> create ();

  private:

    // initialisation
    // ------------------------------------------------------

    void                  initializeHarmoniesExtraOahGroup ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    harmoniesExtraOahGroup ();

    virtual               ~harmoniesExtraOahGroup ();

  public:

    // public services
    // ------------------------------------------------------

    // quiet mode
    void                  enforceGroupQuietness () override;

    // consistency check
    void                  checkGroupOptionsConsistency () override;

  private:

    // private services
    // ------------------------------------------------------

    void                  initializeExtraShowAllHarmoniesStructuresOptions ();

    void                  initializeExtraShowAllHarmoniesContentsOptions ();

    void                  initializeExtraShowHarmonyDetailsOptions ();

    void                  initializeExtraShowHarmonyAnalysisOptions ();

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    void                  printHarmoniesExtraOahHelp ();

    void                  printHarmoniesExtraOahValues (int fieldWidth);

  public:

    // fields
    // ------------------------------------------------------

    std::string           fHarmoniesRootAsString;
};
typedef SMARTP<harmoniesExtraOahGroup> S_harmoniesExtraOahGroup;
EXP std::ostream& operator << (std::ostream& os, const S_harmoniesExtraOahGroup& elt);

EXP extern S_harmoniesExtraOahGroup gGlobalHarmoniesExtraOahGroup;

//______________________________________________________________________________
EXP S_harmoniesExtraOahGroup createGlobalHarmoniesExtraOahGroup ();


}


#endif


#endif
