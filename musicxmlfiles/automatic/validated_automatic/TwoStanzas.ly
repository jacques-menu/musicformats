\version "2.24.4"

% Generated from "TwoStanzas.xml"
% by xml2ly v2026.2 (built on June 09, 2026)
% on Tuesday 2026-06-09 @ 17:03:04 CEST

% The conversion command as supplied was: 
%  xml2ly -auto-output-file-name -lilypond-run-date -lilypond-generation-infos TwoStanzas.xml -comlilylyr
% or, with long option names:
%  xml2ly -auto-output-file-name -lilypond-run-date -lilypond-generation-infos TwoStanzas.xml -comment-lilypond-lyrics
% or, with short option names:
%      TwoStanzas.xml


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
  movementTitle       = "Two stanzas"
  encodingDate        = "2026-05-12"
  software            = "Sibelius 2026.5"
  software            = "Dolet 8.3 for Sibelius"
  title               = "Two stanzas"
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
"Score generated from MusicXML data by xml2ly v2026.2 (built on June 09, 2026) and LilyPond " (lilypond-version))
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
  \key ees \major
  \numericTimeSignature \time 4/4
  \stemUp g'8 [
  bes' bes' g' ]
  aes'4 g'
  \bar "|."
}

Part_POne_Staff_One_Voice_One_Stanza_One = \lyricmode {
  % ===> using automatic lyrics durations mode
  \set ignoreMelismata = ##t
    
    
    
    %{ ================= SYLLABLE DESCRIPTION =================
    getSyllableElementsList      : "1.Sing"
    getSyllableKind              : kSyllableSingle
    getSyllableWholeNotes        : UNKNOWN_WHOLE_NOTES_
    getSyllableExtendKind        : kSyllableExtend_NONE
    
    getLyricsDurationsKind       : automatic
    fOnGoingExtend               : false
    
    noteTheSyllableIsAttachedTo  : [NULL]
    getStanzaNumber              : stanza_1
    getStanzaName                : Part_POne_Staff_One_Voice_1_Stanza_1
    
    getInputLineNumber           : line 109
    ========================================================
    %}
    
    %{ CODE_FOR_SYLLABLE_DURATION_KIND_AUTOMATIC_kSyllableSingle, line 109 %}
    "1.Sing"
    %{ CODE_FOR_EXTENDER_AUTOMATIC_DURATIONS_kSyllableSingle, line 109 %}
    %{ =======>
    [Syllable, fSyllableElementsList: ["1.", "", "Sing"], fSyllableKind: kSyllableSingle, line 109, fSyllableStanzaNumber: "stanza_1", line 109:
      fSyllableKind            : kSyllableSingle
      fSyllableExtendKind      : kSyllableExtend_NONE
      fSyllableWholeNotes      : -1/1 whn
      fSyllableTupletFactor    : -1/-1
      fSyllableUpLinkToMeasure : [NULL]
      fSyllableElementsList    
        [SyllableElement
          fSyllableElementContents        : "1."
          fSyllableElementKind            : kSyllableElementText
          fInputLineNumber                : line 111
        ]
        [SyllableElement
          fSyllableElementContents        : ""
          fSyllableElementKind            : kSyllableElementElision
          fInputLineNumber                : line 112
        ]
        [SyllableElement
          fSyllableElementContents        : "Sing"
          fSyllableElementKind            : kSyllableElementText
          fInputLineNumber                : line 114
        ]
    ]
    
    doGenerateASingleHyphen     : false
    doGenerateASingleUnderscore : false
    doGenerateADoubleUnderscore : false
    %}
    
    
    
    
    %{ ================= SYLLABLE DESCRIPTION =================
    getSyllableElementsList      :
    getSyllableKind              : kSyllableSkipOnRestNote
    getSyllableWholeNotes        : 9/8 whn
    getSyllableExtendKind        : kSyllableExtend_NONE
    
    getLyricsDurationsKind       : automatic
    fOnGoingExtend               : false
    
    noteTheSyllableIsAttachedTo  : [NULL]
    getStanzaNumber              : stanza_1
    getStanzaName                : Part_POne_Staff_One_Voice_1_Stanza_1
    
    getInputLineNumber           : line 135
    ========================================================
    %}
    
    %{ CODE_FOR_SYLLABLE_DURATION_KIND_AUTOMATIC_kSyllableSkipOnRestNote, line 135 %}
    
    
    
    %{ ================= SYLLABLE DESCRIPTION =================
    getSyllableElementsList      : out
    getSyllableKind              : kSyllableSingle
    getSyllableWholeNotes        : UNKNOWN_WHOLE_NOTES_
    getSyllableExtendKind        : kSyllableExtend_NONE
    
    getLyricsDurationsKind       : automatic
    fOnGoingExtend               : false
    
    noteTheSyllableIsAttachedTo  : [NULL]
    getStanzaNumber              : stanza_1
    getStanzaName                : Part_POne_Staff_One_Voice_1_Stanza_1
    
    getInputLineNumber           : line 135
    ========================================================
    %}
    
    %{ CODE_FOR_SYLLABLE_DURATION_KIND_AUTOMATIC_kSyllableSingle, line 135 %}
    out
    %{ CODE_FOR_EXTENDER_AUTOMATIC_DURATIONS_kSyllableSingle, line 135 %}
    %{ =======>
    [Syllable, fSyllableElementsList: ["out"], fSyllableKind: kSyllableSingle, line 135, fSyllableStanzaNumber: "stanza_1", line 135:
      fSyllableKind            : kSyllableSingle
      fSyllableExtendKind      : kSyllableExtend_NONE
      fSyllableWholeNotes      : -1/1 whn
      fSyllableTupletFactor    : -1/-1
      fSyllableUpLinkToMeasure : [NULL]
      fSyllableElementsList    
        [SyllableElement
          fSyllableElementContents        : "out"
          fSyllableElementKind            : kSyllableElementText
          fInputLineNumber                : line 137
        ]
    ]
    
    doGenerateASingleHyphen     : false
    doGenerateASingleUnderscore : false
    doGenerateADoubleUnderscore : false
    %}
    
    
    
    
    %{ ================= SYLLABLE DESCRIPTION =================
    getSyllableElementsList      :
    getSyllableKind              : kSyllableSkipOnRestNote
    getSyllableWholeNotes        : 9/8 whn
    getSyllableExtendKind        : kSyllableExtend_NONE
    
    getLyricsDurationsKind       : automatic
    fOnGoingExtend               : false
    
    noteTheSyllableIsAttachedTo  : [NULL]
    getStanzaNumber              : stanza_1
    getStanzaName                : Part_POne_Staff_One_Voice_1_Stanza_1
    
    getInputLineNumber           : line 155
    ========================================================
    %}
    
    %{ CODE_FOR_SYLLABLE_DURATION_KIND_AUTOMATIC_kSyllableSkipOnRestNote, line 155 %}
    
    
    
    %{ ================= SYLLABLE DESCRIPTION =================
    getSyllableElementsList      : all
    getSyllableKind              : kSyllableSingle
    getSyllableWholeNotes        : UNKNOWN_WHOLE_NOTES_
    getSyllableExtendKind        : kSyllableExtend_NONE
    
    getLyricsDurationsKind       : automatic
    fOnGoingExtend               : false
    
    noteTheSyllableIsAttachedTo  : [NULL]
    getStanzaNumber              : stanza_1
    getStanzaName                : Part_POne_Staff_One_Voice_1_Stanza_1
    
    getInputLineNumber           : line 155
    ========================================================
    %}
    
    %{ CODE_FOR_SYLLABLE_DURATION_KIND_AUTOMATIC_kSyllableSingle, line 155 %}
    all
    %{ CODE_FOR_EXTENDER_AUTOMATIC_DURATIONS_kSyllableSingle, line 155 %}
    %{ =======>
    [Syllable, fSyllableElementsList: ["all"], fSyllableKind: kSyllableSingle, line 155, fSyllableStanzaNumber: "stanza_1", line 155:
      fSyllableKind            : kSyllableSingle
      fSyllableExtendKind      : kSyllableExtend_NONE
      fSyllableWholeNotes      : -1/1 whn
      fSyllableTupletFactor    : -1/-1
      fSyllableUpLinkToMeasure : [NULL]
      fSyllableElementsList    
        [SyllableElement
          fSyllableElementContents        : "all"
          fSyllableElementKind            : kSyllableElementText
          fInputLineNumber                : line 157
        ]
    ]
    
    doGenerateASingleHyphen     : false
    doGenerateASingleUnderscore : false
    doGenerateADoubleUnderscore : false
    %}
    
    
    
    
    %{ ================= SYLLABLE DESCRIPTION =================
    getSyllableElementsList      :
    getSyllableKind              : kSyllableSkipOnRestNote
    getSyllableWholeNotes        : 9/8 whn
    getSyllableExtendKind        : kSyllableExtend_NONE
    
    getLyricsDurationsKind       : automatic
    fOnGoingExtend               : false
    
    noteTheSyllableIsAttachedTo  : [NULL]
    getStanzaNumber              : stanza_1
    getStanzaName                : Part_POne_Staff_One_Voice_1_Stanza_1
    
    getInputLineNumber           : line 174
    ========================================================
    %}
    
    %{ CODE_FOR_SYLLABLE_DURATION_KIND_AUTOMATIC_kSyllableSkipOnRestNote, line 174 %}
    
    
    
    %{ ================= SYLLABLE DESCRIPTION =================
    getSyllableElementsList      : your
    getSyllableKind              : kSyllableSingle
    getSyllableWholeNotes        : UNKNOWN_WHOLE_NOTES_
    getSyllableExtendKind        : kSyllableExtend_NONE
    
    getLyricsDurationsKind       : automatic
    fOnGoingExtend               : false
    
    noteTheSyllableIsAttachedTo  : [NULL]
    getStanzaNumber              : stanza_1
    getStanzaName                : Part_POne_Staff_One_Voice_1_Stanza_1
    
    getInputLineNumber           : line 174
    ========================================================
    %}
    
    %{ CODE_FOR_SYLLABLE_DURATION_KIND_AUTOMATIC_kSyllableSingle, line 174 %}
    your
    %{ CODE_FOR_EXTENDER_AUTOMATIC_DURATIONS_kSyllableSingle, line 174 %}
    %{ =======>
    [Syllable, fSyllableElementsList: ["your"], fSyllableKind: kSyllableSingle, line 174, fSyllableStanzaNumber: "stanza_1", line 174:
      fSyllableKind            : kSyllableSingle
      fSyllableExtendKind      : kSyllableExtend_NONE
      fSyllableWholeNotes      : -1/1 whn
      fSyllableTupletFactor    : -1/-1
      fSyllableUpLinkToMeasure : [NULL]
      fSyllableElementsList    
        [SyllableElement
          fSyllableElementContents        : "your"
          fSyllableElementKind            : kSyllableElementText
          fInputLineNumber                : line 176
        ]
    ]
    
    doGenerateASingleHyphen     : false
    doGenerateASingleUnderscore : false
    doGenerateADoubleUnderscore : false
    %}
    
    
    
    
    %{ ================= SYLLABLE DESCRIPTION =================
    getSyllableElementsList      :
    getSyllableKind              : kSyllableSkipOnRestNote
    getSyllableWholeNotes        : 9/8 whn
    getSyllableExtendKind        : kSyllableExtend_NONE
    
    getLyricsDurationsKind       : automatic
    fOnGoingExtend               : false
    
    noteTheSyllableIsAttachedTo  : [NULL]
    getStanzaNumber              : stanza_1
    getStanzaName                : Part_POne_Staff_One_Voice_1_Stanza_1
    
    getInputLineNumber           : line 193
    ========================================================
    %}
    
    %{ CODE_FOR_SYLLABLE_DURATION_KIND_AUTOMATIC_kSyllableSkipOnRestNote, line 193 %}
    
    
    
    %{ ================= SYLLABLE DESCRIPTION =================
    getSyllableElementsList      : prais
    getSyllableKind              : kSyllableBegin
    getSyllableWholeNotes        : UNKNOWN_WHOLE_NOTES_
    getSyllableExtendKind        : kSyllableExtend_NONE
    
    getLyricsDurationsKind       : automatic
    fOnGoingExtend               : false
    
    noteTheSyllableIsAttachedTo  : [NULL]
    getStanzaNumber              : stanza_1
    getStanzaName                : Part_POne_Staff_One_Voice_1_Stanza_1
    
    getInputLineNumber           : line 193
    ========================================================
    %}
    
    %{ CODE_FOR_SYLLABLE_DURATION_KIND_AUTOMATIC_kSyllableBegin, line 193 %}
    prais
    %{ CODE_FOR_EXTENDER_AUTOMATIC_DURATIONS_kSyllableBegin, line 193 %}
    %{ =======>
    [Syllable, fSyllableElementsList: ["prais"], fSyllableKind: kSyllableBegin, line 193, fSyllableStanzaNumber: "stanza_1", line 193:
      fSyllableKind            : kSyllableBegin
      fSyllableExtendKind      : kSyllableExtend_NONE
      fSyllableWholeNotes      : -1/1 whn
      fSyllableTupletFactor    : -1/-1
      fSyllableUpLinkToMeasure : [NULL]
      fSyllableElementsList    
        [SyllableElement
          fSyllableElementContents        : "prais"
          fSyllableElementKind            : kSyllableElementText
          fInputLineNumber                : line 195
        ]
    ]
    
    syllableUpLinkToNote        : [NULL]
    doGenerateASingleHyphen     : false
    doGenerateADoubleHyphen     : true
    %}
    --
    
    
    
    %{ ================= SYLLABLE DESCRIPTION =================
    getSyllableElementsList      :
    getSyllableKind              : kSyllableSkipOnRestNote
    getSyllableWholeNotes        : 5/4 whn
    getSyllableExtendKind        : kSyllableExtend_NONE
    
    getLyricsDurationsKind       : automatic
    fOnGoingExtend               : false
    
    noteTheSyllableIsAttachedTo  : [NULL]
    getStanzaNumber              : stanza_1
    getStanzaName                : Part_POne_Staff_One_Voice_1_Stanza_1
    
    getInputLineNumber           : line 211
    ========================================================
    %}
    
    %{ CODE_FOR_SYLLABLE_DURATION_KIND_AUTOMATIC_kSyllableSkipOnRestNote, line 211 %}
    
    
    
    %{ ================= SYLLABLE DESCRIPTION =================
    getSyllableElementsList      : es
    getSyllableKind              : kSyllableEnd
    getSyllableWholeNotes        : UNKNOWN_WHOLE_NOTES_
    getSyllableExtendKind        : kSyllableExtend_NONE
    
    getLyricsDurationsKind       : automatic
    fOnGoingExtend               : false
    
    noteTheSyllableIsAttachedTo  : [NULL]
    getStanzaNumber              : stanza_1
    getStanzaName                : Part_POne_Staff_One_Voice_1_Stanza_1
    
    getInputLineNumber           : line 211
    ========================================================
    %}
    
    %{ CODE_FOR_SYLLABLE_DURATION_KIND_AUTOMATIC_kSyllableEnd, line 211 %}
    es
    
    
    %{ ================= SYLLABLE DESCRIPTION =================
    getSyllableElementsList      :
    getSyllableKind              : kSyllableSkipOnRestNote
    getSyllableWholeNotes        : 5/4 whn
    getSyllableExtendKind        : kSyllableExtend_NONE
    
    getLyricsDurationsKind       : automatic
    fOnGoingExtend               : false
    
    noteTheSyllableIsAttachedTo  : [NULL]
    getStanzaNumber              : stanza_1
    getStanzaName                : Part_POne_Staff_One_Voice_1_Stanza_1
    
    getInputLineNumber           : line 78
    ========================================================
    %}
    
    %{ CODE_FOR_SYLLABLE_DURATION_KIND_AUTOMATIC_kSyllableSkipOnRestNote, line 78 %}
    
    
    
    %{ ================= SYLLABLE DESCRIPTION =================
    getSyllableElementsList      :
    getSyllableKind              : kSyllableMeasureEnd
    getSyllableWholeNotes        : 0/1 whn
    getSyllableExtendKind        : kSyllableExtend_NONE
    
    getLyricsDurationsKind       : automatic
    fOnGoingExtend               : false
    
    noteTheSyllableIsAttachedTo  : [NULL]
    getStanzaNumber              : stanza_1
    getStanzaName                : Part_POne_Staff_One_Voice_1_Stanza_1
    
    getInputLineNumber           : line 78
    ========================================================
    %}
    
    %{ CODE_FOR_SYLLABLE_kSyllableMeasureEnd, line 78 %}
    | % 5316  %{ noteTheSyllableIsAttachedTo is NULL %}

} %{ FOFO FOFO %}

