/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef __xmlpart2guido__
#define __xmlpart2guido__

#include <ostream>
#include <stack>
#include <queue>
#include <map>
#include <string>

#include "clefvisitor.h"
#include "exports.h"
#include "guido.h"
#include "keysignvisitor.h"
#include "metronomevisitor.h"
#include "notevisitor.h"
#include "rational.h"
#include "timesignvisitor.h"
#include "typedefs.h"
#include "visitor.h"
#include "xml.h"

namespace MusicXML2 
{

/*!
\ingroup visitors guido
@{
*/

/*!
\brief A score visitor to produce a Guido representation.
*/
//______________________________________________________________________________
class EXP xmlpart2guido : 
	public clefvisitor,
	public timesignvisitor,
	public notevisitor,
	public keysignvisitor,
	public visitor<S_backup>,
	public visitor<S_barline>,
	public visitor<S_coda>,
	public visitor<S_direction>,
	public visitor<S_divisions>,
	public visitor<S_ending>,
	public visitor<S_forward>,
	public visitor<S_measure>,
	public visitor<S_octave_shift>,
	public visitor<S_part>,
	public visitor<S_repeat>,
	public visitor<S_segno>,
	public visitor<S_sound>,
    public visitor<S_wedge>,
	public visitor<S_rehearsal>,     // for rehearsal Markup
    public visitor<S_attributes>         // to get clef, division, staves, time and key in order!
{
	// the guido elements stack
	std::stack<Sguidoelement>	fStack;
	// structure to store delayed elements ie elements enclosed in direction with offset
	typedef struct {
		long delay;
		Sguidoelement element;
	} delayedElement;
	vector<delayedElement>	fDelayed;
	// fields to controls the guido output generation
	bool fGenerateComments, fGenerateBars, fGeneratePositions, fGenerateAutoMeasureNum, fLyricsManualSpacing;

	// internal parsing state
    bool	fInCue, fInGrace, fInhibitNextBar, fPendingBar, fBeamOpened, fBeamGrouping, fMeasureEmpty, fCrescPending,fWavyTrillOpened, fSingleScopeTrill, fNonStandardNoteHead, fDoubleBar, fTremoloInProgress, fShouldStopOctava;
    
    int fTextTagOpen;
    int fTupletOpen;    // Number of opened Tuplets
    std::queue<int> fTiedOpen;      // Number of ongoing opened Tied
        
    std::queue<int> fDirectionEraserStack;        // To skip already visited Directions when looking ahead because of grace notes
    std::vector< std::pair<int, int> > fSlurStack; // first int: Internal num, 2nd int: XML num

    Sguidoelement fLyricOpened;
    
    bool isProcessingChord;
	
	S_measure	fCurrentMeasure;
    S_part      fCurrentPart;

	bool	fNotesOnly;				// a flag to generate notes only (used for several voices on the same staff)
	bool	fSkipDirection;			// a flag to skip direction elements (for notes only mode or due to different staff)
	int		fCurrentStaffIndex;		// the index of the current guido staff
	int		fCurrentStaff;			// the staff we're currently generating events for (0 by default)
	int		fTargetStaff;			// the musicxml target staff (0 by default)
	int		fTargetVoice;			// the musicxml target voice (0 by default)

	long	fCurrentDivision;		// the current measure division, expresses the time unit in division of the quarter note
	long	fCurrentOffset;			// the current direction offset: represents an element relative displacement in current division unit
	rational fCurrentMeasureLength;	// the current measure length (max of the current measure positions)
	rational fCurrentMeasurePosition;// the current position in the measure
	rational fCurrentVoicePosition;	// the current position within a voice
	rational fCurrentTimeSign;		// the current time signature
	int		fMeasNum;

	int		fCurrentStemDirection;	// the current stems direction, used for stem direction changes
	int		fPendingPops;			// elements to be popped at chord exit (like fermata, articulations...)

	void start (Sguidoelement& elt)		{ fStack.push(elt); }
	void add  (Sguidoelement& elt)		{ if (fStack.size()) fStack.top()->add(elt); }
	void addDelayed (Sguidoelement elt, long offset);	// adding elements to the delayed elements
	void checkDelayed (long time);						// checks the delayed elements for ready elements 
	void push (Sguidoelement& elt)		{ add(elt); fStack.push(elt); }
	void pop ()							{ fStack.pop(); }

	void moveMeasureTime (int duration, bool moveVoiceToo=false, int x_default = 0);
	void reset ();
	void stackClean	();

	int  checkArticulation ( const notevisitor& note );			// returns the count of articulations pushed on the stack
    void checkPostArticulation ( const notevisitor& note );      /// Articulations that should be generated in ADD mode after note creation

    int  checkChordOrnaments ( const notevisitor& note );			// returns the count of articulations pushed on the stack
    
    deque<notevisitor>  getChord ( const S_note& note );    // build a chord vector
	std::vector<Sxmlelement>  getChord ( const Sxmlelement& note );	// build a chord vector
	void checkStaff		 (int staff );					// check for staff change
	void checkStem		 ( const S_stem& stem );
	void checkBeamBegin	 ( const std::vector<S_beam>& beams, const S_note& elt );
    void checkBeamEnd	 ( const std::vector<S_beam>& beams );
    void checkTupletBegin( const std::vector<S_tuplet>& tuplets,
                          const notevisitor& nv,
                          const S_note& elt);
    void checkTupletEnd	 ( const std::vector<S_tuplet>& tuplets );
	void checkCue		 ( const notevisitor& nv );
	void checkGrace		 ( const notevisitor& nv );
    void checkGraceEnd(const notevisitor& nv);
	int  checkFermata	 ( const notevisitor& stem );
    void checkSlur     ( const std::vector<S_slur>& slurs );
	void checkSlurBegin	 ( const std::vector<S_slur>& slurs );
	void checkSlurEnd	 ( const std::vector<S_slur>& slurs );
    bool isSlurClosing(S_slur elt);
	void checkTiedBegin	 ( const std::vector<S_tied>& tied );
	void checkTiedEnd	 ( const std::vector<S_tied>& tied );
	void checkVoiceTime	 ( const rational& currTime, const rational& voiceTime);
    int  checkRestFormat	 ( const notevisitor& nv );
    int checkNoteFormatDx	 ( const notevisitor& nv , rational posInMeasure);
    void checkWavyTrillBegin	 ( const notevisitor& nv );
    void checkWavyTrillEnd	 ( const notevisitor& nv );
    void checkTextEnd();
    void checkOctavaEnd();
	void newNote		 ( const notevisitor& nv, rational posInMeasure, const std::vector<Sxmlelement>& fingerings);
    void newChord   (const deque<notevisitor>& nvs, rational posInMeasure);
    
    int checkTremolo(const notevisitor& note, const S_note& elt);
    
	std::string			noteName		( const notevisitor& nv );
	guidonoteduration	noteDuration	( const notevisitor& nv);

	std::vector<S_beam>::const_iterator findValue ( const std::vector<S_beam>& beams, const std::string& val ) const;
	std::vector<S_slur>::const_iterator findTypeValue ( const std::vector<S_slur>& slurs, const std::string& val ) const;
	std::vector<S_tied>::const_iterator findTypeValue ( const std::vector<S_tied>& tied, const std::string& val ) const;
    std::vector< std::pair<int, int> >::const_iterator findSlur ( const int xmlnum ) const;

    /// Lyrics handling by AC
    void checkLyricBegin	 ( const std::vector<S_lyric>& lyrics );
    void checkLyricEnd	 ( const std::vector<S_lyric>& lyrics );
    std::vector<S_lyric>::const_iterator findValue ( const std::vector<S_lyric>& lyrics,
                                                    const std::string& val ) const;
    std::string lyricParams;

	
	static std::string alter2accident ( float alter );

	protected:
		enum { kStemUndefined, kStemUp, kStemDown, kStemNone };
		enum { kLeaveChord=-1, kNoChord, kEnterChord } chordState;

		virtual void visitStart( S_backup& elt);
		virtual void visitStart( S_barline& elt);
		virtual void visitStart( S_coda& elt);
		virtual void visitStart( S_direction& elt);
		virtual void visitStart( S_divisions& elt);
		virtual void visitStart( S_forward& elt);
		virtual void visitStart( S_measure& elt);
		virtual void visitStart( S_note& elt);
		virtual void visitStart( S_octave_shift& elt);
		virtual void visitStart( S_part& elt);
		virtual void visitStart( S_segno& elt);
        virtual void visitStart( S_wedge& elt);
		virtual void visitStart( S_rehearsal& elt);
        virtual void visitStart( S_attributes& elt);

		virtual void visitEnd  ( S_direction& elt);
		virtual void visitEnd  ( S_ending& elt);
		virtual void visitEnd  ( S_key& elt);
		virtual void visitEnd  ( S_measure& elt);
		virtual void visitEnd  ( S_note& elt);
		virtual void visitEnd  ( S_repeat& elt);
		virtual void visitEnd  ( S_sound& elt);
		virtual void visitEnd  ( S_time& elt);
    
    std::string parseMetronome ( metronomevisitor &mv );
    
    bool findNextNote(ctree<xmlelement>::iterator& elt, ctree<xmlelement>::iterator &nextnote);
    float getNoteDistanceFromStaffTop(const notevisitor& nv);
    
    rational durationInCue;
    
    std::map<int, float> fStaffDistance;
        
    bool fIgnoreWedgeWithOffset;
    
    // Internal Parsing facilities
    float xPosFromTimePos(float default_x, float relative_x);           /// Infer X-Position from TimePosition

    public:
				 xmlpart2guido(bool generateComments, bool generateStem, bool generateBar=true);
		virtual ~xmlpart2guido() {}

		Sguidoelement& current ()					{ return fStack.top(); }
		void	initialize (Sguidoelement seq, int staff, int guidostaff, int voice, bool notesonly, rational defaultTimeSign);
		void	generatePositions (bool state)		{ fGeneratePositions = state; }
		const rational& getTimeSign () const		{ return fCurrentTimeSign; }
        bool fHasLyrics;
        bool hasLyrics() const {return fHasLyrics;}
    std::multimap<int,  std::pair< int, std::pair< rational, string > > > staffClefMap;

    std::string getClef(int staffIndex, rational pos, int measureNum);

    /// Containing default-x positions on a fCurrentVoicePosition (rational) of measure(int)
    std::map< int, std::map< rational, std::vector<int> > > timePositions;
    
    void addPosYforNoteHead(const notevisitor& nv, Sxmlelement elt, Sguidoelement& tag, float offset);
    
};


} // namespace MusicXML2


#endif
