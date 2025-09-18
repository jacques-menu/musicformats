\version "2.24.4"

% Generated from "HarmoniesSharingANote.xml"
% by xml2ly 0.9.76-dev (built on September 10, 2025)
% on Wednesday 2025-09-10 @ 17:30:08 CEST

% The conversion command as supplied was: 
%  xml2ly -lilypond-run-date -lilypond-generation-infos -output-file-name HarmoniesSharingANote.ly HarmoniesSharingANote.xml
% or, with short option names:
%     HarmoniesSharingANote.ly HarmoniesSharingANote.xml


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
  movementTitle       = "Harmonies Sharing A Note"
  encodingDate        = "2018-10-19"
  software            = "Sibelius 18.5"
  software            = "Dolet 6.6 for Sibelius"
  title               = "Harmonies Sharing A Note"
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
  
  \clef "treble"
  \key a \minor
  \numericTimeSignature \time 4/4
  a1:sus2 a2:m7 c:maj7.9  | % 3
  \barNumberCheck #3
  d2:m7 g4:5.3 g:m  | % 4
  \barNumberCheck #4
  c8:maj7 s2..  | % 1
  \barNumberCheck #5
}

Part_POne_Staff_One_Voice_One = \absolute {
  \language "nederlands"
  
  \clef "treble"
  \key a \minor
  \numericTimeSignature \time 4/4
  R1 \tempo \markup {
    \concat {
       \smaller \general-align #Y #DOWN \note {4} #UP
      " = "
      130
    } % concat
  }
  \stemUp e'2 \stemDown b'  | % 3
  \barNumberCheck #3
  \stemUp a'2 g'  | % 4
  \barNumberCheck #4
  e'8 [
  d' ]
  c'4 r2  | % 1
  \barNumberCheck #5
}

Part_PTwo_Staff_One_Voice_One = \absolute {
  \language "nederlands"
  
  
  \clef "treble"
  \key a \minor
  \numericTimeSignature \time 4/4
  
  \arpeggioNormal \ottava #1 < b'' e''' b''' > 1 -\arpeggio
  
  \stemUp  < a c' e' g' > 2  < e' g' b'! d'' > 4.  < d' ~  f' ~  a' ~  c'' ~  > 8
  
   < d' f' a' c'' > 8 \stemDown  < g' ~  b' ~  d'' ~  > 4.  < g' b' d'' > 8  < g' bes'! d'' > 4.
  
  \stemUp  < e' g' b'! c'' > 2 r
}

Part_PTwo_Staff_Two_Voice_Five = \absolute {
  \language "nederlands"
  
  \clef "treble"
  \key a \minor
  \numericTimeSignature \time 4/4
  a'1
  \stemUp  < a, e > 2 \stemDown  < c e g > 4.  < d ~  f ~  a ~  > 8
  
   < d f a > 8  < g ~  b ~  d' ~  > 4.  < g b d' > 8  < g bes! d' > 4.
  
  c'2 r4 \stemUp  < a,,, ~  a,, ~  >  
}

\book {

  \score {
    <<
      
      <<
      
        \new PianoStaff
        \with {
          instrumentName = "Part_POne"
          shortInstrumentName = "Voice"
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
          
        >>
        
        \new PianoStaff
        \with {
          instrumentName = "Part_PTwo"
          shortInstrumentName = "Pno."
        }
        
        <<
        
          \new Staff  = "Part_PTwo_Staff_One"
          \with {
          }
          <<
            \context Voice = "Part_PTwo_Staff_One_Voice_One" <<
              \Part_PTwo_Staff_One_Voice_One
            >>
          >>
          \new Staff  = "Part_PTwo_Staff_Two"
          \with {
          }
          <<
            \context Voice = "Part_PTwo_Staff_Two_Voice_Five" <<
              \Part_PTwo_Staff_Two_Voice_Five
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