Part_POne_Staff_One_Voice_One_Stanza_Two = \lyricmode {
  % ===> using automatic lyrics durations mode
  \set ignoreMelismata = ##t
    
    
    
    %{ ================= SYLLABLE DESCRIPTION =================
    getSyllableElementsList      : "2.For"
    getSyllableKind              : kSyllableSingle
    getSyllableWholeNotes        : UNKNOWN_WHOLE_NOTES_
    getSyllableExtendKind        : kSyllableExtend_NONE
    
    getLyricsDurationsKind       : automatic
    fOnGoingExtend               : false
    
    noteTheSyllableIsAttachedTo  : [NULL]
    getStanzaNumber              : stanza_2
    getStanzaName                : Part_POne_Staff_One_Voice_1_Stanza_2
    
    getInputLineNumber           : line 116
    ========================================================
    %}
    
    %{ CODE_FOR_SYLLABLE_DURATION_KIND_AUTOMATIC_kSyllableSingle, line 116 %}
    "2.For"
    %{ CODE_FOR_EXTENDER_AUTOMATIC_DURATIONS_kSyllableSingle, line 116 %}
    %{ =======>
    [Syllable, fSyllableElementsList: ["2.", "", "For"], fSyllableKind: kSyllableSingle, line 116, fSyllableStanzaNumber: "stanza_2", line 116:
      fSyllableKind            : kSyllableSingle
      fSyllableExtendKind      : kSyllableExtend_NONE
      fSyllableWholeNotes      : -1/1 whn
      fSyllableTupletFactor    : -1/-1
      fSyllableUpLinkToMeasure : [NULL]
      fSyllableElementsList    
        [SyllableElement
          fSyllableElementContents        : "2."
          fSyllableElementKind            : kSyllableElementText
          fInputLineNumber                : line 118
        ]
        [SyllableElement
          fSyllableElementContents        : ""
          fSyllableElementKind            : kSyllableElementElision
          fInputLineNumber                : line 119
        ]
        [SyllableElement
          fSyllableElementContents        : "For"
          fSyllableElementKind            : kSyllableElementText
          fInputLineNumber                : line 121
        ]
    ]
    
    doGenerateASingleHyphen     : false
    doGenerateASingleUnderscore : false
    doGenerateADoubleUnderscore : false
    %}
    
    
    
    
    %{ ================= SYLLABLE DESCRIPTION =================
    getSyllableElementsList      :
    getSyllableKind              : kSyllableSkipOnRestNote
    getSyllableWholeNotes        : 9/8 whn
    getSyllableExtendKind        : kSyllableExtend_NONE
    
    getLyricsDurationsKind       : automatic
    fOnGoingExtend               : false
    
    noteTheSyllableIsAttachedTo  : [NULL]
    getStanzaNumber              : stanza_2
    getStanzaName                : Part_POne_Staff_One_Voice_1_Stanza_2
    
    getInputLineNumber           : line 139
    ========================================================
    %}
    
    %{ CODE_FOR_SYLLABLE_DURATION_KIND_AUTOMATIC_kSyllableSkipOnRestNote, line 139 %}
    
    
    
    %{ ================= SYLLABLE DESCRIPTION =================
    getSyllableElementsList      : the
    getSyllableKind              : kSyllableSingle
    getSyllableWholeNotes        : UNKNOWN_WHOLE_NOTES_
    getSyllableExtendKind        : kSyllableExtend_NONE
    
    getLyricsDurationsKind       : automatic
    fOnGoingExtend               : false
    
    noteTheSyllableIsAttachedTo  : [NULL]
    getStanzaNumber              : stanza_2
    getStanzaName                : Part_POne_Staff_One_Voice_1_Stanza_2
    
    getInputLineNumber           : line 139
    ========================================================
    %}
    
    %{ CODE_FOR_SYLLABLE_DURATION_KIND_AUTOMATIC_kSyllableSingle, line 139 %}
    the
    %{ CODE_FOR_EXTENDER_AUTOMATIC_DURATIONS_kSyllableSingle, line 139 %}
    %{ =======>
    [Syllable, fSyllableElementsList: ["the"], fSyllableKind: kSyllableSingle, line 139, fSyllableStanzaNumber: "stanza_2", line 139:
      fSyllableKind            : kSyllableSingle
      fSyllableExtendKind      : kSyllableExtend_NONE
      fSyllableWholeNotes      : -1/1 whn
      fSyllableTupletFactor    : -1/-1
      fSyllableUpLinkToMeasure : [NULL]
      fSyllableElementsList    
        [SyllableElement
          fSyllableElementContents        : "the"
          fSyllableElementKind            : kSyllableElementText
          fInputLineNumber                : line 141
        ]
    ]
    
    doGenerateASingleHyphen     : false
    doGenerateASingleUnderscore : false
    doGenerateADoubleUnderscore : false
    %}
    
    
    
    
    %{ ================= SYLLABLE DESCRIPTION =================
    getSyllableElementsList      :
    getSyllableKind              : kSyllableSkipOnRestNote
    getSyllableWholeNotes        : 9/8 whn
    getSyllableExtendKind        : kSyllableExtend_NONE
    
    getLyricsDurationsKind       : automatic
    fOnGoingExtend               : false
    
    noteTheSyllableIsAttachedTo  : [NULL]
    getStanzaNumber              : stanza_2
    getStanzaName                : Part_POne_Staff_One_Voice_1_Stanza_2
    
    getInputLineNumber           : line 159
    ========================================================
    %}
    
    %{ CODE_FOR_SYLLABLE_DURATION_KIND_AUTOMATIC_kSyllableSkipOnRestNote, line 159 %}
    
    
    
    %{ ================= SYLLABLE DESCRIPTION =================
    getSyllableElementsList      : Son
    getSyllableKind              : kSyllableSingle
    getSyllableWholeNotes        : UNKNOWN_WHOLE_NOTES_
    getSyllableExtendKind        : kSyllableExtend_NONE
    
    getLyricsDurationsKind       : automatic
    fOnGoingExtend               : false
    
    noteTheSyllableIsAttachedTo  : [NULL]
    getStanzaNumber              : stanza_2
    getStanzaName                : Part_POne_Staff_One_Voice_1_Stanza_2
    
    getInputLineNumber           : line 159
    ========================================================
    %}
    
    %{ CODE_FOR_SYLLABLE_DURATION_KIND_AUTOMATIC_kSyllableSingle, line 159 %}
    Son
    %{ CODE_FOR_EXTENDER_AUTOMATIC_DURATIONS_kSyllableSingle, line 159 %}
    %{ =======>
    [Syllable, fSyllableElementsList: ["Son"], fSyllableKind: kSyllableSingle, line 159, fSyllableStanzaNumber: "stanza_2", line 159:
      fSyllableKind            : kSyllableSingle
      fSyllableExtendKind      : kSyllableExtend_NONE
      fSyllableWholeNotes      : -1/1 whn
      fSyllableTupletFactor    : -1/-1
      fSyllableUpLinkToMeasure : [NULL]
      fSyllableElementsList    
        [SyllableElement
          fSyllableElementContents        : "Son"
          fSyllableElementKind            : kSyllableElementText
          fInputLineNumber                : line 161
        ]
    ]
    
    doGenerateASingleHyphen     : false
    doGenerateASingleUnderscore : false
    doGenerateADoubleUnderscore : false
    %}
    
    
    
    
    %{ ================= SYLLABLE DESCRIPTION =================
    getSyllableElementsList      :
    getSyllableKind              : kSyllableSkipOnRestNote
    getSyllableWholeNotes        : 9/8 whn
    getSyllableExtendKind        : kSyllableExtend_NONE
    
    getLyricsDurationsKind       : automatic
    fOnGoingExtend               : false
    
    noteTheSyllableIsAttachedTo  : [NULL]
    getStanzaNumber              : stanza_2
    getStanzaName                : Part_POne_Staff_One_Voice_1_Stanza_2
    
    getInputLineNumber           : line 178
    ========================================================
    %}
    
    %{ CODE_FOR_SYLLABLE_DURATION_KIND_AUTOMATIC_kSyllableSkipOnRestNote, line 178 %}
    
    
    
    %{ ================= SYLLABLE DESCRIPTION =================
    getSyllableElementsList      : has
    getSyllableKind              : kSyllableSingle
    getSyllableWholeNotes        : UNKNOWN_WHOLE_NOTES_
    getSyllableExtendKind        : kSyllableExtend_NONE
    
    getLyricsDurationsKind       : automatic
    fOnGoingExtend               : false
    
    noteTheSyllableIsAttachedTo  : [NULL]
    getStanzaNumber              : stanza_2
    getStanzaName                : Part_POne_Staff_One_Voice_1_Stanza_2
    
    getInputLineNumber           : line 178
    ========================================================
    %}
    
    %{ CODE_FOR_SYLLABLE_DURATION_KIND_AUTOMATIC_kSyllableSingle, line 178 %}
    has
    %{ CODE_FOR_EXTENDER_AUTOMATIC_DURATIONS_kSyllableSingle, line 178 %}
    %{ =======>
    [Syllable, fSyllableElementsList: ["has"], fSyllableKind: kSyllableSingle, line 178, fSyllableStanzaNumber: "stanza_2", line 178:
      fSyllableKind            : kSyllableSingle
      fSyllableExtendKind      : kSyllableExtend_NONE
      fSyllableWholeNotes      : -1/1 whn
      fSyllableTupletFactor    : -1/-1
      fSyllableUpLinkToMeasure : [NULL]
      fSyllableElementsList    
        [SyllableElement
          fSyllableElementContents        : "has"
          fSyllableElementKind            : kSyllableElementText
          fInputLineNumber                : line 180
        ]
    ]
    
    doGenerateASingleHyphen     : false
    doGenerateASingleUnderscore : false
    doGenerateADoubleUnderscore : false
    %}
    
    
    
    
    %{ ================= SYLLABLE DESCRIPTION =================
    getSyllableElementsList      :
    getSyllableKind              : kSyllableSkipOnRestNote
    getSyllableWholeNotes        : 9/8 whn
    getSyllableExtendKind        : kSyllableExtend_NONE
    
    getLyricsDurationsKind       : automatic
    fOnGoingExtend               : false
    
    noteTheSyllableIsAttachedTo  : [NULL]
    getStanzaNumber              : stanza_2
    getStanzaName                : Part_POne_Staff_One_Voice_1_Stanza_2
    
    getInputLineNumber           : line 197
    ========================================================
    %}
    
    %{ CODE_FOR_SYLLABLE_DURATION_KIND_AUTOMATIC_kSyllableSkipOnRestNote, line 197 %}
    
    
    
    %{ ================= SYLLABLE DESCRIPTION =================
    getSyllableElementsList      : ris
    getSyllableKind              : kSyllableBegin
    getSyllableWholeNotes        : UNKNOWN_WHOLE_NOTES_
    getSyllableExtendKind        : kSyllableExtend_NONE
    
    getLyricsDurationsKind       : automatic
    fOnGoingExtend               : false
    
    noteTheSyllableIsAttachedTo  : [NULL]
    getStanzaNumber              : stanza_2
    getStanzaName                : Part_POne_Staff_One_Voice_1_Stanza_2
    
    getInputLineNumber           : line 197
    ========================================================
    %}
    
    %{ CODE_FOR_SYLLABLE_DURATION_KIND_AUTOMATIC_kSyllableBegin, line 197 %}
    ris
    %{ CODE_FOR_EXTENDER_AUTOMATIC_DURATIONS_kSyllableBegin, line 197 %}
    %{ =======>
    [Syllable, fSyllableElementsList: ["ris"], fSyllableKind: kSyllableBegin, line 197, fSyllableStanzaNumber: "stanza_2", line 197:
      fSyllableKind            : kSyllableBegin
      fSyllableExtendKind      : kSyllableExtend_NONE
      fSyllableWholeNotes      : -1/1 whn
      fSyllableTupletFactor    : -1/-1
      fSyllableUpLinkToMeasure : [NULL]
      fSyllableElementsList    
        [SyllableElement
          fSyllableElementContents        : "ris"
          fSyllableElementKind            : kSyllableElementText
          fInputLineNumber                : line 199
        ]
    ]
    
    syllableUpLinkToNote        : [NULL]
    doGenerateASingleHyphen     : false
    doGenerateADoubleHyphen     : true
    %}
    --
    
    
    
    %{ ================= SYLLABLE DESCRIPTION =================
    getSyllableElementsList      :
    getSyllableKind              : kSyllableSkipOnRestNote
    getSyllableWholeNotes        : 5/4 whn
    getSyllableExtendKind        : kSyllableExtend_NONE
    
    getLyricsDurationsKind       : automatic
    fOnGoingExtend               : false
    
    noteTheSyllableIsAttachedTo  : [NULL]
    getStanzaNumber              : stanza_2
    getStanzaName                : Part_POne_Staff_One_Voice_1_Stanza_2
    
    getInputLineNumber           : line 215
    ========================================================
    %}
    
    %{ CODE_FOR_SYLLABLE_DURATION_KIND_AUTOMATIC_kSyllableSkipOnRestNote, line 215 %}
    
    
    
    %{ ================= SYLLABLE DESCRIPTION =================
    getSyllableElementsList      : en
    getSyllableKind              : kSyllableEnd
    getSyllableWholeNotes        : UNKNOWN_WHOLE_NOTES_
    getSyllableExtendKind        : kSyllableExtend_NONE
    
    getLyricsDurationsKind       : automatic
    fOnGoingExtend               : false
    
    noteTheSyllableIsAttachedTo  : [NULL]
    getStanzaNumber              : stanza_2
    getStanzaName                : Part_POne_Staff_One_Voice_1_Stanza_2
    
    getInputLineNumber           : line 215
    ========================================================
    %}
    
    %{ CODE_FOR_SYLLABLE_DURATION_KIND_AUTOMATIC_kSyllableEnd, line 215 %}
    en
    
    
    %{ ================= SYLLABLE DESCRIPTION =================
    getSyllableElementsList      :
    getSyllableKind              : kSyllableSkipOnRestNote
    getSyllableWholeNotes        : 5/4 whn
    getSyllableExtendKind        : kSyllableExtend_NONE
    
    getLyricsDurationsKind       : automatic
    fOnGoingExtend               : false
    
    noteTheSyllableIsAttachedTo  : [NULL]
    getStanzaNumber              : stanza_2
    getStanzaName                : Part_POne_Staff_One_Voice_1_Stanza_2
    
    getInputLineNumber           : line 78
    ========================================================
    %}
    
    %{ CODE_FOR_SYLLABLE_DURATION_KIND_AUTOMATIC_kSyllableSkipOnRestNote, line 78 %}
    
    
    
    %{ ================= SYLLABLE DESCRIPTION =================
    getSyllableElementsList      :
    getSyllableKind              : kSyllableMeasureEnd
    getSyllableWholeNotes        : 0/1 whn
    getSyllableExtendKind        : kSyllableExtend_NONE
    
    getLyricsDurationsKind       : automatic
    fOnGoingExtend               : false
    
    noteTheSyllableIsAttachedTo  : [NULL]
    getStanzaNumber              : stanza_2
    getStanzaName                : Part_POne_Staff_One_Voice_1_Stanza_2
    
    getInputLineNumber           : line 78
    ========================================================
    %}
    
    %{ CODE_FOR_SYLLABLE_kSyllableMeasureEnd, line 78 %}
    | % 5316  %{ noteTheSyllableIsAttachedTo is NULL %}

} %{ FOFO FOFO %}

\book {

  \score {
    <<
      
      <<
      
        \new Staff  = "Part_POne_Staff_One"
        \with {
          instrumentName = "Violins 1"
        }
        <<
          \context Voice = "Part_POne_Staff_One_Voice_One" <<
            \Part_POne_Staff_One_Voice_One
          >>
          \new Lyrics
            \with {
            }
            \lyricsto "Part_POne_Staff_One_Voice_One" { \Part_POne_Staff_One_Voice_One_Stanza_One }
          \new Lyrics
            \with {
            }
            \lyricsto "Part_POne_Staff_One_Voice_One" { \Part_POne_Staff_One_Voice_One_Stanza_Two }
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
