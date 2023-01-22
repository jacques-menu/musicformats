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


#include "mfStaticSettings.h"

#ifdef MF_HARMONIES_EXTRA_IS_ENABLED


#include "msrPitches.h"

#include "oahBasicTypes.h"


namespace MusicFormats
{

//______________________________________________________________________________
class EXP extraDisplayAllHarmoniesStructuresAtom : public oahValueLessAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<extraDisplayAllHarmoniesStructuresAtom> create (
                            const std::string& shortName,
                            const std::string& longName,
                            const std::string& description);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    extraDisplayAllHarmoniesStructuresAtom (
                            const std::string& shortName,
                            const std::string& longName,
                            const std::string& description);

    virtual               ~extraDisplayAllHarmoniesStructuresAtom ();

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

    void                  printAllHarmoniesStructures (std::ostream& os) const;

    void                  displayAtomWithVariableOptionsValues (
                            std::ostream& os,
                            int           valueFieldWidth) const override;

  private:

    // private fields
    // ------------------------------------------------------
};
typedef SMARTP<extraDisplayAllHarmoniesStructuresAtom> S_extraDisplayAllHarmoniesStructuresAtom;
EXP std::ostream& operator << (std::ostream& os, const S_extraDisplayAllHarmoniesStructuresAtom& elt);

//______________________________________________________________________________
class EXP extraDisplayAllHarmoniesContentsAtom : public oahAtomStoringAValue
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<extraDisplayAllHarmoniesContentsAtom> create (
                            const std::string& shortName,
                            const std::string& longName,
                            const std::string& description,
                            const std::string& valueSpecification,
                            const std::string& variableName,
                            std::string&       stringVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    extraDisplayAllHarmoniesContentsAtom (
                            const std::string& shortName,
                            const std::string& longName,
                            const std::string& description,
                            const std::string& valueSpecification,
                            const std::string& variableName,
                            std::string&       stringVariable);

    virtual               ~extraDisplayAllHarmoniesContentsAtom ();

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

    void                  displayAtomWithVariableOptionsValues (
                            std::ostream& os,
                            int           valueFieldWidth) const override;

  private:

    // private fields
    // ------------------------------------------------------

    std::string&          fStringVariable;
};
typedef SMARTP<extraDisplayAllHarmoniesContentsAtom> S_extraDisplayAllHarmoniesContentsAtom;
EXP std::ostream& operator << (std::ostream& os, const S_extraDisplayAllHarmoniesContentsAtom& elt);

//______________________________________________________________________________
class EXP extraDisplayHarmonyDetailsAtom : public oahAtomStoringAValue
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<extraDisplayHarmonyDetailsAtom> create (
                            const std::string& shortName,
                            const std::string& longName,
                            const std::string& description,
                            const std::string& valueSpecification,
                            const std::string& variableName,
                            std::string&       stringVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    extraDisplayHarmonyDetailsAtom (
                            const std::string& shortName,
                            const std::string& longName,
                            const std::string& description,
                            const std::string& valueSpecification,
                            const std::string& variableName,
                            std::string&       stringVariable);

    virtual               ~extraDisplayHarmonyDetailsAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setDisplayHarmonyDetailsVariableValue (
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

    void                  displayAtomWithVariableOptionsValues (
                            std::ostream& os,
                            int           valueFieldWidth) const override;

  private:

    // private fields
    // ------------------------------------------------------

    std::string&          fStringVariable;
};
typedef SMARTP<extraDisplayHarmonyDetailsAtom> S_extraDisplayHarmonyDetailsAtom;
EXP std::ostream& operator << (std::ostream& os, const S_extraDisplayHarmonyDetailsAtom& elt);

//______________________________________________________________________________
class EXP extraDisplayHarmonyAnalysisAtom : public oahAtomStoringAValue
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<extraDisplayHarmonyAnalysisAtom> create (
                            const std::string& shortName,
                            const std::string& longName,
                            const std::string& description,
                            const std::string& valueSpecification,
                            const std::string& variableName,
                            std::string&       stringVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    extraDisplayHarmonyAnalysisAtom (
                            const std::string& shortName,
                            const std::string& longName,
                            const std::string& description,
                            const std::string& valueSpecification,
                            const std::string& variableName,
                            std::string&       stringVariable);

    virtual               ~extraDisplayHarmonyAnalysisAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setDisplayHarmonyAnalysisVariableValue (
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

    void                  displayAtomWithVariableOptionsValues (
                            std::ostream& os,
                            int           valueFieldWidth) const override;

  private:

    // private fields
    // ------------------------------------------------------

    std::string&          fStringVariable;
};
typedef SMARTP<extraDisplayHarmonyAnalysisAtom> S_extraDisplayHarmonyAnalysisAtom;
EXP std::ostream& operator << (std::ostream& os, const S_extraDisplayHarmonyAnalysisAtom& elt);

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

    void                  initializeExtraDisplayAllHarmoniesStructuresOptions ();

    void                  initializeExtraDisplayAllHarmoniesContentsOptions ();

    void                  initializeExtraDisplayHarmonyDetailsOptions ();

    void                  initializeExtraDisplayHarmonyAnalysisOptions ();

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

    void                  displayHarmoniesExtraOahValues (int fieldWidth);

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
