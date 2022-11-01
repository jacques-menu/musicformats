/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___tracingOah___
#define ___tracingOah___


#include "enableTracingIfDesired.h"

#ifdef TRACING_IS_ENABLED // encompasses this whole file


#include <set>

#include "exports.h"

#include "oahAtomsCollection.h"


namespace MusicFormats
{

//______________________________________________________________________________
class EXP tracingOahGroup : public oahGroup
{
  public:

    static SMARTP<tracingOahGroup> create (
      S_oahHandler handler);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    tracingOahGroup (
      S_oahHandler handler);

    virtual               ~tracingOahGroup ();

  public:

    // initialisation
    // ------------------------------------------------------

    void                  createTheTracePrefixes (
                            S_oahHandler handler);

    void                  initializeTracingOahGroup ();

    void                  initializeOptionsTraceAndDisplayOptions ();

    void                  initializePrintLayoutstracingOah ();

    void                  initializeRepeatsToSlashestracingOah ();

    void                  initializeNotestracingOah ();

    void                  initializeNotesAttachmentstracingOah ();

    void                  initializeBooksToVoicestracingOah ();

    void                  initializeSegmentstracingOah ();

    void                  initializeMeasurestracingOah ();

    void                  initializeMeasuresSlicestracingOah ();

    void                  initializeInstrumentstracingOah ();

    void                  initializeChordsAndTupletstracingOah ();

    void                  initializeCreditsToWordstracingOah ();

    void                  initializeHarmoniestracingOah ();

    void                  initializeFiguredBassestracingOah ();

    void                  initializeSpannerstracingOah ();

    void                  initializeInterNotestracingOah ();

    void                  initializeClefsToTempostracingOah ();

    void                  initializeBreaksAndBarLinestracingOah ();

    void                  initializeAboveStaffTracingOah ();

    void                  initializeTranspositionstracingOah ();

    void                  initializeOthertracingOah ();

  public:

    // set and get
    // ------------------------------------------------------

    // options and help trace
    // --------------------------------------

    S_oahPrefix           getShortTracePrefix () const
                              { return fShortTracePrefix; }
    S_oahPrefix           getLongTracePrefix () const
                              { return fLongTracePrefix; }

    void                  setTracingOah ()
                              { fTracingOah = true; }
    Bool                  getTracingOah () const
                              { return fTracingOah; }

    S_oahBooleanAtom      getTracingOahBooleanAtom () const
                              { return fTracingOahBooleanAtom; }

// replaced by getEarlyTracingOahDetails()
//     Bool                  getTracingOahDetails () const
//                               { return fTracingOahDetails; }

    // other
    // --------------------------------------

    // history
    void                  setTraceComponents ()
                              { fTraceComponents = true; }

// replaced by getEarlyTraceComponents()
//     Bool                  getTraceComponents () const
//                               { return fTraceComponents; }

    // passes
    void                  setTracePasses ()
                              { fTracePasses = true; }

// replaced by getEarlyTracePasses()
//     Bool                  getTracePasses () const
//                               { return fTracePasses; }

    S_oahBooleanAtom      getTracePassesBooleanAtom () const
                              { return fTracePassesBooleanAtom; }

    // geometry
    void                  setTraceGeometry ()
                              { fTraceGeometry = true; }
    Bool                  getTraceGeometry () const
                              { return fTraceGeometry; }
    // identification
    void                  setTraceIdentification ()
                              { fTraceIdentification = true; }
    Bool                  getTraceIdentification () const
                              { return fTraceIdentification; }
    // for tests, hidden
    void                  setTraceForTests ()
                              { fTraceForTests = true; }
    Bool                  getTraceForTests () const
                              { return fTraceForTests; }


    // books to voices
    // --------------------------------------

