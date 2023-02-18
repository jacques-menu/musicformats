/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrNotesDurations___
#define ___msrNotesDurations___

#include <string>
#include <ostream>

#include "msrWholeNotes.h"


namespace MusicFormats
{

// durations
//______________________________________________________________________________
enum class msrNotesDurationKind {
  kNotesDuration_UNKNOWN,

  // from longest to shortest for the algorithms
  kNotesDurationMaxima, kNotesDurationLonga, kNotesDurationBreve,
  kNotesDurationWhole, kNotesDurationHalf,
  kNotesDurationQuarter,
  kNotesDurationEighth, kNotesDuration16th, kNotesDuration32nd, kNotesDuration64th,
  kNotesDuration128th, kNotesDuration256th, kNotesDuration512th, kNotesDuration1024th
};

std::string msrNotesDurationKindAsString (msrNotesDurationKind notesNotesDurationKind);

std::ostream& operator << (std::ostream& os, const msrNotesDurationKind& elt);

EXP msrNotesDurationKind msrNotesDurationKindFromMusicXMLString (
  int                inputLineNumber,
  const std::string& durationString);

EXP msrNotesDurationKind msrNotesDurationKindFromInteger (
  int inputLineNumber,
  int durationInteger);

EXP msrNotesDurationKind msrNotesDurationKindFromString (
  int                inputLineNumber,
  const std::string& durationString);

EXP msrWholeNotes wholeNotesFromNotesDurationKindAndDotsNumber (
  msrNotesDurationKind notesNotesDurationKind,
  int                  dotsNumber);

EXP msrWholeNotes msrNotesDurationKindAsWholeNotes (
  msrNotesDurationKind notesNotesDurationKind);

EXP msrNotesDurationKind wholeNotesAsNotesDurationKind (msrWholeNotes wholeNotes);

std::string msrNotesDurationKindAsMusicXMLType (msrNotesDurationKind notesNotesDurationKind);

// durations
//______________________________________________________________________________
class EXP msrNotesDuration
{
  public:

    // constructors/destructor
    // ------------------------------------------------------

                          msrNotesDuration ();

                          msrNotesDuration (
                            msrNotesDurationKind notesNotesDurationKind,
                            int                  dotsNumber);

    virtual               ~msrNotesDuration ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setNotesDurationKind (
    												msrNotesDurationKind notesNotesDurationKind)
                              { fNotesDurationKind = notesNotesDurationKind; }

    msrNotesDurationKind  getNotesDurationKind () const
                              { return fNotesDurationKind; }

    void                  setDotsNumber (int dotsNumber)
                              { fDotsNumber = dotsNumber; }

    int                   getDotsNumber () const
                              { return fDotsNumber; }

  public:

    // public services
    // ------------------------------------------------------

    void                  incrDotsNumber ()
                              { ++fDotsNumber; }

    msrWholeNotes          dottedNotesDurationAsWholeNotes_FOR_TEMPO (
                            int inputLineNumber) const;

    msrWholeNotes          dottedNotesDurationAsWholeNotes (
                            int inputLineNumber) const;

  public:

    // visitors
    // ------------------------------------------------------

/* JMI
    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;
*/

  public:

    // ------------------------------------------------------

    std::string           asString () const;

    void                  print (std::ostream& os) const;

  private:

    // private fields
    // ------------------------------------------------------

    msrNotesDurationKind  fNotesDurationKind;
    int                   fDotsNumber;
};
EXP std::ostream& operator << (std::ostream& os, const msrNotesDuration& elt);

// dotted durations
//______________________________________________________________________________
class EXP msrDottedNotesDuration
{
  public:

    // constructors/destructor
    // ------------------------------------------------------

                          msrDottedNotesDuration ();

                          msrDottedNotesDuration (
                            msrNotesDurationKind notesNotesDurationKind,
                            int                  dotsNumber);

    virtual               ~msrDottedNotesDuration ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setNotesDurationKind (
    												msrNotesDurationKind notesNotesDurationKind)
                              { fNotesDurationKind = notesNotesDurationKind; }

    msrNotesDurationKind  getNotesDurationKind () const
                              { return fNotesDurationKind; }

    void                  setDotsNumber (int dotsNumber)
                              { fDotsNumber = dotsNumber; }

    int                   getDotsNumber () const
                              { return fDotsNumber; }

  public:

    // public services
    // ------------------------------------------------------

    void                  incrDotsNumber ()
                              { ++fDotsNumber; }

    msrWholeNotes          dottedNotesDurationAsWholeNotes_FOR_TEMPO (
                            int inputLineNumber) const;

    msrWholeNotes          dottedNotesDurationAsWholeNotes (
                            int inputLineNumber) const;

  public:

    // visitors
    // ------------------------------------------------------

/* JMI
    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;
*/

  public:

    // ------------------------------------------------------

    std::string           asString () const;

    void                  print (std::ostream& os) const;

  private:

    // private fields
    // ------------------------------------------------------

    msrNotesDurationKind  fNotesDurationKind;
    int                   fDotsNumber;
};
EXP std::ostream& operator << (std::ostream& os, const msrDottedNotesDuration& elt);


}

#endif // ___msrNotesDurations___
