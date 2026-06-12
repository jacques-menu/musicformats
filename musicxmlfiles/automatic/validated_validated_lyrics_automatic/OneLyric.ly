\version "2.24.4"

% Generated from "OneLyric.xml"
% by xml2ly v2026.2 (built on June 09, 2026)
% on Tuesday 2026-06-09 @ 15:44:57 CEST

% The conversion command as supplied was: 
%  xml2ly -auto-output-file-name -lilypond-run-date -lilypond-generation-infos OneLyric.xml -comlilylyr
% or, with long option names:
%  xml2ly -auto-output-file-name -lilypond-run-date -lilypond-generation-infos OneLyric.xml -comment-lilypond-lyrics
% or, with short option names:
%      OneLyric.xml


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
  movementTitle       = "Single Lyric"
  encodingDate        = "2016-12-15"
  software            = "MuseScore 2.0.3.1"
  title               = "Single Lyric"
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
  \key c \major
  \numericTimeSignature \time 4/4
  \stemDown c''4 \tempo \markup {
    \concat {
       \smaller \general-align #Y #DOWN \note {4} #UP
      " = "
      60
    } % concat
  }
r r2 }

Part_POne_Staff_One_Voice_One_Stanza_One = \lyricmode {
  % ===> using automatic lyrics durations mode
  \set ignoreMelismata = ##t
    
    
    
    %{ ================= SYLLABLE DESCRIPTION =================
    getSyllableElementsList      : "jam... "
    getSyllableKind              : kSyllableSingle
    getSyllableWholeNotes        : UNKNOWN_WHOLE_NOTES_
    getSyllableExtendKind        : kSyllableExtend_NONE
    
    getLyricsDurationsKind       : automatic
    fOnGoingExtend               : false
    
    noteTheSyllableIsAttachedTo  : [NULL]
    getStanzaNumber              : stanza_1
    getStanzaName                : Part_POne_Staff_One_Voice_1_Stanza_1
    
    getInputLineNumber           : line 99
    ========================================================
    %}
    
    %{ CODE_FOR_SYLLABLE_DURATION_KIND_AUTOMATIC_kSyllableSingle, line 99 %}
    "jam... "
    %{ CODE_FOR_EXTENDER_AUTOMATIC_DURATIONS_kSyllableSingle, line 99 %}
    %{ =======>
    [Syllable, fSyllableElementsList: ["jam... "], fSyllableKind: kSyllableSingle, line 99, fSyllableStanzaNumber: "stanza_1", line 99:
      fSyllableKind            : kSyllableSingle
      fSyllableExtendKind      : kSyllableExtend_NONE
      fSyllableWholeNotes      : -1/1 whn
      fSyllableTupletFactor    : -1/-1
      fSyllableUpLinkToMeasure : [NULL]
      fSyllableElementsList    
        [SyllableElement
          fSyllableElementContents        : "jam... "
          fSyllableElementKind            : kSyllableElementText
          fInputLineNumber                : line 101
        ]
    ]
    
    doGenerateASingleHyphen     : false
    doGenerateASingleUnderscore : false
    doGenerateADoubleUnderscore : false
    %}
    
    
    
    
    %{ ================= SYLLABLE DESCRIPTION =================
    getSyllableElementsList      :
    getSyllableKind              : kSyllableSkipOnRestNote
    getSyllableWholeNotes        : 2/1 whn
    getSyllableExtendKind        : kSyllableExtend_NONE
    
    getLyricsDurationsKind       : automatic
    fOnGoingExtend               : false
    
    noteTheSyllableIsAttachedTo  : [NULL]
    getStanzaNumber              : stanza_1
    getStanzaName                : Part_POne_Staff_One_Voice_1_Stanza_1
    
    getInputLineNumber           : line 57
    ========================================================
    %}
    
    %{ CODE_FOR_SYLLABLE_DURATION_KIND_AUTOMATIC_kSyllableSkipOnRestNote, line 57 %}
    
    
    
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
    
    getInputLineNumber           : line 57
    ========================================================
    %}
    
    %{ CODE_FOR_SYLLABLE_kSyllableMeasureEnd, line 57 %}
    | % 5316  %{ noteTheSyllableIsAttachedTo is NULL %}

} %{ FOFO FOFO %}

\book {

  \score {
    <<
      
      <<
      
        \new Staff  = "Part_POne_Staff_One"
        \with {
          instrumentName = "Piano"
          shortInstrumentName = "Pia."
        }
        <<
          \context Voice = "Part_POne_Staff_One_Voice_One" <<
            \Part_POne_Staff_One_Voice_One
          >>
          \new Lyrics
            \with {
            }
            \lyricsto "Part_POne_Staff_One_Voice_One" { \Part_POne_Staff_One_Voice_One_Stanza_One }
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
