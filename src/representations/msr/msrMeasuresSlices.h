/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrMeasuresSlices___
#define ___msrMeasuresSlices___

#include "smartpointer.h"

#include "msrNotes.h"


namespace MusicFormats
{

//________________________________________________________________________
enum class msrNoteEventKind {
  kNoteEventStart,
  kNoteEventStop
};

std::string msrNoteEventKindAsString (
  msrNoteEventKind noteEventKind);

std::ostream& operator << (std::ostream& os, const msrNoteEventKind& elt);

//________________________________________________________________________
class msrNoteEvent : public smartable
{
  public:

    // creation
    // ------------------------------------------------------

      static SMARTP<msrNoteEvent> create (
                            const mfPositionInMeasure&  noteEventPositionInMeasure,
                            const S_msrNote&        noteEventNote,
                            msrNoteEventKind noteEventKind);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrNoteEvent (
                            const mfPositionInMeasure&  noteEventPositionInMeasure,
                            const S_msrNote&        noteEventNote,
                            msrNoteEventKind noteEventKind);

    virtual               ~msrNoteEvent ();

  public:

    // set and get
    // ------------------------------------------------------

    mfPositionInMeasure   getNoteEventPositionInMeasure () const
                              { return fNoteEventPositionInMeasure; }

    S_msrNote             getNoteEventNote () const
                              { return fNoteEventNote; }

    msrNoteEventKind      getNoteEventKind () const
                              { return fNoteEventKind; }

  public:

    // public services
    // ------------------------------------------------------

    static bool           compareNoteEventsByIncreasingPositionInMeasure (
                            const SMARTP<msrNoteEvent>& first,
                            const SMARTP<msrNoteEvent>& second);

  public:

    // print
    // ------------------------------------------------------

    virtual std::string   asString () const;

    virtual void          print (std::ostream& os) const;

  private:

    // private fields
    // ------------------------------------------------------

    mfPositionInMeasure   fNoteEventPositionInMeasure;

    S_msrNote             fNoteEventNote;
    msrNoteEventKind      fNoteEventKind;
};
typedef SMARTP<msrNoteEvent> S_msrNoteEvent;
EXP std::ostream& operator << (std::ostream& os, const S_msrNoteEvent& elt);

//________________________________________________________________________
class msrSimultaneousNotesChunk : public smartable
{
  public:

    // creation
    // ------------------------------------------------------

      static SMARTP<msrSimultaneousNotesChunk> create (
                            const mfWholeNotes& chunkPositionInMeasure);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrSimultaneousNotesChunk (
                            const mfWholeNotes& chunkPositionInMeasure);

    virtual               ~msrSimultaneousNotesChunk ();

  public:

    // set and get
    // ------------------------------------------------------

    mfWholeNotes         getChunkPositionInMeasure () const
                              { return fChunkPositionInMeasure; }

    const std::list <S_msrNote>&
                          getChunkNotesList () const
                              { return fChunkNotesList; }


    mfWholeNotes         getChunkDurationWholeNotes () const
                              { return fChunkDurationWholeNotes; }

  public:

    // public services
    // ------------------------------------------------------

    void                  appendNoteToSimultaneousNotesChunk (
                            const S_msrNote& note)
                              { fChunkNotesList.push_back (note); }

    static bool           compareSimultaneousNotesChunksByIncreasingPositionInMeasure (
                            const SMARTP<msrNoteEvent>& first,
                            const SMARTP<msrNoteEvent>& second);

  public:

    // print
    // ------------------------------------------------------

    virtual std::string   asString () const;

    virtual void          print (std::ostream& os) const;

  private:

    // private fields
    // ------------------------------------------------------

    mfWholeNotes         fChunkPositionInMeasure;
    std::list <S_msrNote>  fChunkNotesList;
    mfWholeNotes         fChunkDurationWholeNotes;
};
typedef SMARTP<msrSimultaneousNotesChunk> S_msrSimultaneousNotesChunk;
EXP std::ostream& operator << (std::ostream& os, const S_msrSimultaneousNotesChunk& elt);

//______________________________________________________________________________
// PRE-declaration
class   msrMeasuresSlice;
typedef SMARTP<msrMeasuresSlice> S_msrMeasuresSlice;

class EXP msrMeasuresSlice : public smartable
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrMeasuresSlice> create (
                            const std::string& measureNumber,
                            int                puristMeasureNumber);

    SMARTP<msrMeasuresSlice> createMeasuresSliceShallowCopy ();

    // cloning
    // ------------------------------------------------------

  protected:

                          msrMeasuresSlice (
                            const std::string& measureNumber,
                            int                puristMeasureNumber);

    virtual               ~msrMeasuresSlice ();

  public:

    // set and get
    // ------------------------------------------------------