    // books
    void                  setTraceBooks ()
                              { fTraceBooks = true; }
    Bool                  getTraceBooks () const
                              { return fTraceBooks; }
    // scores
    void                  setTraceScores ()
                              { fTraceScores = true; }
    Bool                  getTraceScores () const
                              { return fTraceScores; }
    // part groups
    void                  setTracePartGroups ()
                              { fTracePartGroups = true; }
    Bool                  getTracePartGroups () const
                              { return fTracePartGroups; }
    Bool                  getTracePartGroupsDetails () const
                              { return fTracePartGroupsDetails; }
    // parts
    void                  setTraceParts ()
                              { fTraceParts = true; }
    Bool                  getTraceParts () const
                              { return fTraceParts; }
    // staves
    void                  setTraceStaves ()
                              { fTraceStaves = true; }
    Bool                  getTraceStaves () const
                              { return fTraceStaves; }
    // staff details
    Bool                  getTraceStaffDetails () const
                              { return fTraceStaffDetails; }
    // staff changes
    Bool                  getTraceStaffChanges () const
                              { return fTraceStaffChanges; }
    // voices
    void                  setTraceVoices ()
                              { fTraceVoices = true; }
    Bool                  getTraceVoices () const
                              { return fTraceVoices; }
    Bool                  getTraceVoicesDetails () const
                              { return fTraceVoicesDetails; }


    // print layouts
    // --------------------------------------

    Bool                  getTracePrintLayouts () const
                              { return fTracePrintLayouts; }

    // segments and measures
    // --------------------------------------

    // segments
    void                  setTraceSegments ()
                              { fTraceSegments = true; }
    Bool                  getTraceSegments () const
                              { return fTraceSegments; }

    Bool                  getTraceSegmentsDetails () const
                              { return fTraceSegmentsDetails; }
    // measures
    Bool                  getTraceMeasuresNumbers () const
                              { return fTraceMeasuresNumbers; }

    void                  setTraceMeasures ()
                              { fTraceMeasures = true; }
    Bool                  getTraceMeasures () const
                              { return fTraceMeasures; }

    Bool                  getTraceMeasuresDetails () const
                              { return fTraceMeasuresDetails; }

    Bool                  getTraceMeasurePositions () const
                              { return fTraceMeasurePositions; }
    Bool                  getTraceMomentsInMeasures () const
                              { return fTraceMomentsInMeasures; }

    set<int>              getTraceDetailedMeasureNumbersSet () const
                              { return fTraceDetailedMeasureNumbersSet; }

    // measures slices
    Bool                  getTraceMeasuresSlices () const
                              { return fTraceMeasuresSlices; }

    Bool                  getTraceMeasuresSlicesDetails () const
                              { return fTraceMeasuresSlicesDetails; }


    // clefs to tempos
    // --------------------------------------

    // clefs
    void                  setTraceClefs ()
                              { fTraceClefs = true; }
    Bool                  getTraceClefs () const
                              { return fTraceClefs; }
    // keys
    void                  setTraceKeys ()
                              { fTraceKeys = true; }
    void                  setTraceKeysDetails ()
                              { fTraceKeysDetails = true; }

    Bool                  getTraceKeys () const
                              { return fTraceKeys; }
    Bool                  getTraceKeysDetails () const
                              { return fTraceKeysDetails; }
    // time signatures
    void                  setTraceTimeSignatures ()
                              { fTraceTimeSignatures = true; }
    Bool                  getTraceTimeSignatures () const
                              { return fTraceTimeSignatures; }
    // tempos
    void                  setTraceTempos ()
                              { fTraceTempos = true; }
    Bool                  getTraceTempos () const
                              { return fTraceTempos; }


    // breaks and barLines
    // --------------------------------------

    // line breaks
    void                  setTraceLineBreaks ()
                              { fTraceLineBreaks = true; }
    Bool                  getTraceLineBreaks () const
                              { return fTraceLineBreaks; }
    // page and line breaks
    void                  setTracePageBreaks ()
                              { fTracePageBreaks = true; }
    Bool                  getTracePageBreaks () const
                              { return fTracePageBreaks; }
    // bar lines
    void                  setTraceBarLines ()
                              { fTraceBarLines = true; }
    Bool                  getTraceBarLines () const
                              { return fTraceBarLines; }
    // bar lines details
    Bool                  getTraceBarLinesDetails () const
                              { return fTraceBarLinesDetails; }


