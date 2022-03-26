/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrGenerationAPIs___
#define ___msrGenerationAPIs___

//#include <ostream>

//#include <map>

//#include "exports.h"
//#include "smartpointer.h"
//#include "basevisitor.h"



#include "oahBasicTypes.h"


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
enum class msrGenerationAPIKind {
  kMsrFunctionsAPIKind,
  kMsrStringsAPIKind
};

EXP extern string msrGenerationAPIKindAsString (
  msrGenerationAPIKind generationAPIKind);

ostream& operator<< (ostream& os, msrGenerationAPIKind& elt);

EXP extern msrGenerationAPIKind msrGenerationAPIKindFromString (
  const string& theString);

extern map<string, msrGenerationAPIKind>
  gGlobalGenerationAPIKindsMap;

string existingGenerationAPIKinds (size_t namesListMaxLength);

void initializeGenerationAPIKindsMap ();

//______________________________________________________________________________
class EXP msrGenerationAPIKindAtom : public oahAtomStoringAValue
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msrGenerationAPIKindAtom> create (
                            const string&         longName,
                            const string&         shortName,
                            const string&         description,
                            const string&         valueSpecification,
                            const string&         variableName,
                            msrGenerationAPIKind& generationAPIKindVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrGenerationAPIKindAtom (
                            const string&         longName,
                            const string&         shortName,
                            const string&         description,
                            const string&         valueSpecification,
                            const string&         variableName,
                            msrGenerationAPIKind& generationAPIKindVariable);

    virtual               ~msrGenerationAPIKindAtom ();

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

    void                  printAtomWithVariableOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const override;

  private:

    // private fields
    // ------------------------------------------------------

    msrGenerationAPIKind& fGenerationAPIKindVariable;
};
typedef SMARTP<msrGenerationAPIKindAtom> S_msrGenerationAPIKindAtom;
EXP ostream& operator<< (ostream& os, const S_msrGenerationAPIKindAtom& elt);

//______________________________________________________________________________
void initializeMsrGenerationAPI ();


}


#endif