    std::string           getSliceMeasureNumber () const
                              { return fSliceMeasureNumber; }

    int                   getSlicePuristMeasureNumber () const
                              { return fSlicePuristMeasureNumber; }

    const std::vector <S_msrMeasure>&
                          getSliceMeasuresVector () const
                              { return fSliceMeasuresVector; }

    int                   getSliceSize () const
                              { return fSliceMeasuresVector.size (); }

    // notes flat list

    const std::list <S_msrNote>&
                          getSliceNotesFlatList () const
                              { return fSliceNotesFlatList; }

    // note events list

    const std::list <S_msrNoteEvent>&
                          getSliceNoteEventsList () const
                              { return fSliceNoteEventsList; }

    // simultaneous notes chunks list

    const std::list <S_msrSimultaneousNotesChunk>&
                          getSliceSimultaneousNotesChunksList () const
                              { return fSliceSimultaneousNotesChunksList; }

  public:

    // public services
    // ------------------------------------------------------

    void                  appendMeasureToMeasureSlice (
                            const S_msrMeasure& measure);

    void                  appendSliceMeasuresFrom (
                            const S_msrMeasuresSlice& otherSlice);

    void                  collectNonSkipNotesFromMeasuresSliceMeasures ();

    void                  buildTheSimutaneousNotesChunksList ();

    void                  identifySoloNotesAndRestsInMeasuresSlice ();

  public:

    // print
    // ------------------------------------------------------

    virtual std::string   asShortString () const;
    virtual std::string   asString () const;

    virtual std::string   asStringForMeasuresSlices () const;

    virtual void          print (std::ostream& os) const;

  protected:

    // protected fields
    // ------------------------------------------------------

    int                   fSlicePuristMeasureNumber;
    std::string           fSliceMeasureNumber;

    // the measures in the slice
    std::vector <S_msrMeasure>
                          fSliceMeasuresVector;

    // notes flat list
    std::list <S_msrNote>  fSliceNotesFlatList;

    // note events list
    std::list <S_msrNoteEvent>
                          fSliceNoteEventsList;

    // simultaneous notes chunks list
    std::list <S_msrSimultaneousNotesChunk>
                          fSliceSimultaneousNotesChunksList;
};
EXP std::ostream& operator << (std::ostream& os, const S_msrMeasuresSlice& elt);

//______________________________________________________________________________
class   msrMeasuresSlicesSequence;
typedef SMARTP<msrMeasuresSlicesSequence> S_msrMeasuresSlicesSequence;

class EXP msrMeasuresSlicesSequence : public smartable
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrMeasuresSlicesSequence> create (
                            const std::string& measuresOrigin);

    static SMARTP<msrMeasuresSlicesSequence> create (
                            const std::string& measuresOrigin,
                            int           measuresNumber);

    SMARTP<msrMeasuresSlicesSequence> createMeasuresSlicesSequenceShallowCopy (
                            const std::string& measuresOrigin);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrMeasuresSlicesSequence (
                            const std::string& measuresOrigin);

                          msrMeasuresSlicesSequence (
                            const std::string& measuresOrigin,
                            int           measuresNumber);

    virtual               ~msrMeasuresSlicesSequence ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setMeasuresOrigin (const std::string& origin)
                              { fMeasuresOrigin = origin; }

    std::string           getMeasuresOrigin () const
                              { return fMeasuresOrigin; }

    const std::vector <S_msrMeasuresSlice>&
                          getMeasuresSlicesVector () const
                              { return fMeasuresSlicesVector; }

    int                   getSlicesSequenceSize () const
                              { return fMeasuresSlicesVector.size (); }

  public:

    // public services
    // ------------------------------------------------------

    void                  appendMeasuresSliceToSequence (
                            const S_msrMeasuresSlice& measuresSlice);

    S_msrMeasuresSlicesSequence
                          mergeWithMeasuresSlicesSequence (
                            int           inputLineNumber,
                            const std::string& measuresOrigin,
                            const S_msrMeasuresSlicesSequence&
                                          otherMeasuresSlicesSequence);

    void                  identifySoloNotesAndRests ();

  public:

    // print
    // ------------------------------------------------------

    virtual std::string   asShortString () const;
    std::string           asString () const;

    virtual std::string   asStringForMeasuresSlices () const;

    void                  print (std::ostream& os) const;

    void                  printMeasuresSlicesVector (std::ostream& os) const;

  private:

    // private fields
    // ------------------------------------------------------

    std::string           fMeasuresOrigin;

    std::vector <S_msrMeasuresSlice>
                          fMeasuresSlicesVector;
};
EXP std::ostream& operator << (std::ostream& os, const S_msrMeasuresSlicesSequence& elt);


}


#endif // ___msrMeasuresSlices___
