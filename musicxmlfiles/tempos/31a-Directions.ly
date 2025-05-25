\version "2.24.4"

% Generated from "31a-Directions.xml"
% on Sunday 2025-05-25 @ 07:17:38 CEST
% by xml2ly v0.9.74 (built May 24, 2025 @ 03:48)

% The conversion command as supplied was: 
%  xml2ly -auto-output-file-name -lilypond-run-date -lilypond-generation-infos -auto-output-file-name -lilypond-run-date -lilypond-generation-infos 31a-Directions.xml -iln
% or, with long option names:
%  xml2ly -auto-output-file-name -lilypond-run-date -lilypond-generation-infos -auto-output-file-name -lilypond-run-date -lilypond-generation-infos 31a-Directions.xml -input-line-numbers
% or, with short option names:
%         31a-Directions.xml


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
"Score generated from MusicXML data by xml2ly v0.9.74 (built May 24, 2025 @ 03:48) and LilyPond " (lilypond-version))
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
  
  \clef "treble" %{ <-- line 38 %}
  \key c \major %{ <-- line 30 %}
  \time 4/4 %{ <-- line 34 %}
   %{ line 48 --> %} c'4  %{  %{ <-- line 48 %} %}
  \once\override Score.RehearsalMark.direction = #DOWN
  \mark\markup { \box { "A" } }
   %{ line 60 --> %} c'  %{  %{ <-- line 60 %} %}
  
  \mark\markup { \box { "B" } }
   %{ line 72 --> %} c'  %{  %{ <-- line 72 %} %}
  
  \mark\markup { \box { "Test" } }
   %{ line 84 --> %} c'  %{  %{ <-- line 84 %} %}
  
  \mark\markup { \circle { "Crc" } }
   %{ line 99 --> %} c'4  %{  %{ <-- line 99 %} %}  %{ line 111 --> %} c' \mark \markup { \musicglyph #"scripts.coda" }
   %{  %{ <-- line 111 %} %}  %{ line 123 --> %} c'  %{  %{ <-- line 123 %} %} -\markup { "words" }  %{ line 135 --> %} c'  %{  %{ <-- line 135 %} %} ^\markup {\eyeglasses}  | % 3 %{ <-- line 93 %}
  \barNumberCheck #3 %{ <-- line 93 %}
   %{ line 153 --> %} c'4  %{  %{ <-- line 153 %} %} \p  %{ line 165 --> %} c'  %{  %{ <-- line 165 %} %} \pp  %{ line 177 --> %} c'  %{  %{ <-- line 177 %} %} \ppp  %{ line 189 --> %} c'  %{  %{ <-- line 189 %} %} \pppp  | % 4 %{ <-- line 147 %}
  \barNumberCheck #4 %{ <-- line 147 %}
   %{ line 203 --> %} c'4  %{  %{ <-- line 203 %} %} \ppppp  %{ line 215 --> %} c'  %{  %{ <-- line 215 %} %} \pppppp  %{ line 227 --> %} c'  %{  %{ <-- line 227 %} %} \f  %{ line 239 --> %} c'  %{  %{ <-- line 239 %} %} \ff  | % 5 %{ <-- line 197 %}
  \barNumberCheck #5 %{ <-- line 197 %}
   %{ line 253 --> %} c'4  %{  %{ <-- line 253 %} %} \fff  %{ line 265 --> %} c'  %{  %{ <-- line 265 %} %} \ffff  %{ line 277 --> %} c'  %{  %{ <-- line 277 %} %} \fffff  %{ line 289 --> %} c'  %{  %{ <-- line 289 %} %} \ffffff  | % 6 %{ <-- line 247 %}
  \barNumberCheck #6 %{ <-- line 247 %}
   %{ line 303 --> %} c'4  %{  %{ <-- line 303 %} %} \mp  %{ line 315 --> %} c'  %{  %{ <-- line 315 %} %} \mf  %{ line 327 --> %} c'  %{  %{ <-- line 327 %} %} \sf  %{ line 339 --> %} c'  %{  %{ <-- line 339 %} %} \sfp  | % 7 %{ <-- line 297 %}
  \barNumberCheck #7 %{ <-- line 297 %}
   %{ line 353 --> %} c'4  %{  %{ <-- line 353 %} %} \sfpp  %{ line 365 --> %} c'  %{  %{ <-- line 365 %} %} \fp  %{ line 377 --> %} c'  %{  %{ <-- line 377 %} %} \rf  %{ line 389 --> %} c'  %{  %{ <-- line 389 %} %} \rfz  | % 8 %{ <-- line 347 %}
  \barNumberCheck #8 %{ <-- line 347 %}
   %{ line 403 --> %} c'4  %{  %{ <-- line 403 %} %} \sfz  %{ line 415 --> %} c'  %{  %{ <-- line 415 %} %} \sffz  %{ line 427 --> %} c'  %{  %{ <-- line 427 %} %} \fz  %{ line 439 --> %} c'  %{  %{ <-- line 439 %} %} -\otherDynamic "abc-ffz"  | % 9 %{ <-- line 397 %}
  \barNumberCheck #9 %{ <-- line 397 %}
   %{ line 455 --> %} c'4  %{  %{ <-- line 455 %} %} \<  %{ line 467 --> %} c'  %{  %{ <-- line 467 %} %} \! \once \override TextSpanner.style = #'dashed-line
   %{ line 479 --> %} c' \startTextSpan
   %{  %{ <-- line 479 %} %}  %{ line 491 --> %} c' \stopTextSpan
   %{  %{ <-- line 491 %} %}  | % 10 %{ <-- line 449 %}
  \barNumberCheck #10 %{ <-- line 449 %}
  
  \once \override Staff.LigatureBracket.edge-height = #'(0 . 0)\[  %{ line 505 --> %} c'4  %{  %{ <-- line 505 %} %} \[  %{ line 517 --> %} c'  %{  %{ <-- line 517 %} %} \] \ottava #-0  %{ line 529 --> %} c'  %{  %{ <-- line 529 %} %} \ottava #0  %{ line 541 --> %} c'  %{  %{ <-- line 541 %} %}  | % 11 %{ <-- line 499 %}
  \barNumberCheck #11 %{ <-- line 499 %}
     %{ line 555 --> %} c'4  %{  %{ <-- line 555 %} %}
    \sustainOn
     %{ line 567 --> %} c'  %{  %{ <-- line 567 %} %}
    %{ kPedalTypeChange %}
     %{ line 574 --> %} c'  %{  %{ <-- line 574 %} %}  %{ line 585 --> %} c'  %{  %{ <-- line 585 %} %}
    \sustainOff
   | % 12 %{ <-- line 549 %}
  \barNumberCheck #12 %{ <-- line 549 %}
   %{ line 600 --> %} c'4  %{  %{ <-- line 600 %} %} \tempo \markup {
    \concat {
       \smaller \general-align #Y #DOWN \note {4} #UP
      " = "
      60
    } % concat
  )
    _\markup { \harp-pedal #"-^^|--v^" }
   %{ line 641 --> %} c'  %{  %{ <-- line 641 %} %}  %{ line 653 --> %} c'  %{  %{ <-- line 653 %} %} ^\markup {\damp}  %{ line 665 --> %} c'  %{  %{ <-- line 665 %} %} ^\markup {\dampAll}  | % 13 %{ <-- line 594 %}
  \barNumberCheck #13 %{ <-- line 594 %}
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
     %{ line 710 --> %} c'4  %{  %{ <-- line 710 %} %}
    }
  >>
  \discant "121"  %{ line 726 --> %} c'  %{  %{ <-- line 726 %} %}  %{ line 733 --> %} r2 %{ <-- line 733 %}   %{  %{ <-- line 733 %} %} \bar "||"  %{ <-- line 739 %}
   %{ <-- line 739 %}  | % 14 %{ <-- line 673 %}
  \barNumberCheck #14 %{ <-- line 673 %}
   %{ line 759 --> %} c'4  %{  %{ <-- line 759 %} %} _\markup { \italic "subito" } _\markup { " " } \p  %{ line 775 --> %} c'  %{  %{ <-- line 775 %} %} \ppp _\<  %{ line 791 --> %} c'  %{  %{ <-- line 791 %} %} \fff \!  %{ line 798 --> %} r %{ <-- line 798 %}   %{  %{ <-- line 798 %} %}
  \bar "|."  %{ <-- line 805 %}
   %{ <-- line 805 %}  | % 15 %{ <-- line 744 %}
  \barNumberCheck #15 %{ <-- line 744 %}
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
