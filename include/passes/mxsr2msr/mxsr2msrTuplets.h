/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2024

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___mxsrTuplets___
#define ___mxsrTuplets___

// #include <map>
//
#include "smartpointer.h"

#include "msrTupletsEnumTypes.h"


using namespace MusicXML2;

namespace MusicFormats
{

/*
  The part groups are described for the neded of this pass by class mxsrPartGroup,
  which is distinct from msrPartGroup, an instance of which they contain.

  The identifiers for msrPartGroups instances contain 'Msr'
  wherever needed to make things clear
*/

//________________________________________________________________________
class mxsrTuplet : public smartable
{
/*
  there can be multiple tuplets starts and stops on a given note,
  so they aref kept aside until they are handled
*/
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<mxsrTuplet> create (
                            int               tupletInputStartLineNumber,
                            int               tupletNumber,
                            msrTupletTypeKind tupletTypeKind);

  public:

    // constructors/destructor
    // ------------------------------------------------------

                          mxsrTuplet (
                            int               tupletInputStartLineNumber,
                            int               tupletNumber,
                            msrTupletTypeKind tupletTypeKind);

    virtual               ~mxsrTuplet ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setTupletInputStartLineNumber (
                            int tupletInputStartLineNumber)
                              { fTupletInputStartLineNumber = tupletInputStartLineNumber; }

    int                   getTupletInputStartLineNumber () const
                              { return fTupletInputStartLineNumber; }

    void                  setTupletNumber (
                            int tupletNumber)
                              { fTupletNumber = tupletNumber; }

    int                   getTupletNumber () const
                              { return fTupletNumber; }

    void                  setTupletTypeKind (
                            msrTupletTypeKind tupletTypeKind)
                              { fTupletTypeKind = tupletTypeKind; }

    msrTupletTypeKind     getTupletTypeKind () const
                              { return fTupletTypeKind; }

  public:

    // public services
    // ------------------------------------------------------

  public:

    // print
    // ------------------------------------------------------

    std::string           asString () const;

    virtual void          print (std::ostream& os) const;

  private:

    // private fields
    // ------------------------------------------------------

    int                   fTupletInputStartLineNumber;
    int                   fTupletNumber;
    msrTupletTypeKind     fTupletTypeKind;
};
typedef SMARTP<mxsrTuplet> S_mxsrTuplet;
EXP std::ostream& operator << (std::ostream& os, const mxsrTuplet& elt);
EXP std::ostream& operator << (std::ostream& os, const S_mxsrTuplet& elt);

//________________________________________________________________________
class mxsr2msrPendingTupletStop : public smartable
{
/*
  tuplets and staff changes can occur in any order in MusicXML data,
  hence the need to handle them per voice
*/
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<mxsr2msrPendingTupletStop> create (
                            int eventSequentialNumber,
                            int noteSequentialNumber,
                            int staffNumber,
                            int voiceNumber,
                            int tupletNumber,
                            int eventInputStartLineNumber,
                            int eventInputEndLineNumber);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          mxsr2msrPendingTupletStop (
                            int eventSequentialNumber,
                            int noteSequentialNumber,
                            int staffNumber,
                            int voiceNumber,
                            int tupletNumber,
                            int eventInputStartLineNumber,
                            int eventInputEndLineNumber);

    virtual               ~mxsr2msrPendingTupletStop ();

  public:

    // set and get
    // ------------------------------------------------------

    int                   getEventSequentialNumber () const
                              { return fEventSequentialNumber; }

    void                  setEventSequentialNumber (
                            int eventSequentialNumber)
                              { fEventSequentialNumber = eventSequentialNumber; }

    int                   getNoteSequentialNumber () const
                              { return fNoteSequentialNumber; }

    int                   getStaffNumber () const
                              { return fStaffNumber; }

    int                   getVoiceNumber () const
                              { return fVoiceNumber; }

    int                   getTupletNumber () const
                              { return fTupletNumber; }

    void                  setEventInputStartLineNumber (
                            int inputStartLineNumber)
                              { fEventInputStartLineNumber = inputStartLineNumber; }

    int                   getEventInputStartLineNumber () const
                              { return fEventInputStartLineNumber; }

    void                  setEventInputEndLineNumber (
                            int inputEndLineNumber)
                              { fEventInputEndLineNumber = inputEndLineNumber; }

    int                   getEventInputEndLineNumber () const
                              { return fEventInputEndLineNumber; }

  public:

    // public services
    // ------------------------------------------------------

  public:

    // print
    // ------------------------------------------------------

    std::string           asString () const;

    virtual void          print (std::ostream& os) const;

  private:

    // private fields
    // ------------------------------------------------------

    int                   fEventSequentialNumber;
    int                   fNoteSequentialNumber;

    int                   fStaffNumber;
    int                   fVoiceNumber;

    int                   fTupletNumber;

    int                   fEventInputStartLineNumber;
    int                   fEventInputEndLineNumber;

  private:

    // private work fields
    // ------------------------------------------------------

  private:

    // private methods
    // ------------------------------------------------------
};
typedef SMARTP<mxsr2msrPendingTupletStop> S_mxsr2msrPendingTupletStop;
EXP std::ostream& operator << (std::ostream& os, const mxsr2msrPendingTupletStop& elt);
EXP std::ostream& operator << (std::ostream& os, const S_mxsr2msrPendingTupletStop& elt);

}

#endif // ___mxsrTuplets___
