/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2024

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___mfTraceOah___
#define ___mfTraceOah___

#ifdef MF_TRACE_IS_ENABLED // encompasses this whole file


#include <set>

#include "exports.h"

#include "oahAtomsCollection.h"
#include "oahOah.h"


namespace MusicFormats
{

//______________________________________________________________________________
class EXP traceOahGroup : public oahGroup
{
  public:

    static SMARTP<traceOahGroup> create (
                            const S_oahHandler& handler);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          traceOahGroup (
                            const S_oahHandler& handler);

    virtual               ~traceOahGroup ();

  public:

    // initialisation
    // ------------------------------------------------------

    void                  createTheTracePrefixes (
                            const S_oahHandler& handler);

    void                  initializeTraceOahGroup ();

    void                  initializeOptionsTraceAndDisplayOptions ();

    void                  initializeMusicXMLPrintLayoutsTraceOah ();

    void                  initializeRepeatsToSlashesTraceOah ();

    void                  initializeNotesDurationsTraceOah ();

    void                  initializeNotesTraceOah ();

    void                  initializeNotesAttachmentsTraceOah ();

    void                  initializeBooksToVoicesTraceOah ();

    void                  initializeSegmentsTraceOah ();

    void                  initializeMeasuresTraceOah ();

    void                  initializeVoicesFlatViewTraceOah ();

    void                  initializeMeasuresSlicesTraceOah ();

    void                  initializeInstrumentsTraceOah ();

    void                  initializeMxsrEventsTraceOah ();

    void                  initializeChordsTraceOah ();

    void                  initializeTupletsTraceOah ();

    void                  initializeCreditsToWordsTraceOah ();

    void                  initializeHarmoniesTraceOah ();

    void                  initializeFiguredBassesTraceOah ();

    void                  initializeSpannersTraceOah ();

    void                  initializeInterNotesTraceOah ();

    void                  initializeClefsToTemposTraceOah ();

    void                  initializeBreaksAndBarLinesTraceOah ();

    void                  initializeAboveStaffTraceOah ();

    void                  initializeTranspositionsTraceOah ();

    void                  initializeOtherTraceOah ();

  public:

    // set and get
    // ------------------------------------------------------

    // options and help trace
    // --------------------------------------

    S_oahPrefix           getShortTracePrefix () const
                              { return fShortTracePrefix; }
    S_oahPrefix           getLongTracePrefix () const
                              { return fLongTracePrefix; }

//     S_oahPrefix           getShortDisplayPrefix () const
//                               { return fShortDisplayPrefix; }
//     S_oahPrefix           getLongDisplayPrefix () const
//                               { return fLongDisplayPrefix; }

    void                  setTraceOah ()
                              { fTraceOah = true; }
    Bool                  getTraceOah () const
                              { return fTraceOah; }

    const S_oahBooleanAtom&
                          getTraceOahBooleanAtom () const
                              { return fTraceOahBooleanAtom; }

// replaced by getTraceEarlyOptionsDetails()
//     Bool                  getTraceOahDetails () const
//                               { return fTraceOahDetails; }

    // other
    // --------------------------------------

    // passes
    const S_oahBooleanAtom&
                          getTracePassesBooleanAtom () const
                              { return fTracePassesBooleanAtom; }

    const S_passIDOahAtom&
                          getTraceOnlyPassBooleanAtom () const
                              { return fTraceOnlyPassIDOahAtom; }

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
    Bool                  getTraceStavesDetails () const
                              { return fTraceStavesDetails; }

    // MXSR events
    void                  setTraceMxsrEvents ()
                              { fTraceMxsrEvents = true; }
    Bool                  getTraceMxsrEvents () const
                              { return fTraceMxsrEvents; }

    S_oahBooleanAtom      getTraceMxsrEventsAtom () const
                              { return fTraceMxsrEventsAtom; }

    // staff changes
    void                  setTraceTraceStaffChangesBasics ()
                              { fTraceStaffChangesBasics = true; }
    void                  setTraceStaffChanges ()
                              { fTraceStaffChanges = true; }

    Bool                  getTraceStaffChangesBasics () const
                              { return fTraceStaffChangesBasics; }
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

    Bool                  getTraceMusicXMLPrintLayouts () const
                              { return fTraceMusicXMLPrintLayouts; }

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

    Bool                  getTraceMeasuresBasics () const
                              { return fTraceMeasuresBasics; }

