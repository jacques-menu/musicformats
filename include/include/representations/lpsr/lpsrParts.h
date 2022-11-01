/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___lpsrParts___
#define ___lpsrParts___

#include "msr.h"

#include "lpsrContexts.h"

#include "lpsrStaves.h"


namespace MusicFormats
{

//______________________________________________________________________________
class EXP lpsrPartBlock : public lpsrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<lpsrPartBlock> create (
                            S_msrPart part);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          lpsrPartBlock (
                            S_msrPart part);

    virtual               ~lpsrPartBlock ();

  public:

    // set and get
    // ------------------------------------------------------

    S_msrPart             getPart () const
                              { return fPart; }

    const list<S_msrElement>&
                          getPartBlockElementsList () const
                              { return fPartBlockElementsList; }

    void                  setPartBlockInstrumentName (
                            const string& instrumentName)
                              {
                                fPartBlockInstrumentName =
                                  instrumentName;
                              }

    string                getPartBlockInstrumentName () const
                              { return fPartBlockInstrumentName; }

    void                  setPartBlockShortInstrumentName (
                            const string& shortInstrumentName)
                              {
                                fPartBlockShortInstrumentName =
                                  shortInstrumentName;
                              }

    string                getPartBlockShortInstrumentName () const
                              { return fPartBlockShortInstrumentName; }

  public:

    // pbulic services
    // ------------------------------------------------------

    void                  appendStaffBlockToPartBlock (
                            S_lpsrStaffBlock staffBlock);

    void                  appendChordNamesContextToPartBlock (
                            int                     inputLineNumber,
                            S_lpsrChordNamesContext chordNamesContext);

    void                  appendFiguredBassContextToPartBlock (
                            S_lpsrFiguredBassContext context);

  private:

    // private services
    // ------------------------------------------------------

    // voices ordering in staves

    static bool           compareElementsToHaveHarmoniesAboveCorrespondingStaff (
                            const S_msrElement& first,
                            const S_msrElement& second);

    static bool           compareStaffBlockWithOtherElement (
                            const S_lpsrStaffBlock& staffBlock,
                            const S_msrElement&     otherElement);

    static bool           compareChordNamesContextWithOtherElement (
                            const S_lpsrChordNamesContext& chordNamesContext,
                            const S_msrElement&            otherElement);

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

  private:

    // private fields
    // ------------------------------------------------------

    S_msrPart             fPart;

    list<S_msrElement>    fPartBlockElementsList;

    string                fPartBlockInstrumentName; // JMI
    string                fPartBlockShortInstrumentName; // JMI
};
typedef SMARTP<lpsrPartBlock> S_lpsrPartBlock;
EXP ostream& operator << (ostream& os, const S_lpsrPartBlock& elt);


}


#endif
