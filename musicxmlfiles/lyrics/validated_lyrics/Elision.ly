\version "2.24.4"

% Generated from "Elision.xml"
% by xml2ly v2026.2 (built on June 02, 2026)
% on Tuesday 2026-06-02 @ 06:32:59 CEST

% The conversion command as supplied was: 
%  xml2ly -auto-output-file-name -lilypond-run-date -lilypond-generation-infos Elision.xml -comlilylyrics
% or, with long option names:
%  xml2ly -auto-output-file-name -lilypond-run-date -lilypond-generation-infos Elision.xml -comment-lilypond-lyrics
% or, with short option names:
%      Elision.xml


% Scheme function(s): "date & time"
% A set of functions to obtain the LilyPond file creation or modification time.

#(define commandLine                  (object->string (command-line)))
#(define loc                          (+ (string-rindex commandLine #\space ) 2))
#(define commandLineLength            (- (string-length commandLine) 2))
#(define lilypondFileName             (substring commandLine loc commandLineLength))

#(define lilypondFileDirName          (dirname lilypondFileName))
#(define lilypondFileBaseName         (basename lilypondFileName))
#(define lilypondFileSuffixlessName   (basename lilypondFileBaseName ".ly"))

#(define pdfFileName                  (string-append lilypondFileSuffixlessName ".pdf"))
#(define pdfFileFullName              (string-append lilypondFileDirName file-name-separator-string pdfFileName))

#(define lilypondVersion              (object->string (lilypond-version)))
#(define currentDate                  (strftime "%d/%m/%Y" (localtime (current-time))))
#(define currentTime                  (strftime "%H:%M:%S" (localtime (current-time))))

#(define lilypondFileModificationTime (stat:mtime (stat lilypondFileName)))

#(define lilypondFileModificationTimeAsString (strftime "%A %d/%m/%Y, %H:%M:%S" (localtime lilypondFileModificationTime)))

#(use-modules (srfi srfi-19))
% https://www.gnu.org/software/guile/manual/html_node/SRFI_002d19-Date-to-string.html
%#(define pdfFileCreationTime (date->string (current-date) "~A, ~B ~e ~Y ~H:~M:~S"))
#(define pdfFileCreationTime (date->string (current-date) "~A ~d/~m/~Y, ~H:~M:~S"))


\header {
  movementTitle       = "Elision"
  encodingDate        = "2026-05-22"
  miscellaneousField  = "Apple Macintosh"
  software            = "MuseScore Studio 4.6.5"
  title               = "Elision"
}

\paper {
  % horizontal-shift = 0.0\mm
  % indent = 0.0\mm
  % short-indent = 0.0\mm
  
  % markup-system-spacing-padding = 0.0\mm
  % between-system-space = 0.0\mm
  % page-top-space = 0.0\mm
  
  % page-count = -1
  % system-count = -1
  
  oddHeaderMarkup = \markup {
    \fill-line {
      \unless \on-first-page {
        \fromproperty #'page:page-number-std::string
        ' '
        \fromproperty #'header:title
        ' '
        \fromproperty #'header:subtitle
      }
    }
  }

  evenHeaderMarkup = \markup {
    \fill-line {
      \unless \on-first-page {
        \fromproperty #'page:page-number-std::string
        ' '
        \fromproperty #'header:title
        ' '
        \fromproperty #'header:subtitle
      }
    }
  }

  oddFooterMarkup = \markup {
    \tiny
    \column {
      \fill-line {
        #(string-append
"Score generated from MusicXML data by xml2ly v2026.2 (built on June 02, 2026) and LilyPond " (lilypond-version))
      }
      \fill-line { \column { \italic { \concat { \lilypondFileName " was modified on " \lilypondFileModificationTimeAsString } } } }
      \fill-line { \column { \italic { \concat { \pdfFileName " was created on " \pdfFileCreationTime } } } }
     \fill-line { \column { \italic { \concat { "lilypondFileDirName: " \lilypondFileDirName } } } }
     \fill-line { \column { \italic { \concat { "pdfFileFullName: " \pdfFileFullName } } } }
%      \fill-line { \column { \italic { \concat { "lilypondFileBaseName: " \lilypondFileBaseName } } } }
%      \fill-line { \column { \italic { \concat { "lilypondFileSuffixlessName: " \lilypondFileSuffixlessName } } } }
%      \fill-line { \column { \italic { \concat { "pdfFileName: " \pdfFileName } } } }
    }
  }

  % evenFooterMarkup = ""
}

