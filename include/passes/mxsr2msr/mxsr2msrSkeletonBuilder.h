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
/*
  The part groups are described for the neded of this pass by class mxsrPartGroup,
  which is distinct from msrPartGroup, an instance of which they contain.

  The identifiers for msrPartGroups instances contain 'Msr'
  wherever needed to make things clear
*/

//________________________________________________________________________
// pre-declaration
class   mxsrPartGroup;
typedef SMARTP<mxsrPartGroup> S_mxsrPartGroup;

//________________________________________________________________________
class mxsrPartGroup : public smartable
{
/*
  positions represent the order in which the parts appear in <part-list />,
  starting at 0 since std::vectors are used
*/

  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<mxsrPartGroup> create (
                            int                   startInputLineNumber,
                            int                   partGroupNumber,
                            const S_msrPartGroup& theMsrPartGroup,
                            int                   identity);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          mxsrPartGroup (
                            int                   startInputLineNumber,
                            int                   partGroupNumber,
                            const S_msrPartGroup& theMsrPartGroup,
                            int                   identity);

    virtual               ~mxsrPartGroup ();

  public:

    // set and get
    // ------------------------------------------------------

    int                   getPartGroupNumber () const
                              { return fPartGroupNumber; }

    void                  setPartGroupIdentity (int identity)
                              { fPartGroupIdentity= identity; }

    int                   getPartGroupIdentity () const
                              { return fPartGroupIdentity; }

    int                   getInputStartLineNumber () const
                              { return fStartInputLineNumber; }

    int                   getStopInputLineNumber () const
                              { return fStopInputLineNumber; }

    S_msrPartGroup        getMsrPartGroup () const
                              { return fMsrPartGroup; }

  public:

    // public services
    // ------------------------------------------------------

    std::string           fetchMsrPartGroupCombinedName () const
                              {
                                return
                                  fMsrPartGroup->getPartGroupCombinedName ();
                              }

    static bool           comparePartGroupsByIncreasingIdentity (
                            const S_mxsrPartGroup& first,
                            const S_mxsrPartGroup& second);

    static bool           comparePartGroupsByDecreasingIdentity (
                            const S_mxsrPartGroup& first,
                            const S_mxsrPartGroup& second);

  public:

    // print
    // ------------------------------------------------------

    std::string           asString () const;

    virtual void          print (std::ostream& os) const;

  private:

    // private fields
    // ------------------------------------------------------

    int                   fPartGroupNumber; // may be reused later

    int                   fPartGroupIdentity; // set upon part group start

    int                   fStartInputLineNumber;
    int                   fStopInputLineNumber;


    S_msrPartGroup        fMsrPartGroup;
};
EXP std::ostream& operator << (std::ostream& os, const S_mxsrPartGroup& elt);

//______________________________________________________________________________
class EXP mxsrPartGroupsList : public smartable
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<mxsrPartGroupsList> create ();

    static SMARTP<mxsrPartGroupsList> create (
                            const S_mxsrPartGroup& partGroup);

    // constructors/destructor
    // ------------------------------------------------------

                          mxsrPartGroupsList ();

                          mxsrPartGroupsList (
                            const S_mxsrPartGroup& partGroup);

    virtual               ~mxsrPartGroupsList ();

  public:

    // set and get
    // ------------------------------------------------------

    const std::list<S_mxsrPartGroup>&
                          getMxsrPartGroupsStdList () const
                              { return fMxsrPartGroupsStdList;}

  public:

    // public services
    // ------------------------------------------------------

    // basic list stuff
    size_t                size () const
                              { return fMxsrPartGroupsStdList.size (); }

    S_mxsrPartGroup&      front ()
                              { return fMxsrPartGroupsStdList.front (); }

    S_mxsrPartGroup&      back ()
                              { return fMxsrPartGroupsStdList.back (); }

    void                  pop_front ()
                              { fMxsrPartGroupsStdList.pop_front (); }

    void                  pop_back ()
                              { fMxsrPartGroupsStdList.pop_back (); }

    void                  push_front (
                            const S_mxsrPartGroup& partGroup)
                              { fMxsrPartGroupsStdList.push_front (partGroup); }

    void                  push_back (
                            const S_mxsrPartGroup& partGroup)
                              { fMxsrPartGroupsStdList.push_back (partGroup); }

    // sort
    void                  sortByDecreasingIdentity();

  public:

    // print
    // ------------------------------------------------------

    void                  print (std::ostream& os) const;

    void                  printWithContext (
                            const std::string& context,
                            char               evidencer,
                            std::ostream&      os) const;

  private:

    // private services
    // ------------------------------------------------------


    // add an mxsrPartGroup
    void                  prependPartGroup (
                            const S_mxsrPartGroup& thePartGroup);

    void                  appendPartGroup (
                            const S_mxsrPartGroup& thePartGroup);

    // private fields
    // ------------------------------------------------------

    std::string           fMxsrPartGroupListName;

    std::list<S_mxsrPartGroup>
                          fMxsrPartGroupsStdList;
};
typedef SMARTP<mxsrPartGroupsList> S_mxsrPartGroupsList;
EXP std::ostream& operator << (std::ostream& os, const mxsrPartGroupsList& elt);
EXP std::ostream& operator << (std::ostream& os, const S_mxsrPartGroupsList& elt);

