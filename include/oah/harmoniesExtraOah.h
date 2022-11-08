/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___harmoniesExtraOah___
#define ___harmoniesExtraOah___


#include "enableHarmoniesExtraOahIfDesired.h"

#ifdef EXTRA_OAH_IS_ENABLED


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
                            const string& shortName,
                            const string& longName,
                            const string& description);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    extraShowAllHarmoniesStructuresAtom (
                            const string& shortName,
                            const string& longName,
                            const string& description);

    virtual               ~extraShowAllHarmoniesStructuresAtom ();

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

    void                  print (ostream& os) const override;

    void                  printAllHarmoniesStructures (ostream& os) const;

    void                  printAtomWithVariableOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const override;

  private:

    // private fields
    // ------------------------------------------------------

    //  JMI ???
};
typedef SMARTP<extraShowAllHarmoniesStructuresAtom> S_extraShowAllHarmoniesStructuresAtom;
EXP ostream& operator << (ostream& os, const S_extraShowAllHarmoniesStructuresAtom& elt);

//______________________________________________________________________________
class EXP extraShowAllHarmoniesContentsAtom : public oahAtomStoringAValue
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<extraShowAllHarmoniesContentsAtom> create (
                            const string& shortName,
                            const string& longName,
                            const string& description,
                            const string& valueSpecification,
                            const string& variableName,
                            string&       stringVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    extraShowAllHarmoniesContentsAtom (
                            const string& shortName,
                            const string& longName,
                            const string& description,
                            const string& valueSpecification,
                            const string& variableName,
                            string&       stringVariable);

    virtual               ~extraShowAllHarmoniesContentsAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setStringVariable (const string& value)
                              { fStringVariable = value; }

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

    void                  print (ostream& os) const override;

    void                  printAllHarmoniesContents (
                            ostream&              os,
                            msrSemiTonesPitchKind semiTonesPitchKind) const;

    void                  printAtomWithVariableOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const override;

  private:

    // private fields
    // ------------------------------------------------------

    string&               fStringVariable;
};
typedef SMARTP<extraShowAllHarmoniesContentsAtom> S_extraShowAllHarmoniesContentsAtom;
EXP ostream& operator << (ostream& os, const S_extraShowAllHarmoniesContentsAtom& elt);

//______________________________________________________________________________
class EXP extraShowHarmonyDetailsAtom : public oahAtomStoringAValue
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<extraShowHarmonyDetailsAtom> create (
                            const string& shortName,
                            const string& longName,
                            const string& description,
                            const string& valueSpecification,
                            const string& variableName,
                            string&       stringVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    extraShowHarmonyDetailsAtom (
                            const string& shortName,
                            const string& longName,
                            const string& description,
                            const string& valueSpecification,
                            const string& variableName,
                            string&       stringVariable);

    virtual               ~extraShowHarmonyDetailsAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setShowHarmonyDetailsVariableValue (
                            const string& value)
                              {  fStringVariable = value; }

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

    void                  print (ostream& os) const override;

    void                  printAtomWithVariableOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const override;

  private:

    // private fields
    // ------------------------------------------------------

    string&               fStringVariable;
};
typedef SMARTP<extraShowHarmonyDetailsAtom> S_extraShowHarmonyDetailsAtom;
EXP ostream& operator << (ostream& os, const S_extraShowHarmonyDetailsAtom& elt);

//______________________________________________________________________________
class EXP extraShowHarmonyAnalysisAtom : public oahAtomStoringAValue
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<extraShowHarmonyAnalysisAtom> create (
                            const string& shortName,
                            const string& longName,
                            const string& description,
                            const string& valueSpecification,
                            const string& variableName,
                            string&       stringVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    extraShowHarmonyAnalysisAtom (
                            const string& shortName,
                            const string& longName,
                            const string& description,
                            const string& valueSpecification,
                            const string& variableName,
                            string&       stringVariable);

    virtual               ~extraShowHarmonyAnalysisAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setShowHarmonyAnalysisVariableValue (
                            const string& value)
                              { fStringVariable = value; }

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

    void                  print (ostream& os) const override;

    void                  printAtomWithVariableOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const override;

  private:

    // private fields
    // ------------------------------------------------------

    string&               fStringVariable;
};
typedef SMARTP<extraShowHarmonyAnalysisAtom> S_extraShowHarmonyAnalysisAtom;
EXP ostream& operator << (ostream& os, const S_extraShowHarmonyAnalysisAtom& elt);

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

    string                fHarmoniesRootAsString;
};
typedef SMARTP<harmoniesExtraOahGroup> S_harmoniesExtraOahGroup;
EXP ostream& operator << (ostream& os, const S_harmoniesExtraOahGroup& elt);

EXP extern S_harmoniesExtraOahGroup gGlobalHarmoniesExtraOahGroup;

//______________________________________________________________________________
EXP S_harmoniesExtraOahGroup createGlobalHarmoniesExtraOahGroup ();


}


#endif


#endif