    // bar checks
    // --------------------------------------

    void                  setTraceBarChecks ()
                              { fTraceBarChecks = true; }
    Bool                  getTraceBarChecks () const
                              { return fTraceBarChecks; }


    // bar number checks
    // --------------------------------------

    void                  setTraceBarNumberChecks ()
                              { fTraceBarNumberChecks = true; }
    Bool                  getTraceBarNumberChecks () const
                              { return fTraceBarNumberChecks; }


    // transpositions
    // --------------------------------------

    // transpositions
    void                  setTraceTraceTranspositions ()
                              { fTraceTranspositions = true; }
    Bool                  getTraceTranspositions () const
                              { return fTraceTranspositions; }
    // octave shifts
    void                  setTraceOctaveShifts ()
                              { fTraceOctaveShifts = true; }
    Bool                  getTraceOctaveShifts () const
                              { return fTraceOctaveShifts; }


    // repeats to slashes
    // --------------------------------------

    // repeats
    void                  setTraceRepeats ()
                              { fTraceRepeats = true; }
    Bool                  getTraceRepeats () const
                              { return fTraceRepeats; }

    Bool                  getTraceRepeatsDetails () const
                              { return fTraceRepeatsDetails; }

    // measure repeats
    void                  setTraceMeasureRepeats ()
                              { fTraceMeasureRepeats = true; }
    Bool                  getTraceMeasureRepeats () const
                              { return fTraceMeasureRepeats; }

    void                  setTraceMeasureRepeatsDetails ()
                              { fTraceMeasureRepeatsDetails = true; }
    Bool                  getTraceMeasureRepeatsDetails () const
                              { return fTraceMeasureRepeatsDetails; }

    // multiple full-bar rests
    void                  setTraceMultipleFullBarRests ()
                              { fTraceMultipleFullBarRests = true; }
    Bool                  getTraceMultipleFullBarRests () const
                              { return fTraceMultipleFullBarRests; }

    void                  setTraceMultipleFullBarRestsDetails ()
                              { fTraceMultipleFullBarRestsDetails = true; }
    Bool                  getTraceMultipleFullBarRestsDetails () const
                              { return fTraceMultipleFullBarRestsDetails; }
    // beats repeats
    void                  setTraceBeatRepeats ()
                              { fTraceBeatRepeats = true; }
    Bool                  getTraceBeatRepeats () const
                              { return fTraceBeatRepeats; }
    // slashes
    void                  setTraceSlashes ()
                              { fTraceSlashes = true; }
    Bool                  getTraceSlashes () const
                              { return fTraceSlashes; }


    // notes
    // --------------------------------------

    // notes
    void                  setTraceNotes ()
                              { fTraceNotes = true; }
    Bool                  getTraceNotes () const
                              { return fTraceNotes; }

    // notes details
    Bool                  getTraceNotesDetails () const
                              { return fTraceNotesDetails; }
    // whole notes
    Bool                  getTraceWholeNotes () const
                              { return fTraceWholeNotes; }
    Bool                  getTraceWholeNotesDetails () const
                              { return fTraceWholeNotesDetails; }

    // skip notes
    void                  setTraceSkipNotes ()
                              { fTraceSkipNotes = true; }
    Bool                  getTraceSkipNotes () const
                              { return fTraceSkipNotes; }

    // rest notes
    void                  setTraceRestNotes ()
                              { fTraceRestNotes = true; }
    Bool                  getTraceRestNotes () const
                              { return fTraceRestNotes; }

    // notes octave entry
    void                  setTraceNotesOctaveEntry ()
                              { fTraceNotesOctaveEntry = true; }
    Bool                  getTraceNotesOctaveEntry () const
                              { return fTraceNotesOctaveEntry; }

    // grace notes
    void                  setTraceGraceNotes ()
                              { fTraceGraceNotes = true; }
    Bool                  getTraceGraceNotes () const
                              { return fTraceGraceNotes; }

    // tremolos
    void                  setTraceTremolos ()
                              { fTraceTremolos = true; }
    Bool                  getTraceTremolos () const
                              { return fTraceTremolos; }


