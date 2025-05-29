\version "2.24.4"

% Generated from "VariousClefs.xml"
% by xml2ly v0.9.74-dev (built May 29, 2025 @ 09:33)
% on Thursday 2025-05-29 @ 14:51:38 CEST

% The conversion command as supplied was: 
%  xml2ly -lilypond-run-date -lilypond-generation-infos -output-file-name VariousClefs.ly VariousClefs.xml
% or, with short option names:
%     VariousClefs.ly VariousClefs.xml


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
  workCreditTypeTitle = "Various Clefs"
  miscellaneousField  = "Various clefs: G, C, F, percussion,
          TAB and none; some are also possible with transposition and on other
          staff lines than their default (e.g. soprano/alto/tenor/baritone C
          clefs); Each measure shows a different clef (measure 17 has the \"none\"
          clef), only measure 18 has the same treble clef as measure
          1."
  title               = "Various Clefs"
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
"Score generated from MusicXML data by xml2ly v0.9.74-dev (built May 29, 2025 @ 09:33) and LilyPond " (lilypond-version))
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
  c'1
  \clef "alto"
  c'1  | % 3
  \barNumberCheck #3
  
  \clef "tenor"
  c'1  | % 4
  \barNumberCheck #4
  
  \clef "bass"
  c'1  | % 5
  \barNumberCheck #5
  
  \clef "percussion"
  c'1  | % 6
  \barNumberCheck #6
  
  \clef "treble_8"
  c'1  | % 7
  \barNumberCheck #7
  
  \clef "bass_8"
  c'1  | % 8
  \barNumberCheck #8
  
  \clef "varbaritone"
  c'1  | % 9
  \barNumberCheck #9
  
  \clef "french"
  c'1  | % 10
  \barNumberCheck #10
  
  \clef "baritone"
  c'1  | % 11
  \barNumberCheck #11
  
  \clef "mezzosoprano"
  c'1  | % 12
  \barNumberCheck #12
  
  \clef "soprano"
  c'1  | % 13
  \barNumberCheck #13
  
  \clef "percussion"
  c'1  | % 14
  \barNumberCheck #14
  
  \clef "treble^8"
  c'1  | % 15
  \barNumberCheck #15
  
  \clef "bass^8"
  c'1  | % 16
  \barNumberCheck #16
  
  \clef "tab"
  
  c'1  | % 17
  \barNumberCheck #17
  
  c'1  | % 18
  \barNumberCheck #18
  
  \clef "treble"
  c'1
  \bar "|."
   | % 19
  \barNumberCheck #19
}

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
