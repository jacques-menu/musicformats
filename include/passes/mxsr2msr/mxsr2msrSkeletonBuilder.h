/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___mxsr2msrSkeletonBuilder___
#define ___mxsr2msrSkeletonBuilder___

#include "typedefs.h"

#include "msrPartGroups.h"
#include "msrPrintObjects.h"


namespace MusicFormats
{

//________________________________________________________________________
struct mxmlPartGroupDescr : public smartable
{
/*
  positions represent the order in which the parts appear in <part-list />
*/

  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<mxmlPartGroupDescr> create (
                            int            startInputLineNumber,
                            int            partGroupNumber,
                            const S_msrPartGroup& partGroup,
                            int            startPosition);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          mxmlPartGroupDescr (
                            int            startInputLineNumber,
                            int            partGroupNumber,
                            const S_msrPartGroup& partGroup,
                            int            startPosition);

    virtual               ~mxmlPartGroupDescr ();

  public:

    // set and get
    // ------------------------------------------------------

    int                   getStartInputLineNumber () const
                              { return fStartInputLineNumber; }

    int                   getStopInputLineNumber () const
                              { return fStopInputLineNumber; }

    int                   getPartGroupNumber () const
                              { return fPartGroupNumber; }

    S_msrPartGroup        getPartGroup () const
                              { return fPartGroup; }

    int                   getStartPosition () const
                              { return fStartPosition; }

    void                  setStopPosition (
                            int stopInputLineNumber,
                            int stopPosition)
                              {
                                fStopInputLineNumber = stopInputLineNumber;
                                fStopPosition = stopPosition;
                              }

    int                   getStopPosition () const
                              { return fStopPosition; }

  public:

    // public services
    // ------------------------------------------------------

    std::string           partGroupDescrAsString () const;

    std::string           getPartGroupDescrCombinedName () const
                              {
                                return
                                  fPartGroup->getPartGroupCombinedName ();
                              }

  public:

    // print
    // ------------------------------------------------------

    virtual void          print (std::ostream& os) const;

  private:

    // private fields
    // ------------------------------------------------------

    int                   fStartInputLineNumber;
    int                   fStopInputLineNumber;

    int                   fPartGroupNumber; // may be reused later

    S_msrPartGroup        fPartGroup;

    int                   fStartPosition;
    int                   fStopPosition;
};
typedef SMARTP<mxmlPartGroupDescr> S_mxmlPartGroupDescr;
EXP std::ostream& operator << (std::ostream& os, const S_mxmlPartGroupDescr& elt);

//________________________________________________________________________
class EXP mxsr2msrSkeletonBuilder :

  // score partwise

  public visitor<S_score_partwise>,

  // rights
  // ------------------------------------------------------

  public visitor<S_work_number>,
  public visitor<S_work_title>,
  public visitor<S_opus>,
  public visitor<S_movement_number>,
  public visitor<S_movement_title>,

  // identification
  // ------------------------------------------------------

  public visitor<S_identification>,
  public visitor<S_creator>,
  public visitor<S_rights>,
  public visitor<S_source>,
  public visitor<S_relation>,

  // encoding
  // ------------------------------------------------------

  public visitor<S_encoding>,
  public visitor<S_encoding_date>,
  public visitor<S_encoder>,
  public visitor<S_software>,
  public visitor<S_encoding_description>,
  public visitor<S_supports>,

  // miscellaneous
  // ------------------------------------------------------

  public visitor<S_miscellaneous>,
  public visitor<S_miscellaneous_field>,

  // credits
  // ------------------------------------------------------

  public visitor<S_credit>,
  public visitor<S_credit_type>,
  public visitor<S_credit_words>,
  public visitor<S_credit_symbol>,
  public visitor<S_credit_image>,

  // parts & part groups
  // ------------------------------------------------------

  public visitor<S_part_list>,

  public visitor<S_display_text>,

  public visitor<S_part_group>,

  public visitor<S_group_name>,
  public visitor<S_group_name_display>,

  public visitor<S_accidental_text>,
  public visitor<S_group_abbreviation>,
  public visitor<S_group_symbol>,
  public visitor<S_group_barline>,

  public visitor<S_score_part>,

  public visitor<S_part_name>,
  public visitor<S_part_name_display>,

  public visitor<S_part_abbreviation>,
  public visitor<S_part_abbreviation_display>,

  public visitor<S_instrument_name>,
  public visitor<S_instrument_abbreviation>,

