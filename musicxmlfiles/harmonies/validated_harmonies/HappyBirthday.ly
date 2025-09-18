\version "2.24.4"

% Generated from "HappyBirthday.xml"
% by xml2ly 0.9.76-dev (built on September 10, 2025)
% on Wednesday 2025-09-10 @ 17:30:07 CEST

% The conversion command as supplied was: 
%  xml2ly -lilypond-run-date -lilypond-generation-infos -output-file-name HappyBirthday.ly HappyBirthday.xml
% or, with short option names:
%     HappyBirthday.ly HappyBirthday.xml


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
  workCreditTypeTitle = "Happy Birthday"
  encodingDate        = "2016-11-21"
  software            = "MuseScore 2.0.3.1"
  title               = "Happy Birthday"
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
"Score generated from MusicXML data by xml2ly 0.9.76-dev (built on September 10, 2025) and LilyPond " (lilypond-version))
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

Part_POne_HARMONIES_Staff_HARMONIES_VoiceEleven = \chordmode {
  \language "nederlands"
  \key c \major
  \numericTimeSignature \time 3/4
  s4 c:5.3 s s4 g2:7  | % 3
  \barNumberCheck #3
  s2.  | % 4
  \barNumberCheck #4
  s4 c2:5.3  | % 5
  \barNumberCheck #5
  s2.  | % 6
  \barNumberCheck #6
  \break | % -1
  
  s2.  | % 7
  \barNumberCheck #7
  s2.  | % 8
  \barNumberCheck #8
  
  s2.
  \bar "|."
  s
}

Part_POne_Staff_One_Voice_One = \absolute {
  \language "nederlands"
  
  \clef "treble"
  \key c \major
  \numericTimeSignature \time 3/4
  \stemUp g'8 [
  g' ]
  a'4 g' \stemDown c''4 b'2  | % 3
  \barNumberCheck #3
  \stemUp g'8 [
  g' ]
  a'4 g'  | % 4
  \barNumberCheck #4
  \stemDown d''4 c''2  | % 5
  \barNumberCheck #5
  \stemUp g'8 [
  g' ]
  \stemDown g''4 e''  | % 6
  \barNumberCheck #6
  \break | % -1
  
  c''4 b' \stemUp a'  | % 7
  \barNumberCheck #7
  \stemDown f''8 [
  f'' ]
  e''4 c''  | % 8
  \barNumberCheck #8
  d''4 c''2
  \bar "|."
   | % 1
  \barNumberCheck #9
}

Part_POne_Staff_Two_Voice_Five = \absolute {
  \language "nederlands"
  
  
  \clef "bass"
  \key c \major
  \numericTimeSignature \time 3/4
  r4 \stemDown  < c e g > 2
  
  r4  < d f g > 2
  
  r4  < b, f g > 2
  
  r4  < c e g > 2
  
  r4  < e ais! c' > 2
  
  \break | % -1
  
  r4  < f a c' > 2
  
  r4  < e g c' > 2
  
   < d f g > 4  < c e g > 2
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
        
          \context ChordNames = "Part_POne_HARMONIES_Staff_HARMONIES_VoiceEleven"
          \Part_POne_HARMONIES_Staff_HARMONIES_VoiceEleven
          
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
