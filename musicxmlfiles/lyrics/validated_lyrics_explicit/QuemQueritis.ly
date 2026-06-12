\version "2.24.4"

% Generated from "QuemQueritis.xml"
% by xml2ly v2026.2 (built on June 02, 2026)
% on Tuesday 2026-06-02 @ 08:20:35 CEST

% The conversion command as supplied was: 
%  xml2ly -lilypond-run-date -lilypond-generation-infos -output-file-name QuemQueritis.ly QuemQueritis.xml -lilypond-lyrics-durations-mode explicit
% or, with short option names:
%     QuemQueritis.ly QuemQueritis.xml explicit


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
  movementTitle       = "Quem queritis"
  encodingDate        = "2003-03-15"
  software            = "Finale 2003 for Windows"
  software            = "Dolet for Finale 1.3"
  right               = "Copyright © 2003 Recordare LLC"
  title               = "Quem queritis"
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
  \stemUp g'4 ^\markup { "Angelus dicit:" } ( f' ) d' ( f' e' ) f' ( g' f' ) g' f' a' a' ( \stemDown c'' b' \stemUp a' ) \stemDown c'' ( \stemUp g' a' g' ) g' ( a' ) g' ( f' ) a' ( \stemDown c'' ) \stemUp a' g' \bar "||"
  
}

Part_POne_Staff_One_Voice_One_Stanza_One = \lyricmode {
  % ===> using explicit lyrics durations mode
  \set associatedVoice  = #"Part_POne_Staff_One_Voice_One#"
  \set ignoreMelismata = ##t
    Quem4 4 que4 -- 2 ri4 -- 2 tis4 in4 se4 -- pul4 -- 2. "chro,"4 __ 2. o4 4 Chri4 -- 4 sti4 -- 4 co4 -- "lae?"4 | % 5316

}

\book {

  \score {
    <<
      
      <<
      
        \new Staff  = "Part_POne_Staff_One"
        \with {
          instrumentName = "Voice"
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
