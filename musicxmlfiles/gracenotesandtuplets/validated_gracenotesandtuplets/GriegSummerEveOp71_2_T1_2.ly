\version "2.24.4"

% Generated from "GriegSummerEveOp71_2_T1_2.xml"
% by xml2ly v0.9.74-dev (built May 29, 2025 @ 13:04)
% on Thursday 2025-05-29 @ 16:03:04 CEST

% The conversion command as supplied was: 
%  xml2ly -lilypond-run-date -lilypond-generation-infos -output-file-name GriegSummerEveOp71_2_T1_2.ly GriegSummerEveOp71_2_T1_2.xml
% or, with short option names:
%     GriegSummerEveOp71_2_T1_2.ly GriegSummerEveOp71_2_T1_2.xml


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
"Score generated from MusicXML data by xml2ly v0.9.74-dev (built May 29, 2025 @ 13:04) and LilyPond " (lilypond-version))
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
  \key des \major
  \numericTimeSignature \time 3/4
  \stemUp f''4 \p \tempo \markup {
    "Allegretto tranquillamente. M.M."\concat {
       \smaller \general-align #Y #DOWN \note {4} #UP
      " = "
      69
    } % concat
  }
  bes'2
  \once \omit TupletBracket
  \tuplet 3/2 {  f''8 [
  \< ( \grace { ees''16  f''16 } ees''8 des'' ]
  ) } bes'2
  \bar "|."
  
}

Part_POne_Staff_One_Voice_Two = \absolute {
  \language "nederlands"
  
  \clef "treble"
  \key des \major
  \numericTimeSignature \time 3/4
  \stemDown f'4 f'2 f'4 f'2 \!
  \bar "|."
   | % 1
  \barNumberCheck #3
}

Part_POne_Staff_Two_Voice_Five = \absolute {
  \language "nederlands"
  
  
  \clef "bass"
  \key des \major
  \numericTimeSignature \time 3/4
  \stemDown  < des aes > 4  < ges des' > 2
  
   < des aes > 4  < ges des' > 2
  \bar "|."
  
}

\book {

  \score {
    <<
      
      <<
      
        \new PianoStaff
        \with {
          instrumentName = "Part_POne"
          shortInstrumentName = "Pno."
        }
        
        <<
        
          \new Staff  = "Part_POne_Staff_One"
          \with {
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
          
          \new Staff  = "Part_POne_Staff_Two"
          \with {
          }
          <<
            \context Voice = "Part_POne_Staff_Two_Voice_Five" <<
              \Part_POne_Staff_Two_Voice_Five
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
