/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2024

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrPartGroups___
#define ___msrPartGroups___

#include <map>

#include "msrPartGroupElements.h"

#include "msrTypesForwardDeclarations.h"

#include "msrScores.h"


namespace MusicFormats
{

//______________________________________________________________________________
// data types

enum class msrPartGroupImplicitKind { // an MSR concept, not present in MusicXML
    kPartGroupImplicitOuterMostYes,
    kPartGroupImplicitOuterMostNo
};

std::string msrPartGroupImplicitKindAsString (
  msrPartGroupImplicitKind partGroupImplicitKind);

std::ostream& operator << (std::ostream& os, const msrPartGroupImplicitKind& elt);

enum class msrPartGroupTypeKind {
  kPartGroupTypeNone,
  kPartGroupTypeStart, kPartGroupTypeStop
};

std::string msrPartGroupTypeKindAsString (
  msrPartGroupTypeKind partGroupTypeKind);

std::ostream& operator << (std::ostream& os, const msrPartGroupTypeKind& elt);

enum class msrPartGroupSymbolKind {
  kPartGroupSymbolNone,
  kPartGroupSymbolBrace, kPartGroupSymbolBracket,
  kPartGroupSymbolLine, kPartGroupSymbolSquare
};

std::string msrPartGroupSymbolKindAsString (
  msrPartGroupSymbolKind partGroupSymbolKind);

std::ostream& operator << (std::ostream& os, const msrPartGroupSymbolKind& elt);

enum class msrPartGroupBarLineKind {
  kPartGroupBarLineYes, kPartGroupBarLineNo
};

std::string msrPartGroupBarLineKindAsString (
  msrPartGroupBarLineKind partGroupBarLineKind);

std::ostream& operator << (std::ostream& os, const msrPartGroupBarLineKind& elt);

//______________________________________________________________________________
// PRE-declaration
class   msrPartGroup;
typedef SMARTP<msrPartGroup> S_msrPartGroup;

//______________________________________________________________________________
class EXP msrPartGroup : public msrPartGroupElement
{
/*
  There is no hierarchy implied in part-group elements.
  All that matters is the sequence of part-group elements relative to score-part elements.
  The sequencing of two consecutive part-group elements does not matter.
  It is the default-x attribute that indicates the left-to-right ordering of the group symbols.

  <part-group number="1" type="start">
  <group-name>Trombones</group-name>
  <group-abbreviation>Trb.</group-abbreviation>
  <group-symbol default-x="-12">brace</group-symbol>
  <group-barLine>yes</group-barLine>
  </part-group>
*/

  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrPartGroup> create (
                            int                      inputLineNumber,
                            int                      partGroupNumber,
                            int                      partGroupAbsoluteNumber,
                            const std::string&       partGroupName,
                            const std::string&       partGroupNameDisplayText,
                            const std::string&       partGroupAccidentalText,
                            const std::string&       partGroupAbbreviation,
                            msrPartGroupSymbolKind   partGroupSymbolKind,
                            int                      partGroupSymbolDefaultX,
                            msrPartGroupImplicitKind partGroupImplicitKind,
                            msrPartGroupBarLineKind  partGroupBarLineKind,
                            const S_msrPartGroup&    partGroupUpLinkToContainingPartGroup,
                            const S_msrScore&        partGroupUpLinkToScore);

    SMARTP<msrPartGroup> createPartGroupNewbornClone (
                            const S_msrPartGroup& containingartGroupClone,
                              // the upLink for nested part groups
                            const S_msrScore&     scoreClon);

    // creation from the API
    // ------------------------------------------------------

    static SMARTP<msrPartGroup> create (
                            int                   inputLineNumber,
                            int                   partGroupNumber,
                            int                   partGroupAbsoluteNumber,
                            const std::string&    partGroupName,
                            const S_msrPartGroup& partGroupUpLinkToContainingPartGroup,
                            const S_msrScore&     partGroupUpLinkToScore);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrPartGroup (
                            int                      inputLineNumber,
                            int                      partGroupNumber,
                            int                      partGroupAbsoluteNumber,
                            const std::string&       partGroupName,
                            const std::string&       partGroupNameDisplayText,
                            const std::string&       partGroupAccidentalText,
                            const std::string&       partGroupAbbreviation,
                            msrPartGroupSymbolKind   partGroupSymbolKind,
                            int                      partGroupSymbolDefaultX,
                            msrPartGroupImplicitKind partGroupImplicitKind,
                            msrPartGroupBarLineKind  partGroupBarLineKind,
                            const S_msrPartGroup&    partGroupUpLinkToContainingPartGroup,
                            const S_msrScore&        partGroupUpLinkToScore);

                          msrPartGroup (
                            int                   inputLineNumber,
                            int                   partGroupNumber,
                            int                   partGroupAbsoluteNumber,
                            const std::string&    partGroupName,
                            const S_msrPartGroup& partGroupUpLinkToContainingPartGroup,
                            const S_msrScore&     partGroupUpLinkToScore);

    virtual               ~msrPartGroup ();

  public:

    // set and get
    // ------------------------------------------------------

    // upLinks
    void                  setPartGroupUpLinkToContainingPartGroup (
                            int                   inputLineNumber,
                            const S_msrPartGroup& containingPartGroup);

