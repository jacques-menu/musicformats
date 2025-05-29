\version "2.24.4"

% Generated from "31a-Directions.xml"
% by xml2ly v0.9.74 (built May 26, 2025 @ 16:51)
% on Wednesday 2025-05-28 @ 10:56:12 CEST

% The conversion command as supplied was: 
%  xml2ly -lilypond-run-date -lilypond-generation-infos -output-file-name 31a-Directions.ly 31a-Directions.xml
% or, with short option names:
%     31a-Directions.ly 31a-Directions.xml


% Scheme function(s): "dampAllMarkup"
% A function to create damp all markups,

dampAll = \markup
%% do not use 'fontsize
%\scale #'(5 . 5)
{
  \combine \bold "O"
  \path #0.2
  #'((moveto -.4 .8)(lineto 2.2 .8)
      (closepath)
      (moveto .9 -.5)(lineto .9 2.1))
}



% Scheme function(s): "dampMarkup"
% A function to create damp markups,

damp = \markup {
%  \scale #'(5 . 5)
  {
    \center-column {
      {
        \override #'(thickness . 1.8)
        \combine \draw-line #'(-1.5 . 0)
        \combine \draw-line #'(0 . -1.5)
        \combine \draw-line #'(0 . 1.5)
        \combine \draw-line #'(1.5 . 0)
        \draw-circle #0.8 #0.2 ##f
      }
    }
  }

}



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



% Scheme function(s): "dynamics"
% Creates variables define dynamics not native to LilyPond.

rf = #(make-dynamic-script "rf")
sfpp = #(make-dynamic-script "sfpp")
sffz = #(make-dynamic-script "sffz")
ppppp = #(make-dynamic-script "ppppp")
pppppp = #(make-dynamic-script "pppppp")
fffff = #(make-dynamic-script "fffff")
ffffff = #(make-dynamic-script "ffffff")



% Scheme function(s): "otherDynamic"
% \\otherDynamic to handle any string as a dynamic.

#(use-modules (ice-9 regex))

otherDynamic =
#(define-event-function (parser location text) (markup?)
   (if (string? text)
       (let* ((underscores-replaced
               (string-map
                (lambda (x) (if (eq? x #\_) #\space x))
                text))
              (split-text (string-split underscores-replaced #\space))
              (formatted (map
                          (lambda (word)
                            (if (string-match "^[mrzfps]*$" word)
                                (markup #:dynamic word)
                                (markup #:normal-text #:italic word)))
                          split-text)))
         #{
           #(make-dynamic-script (make-line-markup formatted))
         #})
       ; user provided a full-blown markup, so we don't mess with it:
       #{
         #(make-dynamic-script (markup #:normal-text text))
       #}))


\header {
  movementTitle       = "MusicXML directions (attached to staff)"
  miscellaneousField  = "All <direction> elements 
          defined in MusicXML. The lyrics for each note describes the direction
          element assigned to that note."
  title               = "MusicXML directions (attached to staff)"
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
  
  \clef "treble"
  \key c \major
  \time 4/4
  c'4
  \once\override Score.RehearsalMark.direction = #DOWN
  \mark\markup { \box { "A" } }
  c'
  
  \mark\markup { \box { "B" } }
  c'
  
  \mark\markup { \box { "Test" } }
  c'
  
  \mark\markup { \circle { "Crc" } }
  c'4 c' \mark \markup { \musicglyph #"scripts.coda" }
  c' -\markup { "words" } c' ^\markup {\eyeglasses}  | % 3
  \barNumberCheck #3
  c'4 \p c' \pp c' \ppp c' \pppp  | % 4
  \barNumberCheck #4
  c'4 \ppppp c' \pppppp c' \f c' \ff  | % 5
  \barNumberCheck #5
  c'4 \fff c' \ffff c' \fffff c' \ffffff  | % 6
  \barNumberCheck #6
  c'4 \mp c' \mf c' \sf c' \sfp  | % 7
  \barNumberCheck #7
  c'4 \sfpp c' \fp c' \rf c' \rfz  | % 8
  \barNumberCheck #8
  c'4 \sfz c' \sffz c' \fz c' -\otherDynamic "abc-ffz"  | % 9
  \barNumberCheck #9
  c'4 \< c' \! \once \override TextSpanner.style = #'dashed-line
  c' \startTextSpan
  c' \stopTextSpan
   | % 10
  \barNumberCheck #10
  
  \once \override Staff.LigatureBracket.edge-height = #'(0 . 0)\[ c'4 \[ c' \] \ottava #-0 c' \ottava #0 c'  | % 11
  \barNumberCheck #11
    c'4
    \sustainOn
    c'
    %{ kPedalTypeChange %}
    c' c'
    \sustainOff
   | % 12
  \barNumberCheck #12
  c'4 \tempo \markup {
    \concat {
       \smaller \general-align #Y #DOWN \note {4} #UP
      " = "
      60
    } % concat
  }
    _\markup { \harp-pedal #"-^^|--v^" }
  c' c' ^\markup {\damp} c' ^\markup {\dampAll}  | % 13
  \barNumberCheck #13
  <<
      \new Staff  \with { alignAboveContext = "Part_POne_Staff_One" } {
        \hide  Staff.Stem
        \hide  Staff.TimeSignature
        \small
        \once \override Score.RehearsalMark.self-alignment-X = #LEFT
        \mark\markup {\small\bold "Scordatura"}
        <ees, a dis g b e>4
      }
    {
    c'4
    }
  >>
  \discant "121" c' r2 \bar "||"
   | % 14
  \barNumberCheck #14
  c'4 _\markup { \italic "subito" } _\markup { " " } \p c' \ppp _\< c' \fff \! r
  \bar "|."
   | % 15
  \barNumberCheck #15
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