    // note attachments
    // --------------------------------------

    // stems
    void                  setTraceStems ()
                              { fTraceStems = true; }
    Bool                  getTraceStems () const
                              { return fTraceStems; }

    // beams
    void                  setTraceBeams ()
                              { fTraceBeams = true; }
    Bool                  getTraceBeams () const
                              { return fTraceBeams; }

    // articulations
    void                  setTraceArticulations ()
                              { fTraceArticulations = true; }
    Bool                  getTraceArticulations () const
                              { return fTraceArticulations; }

    // technicals
    void                  setTraceTechnicals ()
                              { fTraceTechnicals = true; }
    Bool                  getTraceTechnicals () const
                              { return fTraceTechnicals; }

    // ornaments
    void                  setTraceOrnaments ()
                              { fTraceOrnaments = true; }
    Bool                  getTraceOrnaments () const
                              { return fTraceOrnaments; }

    void                  setTraceOrnamentsDetails ()
                              { fTraceOrnamentsDetails = true; }
    Bool                  getTraceOrnamentsDetails () const
                              { return fTraceOrnamentsDetails; }

    // dynamics
    void                  setTraceDynamics ()
                              { fTraceDynamics = true; }
    Bool                  getTraceDynamics () const
                              { return fTraceDynamics; }


    // spanners
    // --------------------------------------

    // spanners
    void                  setTraceSpanners ()
                              { fTraceSpanners = true; }
    Bool                  getTraceSpanners () const
                              { return fTraceSpanners; }
    // cresc/decresc
    void                  setTraceCrescDecrescs ()
                              { fTraceCrescDecrescs = true; }
    Bool                  getTraceCrescDecrescs () const
                              { return fTraceCrescDecrescs; }
    // wedges
    void                  setTraceWedges ()
                              { fTraceWedges = true; }
    Bool                  getTraceWedges () const
                              { return fTraceWedges; }
    // slurs
    void                  setTraceSlurs ()
                              { fTraceSlurs = true; }
    Bool                  getTraceSlurs () const
                              { return fTraceSlurs; }

    Bool                  getTraceSlursDetails () const
                              { return fTraceSlursDetails; }
    // ligatures
    void                  setTraceLigatures ()
                              { fTraceLigatures = true; }
    Bool                  getTraceLigatures () const
                              { return fTraceLigatures; }


    // credits to words
    // --------------------------------------

    // credits
    void                  setTraceCredits ()
                              { fTraceCredits = true; }
    Bool                  getTraceCredits () const
                              { return fTraceCredits; }
    // lyrics
    void                  setTraceLyrics ()
                              { fTraceLyrics = true; }
    Bool                  getTraceLyrics () const
                              { return fTraceLyrics; }

    Bool                  getTraceLyricsDetails () const
                              { return fTraceLyricsDetails; }
    // words
    void                  setTraceWords ()
                              { fTraceWords = true; }
    Bool                  getTraceWords () const
                              { return fTraceWords; }


    // chords and tuplets
    // --------------------------------------

    // chords
    void                  setTraceChords ()
                              { fTraceChords = true; }
    Bool                  getTraceChords () const
                              { return fTraceChords; }

    Bool                  getTraceChordsDetails () const
                              { return fTraceChordsDetails; }
    // tuplets
    void                  setTraceTuplets ()
                              { fTraceTuplets = true; }
    Bool                  getTraceTuplets () const
                              { return fTraceTuplets; }

    Bool                  getTraceTupletsDetails () const
                              { return fTraceTupletsDetails; }

    // inter-notes
    // --------------------------------------

    // ties
    void                  setTraceTies ()
                              { fTraceTies = true; }
    Bool                  getTraceTies () const
                              { return fTraceTies; }

    // glissandos
    // --------------------------------------

    void                  setTraceGlissandos ()
                              { fTraceGlissandos = true; }
    Bool                  getTraceGlissandos () const
                              { return fTraceGlissandos; }

    // above staff
    // --------------------------------------

