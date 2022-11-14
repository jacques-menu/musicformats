/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrGenerationBasicTypes___
#define ___msrGenerationBasicTypes___

#include "oahBasicTypes.h"


namespace MusicFormats
{

//______________________________________________________________________________
enum class msrGenerationAPIKind {
  kMsrFunctionsAPIKind,
  kMsrStringsAPIKind
};

EXP extern std::string msrGenerationAPIKindAsString (
  msrGenerationAPIKind generationAPIKind);

std::ostream& operator << (std::ostream& os, const msrGenerationAPIKind& elt);

EXP extern msrGenerationAPIKind msrGenerationAPIKindFromString (
  const std::string& theString);

extern std::map<std::string, msrGenerationAPIKind>
  gGlobalGenerationAPIKindsMap;

std::string existingGenerationAPIKinds (size_t namesListMaxLength);

void initializeGenerationAPIKindsMap ();

//______________________________________________________________________________
class EXP msrGenerationAPIKindAtom : public oahAtomStoringAValue
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msrGenerationAPIKindAtom> create (
                            const std::string&    longName,
                            const std::string&    shortName,
                            const std::string&    description,
                            const std::string&    valueSpecification,
                            const std::string&    variableName,
                            msrGenerationAPIKind& generationAPIKindVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrGenerationAPIKindAtom (
                            const std::string&    longName,
                            const std::string&    shortName,
                            const std::string&    description,
                            const std::string&    valueSpecification,
                            const std::string&    variableName,
                            msrGenerationAPIKind& generationAPIKindVariable);

    virtual               ~msrGenerationAPIKindAtom ();

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

    void                  printAtomWithVariableOptionsValues (
                            std::ostream& os,
                            int      valueFieldWidth) const override;

  private:

    // private fields
    // ------------------------------------------------------

    msrGenerationAPIKind& fGenerationAPIKindVariable;
};
typedef SMARTP<msrGenerationAPIKindAtom> S_msrGenerationAPIKindAtom;
EXP std::ostream& operator << (std::ostream& os, const S_msrGenerationAPIKindAtom& elt);

//______________________________________________________________________________
void initializeMsrGenerationAPI ();


}


#endif
