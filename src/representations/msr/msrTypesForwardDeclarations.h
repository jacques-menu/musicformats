/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrTypesForwardDeclarations___
#define ___msrTypesForwardDeclarations___

#include "smartpointer.h"


using namespace MusicXML2;

namespace MusicFormats
{

//______________________________________________________________________________
class   msrAccordionRegistration;
typedef SMARTP<msrAccordionRegistration> S_msrAccordionRegistration;

class   msrAfterGraceNotesGroup;
typedef SMARTP<msrAfterGraceNotesGroup> S_msrAfterGraceNotesGroup;

class   msrAfterGraceNotesGroupContents;
typedef SMARTP<msrAfterGraceNotesGroupContents> S_msrAfterGraceNotesGroupContents;

class   msrAppearance;
typedef SMARTP<msrAppearance> S_msrAppearance;

class   msrArpeggiato;
typedef SMARTP<msrArpeggiato> S_msrArpeggiato;

class   msrArticulation;
typedef SMARTP<msrArticulation> S_msrArticulation;

class   msrBarCheck;
typedef SMARTP<msrBarCheck> S_msrBarCheck;

class   msrBassFigure;
typedef SMARTP<msrBassFigure> S_msrBassFigure;

class   msrBarLine;
typedef SMARTP<msrBarLine> S_msrBarLine;

class   msrBarNumberCheck;
typedef SMARTP<msrBarNumberCheck> S_msrBarNumberCheck;

class   msrBeam;
typedef SMARTP<msrBeam> S_msrBeam;

class   msrBeatRepeat;
typedef SMARTP<msrBeatRepeat> S_msrBeatRepeat;

class   msrBook;
typedef SMARTP<msrBook> S_msrBook;

class   msrBookElement;
typedef SMARTP<msrBookElement> S_msrBookElement;

class   msrChord;
typedef SMARTP<msrChord> S_msrChord;

class   msrChordBeamLink;
typedef SMARTP<msrChordBeamLink> S_msrChordBeamLink;

class   msrChordGraceNotesGroupLink;
typedef SMARTP<msrChordGraceNotesGroupLink> S_msrChordGraceNotesGroupLink;

class   msrChordSlurLink;
typedef SMARTP<msrChordSlurLink> S_msrChordSlurLink;

class   msrClef;
typedef SMARTP<msrClef> S_msrClef;

class   msrCoda;
typedef SMARTP<msrCoda> S_msrCoda;

class   msrColorAlphaRGB;
typedef SMARTP<msrColorAlphaRGB> S_msrColorAlphaRGB;

class   msrColorRGB;
typedef SMARTP<msrColorRGB> S_msrColorRGB;

class   msrCredit;
typedef SMARTP<msrCredit> S_msrCredit;

class   msrCreditWords;
typedef SMARTP<msrCreditWords> S_msrCreditWords;

class   msrCrescDecresc;
typedef SMARTP<msrCrescDecresc> S_msrCrescDecresc;

class   msrDalSegno;
typedef SMARTP<msrDalSegno> S_msrDalSegno;

class   msrDamp;
typedef SMARTP<msrDamp> S_msrDamp;

class   msrDamp;
typedef SMARTP<msrDamp> S_msrDamp;

class   msrDampAll;
typedef SMARTP<msrDampAll> S_msrDampAll;

class   msrDistance;
typedef SMARTP<msrDistance> S_msrDistance;

class   msrDoubleTremolo;
typedef SMARTP<msrDoubleTremolo> S_msrDoubleTremolo;

class   msrDynamic;
typedef SMARTP<msrDynamic> S_msrDynamic;

class   msrDottedNotesDuration;
typedef SMARTP<msrDottedNotesDuration> S_msrDottedNotesDuration;

class   msrEyeGlasses;
typedef SMARTP<msrEyeGlasses> S_msrEyeGlasses;

class   msrFermata;
typedef SMARTP<msrFermata> S_msrFermata;

class   msrFrameNote;
typedef SMARTP<msrFrameNote> S_msrFrameNote;

class   msrEyeGlasses;
typedef SMARTP<msrEyeGlasses> S_msrEyeGlasses;

class   msrFiguredBass;
typedef SMARTP<msrFiguredBass> S_msrFiguredBass;

class   msrFrame;
typedef SMARTP<msrFrame> S_msrFrame;

class   msrGlyph;
typedef SMARTP<msrGlyph> S_msrGlyph;

class   msrGlissando;
typedef SMARTP<msrGlissando> S_msrGlissando;

class   msrGraceNotesGroup;
typedef SMARTP<msrGraceNotesGroup> S_msrGraceNotesGroup;

class   msrHarmony;
typedef SMARTP<msrHarmony> S_msrHarmony;

class   msrHarmonyDegree;
typedef SMARTP<msrHarmonyDegree> S_msrHarmonyDegree;

class   msrHarpPedalsTuning;
typedef SMARTP<msrHarpPedalsTuning> S_msrHarpPedalsTuning;

class   msrIdentification;
typedef SMARTP<msrIdentification> S_msrIdentification;

class   msrKey;
typedef SMARTP<msrKey> S_msrKey;

class   msrKey;
typedef SMARTP<msrKey> S_msrKey;

class   msrLigature;
typedef SMARTP<msrLigature> S_msrLigature;

class   msrLineBreak;
typedef SMARTP<msrLineBreak> S_msrLineBreak;

class   msrLineWidth;
typedef SMARTP<msrLineWidth> S_msrLineWidth;

class   msrMarginsGroup;
typedef SMARTP<msrMarginsGroup> S_msrMarginsGroup;

class   msrMeasure;
typedef SMARTP<msrMeasure> S_msrMeasure;

class   msrMeasureElement;
typedef SMARTP<msrMeasureElement> S_msrMeasureElement;

class   msrMeasureLayout;
typedef SMARTP<msrMeasureLayout> S_msrMeasureLayout;

class   msrMeasureRepeat;
typedef SMARTP<msrMeasureRepeat> S_msrMeasureRepeat;

class   msrMeasureRepeat;
typedef SMARTP<msrMeasureRepeat> S_msrMeasureRepeat;

class   msrMeasureRepeatPattern;
typedef SMARTP<msrMeasureRepeatPattern> S_msrMeasureRepeatPattern;

class   msrMeasureRepeatReplicas;
typedef SMARTP<msrMeasureRepeatReplicas> S_msrMeasureRepeatReplicas;

class   msrMeasuresSlicesSequence;
typedef SMARTP<msrMeasuresSlicesSequence> S_msrMeasuresSlicesSequence;

class   msrMultiMeasureRest;
typedef SMARTP<msrMultiMeasureRest> S_msrMultiMeasureRest;

class   msrNonArpeggiato;
typedef SMARTP<msrNonArpeggiato> S_msrNonArpeggiato;

class   msrNote;
typedef SMARTP<msrNote> S_msrNote;

class   msrNotesDuration;
typedef SMARTP<msrNotesDuration> S_msrNotesDuration;

class   msrNoteSize;
typedef SMARTP<msrNoteSize> S_msrNoteSize;

class   msrOctaveShift;
typedef SMARTP<msrOctaveShift> S_msrOctaveShift;

class   msrOrnament;
typedef SMARTP<msrOrnament> S_msrOrnament;

class   msrOrnament;
typedef SMARTP<msrOrnament> S_msrOrnament;

class   msrOtherAppearance;
typedef SMARTP<msrOtherAppearance> S_msrOtherAppearance;

class   msrOtherDynamic;
typedef SMARTP<msrOtherDynamic> S_msrOtherDynamic;

class   msrPageBreak;
typedef SMARTP<msrPageBreak> S_msrPageBreak;

class   msrPageLayout;
typedef SMARTP<msrPageLayout> S_msrPageLayout;

class   msrPart;
typedef SMARTP<msrPart> S_msrPart;

class   msrPartGroup;
typedef SMARTP<msrPartGroup> S_msrPartGroup;

class   msrPathToVoice;
typedef SMARTP<msrPathToVoice> S_msrPathToVoice;

class   msrPedal;
typedef SMARTP<msrPedal> S_msrPedal;

class   msrMusicXMLPrintLayout;
typedef SMARTP<msrMusicXMLPrintLayout> S_msrMusicXMLPrintLayout;

class   msrRehearsalMark;
typedef SMARTP<msrRehearsalMark> S_msrRehearsalMark;

class   msrRepeat;
typedef SMARTP<msrRepeat> S_msrRepeat;

class   msrRepeatCommonPart;
typedef SMARTP<msrRepeatCommonPart> S_msrRepeatCommonPart;

class   msrRepeatEnding;
typedef SMARTP<msrRepeatEnding> S_msrRepeatEnding;

class   msrScaling;
typedef SMARTP<msrScaling> S_msrScaling;

class   msrScordatura;
typedef SMARTP<msrScordatura> S_msrScordatura;

class   msrScore;
typedef SMARTP<msrScore> S_msrScore;

class   msrSegment;
typedef SMARTP<msrSegment> S_msrSegment;

class   msrSegno;
typedef SMARTP<msrSegno> S_msrSegno;

class   msrSegno;
typedef SMARTP<msrSegno> S_msrSegno;

class   msrSemiTonesPitchAndOctave;
typedef SMARTP<msrSemiTonesPitchAndOctave> S_msrSemiTonesPitchAndOctave;

class   msrSingleTremolo;
typedef SMARTP<msrSingleTremolo> S_msrSingleTremolo;

class   msrSlash;
typedef SMARTP<msrSlash> S_msrSlash;

class   msrSlide;
typedef SMARTP<msrSlide> S_msrSlide;

class   msrSlur;
typedef SMARTP<msrSlur> S_msrSlur;

class   msrSpanner;
typedef SMARTP<msrSpanner> S_msrSpanner;

class   msrStaff;
typedef SMARTP<msrStaff> S_msrStaff;

class   msrStaffDetails;
typedef SMARTP<msrStaffDetails> S_msrStaffDetails;

class   msrStanza;
typedef SMARTP<msrStanza> S_msrStanza;

class   msrStem;
typedef SMARTP<msrStem> S_msrStem;

class   msrSyllable;
typedef SMARTP<msrSyllable> S_msrSyllable;

class   msrTechnical;
typedef SMARTP<msrTechnical> S_msrTechnical;

class   msrTechnical;
typedef SMARTP<msrTechnical> S_msrTechnical;

class   msrTechnicalWithFloat;
typedef SMARTP<msrTechnicalWithFloat> S_msrTechnicalWithFloat;

class   msrTechnicalWithInteger;
typedef SMARTP<msrTechnicalWithInteger> S_msrTechnicalWithInteger;

class   msrTechnicalWithString;
typedef SMARTP<msrTechnicalWithString> S_msrTechnicalWithString;

class   msrTempo;
typedef SMARTP<msrTempo> S_msrTempo;

class   msrTie;
typedef SMARTP<msrTie> S_msrTie;

class   msrStaff;
typedef SMARTP<msrStaff> S_msrStaff;

class   msrStaffLayout;
typedef SMARTP<msrStaffLayout> S_msrStaffLayout;

class   msrStaffTuning;
typedef SMARTP<msrStaffTuning> S_msrStaffTuning;

class   msrSystemLayout;
typedef SMARTP<msrSystemLayout> S_msrSystemLayout;

class   msrTempoNote;
typedef SMARTP<msrTempoNote> S_msrTempoNote;

class   msrTempoNotesRelationshipElements;
typedef SMARTP<msrTempoNotesRelationshipElements> S_msrTempoNotesRelationshipElements;

class   msrTempoTuplet;
typedef SMARTP<msrTempoTuplet> S_msrTempoTuplet;

class   msrTimeSignature;
typedef SMARTP<msrTimeSignature> S_msrTimeSignature;

class   msrTimeSignatureItem;
typedef SMARTP<msrTimeSignatureItem> S_msrTimeSignatureItem;

class   msrTransposition;
typedef SMARTP<msrTransposition> S_msrTransposition;

class   msrTuplet;
typedef SMARTP<msrTuplet> S_msrTuplet;

class   msrTupletElement;
typedef SMARTP<msrTupletElement> S_msrTupletElement;

class   msrVoice;
typedef SMARTP<msrVoice> S_msrVoice;

class   msrVoiceElement;
typedef SMARTP<msrVoiceElement> S_msrVoiceElement;

class   msrVoiceStaffChange;
typedef SMARTP<msrVoiceStaffChange> S_msrVoiceStaffChange;

class   msrWedge;
typedef SMARTP<msrWedge> S_msrWedge;

class   msrWords;
typedef SMARTP<msrWords> S_msrWords;


}


#endif // ___msrTypesForwardDeclarations___
