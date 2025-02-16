/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2024

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrMeasureElements___
#define ___msrMeasureElements___

#include "msrTypesForwardDeclarations.h"

#include "msrElements.h"
#include "msrNotesDurations.h"


namespace MusicFormats
{
//______________________________________________________________________________
class   msrMeasure;
typedef SMARTP<msrMeasure> S_msrMeasure;

//______________________________________________________________________________
/*
  Various elements can found in measures, JMI v0.9.66
  hence class msrMeasureElement
*/

class EXP msrMeasureElement : public msrElement
{
  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrMeasureElement (
                            int inputLineNumber);

    virtual               ~msrMeasureElement ();

  public:

    // set and get
    // ------------------------------------------------------

    virtual void          setMeasureElementUpLinkToMeasure (
                            const S_msrMeasure& measure);

    virtual S_msrMeasure  getMeasureElementUpLinkToMeasure () const;

    void                  setMeasureElementSoundingWholeNotes (
                            const msrWholeNotes& wholeNotes,
                            const std::string&   context);

    msrWholeNotes         getMeasureElementSoundingWholeNotes () const;

    // this method is overridden in sub-classes such as those for
    // time signatures, harmonies and figured bass elements,
    // as well as those elements that have an actual whole notes duration

    // the overrides call a class-specific method that can be called directly,
    // such occurrence are more explicit when debugging
    // the computations of measure positions
    virtual void          setMeasureElementPositionInMeasure (
                            const S_msrMeasure&  measure,
                            const msrWholeNotes& positionInMeasure,
                            const std::string&   context);

    msrWholeNotes         getMeasureElementPositionInMeasure () const;

    void                  setMeasureElementVoicePosition (
                            const msrWholeNotes& voicePosition,
                            const std::string&   context);

    msrWholeNotes         getMeasureElementVoicePosition () const;

//     void                  setMeasureMoment (
//                             const msrMoment&   measureMoment,
//                             const std::string& context);
//
//     const msrMoment&      getMeasureMoment () const
//                               { return fMeasureMoment; }
//
//     void                  setVoiceMoment (
//                             const msrMoment&   voiceMoment,
//                             const std::string& context);
//
//     const msrMoment&      getVoiceMoment () const
//                               { return fVoiceMoment; }

  public:

    // public services
    // ------------------------------------------------------

    void                  incrementMeasureElementSoundingWholeNotesBy (
                            const msrWholeNotes& wholeNotes,
                            const std::string&   context);

    static bool           compareMeasureElementsByIncreasingPositionInMeasure (
                            const SMARTP<msrMeasureElement>& first,
                            const SMARTP<msrMeasureElement>& second);

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override; // JMI ??? v0.9.66

  public:

    // print
    // ------------------------------------------------------

    std::string           asShortString () const override;
    std::string           asString () const override;

    void                  print (std::ostream& os) const override;

    void                  printSummary (std::ostream& os) const override;

  protected:

    // protected methods
    // ------------------------------------------------------

  protected:

    // protected fields
    // ------------------------------------------------------

    S_msrMeasure          fMeasureElementUpLinkToMeasure;

    msrWholeNotes         fMeasureElementSoundingWholeNotes;

    msrWholeNotes         fMeasureElementPositionInMeasure;
    msrWholeNotes         fMeasureElementVoicePosition;

//     msrMoment             fMeasureMoment;
//     msrMoment             fVoiceMoment;
};
typedef SMARTP<msrMeasureElement> S_msrMeasureElement;
EXP std::ostream& operator << (std::ostream& os, const S_msrMeasureElement& elt);

}


#endif // ___msrMeasureElements___