    // rehearsal marks
    void                  setTraceRehearsalMarks ()
                              { fTraceRehearsalMarks = true; }
    Bool                  getTraceRehearsalMarks () const
                              { return fTraceRehearsalMarks; }

    // segnos
    void                  setTraceSegnos ()
                              { fTraceSegnos = true; }
    Bool                  getTraceSegnos () const
                              { return fTraceSegnos; }

    // dal segnos
    void                  setTraceDalSegnos ()
                              { fTraceDalSegnos = true; }
    Bool                  getTraceDalSegnos () const
                              { return fTraceDalSegnos; }

    // codas
    void                  setTraceCodas ()
                              { fTraceCodas = true; }
    Bool                  getTraceCodas () const
                              { return fTraceCodas; }

    // eyeglases
    void                  setTraceEyeGlasses ()
                              { fTraceEyeGlasses = true; }
    Bool                  getTraceEyeGlasses () const
                              { return fTraceEyeGlasses; }

    // harmonies
    // --------------------------------------

    void                  setTraceHarmonies ()
                              { fTraceHarmonies = true; }

    Bool                  getTraceHarmonies () const
                              { return fTraceHarmonies; }
    S_oahBooleanAtom      getTraceHarmoniesBooleanAtom () const
                              { return fTraceHarmoniesBooleanAtom; }

    Bool                  getTraceHarmoniesDetails () const
                              { return fTraceHarmoniesDetails; }

    void                  setTraceExtraHarmonies ()
                              { fTraceExtraHarmonies = true; }
    Bool                  getTraceExtraHarmonies () const
                              { return fTraceExtraHarmonies; }

    // figured bass elements
    // --------------------------------------

    void                  setTraceFiguredBass ()
                              { fTraceFiguredBass = true; }

    Bool                  getTraceFiguredBass () const
                              { return fTraceFiguredBass; }
    S_oahBooleanAtom      getTraceFiguredBassBooleanAtom () const
                              { return fTraceFiguredBassBooleanAtom; }

    void                  setTraceFiguredBassDetails ()
                              { fTraceFiguredBassDetails = true; }
    Bool                  getTraceFiguredBassDetails () const
                              { return fTraceFiguredBassDetails; }

    // instruments
    // --------------------------------------

    // frames
    void                  setTraceFrames ()
                              { fTraceFrames = true; }
    Bool                  getTraceFrames () const
                              { return fTraceFrames; }

    // scordaturas
    void                  setTraceScordaturas ()
                              { fTraceScordaturas = true; }
    Bool                  getTraceScordaturas () const
                              { return fTraceScordaturas; }

    // slides
    void                  setTraceSlides ()
                              { fTraceSlides = true; }
    Bool                  getTraceSlides () const
                              { return fTraceSlides; }
    // pedals
    void                  setTracePedals ()
                              { fTracePedals = true; }
    Bool                  getTracePedals () const
                              { return fTracePedals; }

    // accordion registrations
    void                  setTraceAccordionRegistrations ()
                              { fTraceAccordionRegistrations = true; }
    Bool                  getTraceAccordionRegistrations () const
                              { return fTraceAccordionRegistrations; }

    // harp pedals
    void                  setTraceHarpPedals ()
                              { fTraceHarpPedals = true; }
    Bool                  getTraceHarpPedals () const
                              { return fTraceHarpPedals; }

    // harp pedals tuning
    void                  setTraceHarpPedalsTunings ()
                              { fTraceHarpPedalsTunings = true; }
    Bool                  getTraceHarpPedalsTunings () const
                              { return fTraceHarpPedalsTunings; }

    // damps
    void                  setTraceDamps ()
                              { fTraceDamps = true; }
    Bool                  getTraceDamps () const
                              { return fTraceDamps; }

    // dampalls
    void                  setTraceDampAlls ()
                              { fTraceDampAlls = true; }
    Bool                  getTraceDampAlls () const
                              { return fTraceDampAlls; }

    // midi
    void                  setTraceMidi ()
                              { fTraceMidi = true; }
    Bool                  getTraceMidi () const
                              { return fTraceMidi; }

/* JMI
    // msrStreams
    // --------------------------------------
    Bool                  getTraceMsrStreams () const
                              { return fTraceMsrStreams; }

    // layout
*/

