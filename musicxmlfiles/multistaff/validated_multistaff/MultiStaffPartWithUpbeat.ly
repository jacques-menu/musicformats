\version "2.24.4"

% Generated from "MultiStaffPartWithUpbeat.xml"
% on Wednesday 2025-05-28 @ 09:25:50 CEST
% by xml2ly v0.9.74 (built May 26, 2025 @ 16:51)

% The conversion command as supplied was: 
%  xml2ly -lilypond-run-date -lilypond-generation-infos -output-file-name MultiStaffPartWithUpbeat.ly MultiStaffPartWithUpbeat.xml
% or, with short option names:
%     MultiStaffPartWithUpbeat.ly MultiStaffPartWithUpbeat.xml


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
  workCreditTypeTitle = "Multi-staff Part With Upbeat"
  encodingDate        = "2016-10-24"
  software            = "MuseScore 2.0.3"
  title               = "Multi-staff Part With Upbeat"
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
  \partial 2.
  
  \clef "treble"
  \key b \major
  \numericTimeSignature \time 4/4
  \stemUp gis'2.  | % 1
  \barNumberCheck #1
  fis'4 gis' gis' ais'  | % 2
  \barNumberCheck #2
  \stemDown b'4 \stemUp ais' ais' gis'  | % 3
  \barNumberCheck #3
  fis'4 e' e' dis'  | % 4
  \barNumberCheck #4
  e'4 fis' fis' gis'  | % 5
  \barNumberCheck #5
  ais'4 \stemDown b' b' cis''  | % 0
  \barNumberCheck #6
}

Part_POne_Staff_Two_Voice_Five = \absolute {
  \language "nederlands"
  \partial 2.
  
  \clef "bass"
  \key b \major
  \numericTimeSignature \time 4/4
  \stemUp b,2.  | % 1
  \barNumberCheck #1
  b,8 [
  b, b, ]
  cis4. r4  | % 2
  \barNumberCheck #2
  \stemDown dis8 [
  dis dis ]
  e4. r4  | % 3
  \barNumberCheck #3
  dis8 [
  dis dis ]
  \stemUp cis2 r8  | % 4
  \barNumberCheck #4
  b,8 [
  b, b, ]
  ais,2 r8  | % 5
  \barNumberCheck #5
  ais,8 [
  ais, ais, ais, ]
  b,2  | % 0
  \barNumberCheck #6
}

Part_POne_Staff_Three_Voice_Nine = \absolute {
  \language "nederlands"
  \partial 2.
  
  \clef "bass"
  \key b \major
  \numericTimeSignature \time 4/4
  r2.  | % 1
  \barNumberCheck #1
  R1  | % 2
  \barNumberCheck #2
  R1  | % 3
  \barNumberCheck #3
  R1  | % 4
  \barNumberCheck #4
  \stemDown dis8 [
  dis dis ]
  \stemUp cis4. ais,4  | % 5
  \barNumberCheck #5
  ais,1 ~   | % 6
  \barNumberCheck #6
}

\book {

  \score {
    <<
      
      <<
      
        \new PianoStaff
        \with {
          instrumentName = "Part_POne"
          shortInstrumentName = "Org."
        }
        
        <<
        
          \new Staff  = "Part_POne_Staff_One"
          \with {
          }
          <<
            \context Voice = "Part_POne_Staff_One_Voice_One" <<
              \Part_POne_Staff_One_Voice_One
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
          
          \new Staff  = "Part_POne_Staff_Three"
          \with {
          }
          <<
            \context Voice = "Part_POne_Staff_Three_Voice_Nine" <<
              \Part_POne_Staff_Three_Voice_Nine
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