    void                  setTraceMeasures ()
                              { fTraceMeasures = true; }
    Bool                  getTraceMeasures () const
                              { return fTraceMeasures; }

    Bool                  getTraceMeasuresDetails () const
                              { return fTraceMeasuresDetails; }

    S_oahStringAtom       getTraceOnlyMeasurerNumbeOahAtom () const
                              { return fTraceOnlyMeasurerNumbeOahAtom; }

    std::set<int>         getTraceDetailedMeasureNumbersSet () const
                              { return fTraceDetailedMeasureNumbersSet; }

    // positions
    Bool                  getTraceMeasurePositions () const
                              { return fTraceMeasurePositions; }
    void                  setTraceMeasurePositions ()
                              { fTraceMeasurePositions = true; }

    Bool                  getTraceMeasurePositionsDetails () const
                              { return fTraceMeasurePositionsDetails; }
    void                  setTraceMeasurePositionsDetails ()
                              { fTraceMeasurePositionsDetails = true; }

    Bool                  getTraceVoicePositions () const
                              { return fTraceVoicePositions; }
    void                  setTraceVoicePositions ()
                              { fTraceVoicePositions = true; }

    // moments
    Bool                  getTraceMeasureMoments () const
                              { return fTraceMeasureMoments; }
    void                  setTraceMeasureMoments ()
                              { fTraceMeasureMoments = true; }

    Bool                  getTraceVoiceMoments () const
                              { return fTraceVoiceMoments; }
    void                  setTraceVoiceMoments ()
                              { fTraceVoiceMoments = true; }

    // voices flat view
    Bool                  getTraceVoicesFlatView () const
                              { return fTraceVoicesFlatView; }

    Bool                  getTraceVoicesFlatViewDetails () const
                              { return fTraceVoicesFlatViewDetails; }

    // measures slices
    Bool                  getTraceMeasuresSlices () const
                              { return fTraceMeasuresSlices; }

    Bool                  getTraceMeasuresSlicesDetails () const
                              { return fTraceMeasuresSlicesDetails; }


    // clef, key, time signature
    // --------------------------------------

    Bool                  getTraceClefKeyTimeSignatureGroups () const
                              {
                                return
                                  getTraceClefs ()
                                    ||
                                  getTraceKeys ()
                                    ||
                                  getTraceTimeSignatures ();
                              }

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
    // --------------------------------------

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
    void                  setTraceTranspositions ()
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
    Bool                  getTraceRepeatsBasics () const
                              { return fTraceRepeatsBasics; }

    void                  setTraceRepeats ()
                              { fTraceRepeats = true; }
    Bool                  getTraceRepeats () const
                              { return fTraceRepeats; }

    Bool                  getTraceRepeatsDetails () const
                              { return fTraceRepeatsDetails; }

    // measure repeats
    Bool                  getTraceMeasureRepeatsBasics () const
                              { return fTraceMeasureRepeatsBasics; }

    void                  setTraceMeasureRepeats ()
                              { fTraceMeasureRepeats = true; }
    Bool                  getTraceMeasureRepeats () const
                              { return fTraceMeasureRepeats; }

    void                  setTraceMeasureRepeatsDetails ()
                              { fTraceMeasureRepeatsDetails = true; }
    Bool                  getTraceMeasureRepeatsDetails () const
                              { return fTraceMeasureRepeatsDetails; }

    // multi-measure rests
    void                  setTraceMultiMeasureRests ()
                              { fTraceMultiMeasureRests = true; }
    Bool                  getTraceMultiMeasureRests () const
                              { return fTraceMultiMeasureRests; }

    void                  setTraceMultiMeasureRestsDetails ()
                              { fTraceMultiMeasureRestsDetails = true; }
    Bool                  getTraceMultiMeasureRestsDetails () const
                              { return fTraceMultiMeasureRestsDetails; }
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


    // durations
    // --------------------------------------

    void                  setTraceNotesDurations ()
                              { fTraceNotesDurations = true; }
    Bool                  getTraceNotesDurations () const
                              { return fTraceNotesDurations; }

    // notes
    // --------------------------------------

    // notes
    Bool                  getTraceNotesBasics () const
                              { return fTraceNotesBasics; }

    void                  setTraceNotes ()
                              { fTraceNotes = true; }
    Bool                  getTraceNotes () const
                              { return fTraceNotes; }

