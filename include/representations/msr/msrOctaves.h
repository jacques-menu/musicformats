/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrOctaves___
#define ___msrOctaves___

#include <string>
#include <ostream>


namespace MusicFormats
{

// octaves
//______________________________________________________________________________
enum class msrOctaveKind {
  kOctave_NO_,

  kOctave0, kOctave1, kOctave2, kOctave3,
  kOctave4, // that of middle C
  kOctave5, kOctave6, kOctave7, kOctave8, kOctave9
};

string msrOctaveKindAsString (msrOctaveKind octaveKind);

ostream& operator << (ostream& os, const msrOctaveKind& elt);

EXP int octaveNumberFromOctaveKind (msrOctaveKind octaveKind);

// successor and predecessor
msrOctaveKind octaveSucc (msrOctaveKind octaveKind);
msrOctaveKind octavePred (msrOctaveKind octaveKind);

/*
   // Declare prefix and postfix decrement operators.
   Point& operator--();       // Prefix decrement operator.
   Point operator--(int);     // Postfix decrement operator.
*/

// prefix operators
msrOctaveKind& operator++ (msrOctaveKind& octaveKind);
msrOctaveKind& operator-- (msrOctaveKind& octaveKind);

// postfix operators
msrOctaveKind operator++ (msrOctaveKind& octaveKind, int);
msrOctaveKind operator-- (msrOctaveKind& octaveKind, int);

EXP msrOctaveKind msrOctaveKindFromNumber (
  int inputLineNumber,
  int octaveNumber);

EXP msrOctaveKind msrOctaveKindFromCommasOrQuotes (
  int           inputLineNumber,
  const string& octaveIndication);

// octave entry
//______________________________________________________________________________
enum class msrOctaveEntryKind {
  kOctaveEntryRelative,
  kOctaveEntryAbsolute,
  kOctaveEntryFixed
};

string msrOctaveEntryKindAsString (
  msrOctaveEntryKind octaveEntryKind);

ostream& operator << (ostream& os, const msrOctaveEntryKind& elt);

extern map<string, msrOctaveEntryKind>
  gGlobalMsrOctaveEntryKindsMap;

string existingMsrOctaveEntryKinds (size_t namesListMaxLength);

void initializeMsrOctaveEntryKindsMap ();

// semitone pitches and octave
/* can be used as absolute, relative or fixed reference */
//______________________________________________________________________________
class EXP msrSemiTonesPitchAndOctave : public smartable
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrSemiTonesPitchAndOctave> create (
                            msrSemiTonesPitchKind semiTonesPitchKind,
                            msrOctaveKind         octaveKind);

    SMARTP<msrSemiTonesPitchAndOctave> createSemiTonesPitchAndOctaveNewbornClone ();

    // creation from  a string
    // ------------------------------------------------------

    static SMARTP<msrSemiTonesPitchAndOctave> createFromString (
                            int           inputLineNumber,
                            const string& theString);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrSemiTonesPitchAndOctave (
                            msrSemiTonesPitchKind semiTonesPitchKind,
                            msrOctaveKind         octaveKind);

    virtual               ~msrSemiTonesPitchAndOctave ();

  public:

    // set and get
    // ------------------------------------------------------

    msrSemiTonesPitchKind getSemiTonesPitchKind () const
                              { return fSemiTonesPitchKind; }

    void                  incrementOctaveKind ();
    void                  decrementOctaveKind ();

    msrOctaveKind         getOctaveKind () const
                              { return fOctaveKind; }

  public:

    // public services
    // ------------------------------------------------------

    string                asString () const;

  public:

    // print
    // ------------------------------------------------------

    virtual void          print (ostream& os) const;

  private:

    // private fields
    // ------------------------------------------------------

    msrSemiTonesPitchKind fSemiTonesPitchKind;
    msrOctaveKind         fOctaveKind;
};
typedef SMARTP<msrSemiTonesPitchAndOctave> S_msrSemiTonesPitchAndOctave;
EXP ostream& operator << (ostream& os, const S_msrSemiTonesPitchAndOctave& elt);

// semitone pitches and octave
// can be used as absolute, relative or fixed reference
//______________________________________________________________________________
class EXP msrQuarterTonesPitchAndOctave : public smartable
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrQuarterTonesPitchAndOctave> create (
                            msrQuarterTonesPitchKind quarterTonesPitchKind,
                            msrOctaveKind            octaveKind);

    SMARTP<msrQuarterTonesPitchAndOctave> createQuarterTonesPitchAndOctaveNewbornClone ();

    // creation from  a string
    // ------------------------------------------------------

    static SMARTP<msrQuarterTonesPitchAndOctave> createFromString (
                            int           inputLineNumber,
                            const string& theString);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrQuarterTonesPitchAndOctave (
                            msrQuarterTonesPitchKind quarterTonesPitchKind,
                            msrOctaveKind            octaveKind);

    virtual               ~msrQuarterTonesPitchAndOctave ();

  public:

    // set and get
    // ------------------------------------------------------

    msrQuarterTonesPitchKind
                          getQuarterTonesPitchKind () const
                              { return fQuarterTonesPitchKind; }

    void                  incrementOctaveKind ();
    void                  decrementOctaveKind ();

    msrOctaveKind         getOctaveKind () const
                              { return fOctaveKind; }

  public:

    // public services
    // ------------------------------------------------------

  public:

    // print
    // ------------------------------------------------------

    string                asString () const;

    virtual void          print (ostream& os) const;

  private:

    // private fields
    // ------------------------------------------------------

    msrQuarterTonesPitchKind
                          fQuarterTonesPitchKind;
    msrOctaveKind         fOctaveKind;
};
typedef SMARTP<msrQuarterTonesPitchAndOctave> S_msrQuarterTonesPitchAndOctave;
EXP ostream& operator << (ostream& os, const S_msrQuarterTonesPitchAndOctave& elt);


}

#endif
