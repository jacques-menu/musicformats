\version "2.24.4"

% Generated from "MaxBarockAmbitus.xml"
% by xml2ly v0.9.75-dev (built June 13, 2025)
% on Saturday 2025-06-14 @ 08:24:21 CEST

% The conversion command as supplied was: 
%  xml2ly -lilypond-run-date -lilypond-generation-infos -output-file-name MaxBarockAmbitus.ly MaxBarockAmbitus.xml
% or, with short option names:
%     MaxBarockAmbitus.ly MaxBarockAmbitus.xml


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
  workCreditTypeTitle = "MaxBarockAmbitus"
  encodingDate        = "2022-05-09"
  software            = "soundnotation"
  software            = "Dolet 6.6"
  right               = ""
  title               = "MaxBarockAmbitus"
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
"Score generated from MusicXML data by xml2ly v0.9.75-dev (built June 13, 2025) and LilyPond " (lilypond-version))
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
  \key f \major
  \numericTimeSignature \time 4/4
  \stemDown f'''8 [
  e'''16 d''' ]
  c''' [
  bes'' a'' g'' ]
  f''8 [
  e''16 d'' ]
  \stemUp c'' [
  bes' a' g' ]
  f'8 [
  e'16 d' ]
  c'8 [
  d'16 e' ]
  f'8 [
  g'16 a' ]
  \stemDown bes' [
  c'' d'' e'' ]
   | % 3
  \barNumberCheck #3
  \break | % -1
  
  f''8 [
  g''16 a'' ]
  bes'' [
  c''' d''' e''' ]
  f''' [
  c''' a'' c''' ]
  a'' [
  f'' a'' f'' ]
   | % 4
  \barNumberCheck #4
  c''16 [
  f'' c'' a' ]
  \stemUp c'' [
  a' f' a' ]
  f' [
  c' f' c'' ]
  \stemDown f'' [
  c''' f'''8 ]
  
  \bar "|."
   | % 1
  \barNumberCheck #5
}

Part_POne_Staff_Two_Voice_Five = \absolute {
  \language "nederlands"
  
  \clef "bass"
  \key f \major
  \numericTimeSignature \time 4/4
  \stemUp f,,8 [
  g,,16 a,, ]
  bes,, [
  c, d, e, ]
  f,8 [
  g,16 a, ]
  bes, [
  c d e ]
  \stemDown f8 [
  g16 a ]
  bes8 [
  a16 g ]
  f8 [
  e16 d ]
  \stemUp c [
  bes, a, g, ]
   | % 3
  \barNumberCheck #3
  \break | % -1
  
  f,8 [
  e,16 d, ]
  c, [
  bes,, a,, g,, ]
  f,, [
  a,, c, a,, ]
  c, [
  f, c, f, ]
   | % 4
  \barNumberCheck #4
  a,16 [
  f, a, c ]
  a, [
  c f c ]
  \stemDown f [
  a f c ]
  \stemUp f, [
  c, f,,8 ]
  
  \bar "|."
   | % 6
  \barNumberCheck #5
}

\book {

  \score {
    <<
      
      <<
      
        \new PianoStaff
        \with {
          instrumentName = "Part_POne"
          shortInstrumentName = "Klav."
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
