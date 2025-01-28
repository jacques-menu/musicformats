/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2024

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___mxsrVoices___
#define ___mxsrVoices___

#include "msrTuplets.h"


namespace MusicFormats
{

//________________________________________________________________________
class mxsrVoice : public smartable
{
/*
  positions represent the order in which the parts appear in <part-list />,
  starting at 0 since std::vectors are used
*/

  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<mxsrVoice> create (
                            const S_msrVoice& voice);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          mxsrVoice (
                            const S_msrVoice& voice);

    virtual               ~mxsrVoice ();

  public:

    // set and get
    // ------------------------------------------------------

    S_msrVoice            getMsrVoice () const
                              { return fMsrVoice; }

    const std::list <S_msrTuplet>&
                          getTupletsStack () const
                              { return fTupletsStack; }

    const std::size_t     getTupletsStackSize () const
                              { return fTupletsStack.size (); }

    const S_msrTuplet     getTupletsStackTop () const
                              { return fTupletsStack.front (); }

    void                  setLastMetNoteInVoice (S_msrNote note)
                              { fLastMetNoteInVoice = note; }

    S_msrNote             getLastMetNoteInVoice () const
                              { return fLastMetNoteInVoice; }

  public:

    // public services
    // ------------------------------------------------------

    std::string           fetchMsrVoiceName () const
                              { return fMsrVoice->getVoiceName (); }

    void                  pushTupletOntoTupletsStack (const S_msrTuplet& tuplet)
                              { fTupletsStack.push_front (tuplet); }

//     void                  handleTupletStartByHandler (
//                             const S_msrTuplet& tuplet,
//                             const S_msrVoice&  currentNoteVoice);
//
//     void                  handleTupletContinueByHandler (
//                             const S_msrNote&   note,
//                             const S_msrVoice&  currentNoteVoice);
//
//     void                  handleTupletStopByHandler (
//                             const S_msrNote&   note,
//                             const S_msrVoice&  currentNoteVoice);

    void                  popTupletStackTopAndFinalizeIt (
                            int         inputLineNumber,
                            std::string context);

  public:

    // print
    // ------------------------------------------------------

    std::string           asString () const;

    virtual void          print (std::ostream& os) const;

  private:

    // private fields
    // ------------------------------------------------------


    S_msrVoice            fMsrVoice;

    S_msrNote             fLastMetNoteInVoice;

    std::list <S_msrTuplet>
                          fTupletsStack;

    S_msrNote             fCurrentOuterMostTupletFirstNote;
    S_msrTuplet           fCurrentOuterMostTuplet;

    msrWholeNotes         fCurrentOuterMostTupletRelativeOffset;

  private:

    // private work fields
    // ------------------------------------------------------

    // we use a pair containing the staff and voice numbers: JMI v0.9.70
//     std::map <S_msrVoice, S_msrTuplet>
//     std::map <std::pair <int, int>, S_msrTuplet>
//                               fLastHandledTupletInVoiceMap;

//     // the tuplets stops are not always in first-in/first-out order, so:
//     std::set <int>         fExpectedTupletsStopNumbersSet;

  private:

    // private methods
    // ------------------------------------------------------

    void                  displayTupletsStack (
                            const std::string& context);

    void                  handleTupletsPendingOnTupletsStack (
                            int inputLineNumber);

    void                  displayLastHandledTupletInVoiceMap (
                            const std::string& header);
};
typedef SMARTP<mxsrVoice> S_mxsrVoice;

EXP std::ostream& operator << (std::ostream& os, const mxsrVoice& elt);
EXP std::ostream& operator << (std::ostream& os, const S_mxsrVoice& elt);


}


#endif // ___mxsrVoices___