//______________________________________________________________________________
class EXP mxsrPartGroupsStack : public smartable
/*
  C++ std::stack cannot be traversed, so we use std::list instead
*/
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<mxsrPartGroupsStack> create ();

    static SMARTP<mxsrPartGroupsStack> create (
                            const S_mxsrPartGroup& partGroup);

    // constructors/destructor
    // ------------------------------------------------------

                          mxsrPartGroupsStack ();

                          mxsrPartGroupsStack (
                            const S_mxsrPartGroup& partGroup);

    virtual               ~mxsrPartGroupsStack ();

  public:

    // set and get
    // ------------------------------------------------------

    const std::list<S_mxsrPartGroup>&
                          getMxsrPartGroupsStdList () const
                              { return fMxsrPartGroupsStdList;}

  public:

    // public services
    // ------------------------------------------------------

    // basic list stuff
    size_t                size () const
                              { return fMxsrPartGroupsStdList.size (); }

    void                  push (
                            const S_mxsrPartGroup& partGroup)
                              { fMxsrPartGroupsStdList.push_front (partGroup); }

    S_mxsrPartGroup&      top ()
                              { return fMxsrPartGroupsStdList.front (); }

    void                  pop ()
                              { fMxsrPartGroupsStdList.pop_front (); }

  public:

    // print
    // ------------------------------------------------------

    void                  print (std::ostream& os) const;

    void                  printWithContext (
                            const std::string& context,
                            char               evidencer,
                            std::ostream&      os) const;

  private:

    // private services
    // ------------------------------------------------------

  private:

    // private fields
    // ------------------------------------------------------

    std::string           fMxsrPartGroupStackName;

    std::list<S_mxsrPartGroup>
                          fMxsrPartGroupsStdList;
};
typedef SMARTP<mxsrPartGroupsStack> S_mxsrPartGroupsStack;
EXP std::ostream& operator << (std::ostream& os, const mxsrPartGroupsStack& elt);
EXP std::ostream& operator << (std::ostream& os, const S_mxsrPartGroupsStack& elt);

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
virtual void              visitEnd   (S_score_partwise& elt);

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

    S_msrScore            fMsrScore;


    // credits handling
    // ------------------------------------------------------

    S_msrCredit           fCurrentCredit;


    // part groups handling
    // ------------------------------------------------------

    int                   fCurrentPartGroupNumber;
    msrPartGroupTypeKind  fCurrentPartGroupTypeKind;
    std::string           fCurrentPartGroupName;
    std::string           fCurrentPartGroupAbbreviation;

    Bool                  fOnGoingPartGroupNameDisplay;
    std::string           fCurrentPartGroupNameDisplayText;

    std::string           fCurrentPartGroupAccidentalText;
    msrPartGroupSymbolKind
                          fCurrentPartGroupSymbolKind;
    msrPartGroupBarLineKind
                          fCurrentPartGroupBarLineKind;
    int                   fCurrentPartGroupSymbolDefaultX;

    S_msrPartGroup        fImplicitOuterMostMsrPartGroup;
    S_mxsrPartGroup       fImplicitOuterMostPartGroup;

    void                  createTheImplicitOuterPartGroupAndAddItToTheMsrScore (
                            int inputLineNumber);

    std::vector<S_mxsrPartGroup>
                          fPartGroupsVector;

    void                  displayPartGroupsVector (
                            int inputLineNumber);

    std::map<int, S_mxsrPartGroup>
                          fPartGroupsMap;

    void                  displayPartGroupsMap (
                            int inputLineNumber);

    std::map<int, S_mxsrPartGroup>
                          fStartedPartGroupsMap;

    // the part groups may be nested so we need a stack,
    // on which they are pushed in handleBOFPartGroupsNestingBOFAndScorePartsAllocation()
    //
    // the implicit outer-most part group is pushed onto it
    // in createTheImplicitOuterPartGroupAndAddItToTheMsrScore(),
    // called in mxsr2msrSkeletonBuilder::mxsr2msrSkeletonBuilder()
    mxsrPartGroupsStack   fPartGroupsStack;

    void                  displayPartGroupsStack (
                            int inputLineNumber);

    // the part groups 'stop' may not be in strict first-in, first-out order,
    // for example in Finale MusicXML exports
    // we thus have to postpone the association of 'stop's with 'start's
    // until the whole <part-list /> markup has been analyzed
    void                  displayStartedPartGroupsMap (
                            int inputLineNumber);

    S_mxsrPartGroup       fetchStartedPartGroupFromMap (
                            int partGroupNumber);

  private:

    // private methods

    // several part groups may start and/or stop at the same position JMI ??? v0.9.69
    std::vector<S_mxsrPartGroupsList>
                          fStartedPartGroupsListsVector;

    void                  registerPartGroupStart (
                            int                    inputLineNumber,
                            const S_mxsrPartGroup& partGroup);

    void                  displayStartedPartGroupsVector (
                            int inputLineNumber);

    std::vector<S_mxsrPartGroupsList>
                          fStoppedPartGroupsListsVector;

    void                  registerPartGroupStop (
                            int                    inputLineNumber,
                            const S_mxsrPartGroup& partGroup);

    void                  displayStoppedPartGroupsVector (
                           int inputLineNumber);

    void                  displayAllCollectedData (
                            int                inputLineNumber,
                            const std::string& context);

    void                  handlePartGroupStart (
                            int inputLineNumber);

    void                  handlePartGroupStop (
                            int inputLineNumber);

    void                  registerPart (
                            int              inputLineNumber,
                            const S_msrPart& thePart);

    void                  handlePartGroupsNesting (
                            int                    inputLineNumber,
                            const S_mxsrPartGroup& partGroupToBeStopped,
                            const S_mxsrPartGroup& containingPartGroup);

    void                  handleBOFPartGroupsNestingBOFAndScorePartsAllocation (
                            int inputLineNumber);

    // parts handling
    // ------------------------------------------------------

    std::string           fCurrentScorePartID; // from <score-part />
    std::string           fCurrentPartID;      // used throughout, computed

    std::string           fCurrentPartName;
    std::string           fCurrentPartNameDisplayText;
    Bool                  fOnGoingPartNameDisplay;


    std::string           fCurrentPartAbbreviation;
    std::string           fCurrentPartAbbreviationDisplayText;
    Bool                  fOnGoingPartAbbreviationDisplay;

    std::string           fCurrentPartInstrumentName;
    std::string           fCurrentPartInstrumentAbbreviation;

    std::map<std::string, S_msrPart>
                          fPartsMap;

    void                  displayPartsMap (
                            int inputLineNumber);

    // part groups numbers can be re-used, so they're no identifier,
    // so we identify them by an ordinal number, set upon the 'start'
    // this is used for handling nesting/overlapping
    int                   fCurrentPartGroupIdentity;

    std::vector<S_msrPart>
                          fPartsVector;

    void                  displayPartsVector (
                            int inputLineNumber);

    S_msrPart             fCurrentPart; // used throughout


    // staff handling
    // ------------------------------------------------------

    int                   fCurrentStaffMusicXMLNumber; // used throughout

    S_msrStaff            createStaffInCurrentPartIfNotYetDone (
                            int inputLineNumber,
                            int staffNumber);


    // voice handling
    // ------------------------------------------------------

    int                   fCurrentVoiceMusicXMLNumber; // used throughout

    S_msrVoice            createRegularVoiceInStaffIfNotYetDone (
                            int inputLineNumber,
                            int staffNumber,
                            int voiceNumber);


    // measures
    // ------------------------------------------------------

    std::string           fCurrentMeasureNumber;

    std::string           fScoreFirstMeasureNumber;
    std::string           fScoreLastMeasureNumber;

    int                   fScoreMeasuresNumber;
    int                   fPartNumberOfMeasures;


    // print
    // ------------------------------------------------------

    msrPrintObjectKind    fCurrentPrintObjectKind;

    Bool                  fOnGoingPrint;


    // ------------------------------------------------------

    Bool                  fOnGoingNote;


    // lyrics handling
    // ------------------------------------------------------

    std::string           fCurrentStanzaNumber;
    std::string           fCurrentStanzaName;


    // harmonies handling
    // ------------------------------------------------------

    Bool                  fThereAreHarmoniesToBeAttachedToCurrentNote;
    int                   fHarmoniesVoicesCounter;

    S_msrVoice            createPartHarmoniesVoiceIfNotYetDone (
                            int              inputLineNumber,
                            const S_msrPart& thePart);


    // figured bass handling
    // ------------------------------------------------------

    Bool                  fThereAreFiguredBassToBeAttachedToCurrentNote;
    int                   fFiguredBassVoicesCounter;

    S_msrVoice            createPartFiguredBassVoiceIfNotYetDone (
                            int              inputLineNumber,
                            const S_msrPart& thePart);
};


}

#endif // ___mxsr2msrSkeletonBuilder___
