\version "2.24.0"

% Generated by xml2ly v0.9.70 (July 18, 2023)
% on Monday 2023-07-24 @ 15:29:32 CEST
% from "TypeLessExtend.xml"

% The conversion date was: Monday 2023-07-24 @ 15:29:32 CEST

% The conversion command as supplied was:
%  xml2ly TypeLessExtend.xml -lilypond-lyrics-durations-style implicit -lilypond-comments -o TypeLessExtend_implicit.ly -lilypond-comments -lilypond-generation-infos -trace-lyrics -dlpsr -input-line-numbers -trace-whole-notes-durations
% or, with long option names:
%  xml2ly TypeLessExtend.xml -lilypond-lyrics-durations-style implicit -lilypond-comments -output-file-name TypeLessExtend_implicit.ly -lilypond-comments -lilypond-generation-infos -trace-lyrics -display-lpsr -input-line-numbers -trace-whole-notes-durations
% or, with short option names:
%   TypeLessExtend.xml implicit  TypeLessExtend_implicit.ly

\header {
  movementTitle       = "Type Less Extend"
  encodingDate        = "2023-06-01"
  software            = "MuseScore 4.0.2"
  title               = "Type Less Extend"
} % header

\paper {
  % horizontal-shift = 0.0\mm
  % indent = 0.0\mm
  % short-indent = 0.0\mm

  % markup-system-spacing-padding = 0.0\mm
  % between-system-space = 0.0\mm
  % page-top-space = 0.0\mm

  % page-count = -1
  % system-count = -1

  oddHeaderMarkup = ""
  evenHeaderMarkup = ""
  oddFooterMarkup = ""
  evenFooterMarkup = ""
} % paper

\layout {
  \context {
    \Score
    autoBeaming = ##f % to display tuplets brackets
  } % end of score context
  \context {
    \Voice
  } % end of voice context
} % layout

Part_POne_Staff_One_Voice_One = \absolute {
  \language "nederlands"
  % start of segment 1, line 59
  % start of measure 1 (kMeasureKindRegular), line 32

  \clef "treble" %{ 45 %}
  \key c \major %{ 37 %}
  \numericTimeSignature \time 2/4 %{ 41 %}
  c'8  %{ line 64 %} r8 %{ line 70 %}  %{ line 70 %} e'4  %{ line 93 %}  ~ | % 2 %{ line 0 %}
  \barNumberCheck #2 %{ line 0 %}
  % end of measure 1 (kMeasureKindRegular), line 0

  % start of measure 2 (kMeasureKindRegular), line 95
  e'8  %{ line 109 %} g'4  %{ line 123 %} g'8  %{ line 141 %} | % 3 %{ line 0 %}
  \barNumberCheck #3 %{ line 0 %}
  % end of measure 2 (kMeasureKindRegular), line 0

  % start of measure 3 (kMeasureKindRegular), line 143
  c'4  %{ line 161 %} r4 %{ line 167 %}  %{ line 167 %}
  \bar "|."  %{ line 170 %}
   %{ line 170 %}| % 4 %{ line 0 %}
  \barNumberCheck #4 %{ line 0 %}
  % end of measure 3 (kMeasureKindRegular), line 0

  % end of segment 1, line 0
} % end of voice Part_P1_Staff_1_Voice_1