    Bool                  getTraceNotesDetails () const
                              { return fTraceNotesDetails; }

    // whole notes
    Bool                  getTraceWholeNoteDurations () const
                              { return fTraceWholeNoteDurations; }
    Bool                  getTraceWholeNoteDurationsDetails () const
                              { return fTraceWholeNoteDurationsDetails; }

    // measures whole notes vectors
    Bool                  getTraceMeasuresWholeNotesVectors () const
                              { return fTraceMeasuresWholeNotesVectors; }

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
    Bool                  getTraceChordsBasics () const
                              { return fTraceChordsBasics; }

    void                  setTraceChords ()
                              { fTraceChords = true; }
    Bool                  getTraceChords () const
                              { return fTraceChords; }

    Bool                  getTraceChordsDetails () const
                              { return fTraceChordsDetails; }

    // tuplets
    Bool                  getTraceTupletsBasics () const
                              { return fTraceTupletsBasics; }

    void                  setTraceTuplets ()
                              { fTraceTuplets = true; }
    Bool                  getTraceTuplets () const
                              { return fTraceTuplets; }

    Bool                  getTraceTupletsDetails () const
                              { return fTraceTupletsDetails; }

//     void                  setDisplayTuplets ()
//                               { fDisplayTuplets = true; }
//     Bool                  getDisplayTuplets () const
//                               { return fDisplayTuplets; }
//

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
    const S_oahBooleanAtom&
                          getTraceHarmoniesBooleanAtom () const
                              { return fTraceHarmoniesBooleanAtom; }

    Bool                  getTraceHarmoniesDetails () const
                              { return fTraceHarmoniesDetails; }

    void                  setTraceExtraHarmonies ()
                              { fTraceExtraHarmonies = true; }
    Bool                  getTraceExtraHarmonies () const
                              { return fTraceExtraHarmonies; }

    // figured bass elements
    // --------------------------------------

    void                  setTraceFiguredBasses ()
                              { fTraceFiguredBasses = true; }

    Bool                  getTraceFiguredBasses () const
                              { return fTraceFiguredBasses; }
    const S_oahBooleanAtom&
                          getTraceFiguredBassesBooleanAtom () const
                              { return fTraceFiguredBassesBooleanAtom; }

    void                  setTraceFiguredBassesDetails ()
                              { fTraceFiguredBassesDetails = true; }
    Bool                  getTraceFiguredBassesDetails () const
                              { return fTraceFiguredBassesDetails; }

    // instruments
    // --------------------------------------

    // instruments
    void                  setTraceInstruments ()
                              { fTraceInstruments = true; }
    Bool                  getTraceInstruments () const
                              { return fTraceInstruments; }

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

    void                  printTraceOahHelp ();

    void                  displayTraceOahValues (int fieldWidth);

    virtual void          displayAtomWithVariableOptionsValues (
                            std::ostream& os,
                            int           valueFieldWidth) const;

  private:

    // private services
    // ------------------------------------------------------

  private:

    // options and help trace
    // --------------------------------------

    S_oahPrefix           fShortTracePrefix;
    S_oahPrefix           fLongTracePrefix;

//     S_oahPrefix           fShortDisplayPrefix;
//     S_oahPrefix           fLongDisplayPrefix;

    Bool                  fTraceOah;
                            // unused, only for the help
                            // since this option is detected early
    S_oahBooleanAtom      fTraceOahBooleanAtom;

    Bool                  fTraceOahDetails;

    // other
    // --------------------------------------

    // passes
//     Bool                  fTracePasses;
    S_oahBooleanAtom      fTracePassesBooleanAtom;

//     mfPassIDKind          fTraceOnlyPassIDKind;
    S_passIDOahAtom       fTraceOnlyPassIDOahAtom;

  public:

    // scaling
    Bool                  fTraceGeometry;
    // identification
    Bool                  fTraceIdentification; // specialize ??? JMI v0.9.66
    // for tests, hidden
    Bool                  fTraceForTests;


    // credits
    // --------------------------------------

    Bool                  fTraceCredits;


    // scores to voices
    // --------------------------------------

    // books
    // --------------------------------------

    Bool                  fTraceBooks;


    // scores
    // --------------------------------------

    Bool                  fTraceScores;


    // part groups
    // --------------------------------------

    Bool                  fTracePartGroups;
    Bool                  fTracePartGroupsDetails;


    // parts
    // --------------------------------------

    Bool                  fTraceParts;


