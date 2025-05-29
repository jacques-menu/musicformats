\version "2.24.4"


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
  workCreditTypeTitle = "Dynamic Levels "
  encodingDate        = "2025-05-20"
  software            = "Sibelius 20250.4"
  software            = "Dolet 6.6 for Sibelius"
  title               = "Dynamic Levels "
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
  
  % oddHeaderMarkup = ""
  % evenHeaderMarkup = ""
  % oddFooterMarkup = ""
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
  \stemUp a'2 \ppppp a' \pppp a'2 \ppp a' \pp  | % 3
  \barNumberCheck #3
  a'2 \p a' \mp  | % 4
  \barNumberCheck #4
  a'2 \mf a' \f  | % 5
  \barNumberCheck #5
  a'2 \ff a' \fff  | % 6
  \barNumberCheck #6
  \break | % 1333333 \myLineBreak
  
  a'2 \ffff a' \fffff  | % 7
  \barNumberCheck #7
  a'2 \fp a' \sf  | % 8
  \barNumberCheck #8
  a'2 -\otherDynamic "sff" a' -\markup { "sp" }  | % 9
  \barNumberCheck #9
  a'2 -\markup { "spp" } a' \sfz  | % 10
  \barNumberCheck #10
  a'1 \rfz
  \bar "|."
   | % 11
  \barNumberCheck #11
}

\book {

  \score {
    <<
      
      <<
      
        \new Staff  = "Part_POne_Staff_One"
        \with {
          instrumentName = "[Unnamed (treble staff)]"
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