Part_POne_Staff_One_Voice_One_Stanza_One = \lyricmode {
  \set associatedVoice = #"Part_POne_Staff_One_Voice_One"
  \set ignoreMelismata = ##t

    %{ ================= SYLLABLE DESCRIPTION =================
      "Bon"
      getSyllableKind              : kSyllableSingle
      wholeNotes                   : 8
      getLyricsNotesDurationsKind  : implicit
      getSyllableExtendKind        : kSyllableExtend_NONE
      fOnGoingExtend               : false
      noteTheSyllableIsAttachedTo  : c8, measure '1'
      noteTieKind                  : [NULL]
      noteSlurs                    : [NONE]
      line                         : 59
      ========================================================
    %}
    %{ CODE_FOR_SYLLABLE_DURATION_KIND_IMPLICIT_kSyllableSingle, line 59 %}
    "Bon"
    %{ IMPLICIT_DURATIONS_kSyllableSingle, line 59 %}
    %{ =======>
      doGenerateASingleUnderscore : false
      doGenerateADoubleUnderscore : false
      doGenerateASingleHyphen     : false
      doGenerateADoubleHyphen     : false
      doGenerateASkip             : false
    %}


    %{ ================= SYLLABLE DESCRIPTION =================
      ""
      getSyllableKind              : kSyllableSkipRestNote
      wholeNotes                   : 8
      getLyricsNotesDurationsKind  : implicit
      getSyllableExtendKind        : kSyllableExtend_NONE
      fOnGoingExtend               : false
      noteTheSyllableIsAttachedTo  : [NULL]
      line                         : 87
      ========================================================
    %}
    %{ CODE_FOR_SYLLABLE_DURATION_KIND_IMPLICIT_kSyllableSkipRestNote, line 87 %}

    %{ IMPLICIT_DURATIONS_kSyllableSkipRestNote, line 87 %}
    %{ =======>
      doGenerateASingleUnderscore : false
      doGenerateADoubleUnderscore : false
      doGenerateASingleHyphen     : false
      doGenerateADoubleHyphen     : false
      doGenerateASkip             : false
    %}


    %{ ================= SYLLABLE DESCRIPTION =================
      "ma"
      getSyllableKind              : kSyllableBegin
      wholeNotes                   : 4
      getLyricsNotesDurationsKind  : implicit
      getSyllableExtendKind        : kSyllableExtend_NONE
      fOnGoingExtend               : false
      noteTheSyllableIsAttachedTo  : e4, measure '1'
      noteTieKind                  : kTieStart
      noteSlurs                    : [NONE]
      line                         : 87
      ========================================================
    %}
    %{ CODE_FOR_SYLLABLE_DURATION_KIND_IMPLICIT_kSyllableBegin, line 87 %}
    "ma"
    %{ IMPLICIT_DURATIONS_kSyllableBegin, line 87 %}
    %{ =======>
      doGenerateASingleUnderscore : false
      doGenerateADoubleUnderscore : false
      doGenerateASingleHyphen     : false
      doGenerateADoubleHyphen     : true
      doGenerateASkip             : true
    %}
     --  \skip1

    %{ ================= SYLLABLE DESCRIPTION =================
      ""
      getSyllableKind              : kSyllableMeasureEnd
      wholeNotes                   : zero
      getLyricsNotesDurationsKind  : implicit
      getSyllableExtendKind        : kSyllableExtend_NONE
      fOnGoingExtend               : false
      noteTheSyllableIsAttachedTo  : [NULL]
      line                         : 94
      ========================================================
    %}
    | %{ line 94 %}
    %{ =======>
      doGenerateASingleUnderscore : false
      doGenerateADoubleUnderscore : false
      doGenerateASingleHyphen     : false
      doGenerateADoubleHyphen     : false
      doGenerateASkip             : false
    %}


    %{ ================= SYLLABLE DESCRIPTION =================
      ""
      getSyllableKind              : kSyllableSkipRestNote
      wholeNotes                   : 8
      getLyricsNotesDurationsKind  : implicit
      getSyllableExtendKind        : kSyllableExtend_NONE
      fOnGoingExtend               : false
      noteTheSyllableIsAttachedTo  : [NULL]
      line                         : 122
      ========================================================
    %}
    %{ CODE_FOR_SYLLABLE_DURATION_KIND_IMPLICIT_kSyllableSkipRestNote, line 122 %}

    %{ IMPLICIT_DURATIONS_kSyllableSkipRestNote, line 122 %}
    %{ =======>
      doGenerateASingleUnderscore : false
      doGenerateADoubleUnderscore : false
      doGenerateASingleHyphen     : false
      doGenerateADoubleHyphen     : false
      doGenerateASkip             : false
    %}


    %{ ================= SYLLABLE DESCRIPTION =================
      "tin!"
      getSyllableKind              : kSyllableEnd
      wholeNotes                   : 4
      getLyricsNotesDurationsKind  : implicit
      getSyllableExtendKind        : kSyllableExtend_NONE
      fOnGoingExtend               : false
      noteTheSyllableIsAttachedTo  : g4, measure '2'
      noteTieKind                  : [NULL]
      noteSlurs                    : [NONE]
      line                         : 122
      ========================================================
    %}
    "tin!"
    %{ IMPLICIT_DURATIONS_kSyllableEnd, line 122 %}
    %{ =======>
      doGenerateASingleUnderscore : false
      doGenerateADoubleUnderscore : false
      doGenerateASingleHyphen     : false
      doGenerateADoubleHyphen     : false
      doGenerateASkip             : false
    %}


    %{ ================= SYLLABLE DESCRIPTION =================
      "Bon"
      getSyllableKind              : kSyllableSingle
      wholeNotes                   : 8
      getLyricsNotesDurationsKind  : implicit
      getSyllableExtendKind        : kSyllableExtend_NONE
      fOnGoingExtend               : false
      noteTheSyllableIsAttachedTo  : g8, measure '2'
      noteTieKind                  : [NULL]
      noteSlurs                    : [NONE]
      line                         : 136
      ========================================================
    %}
    %{ CODE_FOR_SYLLABLE_DURATION_KIND_IMPLICIT_kSyllableSingle, line 136 %}
    "Bon"
    %{ IMPLICIT_DURATIONS_kSyllableSingle, line 136 %}
    %{ =======>
      doGenerateASingleUnderscore : false
      doGenerateADoubleUnderscore : false
      doGenerateASingleHyphen     : false
      doGenerateADoubleHyphen     : false
      doGenerateASkip             : false
    %}


    %{ ================= SYLLABLE DESCRIPTION =================
      ""
      getSyllableKind              : kSyllableMeasureEnd
      wholeNotes                   : zero
      getLyricsNotesDurationsKind  : implicit
      getSyllableExtendKind        : kSyllableExtend_NONE
      fOnGoingExtend               : false
      noteTheSyllableIsAttachedTo  : [NULL]
      line                         : 142
      ========================================================
    %}
    | %{ line 142 %}
    %{ =======>
      doGenerateASingleUnderscore : false
      doGenerateADoubleUnderscore : false
      doGenerateASingleHyphen     : false
      doGenerateADoubleHyphen     : false
      doGenerateASkip             : false
    %}


    %{ ================= SYLLABLE DESCRIPTION =================
      "jour!"
      getSyllableKind              : kSyllableSingle
      wholeNotes                   : 4
      getLyricsNotesDurationsKind  : implicit
      getSyllableExtendKind        : kSyllableExtend_NONE
      fOnGoingExtend               : false
      noteTheSyllableIsAttachedTo  : c4, measure '3'
      noteTieKind                  : [NULL]
      noteSlurs                    : [NONE]
      line                         : 156
      ========================================================
    %}
    %{ CODE_FOR_SYLLABLE_DURATION_KIND_IMPLICIT_kSyllableSingle, line 156 %}
    "jour!"
    %{ IMPLICIT_DURATIONS_kSyllableSingle, line 156 %}
    %{ =======>
      doGenerateASingleUnderscore : false
      doGenerateADoubleUnderscore : false
      doGenerateASingleHyphen     : false
      doGenerateADoubleHyphen     : false
      doGenerateASkip             : false
    %}


    %{ ================= SYLLABLE DESCRIPTION =================
      ""
      getSyllableKind              : kSyllableSkipRestNote
      wholeNotes                   : 4
      getLyricsNotesDurationsKind  : implicit
      getSyllableExtendKind        : kSyllableExtend_NONE
      fOnGoingExtend               : false
      noteTheSyllableIsAttachedTo  : [NULL]
      line                         : 171
      ========================================================
    %}
    %{ CODE_FOR_SYLLABLE_DURATION_KIND_IMPLICIT_kSyllableSkipRestNote, line 171 %}

    %{ IMPLICIT_DURATIONS_kSyllableSkipRestNote, line 171 %}
    %{ =======>
      doGenerateASingleUnderscore : false
      doGenerateADoubleUnderscore : false
      doGenerateASingleHyphen     : false
      doGenerateADoubleHyphen     : false
      doGenerateASkip             : false
    %}


    %{ ================= SYLLABLE DESCRIPTION =================
      ""
      getSyllableKind              : kSyllableMeasureEnd
      wholeNotes                   : zero
      getLyricsNotesDurationsKind  : implicit
      getSyllableExtendKind        : kSyllableExtend_NONE
      fOnGoingExtend               : false
      noteTheSyllableIsAttachedTo  : [NULL]
      line                         : 171
      ========================================================
    %}
    | %{ line 171 %}
    %{ =======>
      doGenerateASingleUnderscore : false
      doGenerateADoubleUnderscore : false
      doGenerateASingleHyphen     : false
      doGenerateADoubleHyphen     : false
      doGenerateASkip             : false
    %}


}