    S_msrPartGroup        getPartGroupUpLinkToContainingPartGroup () const
                              { return fPartGroupUpLinkToContainingPartGroup; }

    S_msrScore            getPartGroupUpLinkToScore () const
                              { return fPartGroupUpLinkToScore; }

    // numbers
    int                   getPartGroupAbsoluteNumber () const
                              { return fPartGroupAbsoluteNumber; }

    int                   getPartGroupNumber () const
                              { return fPartGroupNumber; }

    // names

    std::string           fetchPartGroupCombinedName () const;

    std::string           fetchPartGroupCombinedNameWithoutEndOfLines () const;
                            // for comments in LilyPond code

    // miscellaneous

    std::string           getPartGroupNameDisplayText () const
                              { return fPartGroupNameDisplayText; }

    std::string           getPartGroupAccidentalText () const
                              { return fPartGroupAccidentalText; }

    std::string           getPartGroupName () const
                              { return fPartGroupName; }

    std::string           getPartGroupAbbreviation () const
                              { return fPartGroupAbbreviation; }

    // symbol

    msrPartGroupSymbolKind
                          getPartGroupSymbolKind () const
                              { return fPartGroupSymbolKind; }

    int                   getPartGroupSymbolDefaultX () const
                              { return fPartGroupSymbolDefaultX; }

    // implicit

    msrPartGroupImplicitKind
                          getPartGroupImplicitKind () const
                              { return fPartGroupImplicitKind; }

    // type


    // barLine

    msrPartGroupBarLineKind
                          getPartGroupBarLineKind () const
                              { return fPartGroupBarLineKind; }

    // instrument name

    void                  setPartGroupInstrumentName (const std::string& name);

    std::string           getPartGroupInstrumentName () const
                              { return fPartGroupInstrumentName; }

    // elements

    const std::list<S_msrPartGroupElement>&
                          getPartGroupElementsList () const
                              { return fPartGroupElementsList; }


    // measures slices sequence

    S_msrMeasuresSlicesSequence
                          getPartGroupMeasuresSlicesSequence () const
                              { return fPartGroupMeasuresSlicesSequence; }

  public:

    // public services
    // ------------------------------------------------------

//     S_msrPart             appendPartToPartGroupByItsPartID ( // JMI superflous v0.9.69
//                             int                inputLineNumber,
//                             const std::string& partID);

    void                  appendPartToPartGroup (S_msrPart part);

    void                  removePartFromPartGroup (
                            int              inputLineNumber,
                            const S_msrPart& partToBeRemoved);

    void                  prependNestedGroupToPartGroup (
                            const S_msrPartGroup& nestedPartGroup);

    void                  appendNestedPartGroupToPartGroup (
                            const S_msrPartGroup& nestedPartGroup);

    S_msrPart             fetchPartFromPartGroupByItsPartID (
                            int                inputLineNumber,
                            const std::string& partID);

    void                  collectPartGroupPartsList (
                            int                   inputLineNumber,
                            std::list<S_msrPart>& partsList);

    // voices

    void                  registerVoiceInPartGroupAllVoicesList (
                            const S_msrVoice& voice);

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    void                  displayPartGroupPartsMap (
                            int inputLineNumber);

    void                  displayPartGroupElementsList (
                            int inputLineNumber) const;

    void                  displayPartGroupElementsListFull (
                            int inputLineNumber) const;

    std::string           asString () const override;

    void                  print (std::ostream& os) const override;
    void                  printFull (std::ostream& os) const override;

    void                  printSummary (std::ostream& os) const override;

    void                  printSlices (std::ostream& os) const;

  private:

    // private services
    // ------------------------------------------------------

//     void                  checkPartGroupElement (
//                             const S_msrPartGroupElement& partGroupElement) const; // TEMP JMI v0.9.69

  private:

    // private fields
    // ------------------------------------------------------

    // upLinks

    S_msrPartGroup        fPartGroupUpLinkToContainingPartGroup;
                            // part groups can be nested

    S_msrScore            fPartGroupUpLinkToScore;

    // numbers

    int                   fPartGroupNumber;
    int                   fPartGroupAbsoluteNumber;

    // name

    std::string           fPartGroupName;
    std::string           fPartGroupNameDisplayText;

    std::string           fPartGroupAccidentalText;

    std::string           fPartGroupAbbreviation;

    // symbol kind

    msrPartGroupSymbolKind
                          fPartGroupSymbolKind;

    // default X

    int                   fPartGroupSymbolDefaultX;

    // implicit

    msrPartGroupImplicitKind
                          fPartGroupImplicitKind;

    // bar line

    msrPartGroupBarLineKind
                          fPartGroupBarLineKind;

    // instrument name

    std::string           fPartGroupInstrumentName; // JMI

    // accessing parts by name

    std::map<std::string, S_msrPart>
                          fPartGroupPartsMap;

    // allowing for both parts and nested part groups as elements

    std::list<S_msrPartGroupElement>
                          fPartGroupElementsList;

    // voices

    std::list<S_msrVoice> fPartGroupAllVoicesList;

    // measures slices sequence

    S_msrMeasuresSlicesSequence
                          fPartGroupMeasuresSlicesSequence;
};
typedef SMARTP<msrPartGroup> S_msrPartGroup;
EXP std::ostream& operator << (std::ostream& os, const S_msrPartGroup& elt);


}

#endif // ___msrPartGroups___
