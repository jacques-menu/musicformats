/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrMeasureElements___
#define ___msrMeasureElements___

#include "msrElements.h"

#include "mfRational.h"


namespace MusicFormats
{

//______________________________________________________________________________
// PRE-declarations

class msrMeasure;
typedef SMARTP<msrMeasure> S_msrMeasure;

//______________________________________________________________________________
/*
  Various elements can found in measures,
  hence class msrMeasureElement
*/

class EXP msrMeasureElement : public msrElement
{
  public:

    // constants
    // ------------------------------------------------------

    static const string K_NO_MEASURE_NUMBER;

    static const Rational K_NO_WHOLE_NOTES;

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrMeasureElement (
                            int          inputLineNumber,
                            S_msrMeasure upLinkToMeasure);

    virtual               ~msrMeasureElement ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setMeasureElementUpLinkToMeasure (
                            S_msrMeasure measure);

    S_msrMeasure          getMeasureElementUpLinkToMeasure () const;

    void                  setMeasureElementSoundingWholeNotes (
                            const Rational& wholeNotes,
                            const string&   context);

    Rational              getMeasureElementSoundingWholeNotes () const
                              { return fMeasureElementSoundingWholeNotes; }

    // this method is overridden in sub-classes such as those for
    // time signatures, harmonies and figured bass elements,
    // as well as those elements that have an actual whole notes duration

    // the overrides call a class-specific method that can be called directly,
    // such occurrence are more explicit when debugging
    // the computations of positions in measures
    virtual void          setMeasureElementMeasurePosition (
                            const S_msrMeasure measure,
                            const Rational&    measurePosition,
                            const string&      context);

    Rational              getMeasureElementMeasurePosition () const
                              { return fMeasureElementMeasurePosition; }

    void                  setMeasureElementVoicePosition (
                            const Rational& voicePosition,
                            const string&   context);

    Rational              getMeasureElementVoicePosition () const
                              { return fMeasureElementVoicePosition; }

//     void                  setMeasureElementMeasureMoment (
//                             const msrMoment& measureMoment,
//                             const string&    context);
//
//     const msrMoment&      getMeasureElementMeasureMoment () const
//                               { return fMeasureElementMeasureMoment; }
//
//     void                  setMeasureElementVoiceMoment (
//                             const msrMoment& voiceMoment,
//                             const string&    context);
//
//     const msrMoment&      getMeasureElementVoiceMoment () const
//                               { return fMeasureElementVoiceMoment; }

  public:

    // public services
    // ------------------------------------------------------

    string                fetchMeasureElementMeasureNumber () const;

    static bool           compareMeasureElementsByIncreasingMeasurePosition (
                            const SMARTP<msrMeasureElement>& first,
                            const SMARTP<msrMeasureElement>& second);

//     virtual void          assignMeasureElementVoicePosition ( // JMI v0.9.66 REMOVE
//                             Rational&     voicePosition,
//                             const string& context);

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override = 0; // JMI ??? v0.9.66

  public:

    // print
    // ------------------------------------------------------

    string                asShortString () const override;
    string                asString () const override;

    void                  print (ostream& os) const override;

    void                  printSummary (ostream& os) const override {}

  protected:

    // protected methods
    // ------------------------------------------------------

    void                  doSetMeasureElementSoundingWholeNotes (
                            const Rational& wholeNotes,
                            const string&   context);

  protected:

    // protected fields
    // ------------------------------------------------------

    S_msrMeasure          fMeasureElementUpLinkToMeasure;

    Rational              fMeasureElementSoundingWholeNotes;

    Rational              fMeasureElementMeasurePosition;
    Rational              fMeasureElementVoicePosition;

//     msrMoment             fMeasureElementMeasureMoment;
//     msrMoment             fMeasureElementVoiceMoment;
};
typedef SMARTP<msrMeasureElement> S_msrMeasureElement;
EXP ostream& operator << (ostream& os, const S_msrMeasureElement& elt);


} // namespace MusicFormats


#endif