    // staves
    // --------------------------------------

    Bool                  fTraceStaves;
    Bool                  fTraceStavesDetails;

    // MXSR events
    // --------------------------------------

    Bool                  fTraceMxsrEvents;
    S_oahBooleanAtom      fTraceMxsrEventsAtom;

    // staff changes
    // --------------------------------------

    Bool                  fTraceStaffChangesBasics;
    Bool                  fTraceStaffChanges;


    // voices
    // --------------------------------------

    Bool                  fTraceVoiceBasics;
    Bool                  fTraceVoices;
    Bool                  fTraceVoicesDetails;


    // MusicXML print layouts
    // --------------------------------------

    Bool                  fTraceMusicXMLPrintLayouts;

    // segments
    // --------------------------------------

    Bool                  fTraceSegments;
    Bool                  fTraceSegmentsDetails;

    // measures
    // --------------------------------------

    Bool                  fTraceMeasuresNumbers;

    Bool                  fTraceMeasuresBasics;
    Bool                  fTraceMeasures;
    Bool                  fTraceMeasuresDetails;

    S_oahStringAtom       fTraceOnlyMeasurerNumbeOahAtom;
    std::string           fTraceOnlyMeasure;

    std::set<int>         fTraceDetailedMeasureNumbersSet; // JMI v0.9.67


    // positions
    Bool                  fTraceMeasurePositions;
    Bool                  fTraceMeasurePositionsDetails;

    Bool                  fTraceVoicePositions;

    // moments
    Bool                  fTraceMeasureMoments;
    Bool                  fTraceVoiceMoments;

    // voices flat view
    // --------------------------------------

    Bool                  fTraceVoicesFlatView;
    Bool                  fTraceVoicesFlatViewDetails;

    // measures slices
    // --------------------------------------

    Bool                  fTraceMeasuresSlices;
    Bool                  fTraceMeasuresSlicesDetails;


    // clefs
    // --------------------------------------

    Bool                  fTraceClefs;


    // keys
    // --------------------------------------

    Bool                  fTraceKeys;
    Bool                  fTraceKeysDetails;


    // time signatures
    // --------------------------------------

    Bool                  fTraceTimeSignatures;


    // tempos
    // --------------------------------------

    Bool                  fTraceTempos;


    // breaks
    // --------------------------------------

    Bool                  fTraceLineBreaks;
    Bool                  fTracePageBreaks;


    // bar lines
    // --------------------------------------

    Bool                  fTraceBarLines;
    Bool                  fTraceBarLinesDetails;


    // bar checks
    // --------------------------------------

    Bool                  fTraceBarChecks;


    // bar number checks
    // --------------------------------------

    Bool                  fTraceBarNumberChecks;


    // transpositions
    // --------------------------------------

    Bool                  fTraceTranspositions;

    // octave shifts
    // --------------------------------------

    Bool                  fTraceOctaveShifts;


    // repeats
    // --------------------------------------

    Bool                  fTraceRepeatsBasics;
    Bool                  fTraceRepeats;
    Bool                  fTraceRepeatsDetails;


    // measure repeats
    // --------------------------------------

    Bool                  fTraceMeasureRepeatsBasics;
    Bool                  fTraceMeasureRepeats;
    Bool                  fTraceMeasureRepeatsDetails;


    // multi-measure rests
    // --------------------------------------

    Bool                  fTraceMultiMeasureRests;
    Bool                  fTraceMultiMeasureRestsDetails;


    // beats repeats
    // --------------------------------------

    Bool                  fTraceBeatRepeats;


    // slashes
    Bool                  fTraceSlashes;


    // notes durations
    // --------------------------------------

    Bool                  fTraceNotesDurations;


    // notes
    // --------------------------------------

    Bool                  fTraceNotesBasics;
    Bool                  fTraceNotes;
    Bool                  fTraceNotesDetails;


    // whole notes durations
    // --------------------------------------

    Bool                  fTraceWholeNoteDurations;
    Bool                  fTraceWholeNoteDurationsDetails;


    // measures whole notes vectors
    // --------------------------------------

    Bool                  fTraceMeasuresWholeNotesVectors;


    // rest and skip notes
    // --------------------------------------

    Bool                  fTraceRestNotes;
    Bool                  fTraceSkipNotes;


    // notes octave entry
    // --------------------------------------
    Bool                  fTraceNotesOctaveEntry;


    // grace notes
    // --------------------------------------

