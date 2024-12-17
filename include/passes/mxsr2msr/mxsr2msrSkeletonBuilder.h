/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2024

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___mxsr2msrSkeletonBuilder___
#define ___mxsr2msrSkeletonBuilder___

#include <list>
#include <map>
#include <vector>

#include "typedefs.h"

#include "mxsr2msrEvents.h"
#include "mxsr2msrPartGroups.h"
#include "mxsr2msrTuplets.h"
#include "mxsr2msrVoices.h"

#include "msrPartGroups.h"


namespace MusicFormats
{

/*
  The part groups are described for the neded of this pass by class mxsrPartGroup,
  which is distinct from msrPartGroup, an instance of which they contain.

  The identifiers for msrPartGroups instances contain 'Msr'
  wherever needed to make things clear
*/

//________________________________________________________________________
class EXP mxsr2msrSkeletonBuilder :

  // score partwise

  public                      visitor<S_score_partwise>,

  // rights
  // ------------------------------------------------------

  public                      visitor<S_work_number>,
  public                      visitor<S_work_title>,
  public                      visitor<S_opus>,
  public                      visitor<S_movement_number>,
  public                      visitor<S_movement_title>,

  // identification
  // ------------------------------------------------------

  public                      visitor<S_identification>,
  public                      visitor<S_creator>,
  public                      visitor<S_rights>,
  public                      visitor<S_source>,
  public                      visitor<S_relation>,

  // encoding
  // ------------------------------------------------------

  public                      visitor<S_encoding>,
  public                      visitor<S_encoding_date>,
  public                      visitor<S_encoder>,
  public                      visitor<S_software>,
  public                      visitor<S_encoding_description>,
  public                      visitor<S_supports>,

  // miscellaneous
  // ------------------------------------------------------

  public                      visitor<S_miscellaneous>,
  public                      visitor<S_miscellaneous_field>,

  // credits
  // ------------------------------------------------------

  public                      visitor<S_credit>,
  public                      visitor<S_credit_type>,
  public                      visitor<S_credit_words>,
  public                      visitor<S_credit_symbol>,
  public                      visitor<S_credit_image>,

  // parts & part groups
  // ------------------------------------------------------

  public                      visitor<S_part_list>,

  public                      visitor<S_display_text>,

  public                      visitor<S_part_group>,

  public                      visitor<S_group_name>,
  public                      visitor<S_group_name_display>,

  public                      visitor<S_accidental_text>,
  public                      visitor<S_group_abbreviation>,
  public                      visitor<S_group_symbol>,
  public                      visitor<S_group_barline>,

  public                      visitor<S_score_part>,

  public                      visitor<S_part_name>,
  public                      visitor<S_part_name_display>,

  public                      visitor<S_part_abbreviation>,
  public                      visitor<S_part_abbreviation_display>,

  public                      visitor<S_instrument_name>,
  public                      visitor<S_instrument_abbreviation>,

  public                      visitor<S_part>,

  // staves
  // ------------------------------------------------------

  public                      visitor<S_staves>,
  public                      visitor<S_staff>,

  // voices
  // ------------------------------------------------------

  public                      visitor<S_voice>,

  // measures
  // ------------------------------------------------------

  public                      visitor<S_measure>,

  // print
  // ------------------------------------------------------

  public                      visitor<S_print>,

  // barlines
  // ------------------------------------------------------

//   public                      visitor<S_barline>,
//   public                      visitor<S_bar_style>,
  public                      visitor<S_repeat>,
//   public                      visitor<S_ending>,

  // notes
  // ------------------------------------------------------

  public                      visitor<S_note>,
  public                      visitor<S_rest>,
  public                      visitor<S_grace>,

  // chords
  // ------------------------------------------------------

  public                      visitor<S_chord>,

  // tuplets
  // ------------------------------------------------------

  public                      visitor<S_time_modification>,
  public                      visitor<S_tuplet>,

  // lyrics
  // ------------------------------------------------------

  public                      visitor<S_lyric>,

  // harmonies
  // ------------------------------------------------------

  public                      visitor<S_harmony>,

  // figured bass
  // ------------------------------------------------------