Part_POne_Staff_One_Voice_One_Stanza_Two = \lyricmode {
  \set associatedVoice = #"Part_POne_Staff_One_Voice_One"
  \set ignoreMelismata = ##t

    %{ ================= SYLLABLE DESCRIPTION =================
      "Bon"
      getSyllableKind              : kSyllableSingle
      wholeNotes                   : 8
      getLyricsNotesDurationsKind  : implicit
      getSyllableExtendKind        : kSyllableExtend_NONE
      fOnGoingExtend               : false
      noteTheSyllableIsAttachedTo  : c8, measure '1'
      noteTieKind                  : [NULL]
      noteSlurs                    : [NONE]
      line                         : 63
      ========================================================
    %}
    %{ CODE_FOR_SYLLABLE_DURATION_KIND_IMPLICIT_kSyllableSingle, line 63 %}
    "Bon"
    %{ IMPLICIT_DURATIONS_kSyllableSingle, line 63 %}
    %{ =======>
      doGenerateASingleUnderscore : false
      doGenerateADoubleUnderscore : false
      doGenerateASingleHyphen     : false
      doGenerateADoubleHyphen     : false
      doGenerateASkip             : false
    %}


    %{ ================= SYLLABLE DESCRIPTION =================
      ""
      getSyllableKind              : kSyllableSkipRestNote
      wholeNotes                   : 8
      getLyricsNotesDurationsKind  : implicit
      getSyllableExtendKind        : kSyllableExtend_NONE
      fOnGoingExtend               : false
      noteTheSyllableIsAttachedTo  : [NULL]
      line                         : 92
      ========================================================
    %}
    %{ CODE_FOR_SYLLABLE_DURATION_KIND_IMPLICIT_kSyllableSkipRestNote, line 92 %}

    %{ IMPLICIT_DURATIONS_kSyllableSkipRestNote, line 92 %}
    %{ =======>
      doGenerateASingleUnderscore : false
      doGenerateADoubleUnderscore : false
      doGenerateASingleHyphen     : false
      doGenerateADoubleHyphen     : false
      doGenerateASkip             : false
    %}


    %{ ================= SYLLABLE DESCRIPTION =================
      "soir!"
      getSyllableKind              : kSyllableSingle
      wholeNotes                   : 4
      getLyricsNotesDurationsKind  : implicit
      getSyllableExtendKind        : kSyllableExtendTypeLess
      fOnGoingExtend               : false
      noteTheSyllableIsAttachedTo  : e4, measure '1'
      noteTieKind                  : kTieStart
      noteSlurs                    : [NONE]
      line                         : 92
      ========================================================
    %}
    %{ CODE_FOR_SYLLABLE_DURATION_KIND_IMPLICIT_kSyllableSingle, line 92 %}
    "soir!"
    %{ IMPLICIT_DURATIONS_kSyllableSingle, line 92 %}
    %{ =======>
      doGenerateASingleUnderscore : true
      doGenerateADoubleUnderscore : true
      doGenerateASingleHyphen     : false
      doGenerateADoubleHyphen     : false
      doGenerateASkip             : true
    %}
     __  _  \skip1

    %{ ================= SYLLABLE DESCRIPTION =================
      ""
      getSyllableKind              : kSyllableMeasureEnd
      wholeNotes                   : zero
      getLyricsNotesDurationsKind  : implicit
      getSyllableExtendKind        : kSyllableExtend_NONE
      fOnGoingExtend               : false
      noteTheSyllableIsAttachedTo  : [NULL]
      line                         : 94
      ========================================================
    %}
    | %{ line 94 %}
    %{ =======>
      doGenerateASingleUnderscore : false
      doGenerateADoubleUnderscore : false
      doGenerateASingleHyphen     : false
      doGenerateADoubleHyphen     : false
      doGenerateASkip             : false
    %}


    %{ ================= SYLLABLE DESCRIPTION =================
      ""
      getSyllableKind              : kSyllableSkipRestNote
      wholeNotes                   : 4.
      getLyricsNotesDurationsKind  : implicit
      getSyllableExtendKind        : kSyllableExtend_NONE
      fOnGoingExtend               : false
      noteTheSyllableIsAttachedTo  : [NULL]
      line                         : 140
      ========================================================
    %}
    %{ CODE_FOR_SYLLABLE_DURATION_KIND_IMPLICIT_kSyllableSkipRestNote, line 140 %}

    %{ IMPLICIT_DURATIONS_kSyllableSkipRestNote, line 140 %}
    %{ =======>
      doGenerateASingleUnderscore : false
      doGenerateADoubleUnderscore : false
      doGenerateASingleHyphen     : false
      doGenerateADoubleHyphen     : false
      doGenerateASkip             : false
    %}


    %{ ================= SYLLABLE DESCRIPTION =================
      "Bonne"
      getSyllableKind              : kSyllableSingle
      wholeNotes                   : 8
      getLyricsNotesDurationsKind  : implicit
      getSyllableExtendKind        : kSyllableExtend_NONE
      fOnGoingExtend               : false
      noteTheSyllableIsAttachedTo  : g8, measure '2'
      noteTieKind                  : [NULL]
      noteSlurs                    : [NONE]
      line                         : 140
      ========================================================
    %}
    %{ CODE_FOR_SYLLABLE_DURATION_KIND_IMPLICIT_kSyllableSingle, line 140 %}
    "Bonne"
    %{ IMPLICIT_DURATIONS_kSyllableSingle, line 140 %}
    %{ =======>
      doGenerateASingleUnderscore : false
      doGenerateADoubleUnderscore : false
      doGenerateASingleHyphen     : false
      doGenerateADoubleHyphen     : false
      doGenerateASkip             : false
    %}


    %{ ================= SYLLABLE DESCRIPTION =================
      ""
      getSyllableKind              : kSyllableMeasureEnd
      wholeNotes                   : zero
      getLyricsNotesDurationsKind  : implicit
      getSyllableExtendKind        : kSyllableExtend_NONE
      fOnGoingExtend               : false
      noteTheSyllableIsAttachedTo  : [NULL]
      line                         : 142
      ========================================================
    %}
    | %{ line 142 %}
    %{ =======>
      doGenerateASingleUnderscore : false
      doGenerateADoubleUnderscore : false
      doGenerateASingleHyphen     : false
      doGenerateADoubleHyphen     : false
      doGenerateASkip             : false
    %}


    %{ ================= SYLLABLE DESCRIPTION =================
      "nuit!"
      getSyllableKind              : kSyllableSingle
      wholeNotes                   : 4
      getLyricsNotesDurationsKind  : implicit
      getSyllableExtendKind        : kSyllableExtend_NONE
      fOnGoingExtend               : false
      noteTheSyllableIsAttachedTo  : c4, measure '3'
      noteTieKind                  : [NULL]
      noteSlurs                    : [NONE]
      line                         : 160
      ========================================================
    %}
    %{ CODE_FOR_SYLLABLE_DURATION_KIND_IMPLICIT_kSyllableSingle, line 160 %}
    "nuit!"
    %{ IMPLICIT_DURATIONS_kSyllableSingle, line 160 %}
    %{ =======>
      doGenerateASingleUnderscore : false
      doGenerateADoubleUnderscore : false
      doGenerateASingleHyphen     : false
      doGenerateADoubleHyphen     : false
      doGenerateASkip             : false
    %}


    %{ ================= SYLLABLE DESCRIPTION =================
      ""
      getSyllableKind              : kSyllableSkipRestNote
      wholeNotes                   : 4
      getLyricsNotesDurationsKind  : implicit
      getSyllableExtendKind        : kSyllableExtend_NONE
      fOnGoingExtend               : false
      noteTheSyllableIsAttachedTo  : [NULL]
      line                         : 171
      ========================================================
    %}
    %{ CODE_FOR_SYLLABLE_DURATION_KIND_IMPLICIT_kSyllableSkipRestNote, line 171 %}

    %{ IMPLICIT_DURATIONS_kSyllableSkipRestNote, line 171 %}
    %{ =======>
      doGenerateASingleUnderscore : false
      doGenerateADoubleUnderscore : false
      doGenerateASingleHyphen     : false
      doGenerateADoubleHyphen     : false
      doGenerateASkip             : false
    %}


    %{ ================= SYLLABLE DESCRIPTION =================
      ""
      getSyllableKind              : kSyllableMeasureEnd
      wholeNotes                   : zero
      getLyricsNotesDurationsKind  : implicit
      getSyllableExtendKind        : kSyllableExtend_NONE
      fOnGoingExtend               : false
      noteTheSyllableIsAttachedTo  : [NULL]
      line                         : 171
      ========================================================
    %}
    | %{ line 171 %}
    %{ =======>
      doGenerateASingleUnderscore : false
      doGenerateADoubleUnderscore : false
      doGenerateASingleHyphen     : false
      doGenerateADoubleHyphen     : false
      doGenerateASkip             : false
    %}


}

