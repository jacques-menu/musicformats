/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrMeasureElements___
#define ___msrMeasureElements___

#include "msrTypesForwardDeclarations.h"

#include "msrElements.h"

#include "mfRational.h"


namespace MusicFormats
{

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

    // the overrides call a class-specific method that can be called directly
    virtual void          setMeasureElementUpLinkToMeasure (
                            const S_msrMeasure& measure) = 0;

    // the overrides call a class-specific method that can be called directly
    virtual void          getMeasureElementUpLinkToMeasure (
                            S_msrMeasure& upLinkToMeasure) const = 0;

    void                  setMeasureElementSoundingWholeNotes (
                            const Rational&    wholeNotes,
                            const std::string& context);

    Rational              getMeasureElementSoundingWholeNotes () const
                              { return fMeasureElementSoundingWholeNotes; }

    // this method is overridden in sub-classes such as those for
    // time signatures, harmonies and figured bass elements,
    // as well as those elements that have an actual whole notes duration

    // the overrides call a class-specific method that can be called directly,
    // such occurrence are more explicit when debugging
    // the computations of measure positions
    virtual void          setMeasureElementMeasurePosition (
                            const S_msrMeasure& measure,
                            const Rational&     measurePosition,
                            const std::string&  context);

    Rational              getMeasureElementMeasurePosition () const
                              { return fMeasureElementMeasurePosition; }

    void                  setMeasureElementVoicePosition (
                            const Rational&    voicePosition,
                            const std::string& context);

    Rational              getMeasureElementVoicePosition () const
                              { return fMeasureElementVoicePosition; }

//     void                  setMeasureElementMeasureMoment (
//                             const msrMoment& measureMoment,
//                             const std::string&         context);
//
//     const msrMoment&      getMeasureElementMeasureMoment () const
//                               { return fMeasureElementMeasureMoment; }
//
//     void                  setMeasureElementVoiceMoment (
//                             const msrMoment& voiceMoment,
//                             const std::string&         context);
//
//     const msrMoment&      getMeasureElementVoiceMoment () const
//                               { return fMeasureElementVoiceMoment; }

  public:

    // public services
    // ------------------------------------------------------

    static bool           compareMeasureElementsByIncreasingMeasurePosition (
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

    void                  doSetMeasureElementSoundingWholeNotes (
                            const Rational&    wholeNotes,
                            const std::string& context);

  protected:

    // protected fields
    // ------------------------------------------------------

    Rational              fMeasureElementSoundingWholeNotes;

    Rational              fMeasureElementMeasurePosition;
    Rational              fMeasureElementVoicePosition;

//     msrMoment             fMeasureElementMeasureMoment;
//     msrMoment             fMeasureElementVoiceMoment;
};
typedef SMARTP<msrMeasureElement> S_msrMeasureElement;
EXP std::ostream& operator << (std::ostream& os, const S_msrMeasureElement& elt);

}


#endif
