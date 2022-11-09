/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrBeams___
#define ___msrBeams___

#include "msrElements.h"


namespace MusicFormats
{

// beams
//______________________________________________________________________________

/*
  MusicXML beam hooks:
    see https://forums.makemusic.com/viewtopic.php?f=12&t=1397
    and the examples at https://github.com/jacques-menu/musicformats/tree/master/files

  Michael Good Recordare LLC:
    An example of a "backward hook" is the 16th-note beam when a dotted eighth is beamed to a 16th.
    A "forward hook" would be the 16th note beam when a 16th is beamed to a dotted eighth.
    So in both these cases, the hook would be beam number 2 (the 16th beam).

    In most cases, the hooks follow what would be "normal" beaming for the given durations.
    But this is not always true, especially when there are secondary beam breaks.
    The Ave Maria example has both hooks and secondary beam breaks in measure 6 of the voice part,
    though not simultaneously.

    Say you beamed a dotted-eighth, sixteenth, sixteenth, dotted-eighth pattern together.
    If you broke the 16th beam, you would use "backward hook" followed by "forward hook".
    If not, you would use "begin" followed by "end".
*/

enum class msrBeamKind {
  kBeam_NO_,

  kBeamBegin, kBeamContinue, kBeamEnd,
  kBeamForwardHook, kBeamBackwardHook
};

EXP string msrBeamKindAsString (
  msrBeamKind beamKind);

ostream& operator << (ostream& os, const msrBeamKind& elt);

//______________________________________________________________________________
class EXP msrBeam : public msrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrBeam> create (
                            int         inputLineNumber,
                            int         number,
                            msrBeamKind beamKind);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrBeam (
                            int         inputLineNumber,
                            int         number,
                            msrBeamKind beamKind);

    virtual               ~msrBeam ();

  public:

    // set and get
    // ------------------------------------------------------

    msrBeamKind           getBeamKind   () const
                              { return fBeamKind; }

    int                   getBeamNumber () const
                              { return fBeamNumber; }

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

    int                   fBeamNumber;
    msrBeamKind           fBeamKind;
};
typedef SMARTP<msrBeam> S_msrBeam;
EXP ostream& operator << (ostream& os, const S_msrBeam& elt);


} // namespace MusicFormats


#endif