    Bool                  fTraceGraceNotes;


    // tremolos
    // --------------------------------------

    Bool                  fTraceTremolos;


    // stems
    // --------------------------------------
    Bool                  fTraceStems;


    // beams
    // --------------------------------------
    Bool                  fTraceBeams;


    // articulations
    // --------------------------------------

    Bool                  fTraceArticulations;


    // technicals
    // --------------------------------------

    Bool                  fTraceTechnicals;


    // ornaments
    // --------------------------------------

    Bool                  fTraceOrnaments;
    Bool                  fTraceOrnamentsDetails;


    // dynamics
    // --------------------------------------

    Bool                  fTraceDynamics;


    // spanners
    // --------------------------------------

    Bool                  fTraceSpanners;


    // cresc/decresc
    // --------------------------------------

    Bool                  fTraceCrescDecrescs;


    // wedges
    // --------------------------------------

    Bool                  fTraceWedges;


    // slurs
    // --------------------------------------

    Bool                  fTraceSlurs;
    Bool                  fTraceSlursDetails;


    // ligatures
    // --------------------------------------

    Bool                  fTraceLigatures;


    // lyrics
    // --------------------------------------

    Bool                  fTraceLyrics;
    Bool                  fTraceLyricsDetails;


    // words
    // --------------------------------------

    Bool                  fTraceWords;


    // chords
    // --------------------------------------

    Bool                  fTraceChordsBasics;
    Bool                  fTraceChords;
    Bool                  fTraceChordsDetails;


    // tuplets
    // --------------------------------------

    Bool                  fTraceTupletsBasics;
    Bool                  fTraceTuplets;
    Bool                  fTraceTupletsDetails;


    // ties
    // --------------------------------------

    Bool                  fTraceTies;


    // glissandos
    // --------------------------------------

    Bool                  fTraceGlissandos;


    // rehearsal marks
    // --------------------------------------

    Bool                  fTraceRehearsalMarks;


    // segnos
    // --------------------------------------

    Bool                  fTraceSegnos;


    // dal segnos
    // --------------------------------------

    Bool                  fTraceDalSegnos;


    // codas
    // --------------------------------------

    Bool                  fTraceCodas;


    // eyeglases
    // --------------------------------------

    Bool                  fTraceEyeGlasses;

    // harmonies
    // --------------------------------------

    Bool                  fTraceHarmonies;
    S_oahBooleanAtom      fTraceHarmoniesBooleanAtom;

    Bool                  fTraceHarmoniesDetails;

    Bool                  fTraceExtraHarmonies;

    // figured bass
    // --------------------------------------

    Bool                  fTraceFiguredBasses;
    S_oahBooleanAtom      fTraceFiguredBassesBooleanAtom;

    Bool                  fTraceFiguredBassesDetails;

    // instruments
    // --------------------------------------

    // instruments
    Bool                  fTraceInstruments;


    // frames
    // --------------------------------------

    Bool                  fTraceFrames;


    // scordaturas
    // --------------------------------------

    Bool                  fTraceScordaturas;


    // slides
    // --------------------------------------

    Bool                  fTraceSlides;


    // pedals
    // --------------------------------------

    Bool                  fTracePedals;


    // accordion registrations
    // --------------------------------------

    Bool                  fTraceAccordionRegistrations;


    // harp pedals
    // --------------------------------------

    Bool                  fTraceHarpPedals;
    Bool                  fTraceHarpPedalsTunings;


    // damps
    // --------------------------------------

    Bool                  fTraceDamps;
    Bool                  fTraceDampAlls;


    // midi
    // --------------------------------------

    Bool                  fTraceMidi;


/* JMI
    // msrStreams
    // --------------------------------------
    Bool                  fTraceMsrStreams;

    // layout
*/
};
typedef SMARTP<traceOahGroup> S_traceOahGroup;
EXP std::ostream& operator << (std::ostream& os, const S_traceOahGroup& elt);

//______________________________________________________________________________
// hidden global trace OAH group variable
EXP S_traceOahGroup getGlobalTraceOahGroup ();

// a handy shortcut
#define gTraceOahGroup getGlobalTraceOahGroup ()

//______________________________________________________________________________
EXP S_traceOahGroup createGlobalTraceOahGroup (
  const S_oahHandler& handler);


}


#endif // MF_TRACE_IS_ENABLED


#endif // ___mfTraceOah___
