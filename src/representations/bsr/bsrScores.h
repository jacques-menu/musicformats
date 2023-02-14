/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___bsrScores___
#define ___bsrScores___

#include <sstream>
#include <string>
#include <list>

#include "msrScores.h"

#include "bsrElements.h"

#include "bsrPages.h"
#include "bsrTranscriptionNotes.h"

#include "bsrOah.h"


namespace MusicFormats
{

//______________________________________________________________________________
class EXP bsrScore : public bsrElement
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<bsrScore> create (
                            int            inputLineNumber,
                            const S_msrScore&     theMsrScore);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          bsrScore (
                            int            inputLineNumber,
                            const S_msrScore&     theMsrScore);

    virtual               ~bsrScore ();

  public:

    // set and get
    // ------------------------------------------------------

    S_msrScore            getMsrScore () const
                              { return fMsrScore; }

    S_bsrTranscriptionNotes
                          getTranscriptionNotes () const
                              { return fTranscriptionNotes; }

    int                   getBrailleLineLength () const
                              { return fBrailleLineLength; }

    int                   getBraillePageLength () const
                              { return fBraillePageLength; }

  public:

    // public services
    // ------------------------------------------------------

    void                  appendPageToScore (S_bsrPage page)
                              { fScorePagesList.push_back (page); }

    void                  appendTranscriptionNotesElementToScore ( // JMI
                            const S_bsrTranscriptionNotesElement&
                              transcriptionNotesElement)
                              {
                                fTranscriptionNotes->
                                  appendElementToTranscriptionNotes (
                                    transcriptionNotesElement);
                              }

  private:

    // private services
    // ------------------------------------------------------

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

  private:

    // private fields
    // ------------------------------------------------------

    // MSR data
    S_msrScore            fMsrScore;

    S_bsrTranscriptionNotes
                          fTranscriptionNotes;

    int                   fBrailleLineLength; // cells JMI ???
    int                   fBraillePageLength;

    std::list<S_bsrPage>       fScorePagesList;
};
typedef SMARTP<bsrScore> S_bsrScore;
EXP std::ostream& operator << (std::ostream& os, const S_bsrScore& score);


}


#endif // ___bsrScores___
