/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrMidi___
#define ___msrMidi___

#include "mfBasicTypes.h"

#include "msrElements.h"


namespace MusicFormats
{

//______________________________________________________________________________
class EXP msrMidiTempo : public msrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrMidiTempo> create (
                            const mfInputLineNumber& inputLineNumber,
                            const std::string& midiTempoNotesDuration,
                            int           midiTempoPerSecond);

    SMARTP<msrMidiTempo> createMsrMidiTempoNewbornClone ();

  public:

    // constructors/destructor
    // ------------------------------------------------------

                          msrMidiTempo (
                            const mfInputLineNumber& inputLineNumber,
                            const std::string& midiTempoNotesDuration,
                            int           midiTempoPerSecond);

    msrMidiTempo ();

    virtual               ~msrMidiTempo ();

  public:

    // set and get
    // ------------------------------------------------------
    void                  setMidiTempoNotesDuration (const std::string& value)
                              { fMidiTempoNotesDuration = value; }

    std::string           getMidiTempoNotesDuration () const
                              { return fMidiTempoNotesDuration; }

    void                  setMidiTempoPerSecond (int value)
                              { fMidiTempoPerSecond = value; }

    int                   getMidiTempoPerSecond () const
                              { return fMidiTempoPerSecond; }

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

    std::string           fMidiTempoNotesDuration;
    int                   fMidiTempoPerSecond;
};
typedef SMARTP<msrMidiTempo> S_msrMidiTempo;
EXP std::ostream& operator << (std::ostream& os, const S_msrMidiTempo& elt);


}


#endif // ___msrMidi___