  public                      visitor<S_figured_bass>

{
  public:

    // constructors/destructor
    // ------------------------------------------------------

                              mxsr2msrSkeletonBuilder (
                                mxsrEventsCollection&
                                  theResultingEventsCollection);

    virtual                   ~mxsr2msrSkeletonBuilder ();

    // set and get
    // ------------------------------------------------------

    S_msrScore                getMsrScore () const
                                  { return fMsrScore; }


  public:

    // public services
    // ------------------------------------------------------

    // visitors
    // ------------------------------------------------------

    void                      browseMxsr (
                                const Sxmlelement& xmlTree);

  protected:

    // score partwise
    // ------------------------------------------------------

    virtual void              visitStart (S_score_partwise& elt);
    virtual void              visitEnd   (S_score_partwise& elt);

    // identification
    // ------------------------------------------------------

    virtual void              visitStart (S_identification& elt);
    virtual void              visitEnd   (S_identification& elt);
    virtual void              visitStart (S_creator& elt);
    virtual void              visitStart (S_rights& elt);
    virtual void              visitStart (S_source& elt);
    virtual void              visitStart (S_relation& elt);

    // encoding
    // ------------------------------------------------------

    virtual void              visitStart (S_encoding& elt);
    virtual void              visitEnd   (S_encoding& elt);
    virtual void              visitStart (S_encoding_date& elt);
    virtual void              visitStart (S_encoder& elt);
    virtual void              visitStart (S_software& elt);
    virtual void              visitStart (S_encoding_description& elt);
    virtual void              visitStart (S_supports& elt);

    // work & movement
    // ------------------------------------------------------

    virtual void              visitStart (S_work_number& elt);
    virtual void              visitStart (S_work_title& elt);
    virtual void              visitStart (S_opus& elt);
    virtual void              visitStart (S_movement_number& elt);
    virtual void              visitStart (S_movement_title& elt);

    // miscellaneous
    // ------------------------------------------------------

    virtual void              visitStart (S_miscellaneous& elt);
    virtual void              visitEnd   (S_miscellaneous& elt);
    virtual void              visitStart (S_miscellaneous_field& elt);

    // rights JMI v0.9.71
    // ------------------------------------------------------

    // credits
    // ------------------------------------------------------

    virtual void              visitStart (S_credit& elt);
    virtual void              visitEnd   (S_credit& elt);
    virtual void              visitStart (S_credit_type& elt);
    virtual void              visitStart (S_credit_words& elt);
    virtual void              visitStart (S_credit_symbol& elt);
    virtual void              visitStart (S_credit_image& elt);

    // parts & part groups
    // ------------------------------------------------------

    virtual void              visitStart (S_part_list& elt);
    virtual void              visitEnd   (S_part_list& elt);

    virtual void              visitStart (S_display_text& elt);

    virtual void              visitStart (S_part_group& elt);
    virtual void              visitEnd   (S_part_group& elt);

    virtual void              visitStart (S_group_name& elt);
    virtual void              visitStart (S_group_name_display& elt);
    virtual void              visitEnd   (S_group_name_display& elt);

    virtual void              visitStart (S_accidental_text& elt);
    virtual void              visitStart (S_group_abbreviation& elt);
    virtual void              visitStart (S_group_symbol& elt);
    virtual void              visitStart (S_group_barline& elt);

    virtual void              visitStart (S_score_part& elt);
    virtual void              visitEnd   (S_score_part& elt);

    virtual void              visitStart (S_part_name& elt);
    virtual void              visitStart (S_part_name_display& elt);
    virtual void              visitEnd   (S_part_name_display& elt);

    virtual void              visitStart (S_part_abbreviation& elt);
    virtual void              visitStart (S_part_abbreviation_display& elt);
    virtual void              visitEnd   (S_part_abbreviation_display& elt);

    virtual void              visitStart (S_instrument_name& elt);
    virtual void              visitStart (S_instrument_abbreviation& elt);

    virtual void              visitStart (S_part& elt);
    virtual void              visitEnd   (S_part& elt);

    // staves
    // ------------------------------------------------------

//     virtual void              visitStart (S_staves& elt);

    virtual void              visitStart (S_staff& elt);

    // voices
    // ------------------------------------------------------

    virtual void              visitStart (S_voice& elt);

    // measures
    // ------------------------------------------------------

    virtual void              visitStart (S_measure& elt);
    virtual void              visitEnd   (S_measure& elt);

    // print
    // ------------------------------------------------------

    virtual void              visitStart (S_print& elt);
    virtual void              visitEnd   (S_print& elt);

    // backup
    // ------------------------------------------------------

    virtual void              visitStart (S_backup& elt);

    // forward
    // ------------------------------------------------------

    virtual void              visitStart (S_forward& elt);

    // repeats
    // ------------------------------------------------------

    virtual void              visitStart (S_repeat& elt);

    // notes
    // ------------------------------------------------------

    virtual void              visitStart (S_note& elt);
    virtual void              visitEnd   (S_note& elt);

    virtual void              visitStart (S_rest& elt);

    virtual void              visitStart (S_grace& elt);

    // chords
    // ------------------------------------------------------

    virtual void              visitStart (S_chord& elt);
    virtual void              visitEnd   (S_chord& elt);

    // tuplets
    // ------------------------------------------------------

    virtual void              visitStart (S_time_modification& elt);
    virtual void              visitStart (S_tuplet& elt);

    // lyrics
    // ------------------------------------------------------

    virtual void              visitStart (S_lyric& elt);
    virtual void              visitEnd   (S_lyric& elt);

    // harmonies
    // ------------------------------------------------------

    virtual void              visitStart (S_harmony& elt);

    // figured bass
    // ------------------------------------------------------

    virtual void              visitStart (S_figured_bass& elt);

  private:

    // private fields

    // the MSR score we're building
    // ------------------------------------------------------

    S_msrScore                fMsrScore;


    // the score notes events we shall collect for  mxsr2msrSkeletonBuilder
    // ------------------------------------------------------

    mxsrEventsCollection&     fResultingEventsCollection;


    // credits handling
    // ------------------------------------------------------

    S_msrCredit               fCurrentCredit;


    // part groups handling
    // ------------------------------------------------------

    int                       fCurrentPartGroupNumber;
    msrPartGroupTypeKind      fCurrentPartGroupTypeKind;
    std::string               fCurrentPartGroupName;
    std::string               fCurrentPartGroupAbbreviation;

    Bool                      fOnGoingPartGroupNameDisplay;
    std::string               fCurrentPartGroupNameDisplayText;

    std::string               fCurrentPartGroupAccidentalText;
    msrPartGroupSymbolKind    fCurrentPartGroupSymbolKind;
    msrPartGroupBarLineKind   fCurrentPartGroupBarLineKind;
    int                       fCurrentPartGroupSymbolDefaultX;

    S_msrPartGroup            fImplicitOuterMostMsrPartGroup;
    S_mxsrPartGroup           fImplicitOuterMostPartGroup;

    void                      createTheImplicitOuterPartGroupAndAddItToTheMsrScore (
                                int inputLineNumber);

    std::vector <S_mxsrPartGroup>
                              fPartGroupsVector;

    void                      displayPartGroupsVector (
                                int inputLineNumber);

    std::map <int, S_mxsrPartGroup>
                              fPartGroupsMap;

    void                      displayPartGroupsMap (
                                int inputLineNumber);

    std::map <int, S_mxsrPartGroup>
                              fStartedPartGroupsMap;

    // the part groups may be nested so we need a stack,
    // on which they are pushed in handleBOFPartGroupsNestingBOFAndScorePartsAllocation()
    //
    // the implicit outer-most part group is pushed onto it
    // in createTheImplicitOuterPartGroupAndAddItToTheMsrScore(),
    // called in mxsr2msrSkeletonBuilder::mxsr2msrSkeletonBuilder()
    mxsrPartGroupsStack       fPartGroupsStack;

    void                      displayPartGroupsStack (
                                int inputLineNumber);

    // the part groups 'stop' may not be in strict first-in, first-out order,
    // for example in Finale MusicXML exports
    // we thus have to postpone the association of 'stop's with 'start's
    // until the whole <part-list /> markup has been analyzed
    void                      displayStartedPartGroupsMap (
                                int inputLineNumber);

    S_mxsrPartGroup           fetchStartedPartGroupFromMap (
                                int partGroupNumber);

  private:

    // private methods

    // several part groups may start and/or stop at the same position JMI ??? v0.9.69
    std::vector <S_mxsrPartGroupsList>
                              fStartedPartGroupsListsVector;

    void                      registerPartGroupStart (
                                int                    inputLineNumber,
                                const S_mxsrPartGroup& partGroup);

    void                      displayStartedPartGroupsVector (
                                int inputLineNumber);

    std::vector <S_mxsrPartGroupsList>
                              fStoppedPartGroupsListsVector;

    void                      registerPartGroupStop (
                                int                    inputLineNumber,
                                const S_mxsrPartGroup& partGroup);

    void                      displayStoppedPartGroupsVector (
                              int inputLineNumber);

    void                      displayAllCollectedData (
                                int                inputLineNumber,
                                const std::string& context);

    void                      handlePartGroupStart (
                                int inputLineNumber);

    void                      handlePartGroupStop (
                                int inputLineNumber);

    void                      registerPart (
                                int              inputLineNumber,
                                const S_msrPart& thePart);

    void                      handlePartGroupsNesting (
                                int                    inputLineNumber,
                                const S_mxsrPartGroup& partGroupToBeStopped,
                                const S_mxsrPartGroup& containingPartGroup);

    void                      handleBOFPartGroupsNestingBOFAndScorePartsAllocation (
                                int inputLineNumber);

    // parts handling
    // ------------------------------------------------------

    std::string               fCurrentScorePartID; // from <score-part />
    std::string               fCurrentPartID;      // used throughout, computed

    std::string               fCurrentPartName;
    std::string               fCurrentPartNameDisplayText;
    Bool                      fOnGoingPartNameDisplay;


    std::string               fCurrentPartAbbreviation;
    std::string               fCurrentPartAbbreviationDisplayText;
    Bool                      fOnGoingPartAbbreviationDisplay;

    std::string               fCurrentPartInstrumentName;
    std::string               fCurrentPartInstrumentAbbreviation;

    std::map <std::string, S_msrPart>
                              fPartsMap;

    void                      displayPartsMap (
                                int inputLineNumber);

    // part groups numbers can be re-used, so they're no identifier,
    // so we identify them by an ordinal number, set upon the 'start'
    // this is used for handling nesting/overlapping
    int                       fCurrentPartGroupIdentity;

    std::vector <S_msrPart>   fPartsVector;

    void                      displayPartsVector (
                                int inputLineNumber);

    S_msrPart                 fCurrentPart; // used throughout


    // staff handling
    // ------------------------------------------------------

    S_msrStaff                createStaffInCurrentPartIfNotYetDone (
                                int inputLineNumber,
                                int staffNumber);


    // voice handling
    // ------------------------------------------------------

    int                       fCurrentMusicXMLVoiceNumber; // used throughout

    S_msrMeasureElement       fPreviousMeasureElement;

    S_msrVoice                fetchFirstVoiceFromCurrentPart (
                                int inputLineNumber);


    /*
      the order in which a tuplet'members are present in a MusicXML file
      is up to the exporter that created it,
      and there may backups to other voices before all of them have been seen

      we therefore need to store tuplets internally until they are complete.

      this is done with a vector, whose indices are the voices numbers
    */

    // we need a fast access to the voices and their handlers, hence:
    std::vector <S_msrVoice>  fCurrentPartVoicesVector;

//     std::vector <S_mxsr2msrBuilderVoiceHandler>
//                               fCurrentPartBuilderVoicesHandlersVector;

    void                      populateCurrentPartVoicesVectorsFromPart (
                                  const S_msrPart& part);

		void											displayCurrentPartVoicesVector (
                                int                inputLineNumber,
																const std::string& context);

    S_msrVoice                createRegularVoiceInStaffIfNotYetDone (
                                int inputLineNumber,
                                int staffNumber,
                                int voiceNumber);


    // implicit initial repeat handling
    // ------------------------------------------------------

		Bool                      fAFirstForwardRepeatHasBeenMet;

    // measures
    // ------------------------------------------------------

    std::string               fCurrentMeasureNumber;

    std::string               fScoreFirstMeasureNumber;
    std::string               fScoreLastMeasureNumber;

    int                       fScoreMeasuresNumber;
    int                       fPartNumberOfMeasures;


    // print
    // ------------------------------------------------------

    msrPrintObjectKind        fCurrentPrintObjectKind;

    Bool                      fOnGoingPrint;


    // notes handling
    // ------------------------------------------------------

		int	                      fCurrentNoteSequentialNumber;
		int	                      fPreviousNoteSequentialNumber;

		int	                      fCurrentNoteStartInputLineNumber;
		int	                      fCurrentNoteEndInputLineNumber;

		int	                      fPreviousNoteStartInputLineNumber;
		int	                      fPreviousNoteEndInputLineNumber;

    int                       fCurrentNoteStaffNumber; // used throughout
    int                       fPreviousNoteStaffNumber;

    int                       fCurrentNoteVoiceNumber; // used throughout
    int                       fPreviousNoteVoiceNumber;

    Bool                      fOnGoingNote;

    Bool                      fCurrentNoteIsARest;

    // grace notes handling
    // ------------------------------------------------------

    Bool                      fCurrentNoteIsAGraceNote;
    Bool                      fPreviousNoteIsAGraceNote;

    void                      registerGraceNoteEventIfAny ();

    // staff changes handling
    // ------------------------------------------------------

    int                       fCurrentStaffNumber;

		Bool                      fPreviousNoteIsATakeOffCandidate;

    Bool                      handleStaffChangeIfAny (
                                int inputStartLineNumber);

    // chords handling
    // ------------------------------------------------------

    Bool                      fCurrentNoteBelongsToAChord;
    Bool                      fPreviousNoteBelongsToAChord;

//     Bool                      fOnGoingChord;

    void                      registerChordEventIfAny ();

    // tuplets handling
    // ------------------------------------------------------

    // nested tuplets are numbered 1, 2, ...
    int                       fCurrentTupletNumber;
//     int                       fPreviousTupletNumber;

    Bool                      fCurrentNoteBelongsToATuplet;

    // there can be multiple tuplets starts on a given note,
    // so they aref kept aside until they are handled
    std::list <S_mxsrTuplet>  fPendingTupletsList;

    void                      handleChordMemberNoteIfRelevant (
                                int inputStartLineNumber);

    void                      displayPendingTupletsList (
                                const std::string& title,
                                int                inputStartLineNumber) const;

    // a tuplet stop may occur in a chord before the latter's last note,
    // and the can be several tuplets stop in a single note:
    // we store their description in a map indexed by a sequential number
    std::multimap <int, S_mxsr2msrPendingTupletStop>
                              fPendingTupletsStopsMap; // CHORD_TUP JMI v0.9.71

    void                      handleTupletEventIfAny ();

    void                      displayPendingTupletsStopsMap (
                                const std::string& title,
                                int                inputStartLineNumber) const;

    void                      handePendingTupletsStopsIfAny (
                                int inputStartLineNumber);


    // lyrics handling
    // ------------------------------------------------------

    std::string               fCurrentStanzaNumber;
    std::string               fCurrentStanzaName;


    // harmonies handling
    // ------------------------------------------------------

    Bool                      fThereAreHarmoniesToBeAttachedToCurrentNote;
    int                       fHarmoniesVoicesCounter;

    S_msrVoice                createPartHarmoniesVoiceIfNotYetDone (
                                int              inputLineNumber,
                                const S_msrPart& thePart);


    // figured bass handling
    // ------------------------------------------------------

    Bool                      fThereAreFiguredBassToBeAttachedToCurrentNote;
    int                       fFiguredBassVoicesCounter;

    S_msrVoice                createPartFiguredBassVoiceIfNotYetDone (
                                int              inputLineNumber,
                                const S_msrPart& thePart);
};


}

#endif // ___mxsr2msrSkeletonBuilder___