  public:

    // public services
    // ------------------------------------------------------

    // quiet mode
    void                  enforceGroupQuietness () override;

    // consistency check
    void                  checkGroupOptionsConsistency () override;

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    void                  printtracingOahHelp ();

    void                  printtracingOahValues (int fieldWidth);

    virtual void          printAtomWithVariableOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const;

  private:

    // private services
    // ------------------------------------------------------

  private:

    // options and help trace
    // --------------------------------------

    S_oahPrefix           fShortTracePrefix;
    S_oahPrefix           fLongTracePrefix;

    Bool                  fTracingOah;
                            // unused, only for the help
                            // since this option is detected early
    S_oahBooleanAtom      fTracingOahBooleanAtom;

    Bool                  fTracingOahDetails;

    // other
    // --------------------------------------

    // history
    Bool                  fTraceComponents;

    // passes
    Bool                  fTracePasses;
    S_oahBooleanAtom      fTracePassesBooleanAtom;

  public:

    // scaling
    Bool                  fTraceGeometry;
    // identification
    Bool                  fTraceIdentification; // specialize ??? JMI
    // for tests, hidden
    Bool                  fTraceForTests;


    // scores to voices
    // --------------------------------------

    // books
    Bool                  fTraceBooks;
    // scores
    Bool                  fTraceScores;
    // part groups
    Bool                  fTracePartGroups;
    Bool                  fTracePartGroupsDetails;
    // parts
    Bool                  fTraceParts;
    // staves
    Bool                  fTraceStaves;
    // staff details
    Bool                  fTraceStaffDetails;
    // staff changes
    Bool                  fTraceStaffChanges;
    // voices
    Bool                  fTraceVoices;
    Bool                  fTraceVoicesDetails;


    // print layouts
    // --------------------------------------

    Bool                  fTracePrintLayouts;

    // segments and measures
    // --------------------------------------

    // segments
    Bool                  fTraceSegments;
    Bool                  fTraceSegmentsDetails;
    // measures
    Bool                  fTraceMeasuresNumbers;

    Bool                  fTraceMeasures;
    Bool                  fTraceMeasuresDetails;

    Bool                  fTraceMeasurePositions;
    Bool                  fTraceMomentsInMeasures;

    set<int>              fTraceDetailedMeasureNumbersSet;


    // measures slices
    // --------------------------------------

    Bool                  fTraceMeasuresSlices;
    Bool                  fTraceMeasuresSlicesDetails;


    // clefs to tempos
    // --------------------------------------

    // clefs
    Bool                  fTraceClefs;
    // keys
    Bool                  fTraceKeys;
    Bool                  fTraceKeysDetails;
    // time signatures
    Bool                  fTraceTimeSignatures;
    // tempos
    Bool                  fTraceTempos;


    // breaks and barLines
    // --------------------------------------

    // line breaks
    Bool                  fTraceLineBreaks;
    // page and line breaks
    Bool                  fTracePageBreaks;
    // bar lines
    Bool                  fTraceBarLines;
    // bar lines details
    Bool                  fTraceBarLinesDetails;


    // bar checks
    // --------------------------------------

    Bool                  fTraceBarChecks;


    // bar number checks
    // --------------------------------------

    Bool                  fTraceBarNumberChecks;


    // transpositions
    // --------------------------------------

    // transpositions
    Bool                  fTraceTranspositions;
    // octave shifts
    Bool                  fTraceOctaveShifts;


    // repeats to slashes
    // --------------------------------------

    // repeats
    Bool                  fTraceRepeats;
    Bool                  fTraceRepeatsDetails;
    // measure repeats
    Bool                  fTraceMeasureRepeats;
    Bool                  fTraceMeasureRepeatsDetails;
    // multiple full-bar rests
    Bool                  fTraceMultipleFullBarRests;
    Bool                  fTraceMultipleFullBarRestsDetails;
    // beats repeats
    Bool                  fTraceBeatRepeats;
    // slashes
    Bool                  fTraceSlashes;