\layout {
  \context { \Score
    autoBeaming = ##f % to display tuplets brackets
  }
  \context { \Voice
  }
}

Part_POne_Staff_One_Voice_One = \absolute {
  \language "nederlands"
  
  \clef "treble"
  \key bes \major
  \time 4/4
  \stemDown bes'4 bes'8. c''16 \stemUp bes'8 [
  ( a' ]
  ) g'8. [
  ( a'16 ]
  ) \stemDown bes'4. ( c''8 ) d''4 r
  \bar "|."
}

Part_POne_Staff_One_Voice_One_Stanza_One = \lyricmode {
  % ===> using explicit lyrics durations mode
  \set associatedVoice  = #"Part_POne_Staff_One_Voice_One#"
  \set ignoreMelismata = ##t
    
    
    
    %{ ================= SYLLABLE DESCRIPTION =================
    getSyllableElementsList      : Il
    getSyllableKind              : kSyllableSingle
    getSyllableWholeNotes        : 1/4 whn
    getSyllableExtendKind        : kSyllableExtend_NONE
    
    getLyricsDurationsKind       : explicit
    fOnGoingExtend               : false
    
    noteTheSyllableIsAttachedTo  : [NULL]
    getStanzaNumber              : stanza_1
    getStanzaName                : Part_POne_Staff_One_Voice_1_Stanza_1
    
    getInputLineNumber           : line 128
    ========================================================
    %}
    
    %{ CODE_FOR_SYLLABLE_DURATION_KIND_EXPLICIT_kSyllableSingle, line 128 %}
    Il4
    %{ CODE_FOR_EXTENDER_EXPLICIT_DURATIONS_kSyllableSingle, line 128 %}
    %{ =======>
    [Syllable, fSyllableElementsList: ["Il"], fSyllableKind: kSyllableSingle, line 128, fSyllableStanzaNumber: "stanza_1", line 128:
      fSyllableKind            : kSyllableSingle
      fSyllableExtendKind      : kSyllableExtend_NONE
      fSyllableWholeNotes      : 1/4 whn
      fSyllableTupletFactor    : -1/-1
      fSyllableUpLinkToMeasure: [NULL]
      fSyllableElementsList    
        [SyllableElement
          fSyllableElementContents        : "Il"
          fSyllableElementKind            : kSyllableElementText
          fInputLineNumber                : line 130
        ]
    ]
    
    doGenerateASingleHyphen     : false
    doGenerateADoubleUnderscore : false
    %}
    
    
    
    
    %{ ================= SYLLABLE DESCRIPTION =================
    getSyllableElementsList      : mio
    getSyllableKind              : kSyllableSingle
    getSyllableWholeNotes        : 3/16 whn
    getSyllableExtendKind        : kSyllableExtend_NONE
    
    getLyricsDurationsKind       : explicit
    fOnGoingExtend               : false
    
    noteTheSyllableIsAttachedTo  : [NULL]
    getStanzaNumber              : stanza_1
    getStanzaName                : Part_POne_Staff_One_Voice_1_Stanza_1
    
    getInputLineNumber           : line 144
    ========================================================
    %}
    
    %{ CODE_FOR_SYLLABLE_DURATION_KIND_EXPLICIT_kSyllableSingle, line 144 %}
    mio8.
    %{ CODE_FOR_EXTENDER_EXPLICIT_DURATIONS_kSyllableSingle, line 144 %}
    %{ =======>
    [Syllable, fSyllableElementsList: ["mio"], fSyllableKind: kSyllableSingle, line 144, fSyllableStanzaNumber: "stanza_1", line 144:
      fSyllableKind            : kSyllableSingle
      fSyllableExtendKind      : kSyllableExtend_NONE
      fSyllableWholeNotes      : 3/16 whn
      fSyllableTupletFactor    : -1/-1
      fSyllableUpLinkToMeasure: [NULL]
      fSyllableElementsList    
        [SyllableElement
          fSyllableElementContents        : "mio"
          fSyllableElementKind            : kSyllableElementText
          fInputLineNumber                : line 146
        ]
    ]
    
    doGenerateASingleHyphen     : false
    doGenerateADoubleUnderscore : false
    %}
    
    
    
    
    %{ ================= SYLLABLE DESCRIPTION =================
    getSyllableElementsList      : te
    getSyllableKind              : kSyllableBegin
    getSyllableWholeNotes        : 1/16 whn
    getSyllableExtendKind        : kSyllableExtend_NONE
    
    getLyricsDurationsKind       : explicit
    fOnGoingExtend               : false
    
    noteTheSyllableIsAttachedTo  : [NULL]
    getStanzaNumber              : stanza_1
    getStanzaName                : Part_POne_Staff_One_Voice_1_Stanza_1
    
    getInputLineNumber           : line 158
    ========================================================
    %}
    
    %{ CODE_FOR_SYLLABLE_DURATION_KIND_EXPLICIT_kSyllableBegin, line 158 %}
    te16
    %{ %{ CODE_FOR_EXTENDER_EXPLICIT_DURATIONS_kSyllableBegin, line 158 %}
    %{ =======>
    [Syllable, fSyllableElementsList: ["te"], fSyllableKind: kSyllableBegin, line 158, fSyllableStanzaNumber: "stanza_1", line 158:
      fSyllableKind            : kSyllableBegin
      fSyllableExtendKind      : kSyllableExtend_NONE
      fSyllableWholeNotes      : 1/16 whn
      fSyllableTupletFactor    : -1/-1
      fSyllableUpLinkToMeasure: [NULL]
      fSyllableElementsList    
        [SyllableElement
          fSyllableElementContents        : "te"
          fSyllableElementKind            : kSyllableElementText
          fInputLineNumber                : line 160
        ]
    ]
    
    doGenerateASingleHyphen     : false
    doGenerateADoubleHyphen     : true
    %}
    --
    
    
    
    %{ ================= SYLLABLE DESCRIPTION =================
    getSyllableElementsList      : so
    getSyllableKind              : kSyllableMiddle
    getSyllableWholeNotes        : 1/8 whn
    getSyllableExtendKind        : kSyllableExtend_NONE
    
    getLyricsDurationsKind       : explicit
    fOnGoingExtend               : false
    
    noteTheSyllableIsAttachedTo  : [NULL]
    getStanzaNumber              : stanza_1
    getStanzaName                : Part_POne_Staff_One_Voice_1_Stanza_1
    
    getInputLineNumber           : line 177
    ========================================================
    %}
    
    %{ CODE_FOR_SYLLABLE_DURATION_KIND_EXPLICIT_kSyllableMiddle, line 177 %}
    so8 %{ CODE_FOR_SYLLABLE_DURATION_KIND_EXPLICIT_kSyllableMiddle, line 177 %}
    
    %{ %{ CODE_FOR_EXTENDER_EXPLICIT_DURATIONS_kSyllableMiddle, line 177 %}
    %{ =======>
    [Syllable, fSyllableElementsList: ["so"], fSyllableKind: kSyllableMiddle, line 177, fSyllableStanzaNumber: "stanza_1", line 177:
      fSyllableKind            : kSyllableMiddle
      fSyllableExtendKind      : kSyllableExtend_NONE
      fSyllableWholeNotes      : 1/8 whn
      fSyllableTupletFactor    : -1/-1
      fSyllableUpLinkToMeasure: [NULL]
      fSyllableElementsList    
        [SyllableElement
          fSyllableElementContents        : "so"
          fSyllableElementKind            : kSyllableElementText
          fInputLineNumber                : line 179
        ]
    ]
    
    doGenerateASingleHyphen     : false
    doGenerateADoubleHyphen     : true
    doGenerateASingleUnderscore : false
    %}
    --
    
    
    
    %{ ================= SYLLABLE DESCRIPTION =================
    getSyllableElementsList      :
    getSyllableKind              : kSyllableSkipOnRestNote
    getSyllableWholeNotes        : 1/8 whn
    getSyllableExtendKind        : kSyllableExtend_NONE
    
    getLyricsDurationsKind       : explicit
    fOnGoingExtend               : false
    
    noteTheSyllableIsAttachedTo  : [NULL]
    getStanzaNumber              : stanza_1
    getStanzaName                : Part_POne_Staff_One_Voice_1_Stanza_1
    
    getInputLineNumber           : line 210
    ========================================================
    %}
    
    %{ CODE_FOR_SYLLABLE_DURATION_KIND_EXPLICIT_kSyllableSkipOnRestNote, line 210 %}
    8
    
    
    %{ ================= SYLLABLE DESCRIPTION =================
    getSyllableElementsList      : "r‿n"
    getSyllableKind              : kSyllableBegin
    getSyllableWholeNotes        : 3/16 whn
    getSyllableExtendKind        : kSyllableExtend_NONE
    
    getLyricsDurationsKind       : explicit
    fOnGoingExtend               : false
    
    noteTheSyllableIsAttachedTo  : [NULL]
    getStanzaNumber              : stanza_1
    getStanzaName                : Part_POne_Staff_One_Voice_1_Stanza_1
    
    getInputLineNumber           : line 210
    ========================================================
    %}
    
    %{ CODE_FOR_SYLLABLE_DURATION_KIND_EXPLICIT_kSyllableBegin, line 210 %}
    "r‿n"8.
    %{ %{ CODE_FOR_EXTENDER_EXPLICIT_DURATIONS_kSyllableBegin, line 210 %}
    %{ =======>
    [Syllable, fSyllableElementsList: ["r‿n"], fSyllableKind: kSyllableBegin, line 210, fSyllableStanzaNumber: "stanza_1", line 210:
      fSyllableKind            : kSyllableBegin
      fSyllableExtendKind      : kSyllableExtend_NONE
      fSyllableWholeNotes      : 3/16 whn
      fSyllableTupletFactor    : -1/-1
      fSyllableUpLinkToMeasure: [NULL]
      fSyllableElementsList    
        [SyllableElement
          fSyllableElementContents        : "r‿n"
          fSyllableElementKind            : kSyllableElementText
          fInputLineNumber                : line 212
        ]
    ]
    
    doGenerateASingleHyphen     : false
    doGenerateADoubleHyphen     : true
    %}
    --
    
    
    
    %{ ================= SYLLABLE DESCRIPTION =================
    getSyllableElementsList      :
    getSyllableKind              : kSyllableSkipOnRestNote
    getSyllableWholeNotes        : 1/16 whn
    getSyllableExtendKind        : kSyllableExtend_NONE
    
    getLyricsDurationsKind       : explicit
    fOnGoingExtend               : false
    
    noteTheSyllableIsAttachedTo  : [NULL]
    getStanzaNumber              : stanza_1
    getStanzaName                : Part_POne_Staff_One_Voice_1_Stanza_1
    
    getInputLineNumber           : line 90
    ========================================================
    %}
    
    %{ CODE_FOR_SYLLABLE_DURATION_KIND_EXPLICIT_kSyllableSkipOnRestNote, line 90 %}
    16
    
    
    %{ ================= SYLLABLE DESCRIPTION =================
    getSyllableElementsList      :
    getSyllableKind              : kSyllableMeasureEnd
    getSyllableWholeNotes        : 0/1 whn
    getSyllableExtendKind        : kSyllableExtend_NONE
    
    getLyricsDurationsKind       : explicit
    fOnGoingExtend               : false
    
    noteTheSyllableIsAttachedTo  : [NULL]
    getStanzaNumber              : stanza_1
    getStanzaName                : Part_POne_Staff_One_Voice_1_Stanza_1
    
    getInputLineNumber           : line 90
    ========================================================
    %}
    
    %{ CODE_FOR_SYLLABLE_kSyllableMeasureEnd, line 90 %}
    | % 5316  %{ noteTheSyllableIsAttachedTo is NULL %}
    
    
    
    %{ ================= SYLLABLE DESCRIPTION =================
    getSyllableElementsList      : tan
    getSyllableKind              : kSyllableMiddle
    getSyllableWholeNotes        : 3/8 whn
    getSyllableExtendKind        : kSyllableExtend_NONE
    
    getLyricsDurationsKind       : explicit
    fOnGoingExtend               : false
    
    noteTheSyllableIsAttachedTo  : [NULL]
    getStanzaNumber              : stanza_1
    getStanzaName                : Part_POne_Staff_One_Voice_1_Stanza_1
    
    getInputLineNumber           : line 246
    ========================================================
    %}
    
    %{ CODE_FOR_SYLLABLE_DURATION_KIND_EXPLICIT_kSyllableMiddle, line 246 %}
    tan4. %{ CODE_FOR_SYLLABLE_DURATION_KIND_EXPLICIT_kSyllableMiddle, line 246 %}
    
    %{ %{ CODE_FOR_EXTENDER_EXPLICIT_DURATIONS_kSyllableMiddle, line 246 %}
    %{ =======>
    [Syllable, fSyllableElementsList: ["tan"], fSyllableKind: kSyllableMiddle, line 246, fSyllableStanzaNumber: "stanza_1", line 246:
      fSyllableKind            : kSyllableMiddle
      fSyllableExtendKind      : kSyllableExtend_NONE
      fSyllableWholeNotes      : 3/8 whn
      fSyllableTupletFactor    : -1/-1
      fSyllableUpLinkToMeasure: [NULL]
      fSyllableElementsList    
        [SyllableElement
          fSyllableElementContents        : "tan"
          fSyllableElementKind            : kSyllableElementText
          fInputLineNumber                : line 248
        ]
    ]
    
    doGenerateASingleHyphen     : false
    doGenerateADoubleHyphen     : true
    doGenerateASingleUnderscore : false
    %}
    --
    
    
    
    %{ ================= SYLLABLE DESCRIPTION =================
    getSyllableElementsList      :
    getSyllableKind              : kSyllableSkipOnRestNote
    getSyllableWholeNotes        : 1/8 whn
    getSyllableExtendKind        : kSyllableExtend_NONE
    
    getLyricsDurationsKind       : explicit
    fOnGoingExtend               : false
    
    noteTheSyllableIsAttachedTo  : [NULL]
    getStanzaNumber              : stanza_1
    getStanzaName                : Part_POne_Staff_One_Voice_1_Stanza_1
    
    getInputLineNumber           : line 273
    ========================================================
    %}
    
    %{ CODE_FOR_SYLLABLE_DURATION_KIND_EXPLICIT_kSyllableSkipOnRestNote, line 273 %}
    8
    
    
    %{ ================= SYLLABLE DESCRIPTION =================
    getSyllableElementsList      : to
    getSyllableKind              : kSyllableEnd
    getSyllableWholeNotes        : 1/4 whn
    getSyllableExtendKind        : kSyllableExtend_NONE
    
    getLyricsDurationsKind       : explicit
    fOnGoingExtend               : false
    
    noteTheSyllableIsAttachedTo  : [NULL]
    getStanzaNumber              : stanza_1
    getStanzaName                : Part_POne_Staff_One_Voice_1_Stanza_1
    
    getInputLineNumber           : line 273
    ========================================================
    %}
    
    %{ CODE_FOR_SYLLABLE_DURATION_KIND_EXPLICIT_kSyllableEnd, line 273 %}
    to4
    
    
    %{ ================= SYLLABLE DESCRIPTION =================
    getSyllableElementsList      :
    getSyllableKind              : kSyllableSkipOnRestNote
    getSyllableWholeNotes        : 1/4 whn
    getSyllableExtendKind        : kSyllableExtend_NONE
    
    getLyricsDurationsKind       : explicit
    fOnGoingExtend               : false
    
    noteTheSyllableIsAttachedTo  : [NULL]
    getStanzaNumber              : stanza_1
    getStanzaName                : Part_POne_Staff_One_Voice_1_Stanza_1
    
    getInputLineNumber           : line 231
    ========================================================
    %}
    
    %{ CODE_FOR_SYLLABLE_DURATION_KIND_EXPLICIT_kSyllableSkipOnRestNote, line 231 %}
    4
    
    
    %{ ================= SYLLABLE DESCRIPTION =================
    getSyllableElementsList      :
    getSyllableKind              : kSyllableMeasureEnd
    getSyllableWholeNotes        : 0/1 whn
    getSyllableExtendKind        : kSyllableExtend_NONE
    
    getLyricsDurationsKind       : explicit
    fOnGoingExtend               : false
    
    noteTheSyllableIsAttachedTo  : [NULL]
    getStanzaNumber              : stanza_1
    getStanzaName                : Part_POne_Staff_One_Voice_1_Stanza_1
    
    getInputLineNumber           : line 231
    ========================================================
    %}
    
    %{ CODE_FOR_SYLLABLE_kSyllableMeasureEnd, line 231 %}
    | % 5316  %{ noteTheSyllableIsAttachedTo is NULL %}

} %{ FOFO FOFO %}

\book {

  \score {
    <<
      
      <<
      
        \new Staff  = "Part_POne_Staff_One"
        \with {
          instrumentName = "[Unnamed (treble staff)]"
        }
        <<
          \context Voice = "Part_POne_Staff_One_Voice_One" <<
            \Part_POne_Staff_One_Voice_One
          >>
          \new Lyrics
            \with {
            }
            \Part_POne_Staff_One_Voice_One_Stanza_One
        >>
      
      >>
    
    >>
    
    \layout {
      \context { \Score
        autoBeaming = ##f % to display tuplets brackets
      }
      \context { \Voice
      }
    }
    
    \midi {
      \tempo 16 = 360
    }
  }
  
}
