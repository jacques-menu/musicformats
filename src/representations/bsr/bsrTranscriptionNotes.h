/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___bsrTranscriptionNotes___
#define ___bsrTranscriptionNotes___

#include "bsrElements.h"
#include "bsrLines.h"

#include "bsrCellsLists.h"


namespace MusicFormats
{

//______________________________________________________________________________
class EXP bsrTranscriptionNotesElement : public bsrElement
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<bsrTranscriptionNotesElement> create (
                            int           inputLineNumber,
                            const std::string& transcriptionNoteText);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          bsrTranscriptionNotesElement (
                            int           inputLineNumber,
                            const std::string& transcriptionNoteText);

    virtual               ~bsrTranscriptionNotesElement ();

  public:

    // set and get
    // ------------------------------------------------------

    std::string           getTranscriptionNoteText () const
                              { return fTranscriptionNoteText; }

  public:

    // public services
    // ------------------------------------------------------

    S_bsrCellsList        fetchCellsList () const
                              { return buildCellsList (); }

  private:

    // private services
    // ------------------------------------------------------

    S_bsrCellsList        buildCellsList () const;

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

    std::string           fTranscriptionNoteText;
};
typedef SMARTP<bsrTranscriptionNotesElement> S_bsrTranscriptionNotesElement;
EXP std::ostream& operator << (std::ostream& os, const S_bsrTranscriptionNotesElement& elt);

//______________________________________________________________________________
class EXP bsrTranscriptionNotes : public bsrLine
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<bsrTranscriptionNotes> create (
                            int inputLineNumber);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          bsrTranscriptionNotes (
                            int inputLineNumber);

    virtual               ~bsrTranscriptionNotes ();

  public:

    // set and get
    // ------------------------------------------------------

  public:

    // public services
    // ------------------------------------------------------

    void                  appendElementToTranscriptionNotes (
                            const S_bsrTranscriptionNotesElement& transcriptionNotesElement)
                              {
                                fTranscriptionNotesElementsList.push_back (
                                  transcriptionNotesElement);
                              }

    int                   fetchLineContentsNumber () const override
                              { return fLineContentsList.size (); }

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

  private:

    // private fields
    // ------------------------------------------------------

    std::list<S_bsrTranscriptionNotesElement>
                          fTranscriptionNotesElementsList;
};
typedef SMARTP<bsrTranscriptionNotes> S_bsrTranscriptionNotes;
EXP std::ostream& operator << (std::ostream& os, const S_bsrTranscriptionNotes& elt);


}


#endif
