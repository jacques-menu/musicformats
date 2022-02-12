/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___multiGeneration___
#define ___multiGeneration___

#include <map>

#include "oahBasicTypes.h"

#include "oahAtomsCollection.h"


using namespace std;

namespace MusicFormats
{

//_______________________________________________________________________________
// constants
EXP extern const string K_GENERATED_OUTPUT_KIND_LIlYPOND_NAME;
EXP extern const string K_GENERATED_OUTPUT_KIND_BRAILLE_NAME;
EXP extern const string K_GENERATED_OUTPUT_KIND_MUSICXML_NAME;
EXP extern const string K_GENERATED_OUTPUT_KIND_GUIDO_NAME;
EXP extern const string K_GENERATED_OUTPUT_KIND_MIDI_NAME;

//______________________________________________________________________________
enum class mfMultiGenerationOutputKind {
  k_NoGeneration,
  kGenerationLilypond,
  kGenerationBraille,
  kGenerationMusicXML,
  kGenerationGuido,
  kGenerationMidi
};

EXP extern string mfMultiGenerationOutputKindAsString (
  mfMultiGenerationOutputKind multiGenerationOutputKind);

EXP extern mfMultiGenerationOutputKind mfMultiGenerationOutputKindFromString (
  const string& theString);

extern map<string, mfMultiGenerationOutputKind>
  gGlobalMultiGenerationOutputKindsMap;

string existingMultiGenerationOutputKinds (unsigned int namesListMaxLength);

void initializeMultiGenerationOutputKindsMap ();

//______________________________________________________________________________
EXP extern mfMultiGenerationOutputKind fetchGeneratedOutputKindFromRunData ();

//______________________________________________________________________________
class EXP mfMultiGenerationOutputKindAtom : public oahAtomImplicitlyStoringAValue
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<mfMultiGenerationOutputKindAtom> create (
                          const string&   longName,
                          const string&   shortName,
                          const string&   description,
                          const string&   variableName,
                          mfMultiGenerationOutputKind&
                                          multiGenerationOutputKindVariable,
                          mfMultiGenerationOutputKind
                                          multiGenerationOutputKindValue);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          mfMultiGenerationOutputKindAtom (
                          const string&   longName,
                          const string&   shortName,
                          const string&   description,
                          const string&   variableName,
                          mfMultiGenerationOutputKind&
                                          multiGenerationOutputKindVariable,
                          mfMultiGenerationOutputKind
                                          multiGenerationOutputKindValue);

    virtual               ~mfMultiGenerationOutputKindAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    mfMultiGenerationOutputKind&
                          getMultiGenerationOutputKindVariable () const
                              { return fMultiGenerationOutputKindVariable; }
    mfMultiGenerationOutputKind
                          getMultiGenerationOutputKindValue () const
                              { return fMultiGenerationOutputKindValue; }

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

    string                asShortNamedOptionString () const override;
    string                asActualLongNamedOptionString () const override;

    void                  print (ostream& os) const override;

    void                  printAtomWithVariableOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const override;

  private:

    // private fields
    // ------------------------------------------------------

    mfMultiGenerationOutputKind&
                          fMultiGenerationOutputKindVariable;
    mfMultiGenerationOutputKind
                          fMultiGenerationOutputKindValue;
};
typedef SMARTP<mfMultiGenerationOutputKindAtom> S_mfMultiGenerationOutputKindAtom;
EXP ostream& operator<< (ostream& os, const S_mfMultiGenerationOutputKindAtom& elt);

//______________________________________________________________________________
class EXP mfMultiGenerationOahGroup : public oahGroup
{
  public:

    static SMARTP<mfMultiGenerationOahGroup> create ();

  public:

    // initialisation
    // ------------------------------------------------------

    void                  initializeMultiGenerationOahGroup ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    mfMultiGenerationOahGroup ();

    virtual               ~mfMultiGenerationOahGroup ();

  public:

    // set and get
    // ------------------------------------------------------

    mfMultiGenerationOutputKind
                          getMultiGenerationOutputKind () const
                              { return fMultiGenerationOutputKind; }

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

    void                  initializeMultiGenerationOutputKindOptions ();

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    void                  printMultiGenerationOahHelp ();

    void                  printMultiGenerationOahValues (int fieldWidth);

  private:

    mfMultiGenerationOutputKind
                          fMultiGenerationOutputKind;
    S_mfMultiGenerationOutputKindAtom
                          fMultiGenerationOutputKindAtom;
};
typedef SMARTP<mfMultiGenerationOahGroup> S_multiGenerationOahGroup;
EXP ostream& operator<< (ostream& os, const S_multiGenerationOahGroup& elt);

EXP extern S_multiGenerationOahGroup gGlobalMultiGenerationOahGroup;

//______________________________________________________________________________
EXP S_multiGenerationOahGroup createGlobalMultiGenerationOahGroup ();


}


#endif