  public visitor<S_part>,

  // staves
  // ------------------------------------------------------

  public visitor<S_staves>,
  public visitor<S_staff>,

  // voices
  // ------------------------------------------------------

  public visitor<S_voice>,

  // measures
  // ------------------------------------------------------

  public visitor<S_measure>,

  // print
  // ------------------------------------------------------

  public visitor<S_print>,

  // notes
  // ------------------------------------------------------

  public visitor<S_note>,

  // lyrics
  // ------------------------------------------------------

  public visitor<S_lyric>,
  public visitor<S_syllabic>,
  public visitor<S_text>,
  public visitor<S_elision>,
  public visitor<S_extend>,

  // harmonies
  // ------------------------------------------------------

  public visitor<S_harmony>,

  // figured bass
  // ------------------------------------------------------

  public visitor<S_figured_bass>

{
  public:

    // constructors/destructor
    // ------------------------------------------------------

                          mxsr2msrSkeletonBuilder ();

    virtual               ~mxsr2msrSkeletonBuilder ();

    // set and get
    // ------------------------------------------------------

    S_msrScore            getMsrScore () const
                              { return fMsrScore; }


  public:

    // public services
    // ------------------------------------------------------

    // visitors
    // ------------------------------------------------------

    void                  browseMxsr (
                            const Sxmlelement& xmlTree);

  protected:

    // score partwise
    // ------------------------------------------------------

    virtual void          visitStart (S_score_partwise& elt);
    virtual void          visitEnd   (S_score_partwise& elt);

    // identification
    // ------------------------------------------------------

    virtual void          visitStart (S_identification& elt);
    virtual void          visitEnd   (S_identification& elt);
    virtual void          visitStart (S_creator& elt);
    virtual void          visitStart (S_rights& elt);
    virtual void          visitStart (S_source& elt);
    virtual void          visitStart (S_relation& elt);

    // encoding
    // ------------------------------------------------------

    virtual void          visitStart (S_encoding& elt);
    virtual void          visitEnd   (S_encoding& elt);
    virtual void          visitStart (S_encoding_date& elt);
    virtual void          visitStart (S_encoder& elt);
    virtual void          visitStart (S_software& elt);
    virtual void          visitStart (S_encoding_description& elt);
    virtual void          visitStart (S_supports& elt);



    virtual void          visitStart (S_work_number& elt);
    virtual void          visitStart (S_work_title& elt);
    virtual void          visitStart (S_opus& elt);
    virtual void          visitStart (S_movement_number& elt);
    virtual void          visitStart (S_movement_title& elt);

    // miscellaneous
    // ------------------------------------------------------

    virtual void          visitStart (S_miscellaneous& elt);
    virtual void          visitEnd   (S_miscellaneous& elt);
    virtual void          visitStart (S_miscellaneous_field& elt);

    // rights JMI
    // ------------------------------------------------------

    // credits
    // ------------------------------------------------------

    virtual void          visitStart (S_credit& elt);
    virtual void          visitEnd   (S_credit& elt);
    virtual void          visitStart (S_credit_type& elt);
    virtual void          visitStart (S_credit_words& elt);
    virtual void          visitStart (S_credit_symbol& elt);
    virtual void          visitStart (S_credit_image& elt);

    // parts & part groups
    // ------------------------------------------------------

    virtual void          visitStart (S_part_list& elt);
    virtual void          visitEnd   (S_part_list& elt);

    virtual void          visitStart (S_display_text& elt);

    virtual void          visitStart (S_part_group& elt);
    virtual void          visitEnd   (S_part_group& elt);

    virtual void          visitStart (S_group_name& elt);
    virtual void          visitStart (S_group_name_display& elt);
    virtual void          visitEnd   (S_group_name_display& elt);

    virtual void          visitStart (S_accidental_text& elt);
    virtual void          visitStart (S_group_abbreviation& elt);
    virtual void          visitStart (S_group_symbol& elt);
    virtual void          visitStart (S_group_barline& elt);

    virtual void          visitStart (S_score_part& elt);
    virtual void          visitEnd   (S_score_part& elt);

    virtual void          visitStart (S_part_name& elt);
    virtual void          visitStart (S_part_name_display& elt);
    virtual void          visitEnd   (S_part_name_display& elt);

    virtual void          visitStart (S_part_abbreviation& elt);
    virtual void          visitStart (S_part_abbreviation_display& elt);
    virtual void          visitEnd   (S_part_abbreviation_display& elt);

    virtual void          visitStart (S_instrument_name& elt);
    virtual void          visitStart (S_instrument_abbreviation& elt);

    virtual void          visitStart (S_part& elt);
    virtual void          visitEnd   (S_part& elt);

    // staves
    // ------------------------------------------------------

    virtual void          visitStart (S_staves& elt);

    virtual void          visitStart (S_staff& elt);

    // voices
    // ------------------------------------------------------

    virtual void          visitStart (S_voice& elt);

    // measures
    // ------------------------------------------------------

    virtual void          visitStart (S_measure& elt);
    virtual void          visitEnd   (S_measure& elt);

    // print
    // ------------------------------------------------------

    virtual void          visitStart (S_print& elt);
    virtual void          visitEnd   (S_print& elt);

    // notes
    // ------------------------------------------------------

    virtual void          visitStart (S_note& elt);
    virtual void          visitEnd   (S_note& elt);

    // lyrics
    // ------------------------------------------------------

    virtual void          visitStart (S_lyric& elt);
    virtual void          visitEnd   (S_lyric& elt);

    // harmonies
    // ------------------------------------------------------

    virtual void          visitStart (S_harmony& elt);

    // figured bass
    // ------------------------------------------------------

    virtual void          visitStart (S_figured_bass& elt);

  private:

    // private fields

    // the MSR score we're building
    // ------------------------------------------------------

    S_msrScore                fMsrScore;


    // credits handling
    // ------------------------------------------------------

    S_msrCredit               fCurrentCredit;


    // part groups handling
    // ------------------------------------------------------

    int                       fCurrentPartGroupNumber;
    msrPartGroupTypeKind
                              fCurrentPartGroupTypeKind;
    std::string               fCurrentPartGroupName;
    std::string               fCurrentPartGroupAbbreviation;

    Bool                      fOnGoingPartGroupNameDisplay;
    std::string               fCurrentPartGroupNameDisplayText;

    std::string               fCurrentPartGroupAccidentalText;
    msrPartGroupSymbolKind    fCurrentPartGroupSymbolKind;
    msrPartGroupBarLineKind   fCurrentPartGroupBarLineKind;
    int                       fCurrentPartGroupSymbolDefaultX;

    // an implicit part group has to be created to contain everything,
    // since there can be parts out of any explicit part group
    S_mxmlPartGroupDescr      fImplicitPartGroupDescr;
    S_msrPartGroup            fImplicitPartGroup;

    void                      createImplicitPartGroup ();

    // part groups numbers can be re-used, they're no identifier
    // we use a map to access them by part group number
    int                       fPartGroupsCounter;
    std::vector<S_mxmlPartGroupDescr>
                              fPartGroupDescsVector;
    std::map<int, S_mxmlPartGroupDescr>
                              fAllPartGroupDescrsMap;
    std::map<int, S_mxmlPartGroupDescr>
                              fStartedPartGroupDescrsMap;

    void                      showPartGroupDescrsVector (
                                int inputLineNumber);

    void                      showAllPartGroupDescrsMap (
                                int inputLineNumber);

    void                      showStartedPartGroupDescrsMap (
                                int inputLineNumber);

    S_mxmlPartGroupDescr      fetchStartedPartGroupDescr (
                                int partGroupNumber);

    std::list<S_mxmlPartGroupDescr>
                              fPartGroupsDescrStack;

  private:

    // private methods

    void                      showPartGroupsStack (
                                int inputLineNumber);

    S_mxmlPartGroupDescr      fetchPartGroupDescrStackTop ();

    void                      registerPartGroupDescrAsStarted (
                                int                  inputLineNumber,
                                const S_mxmlPartGroupDescr& partGroupDescr);

    void                      registerPartGroupDescrAsStopped (
                                int                  inputLineNumber,
                                const S_mxmlPartGroupDescr& partGroupDescr);

    void                      showPartGroupsData (
                                int           inputLineNumber,
                                const std::string& context);

    void                      handlePartGroupStart (
                                int inputLineNumber);

    void                      handlePartGroupStop (
                                int inputLineNumber);

    std::vector<std::list<S_mxmlPartGroupDescr> >
                              fPositionStartingPartGroupDescrsVector;
    std::vector<std::list<S_mxmlPartGroupDescr> >
                              fPositionStoppingPartGroupDescrsVector;

    void                      showPositionStartingPartGroupDescrsVector (
                                int inputLineNumber);

    void                      showPositionStoppingPartGroupDescrsVector (
                                int inputLineNumber);

    void                      insertPartGroupDescInStartingList (
                                int                  inputLineNumber,
                                const S_mxmlPartGroupDescr& partGroupDescr,
                                std::list<S_mxmlPartGroupDescr>&
                                                     startingPartGroupDescrsList);

    void                      insertPartGroupDescInStoppingList (
                                int                  inputLineNumber,
                                const S_mxmlPartGroupDescr& partGroupDescr,
                                std::list<S_mxmlPartGroupDescr>&
                                                     stoppingPartGroupDescrsList);

    void                      registerPartGroupDescrAsStoppingAtCurrentPosition (
                                int                  inputLineNumber,
                                const S_mxmlPartGroupDescr& partGroupDescr);

    void                      registerPart (
                                int       inputLineNumber,
                                int       partPosition,
                                const S_msrPart& part);

    void                      doNestPartGroupDescrInItsContainer (
                                int                         inputLineNumber,
                                const S_mxmlPartGroupDescr& partGroupDescrToBeStopped,
                                const S_mxmlPartGroupDescr& containingPartGroupDescr);

    void                      doPartGroupsNestingAndPartsAllocation (
                                int inputLineNumber);



    // parts handling
    // ------------------------------------------------------

    std::string               fCurrentPartID; // used throughout

    std::string               fCurrentPartName;
    std::string               fCurrentPartNameDisplayText;
    Bool                      fOnGoingPartNameDisplay;


    std::string               fCurrentPartAbbreviation;
    std::string               fCurrentPartAbbreviationDisplayText;
    Bool                      fOnGoingPartAbbreviationDisplay;

    std::string               fCurrentPartInstrumentName;
    std::string               fCurrentPartInstrumentAbbreviation;

    std::map<std::string, S_msrPart>    fPartsMap;

    int                       fCurrentNoIDPartNumber;

    // handling the part, each one incrementing the 'position'
    // of the part groups, used for handling nesting/overlapping
    int                       fCurrentPartsPosition;
    std::vector<S_msrPart>         fPartsVector;

    void                      showPartsVector (
                                int inputLineNumber);

    S_msrPart                 fCurrentPart; // used throughout


    // staff handling
    // ------------------------------------------------------

    int                       fCurrentStaffMusicXMLNumber; // used throughout

    S_msrStaff                createStaffInCurrentPartIfNotYetDone (
                                int inputLineNumber,
                                int staffNumber);


    // voice handling
    // ------------------------------------------------------

    int                       fCurrentVoiceMusicXMLNumber; // used throughout

    S_msrVoice                createRegularVoiceInStaffIfNotYetDone (
                                int inputLineNumber,
                                int staffNumber,
                                int voiceNumber);

    S_msrVoice                fetchFirstRegularVoiceFromStaff (
                                int inputLineNumber,
                                int staffNumber);


    // measures
    // ------------------------------------------------------

    std::string               fCurrentMeasureNumber;

    int                       fScoreNumberOfMeasures;
    int                       fPartNumberOfMeasures;


    // print
    // ------------------------------------------------------

    msrPrintObjectKind        fCurrentPrintObjectKind;

    Bool                      fOnGoingPrint;


    // ------------------------------------------------------

    Bool                      fOnGoingNote;


    // lyrics handling
    // ------------------------------------------------------

    std::string               fCurrentStanzaNumber;
    std::string               fCurrentStanzaName;


    // harmonies handling
    // ------------------------------------------------------

    Bool                      fThereAreHarmoniesToBeAttachedToCurrentNote;
    int                       fHarmoniesVoicesCounter;

//     S_msrVoice                createRegularVoiceHarmoniesVoiceIfNotYetDone ( JMI v0.9.66
//                                 int        inputLineNumber,
//                                 const S_msrVoice& voice);

    S_msrVoice                createPartHarmoniesVoiceIfNotYetDone (
                                int        inputLineNumber,
                                const S_msrPart&  part);


    // figured bass handling
    // ------------------------------------------------------

    Bool                      fThereAreFiguredBassToBeAttachedToCurrentNote;
    int                       fFiguredBassVoicesCounter;

    S_msrVoice                createPartFiguredBassVoiceIfNotYetDone (
                                int        inputLineNumber,
                                const S_msrPart&  part);
};


}

#endif
