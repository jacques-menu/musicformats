/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2024

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrAfterGraceNotes___
#define ___msrAfterGraceNotes___

#include "msrElements.h"
#include "msrMeasureElements.h"
#include "msrParts.h"
#include "msrVoices.h"


namespace MusicFormats
{

//______________________________________________________________________________
class EXP msrAfterGraceNotesGroupContents : public msrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrAfterGraceNotesGroupContents> create (
                            int               inputLineNumber,
                            const S_msrVoice& afterGraceNotesGroupContentsUpLinkToVoice);

    SMARTP<msrAfterGraceNotesGroupContents> createAfterGraceNotesGroupContentsNewbornClone (
                            const S_msrVoice& containingVoice);

    SMARTP<msrVoice> createAfterGraceNotesGroupContentsDeepClone (
                            const S_msrVoice& containingVoice);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrAfterGraceNotesGroupContents (
                            int               inputLineNumber,
                            const S_msrVoice& afterGraceNotesGroupContentsUpLinkToVoice);

    virtual               ~msrAfterGraceNotesGroupContents ();

  public:

    // set and get
    // ------------------------------------------------------

    const std::list <S_msrMeasureElement>&
                          getAfterGraceNotesGroupContentsNotesList () const
                              {
                                return
                                  fAfterGraceNotesGroupContentsNotesList;
                              }

  public:

    // public services
    // ------------------------------------------------------

    // upLinks
    S_msrPart             fetchAfterGraceNotesGroupContentsUpLinkToPart () const;

    // notes
    void                  appendNoteToAfterGraceNotesGroupContents (
                            const S_msrNote& note);
    void                  appendNoteToAfterGraceNotesGroupContents (
                            const S_msrChord& chord);

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    std::string           asShortString () const override;

    void                  print (std::ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    // upLinks
    S_msrVoice            fAfterGraceNotesGroupContentsUpLinkToVoice;

    // the notes list
    std::list <S_msrMeasureElement>
                          fAfterGraceNotesGroupContentsNotesList;
};
typedef SMARTP<msrAfterGraceNotesGroupContents> S_msrAfterGraceNotesGroupContents;
EXP std::ostream& operator << (std::ostream& os, const S_msrAfterGraceNotesGroupContents& elt);

//______________________________________________________________________________
class EXP msrAfterGraceNotesGroup : public msrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrAfterGraceNotesGroup> create (
                            int          inputLineNumber,
                            const S_msrElement& afterGraceNotesGroupElement,
                            Bool         afterGraceNotesGroupIsSlashed,
                            const S_msrVoice&   afterGraceNotesGroupUpLinkToVoice);

    SMARTP<msrAfterGraceNotesGroup> createAfterGraceNotesGroupNewbornClone (
                            const S_msrNote&  noteClone,
                            const S_msrVoice& containingVoice);

    SMARTP<msrVoice> createAfterGraceNotesGroupDeepClone (
                            const S_msrNote&  noteClone,
                            const S_msrVoice& containingVoice);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrAfterGraceNotesGroup (
                            int          inputLineNumber,
                            const S_msrElement& afterGraceNotesGroupElement,
                            Bool         afterGraceNotesGroupIsSlashed,
                            const S_msrVoice&   afterGraceNotesGroupUpLinkToVoice);

    virtual               ~msrAfterGraceNotesGroup ();

  public:

    // set and get
    // ------------------------------------------------------

    S_msrElement          getAfterGraceNotesGroupElement () const
                              { return fAfterGraceNotesGroupElement; }

    S_msrAfterGraceNotesGroupContents
                          getAfterGraceNotesGroupContents () const
                              { return fAfterGraceNotesGroupContents; }

  public:

    // public services
    // ------------------------------------------------------

    // upLinks
    S_msrPart             fetchAfterGraceNotesGroupUpLinkToPart () const;

    // notes
    void                  appendNoteToAfterGraceNotesGroupContents (
                            const S_msrNote& note);

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    std::string           asShortString () const override;

    void                  print (std::ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    // upLinks
    S_msrVoice            fAfterGraceNotesGroupUpLinkToVoice;

    S_msrElement          fAfterGraceNotesGroupElement;

    Bool                  fAfterGraceNotesGroupIsSlashed;

    S_msrAfterGraceNotesGroupContents
                          fAfterGraceNotesGroupContents;
};
typedef SMARTP<msrAfterGraceNotesGroup> S_msrAfterGraceNotesGroup;
EXP std::ostream& operator << (std::ostream& os, const S_msrAfterGraceNotesGroup& elt);


}


#endif // ___msrAfterGraceNotes___
