/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrMeasuresSlices___
#define ___msrMeasuresSlices___

#include "smartpointer.h"

#include "msrMeasures.h"
#include "msrNotes.h"


using namespace std;

namespace MusicFormats
{

//________________________________________________________________________
enum class msrNoteEventKind {
  kNoteEventStart,
  kNoteEventStop
};

string noteEventKindAsString (
  msrNoteEventKind noteEventKind);

//________________________________________________________________________
class msrNoteEvent : public smartable
{
  public:

    // creation
    // ------------------------------------------------------

      static SMARTP<msrNoteEvent> create (
                            const rational&  noteEventPositionInMeasure,
                            S_msrNote        noteEventNote,
                            msrNoteEventKind noteEventKind);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrNoteEvent (
                            const rational&  noteEventPositionInMeasure,
                            S_msrNote        noteEventNote,
                            msrNoteEventKind noteEventKind);

    virtual               ~msrNoteEvent ();

  public:

    // set and get
    // ------------------------------------------------------

    rational              getNoteEventPositionInMeasure () const
                              { return fNoteEventPositionInMeasure; }

    S_msrNote             getNoteEventNote () const
                              { return fNoteEventNote; }

    msrNoteEventKind      getNoteEventKind () const
                              { return fNoteEventKind; }

  public:

    // public services
    // ------------------------------------------------------

    static bool           compareNotesEventsByIncreasingPositionInMeasure (
                            const SMARTP<msrNoteEvent>& first,
                            const SMARTP<msrNoteEvent>& second);

  public:

    // print
    // ------------------------------------------------------

    virtual string        asString () const;

    virtual void          print (ostream& os) const;

  private:

    // private fields
    // ------------------------------------------------------

    rational              fNoteEventPositionInMeasure;
    S_msrNote             fNoteEventNote;
    msrNoteEventKind      fNoteEventKind;
};
typedef SMARTP<msrNoteEvent> S_msrNoteEvent;
EXP ostream& operator<< (ostream& os, const S_msrNoteEvent& elt);

//________________________________________________________________________
class msrSimultaneousNotesChunk : public smartable
{
  public:

    // creation
    // ------------------------------------------------------

      static SMARTP<msrSimultaneousNotesChunk> create (
                            const rational& chunkPositionInMeasure);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrSimultaneousNotesChunk (
                            const rational& chunkPositionInMeasure);

    virtual               ~msrSimultaneousNotesChunk ();

  public:

    // set and get
    // ------------------------------------------------------

    rational              getChunkPositionInMeasure () const
                              { return fChunkPositionInMeasure; }

    const list<S_msrNote>&
                          getChunkNotesList () const
                              { return fChunkNotesList; }


    rational              getChunkWholeNotes () const
                              { return fChunkWholeNotes; }

  public:

    // public services
    // ------------------------------------------------------

    void                  appendNoteToSimultaneousNotesChunk (
                            S_msrNote note)
                              { fChunkNotesList.push_back (note); }

    static bool           compareSimultaneousNotesChunksByIncreasingPositionInMeasure (
                            const SMARTP<msrNoteEvent>& first,
                            const SMARTP<msrNoteEvent>& second);

  public:

    // print
    // ------------------------------------------------------

    virtual string        asString () const;

    virtual void          print (ostream& os) const;

  private:

    // private fields
    // ------------------------------------------------------

    rational              fChunkPositionInMeasure;
    list<S_msrNote>       fChunkNotesList;
    rational              fChunkWholeNotes;
};
typedef SMARTP<msrSimultaneousNotesChunk> S_msrSimultaneousNotesChunk;
EXP ostream& operator<< (ostream& os, const S_msrSimultaneousNotesChunk& elt);

//______________________________________________________________________________
class msrMeasuresSlice;
typedef SMARTP<msrMeasuresSlice> S_msrMeasuresSlice;

class EXP msrMeasuresSlice : public smartable
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrMeasuresSlice> create (
                            const string& measureNumber,
                            int           puristMeasureNumber);

    SMARTP<msrMeasuresSlice> createMeasuresSliceShallowCopy ();

    // cloning
    // ------------------------------------------------------

  protected:

                          msrMeasuresSlice (
                            const string& measureNumber,
                            int           puristMeasureNumber);

    virtual               ~msrMeasuresSlice ();

  public:

    // set and get
    // ------------------------------------------------------

    string                getSliceMeasureNumber () const
                              { return fSliceMeasureNumber; }

    int                   getSlicePuristMeasureNumber () const
                              { return fSlicePuristMeasureNumber; }

