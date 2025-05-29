\version "2.24.4"

% Generated from "SATBExample.xml"
% on Wednesday 2025-05-28 @ 09:25:52 CEST
% by xml2ly v0.9.74 (built May 26, 2025 @ 16:51)

% The conversion command as supplied was: 
%  xml2ly -lilypond-run-date -lilypond-generation-infos -output-file-name SATBExample.ly SATBExample.xml
% or, with short option names:
%     SATBExample.ly SATBExample.xml


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
  workCreditTypeTitle = "SATB Example"
  encodingDate        = "2024-01-05"
  software            = "MuseScore 4.1.1"
  title               = "SATB Example"
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
"Score generated from MusicXML data by xml2ly v0.9.74 (built May 26, 2025 @ 16:51) and LilyPond " (lilypond-version))
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
  \partial 4
  
  \clef "treble"
  \key c \major
  \time 4/4
  \stemUp c''4 \tempo \markup {
    \concat {
       \smaller \general-align #Y #DOWN \note {4} #UP
      " = "
      80
    } % concat
  }
   | % 2
  \barNumberCheck #1
  e''4. e''8 g''4 g''  | % 3
  \barNumberCheck #2
  a''4 a'' g''
  \bar "|."
}

Part_POne_Staff_One_Voice_Two = \absolute {
  \language "nederlands"
  \partial 4
  
  \clef "treble"
  \key c \major
  \time 4/4
  \tempo \markup {
    \concat {
       \smaller \general-align #Y #DOWN \note {4} #UP
      " = "
      80
    } % concat
  }
  \stemDown c''4  | % 1
  \barNumberCheck #1
  c''4. c''8 e''4 e''  | % 2
  \barNumberCheck #2
  f''4 f'' e''
  \bar "|."
}

Part_PTwo_Staff_One_Voice_One = \absolute {
  \language "nederlands"
  \partial 4
  
  \clef "bass"
  \key c \major
  \time 4/4
  \stemUp e4  | % 2
  \barNumberCheck #1
  g4. g8 c'4. b8  | % 3
  \barNumberCheck #2
  a8 [
  b c' d' ]
  e'4
  \bar "|."
}

Part_PTwo_Staff_One_Voice_Two = \absolute {
  \language "nederlands"
  \partial 4
  
  \clef "bass"
  \key c \major
  \time 4/4
  \stemDown c4  | % 1
  \barNumberCheck #1
  c4. c8 c4 c  | % 2
  \barNumberCheck #2
  f8 [
  g a b ]
  c'4
  \bar "|."
}

\book {

  \score {
    <<
      
      <<
      
        \new StaffGroup
        
        <<
        
          \new Staff  = "Part_POne_Staff_One"
          \with {
            instrumentName = "Part_POne"
          }
          <<
            \context Voice = "Part_POne_Staff_One_Voice_One" <<
              \voiceOne % out of 2 regular voices
              \Part_POne_Staff_One_Voice_One
            >>
            \context Voice = "Part_POne_Staff_One_Voice_Two" <<
              \voiceTwo % out of 2 regular voices
              \Part_POne_Staff_One_Voice_Two
            >>
          >>
          \new Staff  = "Part_PTwo_Staff_One"
          \with {
            instrumentName = "Part_PTwo"
          }
          <<
            \context Voice = "Part_PTwo_Staff_One_Voice_One" <<
              \voiceOne % out of 2 regular voices
              \Part_PTwo_Staff_One_Voice_One
            >>
            \context Voice = "Part_PTwo_Staff_One_Voice_Two" <<
              \voiceTwo % out of 2 regular voices
              \Part_PTwo_Staff_One_Voice_Two
            >>
          >>
        
        
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