    // notes
    // --------------------------------------

    // notes
    Bool                  fTraceNotes;
    // notes details
    Bool                  fTraceNotesDetails;
    // whole notes
    Bool                  fTraceWholeNotes;
    Bool                  fTraceWholeNotesDetails;
    // skip notes
    Bool                  fTraceSkipNotes;
    // rest notes
    Bool                  fTraceRestNotes;
    // notes octave entry
    Bool                  fTraceNotesOctaveEntry;
    // grace notes
    Bool                  fTraceGraceNotes;
    // tremolos
    Bool                  fTraceTremolos;


    // note attachments
    // --------------------------------------

    // stems
    Bool                  fTraceStems;
    // beams
    Bool                  fTraceBeams;
    // articulations
    Bool                  fTraceArticulations;
    // technicals
    Bool                  fTraceTechnicals;
    // ornaments
    Bool                  fTraceOrnaments;

    Bool                  fTraceOrnamentsDetails;
    // dynamics
    Bool                  fTraceDynamics;


    // spanners
    // --------------------------------------

    // spanners
    Bool                  fTraceSpanners; // ???
    // cresc/decresc
    Bool                  fTraceCrescDecrescs;
    // wedges
    Bool                  fTraceWedges;
    // spanners
    // slurs
    Bool                  fTraceSlurs;
    Bool                  fTraceSlursDetails;
    // ligatures
    Bool                  fTraceLigatures;


    // credits to words
    // --------------------------------------

    // credits
    Bool                  fTraceCredits;
    // lyrics
    Bool                  fTraceLyrics;
    Bool                  fTraceLyricsDetails;
    // words
    Bool                  fTraceWords;


    // chords and tuplets
    // --------------------------------------

    // chords
    Bool                  fTraceChords;
    Bool                  fTraceChordsDetails;
    // tuplets
    Bool                  fTraceTuplets;
    Bool                  fTraceTupletsDetails;

    // inter-notes
    // --------------------------------------

    // ties
    Bool                  fTraceTies;
    // glissandos
    Bool                  fTraceGlissandos;

    // above staff
    // --------------------------------------

    // rehearsal marks
    Bool                  fTraceRehearsalMarks;
    // segnos
    Bool                  fTraceSegnos;
    // dal segnos
    Bool                  fTraceDalSegnos;
    // codas
    Bool                  fTraceCodas;
    // eyeglases
    Bool                  fTraceEyeGlasses;

    // harmonies
    // --------------------------------------

    Bool                  fTraceHarmonies;
    S_oahBooleanAtom      fTraceHarmoniesBooleanAtom;

    Bool                  fTraceHarmoniesDetails;

    Bool                  fTraceExtraHarmonies;

    // figured bass elements
    // --------------------------------------

    Bool                  fTraceFiguredBass;
    S_oahBooleanAtom      fTraceFiguredBassBooleanAtom;

    Bool                  fTraceFiguredBassDetails;

    // instruments
    // --------------------------------------

    // frames
    Bool                  fTraceFrames;
    // scordaturas
    Bool                  fTraceScordaturas;
    // slides
    Bool                  fTraceSlides;
    // pedals
    Bool                  fTracePedals;
    // accordion registrations
    Bool                  fTraceAccordionRegistrations;
    // harp pedals
    Bool                  fTraceHarpPedals;
    // harp pedals tuning
    Bool                  fTraceHarpPedalsTunings;
    // damps
    Bool                  fTraceDamps;
    // dampalls
    Bool                  fTraceDampAlls;
    // midi
    Bool                  fTraceMidi;

/* JMI
    // msrStreams
    // --------------------------------------
    Bool                  fTraceMsrStreams;

    // layout
*/
};
typedef SMARTP<tracingOahGroup> S_tracingOahGroup;
EXP ostream& operator << (ostream& os, const S_tracingOahGroup& elt);

EXP extern S_tracingOahGroup gGlobalTracingOahGroup;

//______________________________________________________________________________
EXP S_tracingOahGroup createGlobalTracingOahGroup (
  S_oahHandler handler);


}


#endif


#endif