    const vector<S_msrMeasure>&
                          getSliceMeasuresVector () const
                              { return fSliceMeasuresVector; }

    int                   getSliceSize () const
                              { return fSliceMeasuresVector.size (); }

    // notes flat list

    const list<S_msrNote>&
                          getSliceNotesFlatList () const
                              { return fSliceNotesFlatList; }

    // note events list

    const list<S_msrNoteEvent>&
                          getSliceNoteEventsList () const
                              { return fSliceNoteEventsList; }

    // simultaneous notes chunks list

    const list<S_msrSimultaneousNotesChunk>&
                          getSliceSimultaneousNotesChunksList () const
                              { return fSliceSimultaneousNotesChunksList; }

  public:

    // public services
    // ------------------------------------------------------

    void                  appendMeasureToMeasureSlice (
                            int          inputLineNumber,
                            S_msrMeasure measure);

    void                  appendSliceMeasuresFrom (
                            int                inputLineNumber,
                            S_msrMeasuresSlice otherSlice);

    void                  collectNonSkipNotesFromMeasuresSliceMeasures ();

    void                  buildTheSimutaneousNotesChunksList ();

    void                  identifySoloNotesAndRestsInMeasuresSlice ();

  public:

    // print
    // ------------------------------------------------------

    virtual string        asShortString () const;
    virtual string        asString () const;
    virtual string        asShortStringForMeasuresSlices () const;

    virtual void          print (ostream& os) const;

    virtual void          printShort (ostream& os) const;

  protected:

    // protected fields
    // ------------------------------------------------------

    int                   fSlicePuristMeasureNumber;
    string                fSliceMeasureNumber;

    // the measures in the slice
    vector<S_msrMeasure>  fSliceMeasuresVector;

    // notes flat list
    list<S_msrNote>       fSliceNotesFlatList;

    // note events list
    list<S_msrNoteEvent>  fSliceNoteEventsList;

    // simultaneous notes chunks list
    list<S_msrSimultaneousNotesChunk>
                          fSliceSimultaneousNotesChunksList;
};
EXP ostream& operator<< (ostream& os, const S_msrMeasuresSlice& elt);

//______________________________________________________________________________
class msrMeasuresSlicesSequence;
typedef SMARTP<msrMeasuresSlicesSequence> S_msrMeasuresSlicesSequence;

class EXP msrMeasuresSlicesSequence : public smartable
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrMeasuresSlicesSequence> create (
                            const string& measuresOrigin);

    static SMARTP<msrMeasuresSlicesSequence> create (
                            const string& measuresOrigin,
                            int           measuresNumber);

    SMARTP<msrMeasuresSlicesSequence> createMeasuresSlicesSequenceShallowCopy (
                            const string& measuresOrigin);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrMeasuresSlicesSequence (
                            const string& measuresOrigin);

                          msrMeasuresSlicesSequence (
                            const string& measuresOrigin,
                            int           measuresNumber);

    virtual               ~msrMeasuresSlicesSequence ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setMeasuresOrigin (const string& origin)
                              { fMeasuresOrigin = origin; }

    string                getMeasuresOrigin () const
                              { return fMeasuresOrigin; }

    const vector<S_msrMeasuresSlice>&
                          getMeasuresSlicesVector () const
                              { return fMeasuresSlicesVector; }

    int                   getSlicesSequenceSize () const
                              { return fMeasuresSlicesVector.size (); }

  public:

    // public services
    // ------------------------------------------------------

    void                  appendMeasuresSliceToSequence (
                            S_msrMeasuresSlice measuresSlice);

    S_msrMeasuresSlicesSequence
                          mergeWithMeasuresSlicesSequence (
                            int           inputLineNumber,
                            const string& measuresOrigin,
                            S_msrMeasuresSlicesSequence
                                          otherMeasuresSlicesSequence);

    void                  identifySoloNotesAndRests ();

  public:

    // print
    // ------------------------------------------------------

    virtual string        asShortString () const;
    string                asString () const;
    virtual string        asShortStringForMeasuresSlices () const;

    void                  print (ostream& os) const;

    virtual void          printShort (ostream& os) const;

  private:

    // private fields
    // ------------------------------------------------------

    string                fMeasuresOrigin;

    vector<S_msrMeasuresSlice>
                          fMeasuresSlicesVector;
};
EXP ostream& operator<< (ostream& os, const S_msrMeasuresSlicesSequence& elt);


} // namespace MusicFormats


#endif
