/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrTablatures___
#define ___msrTablatures___

#include <list>
#include <stack>

#include "msrElements.h"


namespace MusicFormats
{

//______________________________________________________________________________
// data types

enum class msrBarreTypeKind {
  kBarreTypeNone,
  kBarreTypeStart, kBarreTypeStop
};

std::string msrBarreTypeKindAsString (
  msrBarreTypeKind barreTypeKind);

std::ostream& operator << (std::ostream& os, const msrBarreTypeKind& elt);

//______________________________________________________________________________
class EXP msrFrameNote : public msrElement
{
  public:

/*
          <frame-note>
            <std::string>6</std::string>
            <fret>0</fret>
          </frame-note>
*/

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrFrameNote> create (
                            const mfInputLineNumber& inputLineNumber,
                            int              frameNoteStringNumber,
                            int              frameNoteFretNumber,
                            int              frameNoteFingering,
                            msrBarreTypeKind frameNoteBarreTypeKind);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrFrameNote (
                            const mfInputLineNumber& inputLineNumber,
                            int              frameNoteStringNumber,
                            int              frameNoteFretNumber,
                            int              frameNoteFingering,
                            msrBarreTypeKind frameNoteBarreTypeKind);

    virtual               ~msrFrameNote ();

  public:

    // set and get
    // ------------------------------------------------------

    int                   getFrameNoteStringNumber () const
                              { return fFrameNoteStringNumber; }

    int                   getFrameNoteFretNumber () const
                              { return fFrameNoteFretNumber; }

    int                   getFrameNoteFingering () const
                              { return fFrameNoteFingering; }

    msrBarreTypeKind      getFrameNoteBarreTypeKind () const
                              { return fFrameNoteBarreTypeKind; }

  public:

    // public services
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

    std::string           asString () const override;

    void                  print (std::ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    int                   fFrameNoteStringNumber;
    int                   fFrameNoteFretNumber;

    int                   fFrameNoteFingering;

    msrBarreTypeKind      fFrameNoteBarreTypeKind;
};
typedef SMARTP<msrFrameNote> S_msrFrameNote;
EXP std::ostream& operator << (std::ostream& os, const S_msrFrameNote& elt);

//______________________________________________________________________________
class EXP msrBarre
{
  public:

    // constructors/destructor
    // ------------------------------------------------------

                          msrBarre (
                            int barreStartString,
                            int barreStopString,
                            int barreFretNumber)
                              {
                                fBarreStartString = barreStartString;
                                fBarreStopString = barreStopString;
                                fBarreFretNumber = barreFretNumber;
                              }

   public:

    // set and get
    // ------------------------------------------------------

    int                   getBarreStartString () const
                              { return fBarreStartString; }

    int                   getBarreStopString () const
                              { return fBarreStopString; }

    int                   getBarreFretNumber () const
                              { return fBarreFretNumber; }

 private:

    // fields
    // ------------------------------------------------------

    int                   fBarreStartString;
    int                   fBarreStopString;
    int                   fBarreFretNumber;
};

//______________________________________________________________________________
class EXP msrFrame : public msrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrFrame> create (
                            const mfInputLineNumber& inputLineNumber,
                            int frameStringsNumber,
                            int frameFretsNumber,
                            int frameFirstFretNumber);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrFrame (
                            const mfInputLineNumber& inputLineNumber,
                            int frameStringsNumber,
                            int frameFretsNumber,
                            int frameFirstFretNumber);

    virtual               ~msrFrame ();

  public:

    // set and get
    // ------------------------------------------------------


    int                   getFrameStringsNumber () const
                              { return fFrameStringsNumber; }

    int                   getFrameFretsNumber () const
                              { return fFrameFretsNumber; }

    int                   getFrameFirstFretNumber () const
                              { return fFrameFirstFretNumber; }

    const std::list <S_msrFrameNote>&
                          getFrameFrameNotesList () const
                              { return fFrameFrameNotesList; }

    const std::list <msrBarre>& getFrameBarresList ()
                              { return fFrameBarresList; }

    Bool                  getFrameContainsFingerings () const
                              { return fFrameContainsFingerings; }

  public:

    // public services
    // ------------------------------------------------------

    void                  appendFrameNoteToFrame (
                            const S_msrFrameNote& frameNote);

  private:

    // private services
    // ------------------------------------------------------

    void                  appendBarreToFrame ( // JMI
                            msrBarre barre)
                              {
                                fFrameBarresList.push_back (barre);
                              }

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    std::string           asString () const override;

    void                  print (std::ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    int                   fFrameStringsNumber;
    int                   fFrameFretsNumber;
    int                   fFrameFirstFretNumber;

    std::list <S_msrFrameNote>  fFrameFrameNotesList;

    std::list <msrBarre >       fFrameBarresList;

    // a barre start remains pending
    // until the matching stop is appended to the frame
    std::list <S_msrFrameNote> fPendingBarreStartFrameNotes;

    // optimizing computation
    Bool                  fFrameContainsFingerings;
};
typedef SMARTP<msrFrame> S_msrFrame;
EXP std::ostream& operator << (std::ostream& os, const S_msrFrame& elt);


}


#endif // ___msrTablatures___
