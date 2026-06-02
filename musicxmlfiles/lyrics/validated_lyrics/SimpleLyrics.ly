\version "2.24.4"

% Generated from "SimpleLyrics.xml"
% by xml2ly v2026.2 (built on June 02, 2026)
% on Tuesday 2026-06-02 @ 06:34:30 CEST

% The conversion command as supplied was: 
%  xml2ly -auto-output-file-name -lilypond-run-date -lilypond-generation-infos SimpleLyrics.xml -comlilylyrics
% or, with long option names:
%  xml2ly -auto-output-file-name -lilypond-run-date -lilypond-generation-infos SimpleLyrics.xml -comment-lilypond-lyrics
% or, with short option names:
%      SimpleLyrics.xml


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
  movementTitle       = "Simple Lyrics"
  miscellaneousField  = "Some notes with simple lyrics:
            Syllables, notes without a syllable, syllable
            spanners."
  title               = "Simple Lyrics"
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
  \key c \major
  \time 4/4
  a'4 a' a' a' a'4 a' a' a' a'4 a' a'2
  \bar "|."
}

Part_POne_Staff_One_Voice_One_Stanza_One = \lyricmode {
  % ===> using explicit lyrics durations mode
  \set associatedVoice  = #"Part_POne_Staff_One_Voice_One#"
  \set ignoreMelismata = ##t
    
    
    
    %{ ================= SYLLABLE DESCRIPTION =================
    getSyllableElementsList      : Tra
    getSyllableKind              : kSyllableBegin
    getSyllableWholeNotes        : 1/4 whn
    getSyllableExtendKind        : kSyllableExtend_NONE
    
    getLyricsDurationsKind       : explicit
    fOnGoingExtend               : false
    
    noteTheSyllableIsAttachedTo  : [NULL]
    getStanzaNumber              : stanza_1
    getStanzaName                : Part_POne_Staff_One_Voice_1_Stanza_1
    
    getInputLineNumber           : line 44
    ========================================================
    %}
    
    %{ CODE_FOR_SYLLABLE_DURATION_KIND_EXPLICIT_kSyllableBegin, line 44 %}
    Tra4
    %{ %{ CODE_FOR_EXTENDER_EXPLICIT_DURATIONS_kSyllableBegin, line 44 %}
    %{ =======>
    [Syllable, fSyllableElementsList: ["Tra"], fSyllableKind: kSyllableBegin, line 44, fSyllableStanzaNumber: "stanza_1", line 44:
      fSyllableKind            : kSyllableBegin
      fSyllableExtendKind      : kSyllableExtend_NONE
      fSyllableWholeNotes      : 1/4 whn
      fSyllableTupletFactor    : -1/-1
      fSyllableUpLinkToMeasure: [NULL]
      fSyllableElementsList    
        [SyllableElement
          fSyllableElementContents        : "Tra"
          fSyllableElementKind            : kSyllableElementText
          fInputLineNumber                : line 46
        ]
    ]
    
    doGenerateASingleHyphen     : false
    doGenerateADoubleHyphen     : true
    %}
    --
    
    
    
    %{ ================= SYLLABLE DESCRIPTION =================
    getSyllableElementsList      : la
    getSyllableKind              : kSyllableMiddle
    getSyllableWholeNotes        : 1/4 whn
    getSyllableExtendKind        : kSyllableExtend_NONE
    
    getLyricsDurationsKind       : explicit
    fOnGoingExtend               : false
    
    noteTheSyllableIsAttachedTo  : [NULL]
    getStanzaNumber              : stanza_1
    getStanzaName                : Part_POne_Staff_One_Voice_1_Stanza_1
    
    getInputLineNumber           : line 57
    ========================================================
    %}
    
    %{ CODE_FOR_SYLLABLE_DURATION_KIND_EXPLICIT_kSyllableMiddle, line 57 %}
    la4 %{ CODE_FOR_SYLLABLE_DURATION_KIND_EXPLICIT_kSyllableMiddle, line 57 %}
    
    %{ %{ CODE_FOR_EXTENDER_EXPLICIT_DURATIONS_kSyllableMiddle, line 57 %}
    %{ =======>
    [Syllable, fSyllableElementsList: ["la"], fSyllableKind: kSyllableMiddle, line 57, fSyllableStanzaNumber: "stanza_1", line 57:
      fSyllableKind            : kSyllableMiddle
      fSyllableExtendKind      : kSyllableExtend_NONE
      fSyllableWholeNotes      : 1/4 whn
      fSyllableTupletFactor    : -1/-1
      fSyllableUpLinkToMeasure: [NULL]
      fSyllableElementsList    
        [SyllableElement
          fSyllableElementContents        : "la"
          fSyllableElementKind            : kSyllableElementText
          fInputLineNumber                : line 59
        ]
    ]
    
    doGenerateASingleHyphen     : false
    doGenerateADoubleHyphen     : true
    doGenerateASingleUnderscore : false
    %}
    --
    
    
    
    %{ ================= SYLLABLE DESCRIPTION =================
    getSyllableElementsList      : li
    getSyllableKind              : kSyllableEnd
    getSyllableWholeNotes        : 1/4 whn
    getSyllableExtendKind        : kSyllableExtend_NONE
    
    getLyricsDurationsKind       : explicit
    fOnGoingExtend               : false
    
    noteTheSyllableIsAttachedTo  : [NULL]
    getStanzaNumber              : stanza_1
    getStanzaName                : Part_POne_Staff_One_Voice_1_Stanza_1
    
    getInputLineNumber           : line 70
    ========================================================
    %}
    
    %{ CODE_FOR_SYLLABLE_DURATION_KIND_EXPLICIT_kSyllableEnd, line 70 %}
    li4
    
    
    %{ ================= SYLLABLE DESCRIPTION =================
    getSyllableElementsList      : "Ja!"
    getSyllableKind              : kSyllableSingle
    getSyllableWholeNotes        : 1/4 whn
    getSyllableExtendKind        : kSyllableExtend_NONE
    
    getLyricsDurationsKind       : explicit
    fOnGoingExtend               : false
    
    noteTheSyllableIsAttachedTo  : [NULL]
    getStanzaNumber              : stanza_1
    getStanzaName                : Part_POne_Staff_One_Voice_1_Stanza_1
    
    getInputLineNumber           : line 83
    ========================================================
    %}
    
    %{ CODE_FOR_SYLLABLE_DURATION_KIND_EXPLICIT_kSyllableSingle, line 83 %}
    "Ja!"4
    %{ CODE_FOR_EXTENDER_EXPLICIT_DURATIONS_kSyllableSingle, line 83 %}
    %{ =======>
    [Syllable, fSyllableElementsList: ["Ja!"], fSyllableKind: kSyllableSingle, line 83, fSyllableStanzaNumber: "stanza_1", line 83:
      fSyllableKind            : kSyllableSingle
      fSyllableExtendKind      : kSyllableExtend_NONE
      fSyllableWholeNotes      : 1/4 whn
      fSyllableTupletFactor    : -1/-1
      fSyllableUpLinkToMeasure: [NULL]
      fSyllableElementsList    
        [SyllableElement
          fSyllableElementContents        : "Ja!"
          fSyllableElementKind            : kSyllableElementText
          fInputLineNumber                : line 85
        ]
    ]
    
    doGenerateASingleHyphen     : false
    doGenerateADoubleUnderscore : false
    %}
    
    
    
    
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
    
    getInputLineNumber           : line 20
    ========================================================
    %}
    
    %{ CODE_FOR_SYLLABLE_kSyllableMeasureEnd, line 20 %}
    | % 5316  %{ noteTheSyllableIsAttachedTo is NULL %}
    
    
    
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
    
    getInputLineNumber           : line 108
    ========================================================
    %}
    
    %{ CODE_FOR_SYLLABLE_DURATION_KIND_EXPLICIT_kSyllableSkipOnRestNote, line 108 %}
    4
    
    
    %{ ================= SYLLABLE DESCRIPTION =================
    getSyllableElementsList      : Tra
    getSyllableKind              : kSyllableBegin
    getSyllableWholeNotes        : 1/4 whn
    getSyllableExtendKind        : kSyllableExtend_NONE
    
    getLyricsDurationsKind       : explicit
    fOnGoingExtend               : false
    
    noteTheSyllableIsAttachedTo  : [NULL]
    getStanzaNumber              : stanza_1
    getStanzaName                : Part_POne_Staff_One_Voice_1_Stanza_1
    
    getInputLineNumber           : line 108
    ========================================================
    %}
    
    %{ CODE_FOR_SYLLABLE_DURATION_KIND_EXPLICIT_kSyllableBegin, line 108 %}
    Tra4
    %{ %{ CODE_FOR_EXTENDER_EXPLICIT_DURATIONS_kSyllableBegin, line 108 %}
    %{ =======>
    [Syllable, fSyllableElementsList: ["Tra"], fSyllableKind: kSyllableBegin, line 108, fSyllableStanzaNumber: "stanza_1", line 108:
      fSyllableKind            : kSyllableBegin
      fSyllableExtendKind      : kSyllableExtend_NONE
      fSyllableWholeNotes      : 1/4 whn
      fSyllableTupletFactor    : -1/-1
      fSyllableUpLinkToMeasure: [NULL]
      fSyllableElementsList    
        [SyllableElement
          fSyllableElementContents        : "Tra"
          fSyllableElementKind            : kSyllableElementText
          fInputLineNumber                : line 110
        ]
    ]
    
    doGenerateASingleHyphen     : false
    doGenerateADoubleHyphen     : true
    %}
    --
    
    
    
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
    
    getInputLineNumber           : line 130
    ========================================================
    %}
    
    %{ CODE_FOR_SYLLABLE_DURATION_KIND_EXPLICIT_kSyllableSkipOnRestNote, line 130 %}
    4
    
    
    %{ ================= SYLLABLE DESCRIPTION =================
    getSyllableElementsList      : "ra!"
    getSyllableKind              : kSyllableEnd
    getSyllableWholeNotes        : 1/4 whn
    getSyllableExtendKind        : kSyllableExtend_NONE
    
    getLyricsDurationsKind       : explicit
    fOnGoingExtend               : false
    
    noteTheSyllableIsAttachedTo  : [NULL]
    getStanzaNumber              : stanza_1
    getStanzaName                : Part_POne_Staff_One_Voice_1_Stanza_1
    
    getInputLineNumber           : line 130
    ========================================================
    %}
    
    %{ CODE_FOR_SYLLABLE_DURATION_KIND_EXPLICIT_kSyllableEnd, line 130 %}
    "ra!"4
    
    
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
    getSyllableElementsList      :
    getSyllableKind              : kSyllableSkipOnRestNote
    getSyllableWholeNotes        : 1/4 whn
    getSyllableExtendKind        : kSyllableExtend_NONE
    
    getLyricsDurationsKind       : explicit
    fOnGoingExtend               : false
    
    noteTheSyllableIsAttachedTo  : [NULL]
    getStanzaNumber              : stanza_1
    getStanzaName                : Part_POne_Staff_One_Voice_1_Stanza_1
    
    getInputLineNumber           : line 155
    ========================================================
    %}
    
    %{ CODE_FOR_SYLLABLE_DURATION_KIND_EXPLICIT_kSyllableSkipOnRestNote, line 155 %}
    4
    
    
    %{ ================= SYLLABLE DESCRIPTION =================
    getSyllableElementsList      : "Bah!"
    getSyllableKind              : kSyllableSingle
    getSyllableWholeNotes        : 1/4 whn
    getSyllableExtendKind        : kSyllableExtend_NONE
    
    getLyricsDurationsKind       : explicit
    fOnGoingExtend               : false
    
    noteTheSyllableIsAttachedTo  : [NULL]
    getStanzaNumber              : stanza_1
    getStanzaName                : Part_POne_Staff_One_Voice_1_Stanza_1
    
    getInputLineNumber           : line 155
    ========================================================
    %}
    
    %{ CODE_FOR_SYLLABLE_DURATION_KIND_EXPLICIT_kSyllableSingle, line 155 %}
    "Bah!"4
    %{ CODE_FOR_EXTENDER_EXPLICIT_DURATIONS_kSyllableSingle, line 155 %}
    %{ =======>
    [Syllable, fSyllableElementsList: ["Bah!"], fSyllableKind: kSyllableSingle, line 155, fSyllableStanzaNumber: "stanza_1", line 155:
      fSyllableKind            : kSyllableSingle
      fSyllableExtendKind      : kSyllableExtend_NONE
      fSyllableWholeNotes      : 1/4 whn
      fSyllableTupletFactor    : -1/-1
      fSyllableUpLinkToMeasure: [NULL]
      fSyllableElementsList    
        [SyllableElement
          fSyllableElementContents        : "Bah!"
          fSyllableElementKind            : kSyllableElementText
          fInputLineNumber                : line 157
        ]
    ]
    
    doGenerateASingleHyphen     : false
    doGenerateADoubleUnderscore : false
    %}
    
    
    
    
    %{ ================= SYLLABLE DESCRIPTION =================
    getSyllableElementsList      :
    getSyllableKind              : kSyllableSkipOnRestNote
    getSyllableWholeNotes        : 1/2 whn
    getSyllableExtendKind        : kSyllableExtend_NONE
    
    getLyricsDurationsKind       : explicit
    fOnGoingExtend               : false
    
    noteTheSyllableIsAttachedTo  : [NULL]
    getStanzaNumber              : stanza_1
    getStanzaName                : Part_POne_Staff_One_Voice_1_Stanza_1
    
    getInputLineNumber           : line 137
    ========================================================
    %}
    
    %{ CODE_FOR_SYLLABLE_DURATION_KIND_EXPLICIT_kSyllableSkipOnRestNote, line 137 %}
    2
    
    
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
    
    getInputLineNumber           : line 137
    ========================================================
    %}
    
    %{ CODE_FOR_SYLLABLE_kSyllableMeasureEnd, line 137 %}
    | % 5316  %{ noteTheSyllableIsAttachedTo is NULL %}

} %{ FOFO FOFO %}

\book {

  \score {
    <<
      
      <<
      
        \new Staff  = "Part_POne_Staff_One"
        \with {
          instrumentName = "MusicXML Part"
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