\book {

  \score {
    << % start of parallel music
      % start of implicit part group block
      \new Staff = "Part_POne_Staff_One"
      \with {
        instrumentName = "Part_POne"
      }
      << % start of staff block for "Part_POne_Staff_One"
        \context Voice = "Part_POne_Staff_One_Voice_One" <<
          \Part_POne_Staff_One_Voice_One
        >> % end of voice command for voice Part_P1_Staff_1_Voice_1
        \new Lyrics
          \with {
          }
          \lyricsto "Part_POne_Staff_One_Voice_One" {\Part_POne_Staff_One_Voice_One_Stanza_One}
        \new Lyrics
          \with {
          }
          \lyricsto "Part_POne_Staff_One_Voice_One" {\Part_POne_Staff_One_Voice_One_Stanza_Two}
      >> % end of staff block for "Part_POne_Staff_One", fStaffBlocksCounter: 1, fNumberOfStaffBlocksElements: 1
       % end of implicit part group block PartGroup_0 ('-1', partGroupName "***IMPLICIT OUTER-MOST PART GROUP***", fPartGroupImplicitKind: kPartGroupImplicitOuterMostYes % end of explicit part group block PartGroup_0 ('-1', partGroupName "***IMPLICIT OUTER-MOST PART GROUP***", fPartGroupImplicitKind: kPartGroupImplicitOuterMostYes
    >> % end of parallel music

    \layout {
      \context {
        \Score
        autoBeaming = ##f % to display tuplets brackets
      } % end of score context
      \context {
        \Voice
      } % end of voice context
    } % layout

    \midi {
      \tempo 16= 360
    }
  } % end of score

} % end of book
