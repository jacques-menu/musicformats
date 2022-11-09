/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrMidi___
#define ___msrMidi___

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
                            int           inputLineNumber,
                            const string& midiTempoDuration,
                            int           midiTempoPerSecond);

    SMARTP<msrMidiTempo> createMsrMidiTempoNewbornClone ();

  public:

    // constructors/destructor
    // ------------------------------------------------------

                          msrMidiTempo (
                            int           inputLineNumber,
                            const string& midiTempoDuration,
                            int           midiTempoPerSecond);

    msrMidiTempo ();

    virtual               ~msrMidiTempo ();

  public:

    // set and get
    // ------------------------------------------------------
    void                  setMidiTempoDuration (const string& value)
                              { fMidiTempoDuration = value; }

    string                getMidiTempoDuration () const
                              { return fMidiTempoDuration; }

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

    string                asString () const override;

    void                  print (ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    string                fMidiTempoDuration;
    int                   fMidiTempoPerSecond;
};
typedef SMARTP<msrMidiTempo> S_msrMidiTempo;
EXP ostream& operator << (ostream& os, const S_msrMidiTempo& elt);


} // namespace MusicFormats


#endif
