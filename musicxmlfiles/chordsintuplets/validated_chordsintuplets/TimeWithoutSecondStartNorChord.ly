\version "2.24.4"

% Generated from "TimeWithoutSecondStartNorChord.xml"
% by xml2ly v0.9.74-dev (built May 29, 2025 @ 13:04)
% on Thursday 2025-05-29 @ 15:58:31 CEST

% The conversion command as supplied was: 
%  xml2ly -lilypond-run-date -lilypond-generation-infos -output-file-name TimeWithoutSecondStartNorChord.ly TimeWithoutSecondStartNorChord.xml
% or, with short option names:
%     TimeWithoutSecondStartNorChord.ly TimeWithoutSecondStartNorChord.xml


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
  workCreditTypeTitle = "wrong durations in chorded notes with time-modifications"
  miscellaneousField  = "
                Sibelius 7.0.0 exports wrong duration values
                for chorded notes in tuplets.  The exported value in this example is 256 instead of 170.
                170 is not an optimal value for a quarter note in a triplet of eighths as there
                is a rounding difference (256*2/3=170.6666...).  The same holds for an eighth note in a
                triplet of eighths (128*2/3=85.3333... instead of 86 used here).   musicxml2ly does not
                like rounding errors.  The resulting LilyPond-Code contains wild scaling durations
                and a superfluous spacer rest:
                \times 2/3 { <c'' a'>4*255/256 s512*43 r8*129/128 } r4 r2 }.
                This is bad because we have a bar check problem here!
                Sometimes music applications export suboptimal/false values for divisions and durations.
                A solution: musicxml2ly should recalculate all durations on the basis of a divisions value of 6720.
                This figure is divisible without remainder by e.g. 2, 3, 4, 5, 6, 7, 8, 16, 32, 64 and
                therefore can provide for note durations up to 256th.  A quarter note (6720) in a triplet of eighths
                would have a value of 4480 (6720*2/3); An eighth note (6720/2=3360) in a triplet of eighths would have
                a value of 2240 (3360*2/3).  (half note: 6720*2 = 13440.)
            "
  title               = "wrong durations in chorded notes with time-modifications"
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
  \key c \major
  \numericTimeSignature \time 4/4
  
  \once\override TupletBracket.bracket-visibility = ##t
  \tuplet 3/2 {  \stemUp c''4 r8 } r4 r2
}

\book {

  \score {
    <<
      
      <<
      
        \new Staff  = "Part_POne_Staff_One"
        \with {
          instrumentName = "Part_POne"
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
