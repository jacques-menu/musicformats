\version "2.24.4"


% Scheme function(s): "tupletsCurvedBrackets"
% A function to draw curved tuplets brackets, not native to LilyPond.
% Thanks to Ben, mailto:soundsfromsound@gmail.com

tupletsCurvedBrackets = {
  % Use slur-stencil
  \override TupletBracket.stencil = #ly:slur::print
  %% Use 'thickness from Slur
  \override TupletBracket.thickness = #1.2
  %% 'control-points need to be set
  \override TupletBracket.control-points =
  #(lambda (grob)
     (let* ((x-pos (ly:grob-property grob 'X-positions))
            (pos (ly:grob-property grob 'positions))
            (x-ln (interval-length x-pos))
            (dir (ly:grob-property grob 'direction))
            ; read out the height of the TupletBracket, may be
            ; negative!
            (height (- (cdr pos) (car pos)))
            ; height-corr is introduced because sometimes the shape
            ; of the slur needs to be adjusted.
            ; It is used in the 2nd/3rd control-point.
            ; The value of 0.3 is found by trial and error
            (height-corr (* 0.3 dir height))
            (edge-height (ly:grob-property grob 'edge-height
                           '(0.7 . 0.7)))
            (pad 1.0))
       (list
        ; first cp
        (cons
         (+ (car x-pos) 0.5)
         (- (+ (* dir pad) (+ (car pos) (* -1 dir
                                          (car edge-height))))
           (if (= dir -1)
               (if (> height 3)
                   (/ dir 2.0)
                   0.0)
               (if (< height -3)
                   (/ dir 2.0)
                   0.0))))
        ; second cp
        (cons
         (+ (car x-pos) (* x-ln 1/4))
         (+ (* dir pad) (+ (car pos) (* dir (+ 0.5 height-corr)))))
        ; third cp
        (cons
         (+ (car x-pos) (* x-ln 3/4))
         (+ (* dir pad) (+ (cdr pos) (* dir (- 0.5 height-corr)))))
        ; fourth cp
        (cons
         (- (cdr x-pos) 0.5)
         (+ (* dir pad) (+ (cdr pos) (* -1 dir (cdr edge-height)))))
        )))
  \override TupletBracket.staff-padding = #' ()
  #(define (invert-direction x) (if (eq? UP
                                         (ly:tuplet-bracket::calc-direction x)) DOWN UP))
  % \override TupletBracket.direction = #invert-direction
}


\header {
  movementTitle       = "Curved Tuplets"
  encodingDate        = "2016-09-24"
  software            = "Frescobaldi 2.18.1"
  title               = "Curved Tuplets"
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
  \time 2/4
  
  \once\override TupletBracket.bracket-visibility = ##t
  \temporary \tupletsCurvedBrackets
  \tuplet 3/2 {  b'4 r d'' } \undo \tupletsCurvedBrackets
  
  
  
  \clef "treble"
  
  \once\override TupletBracket.bracket-visibility = ##t
  \temporary \tupletsCurvedBrackets
  \once \override TupletNumber.text = #tuplet-number::calc-fraction-text
  \tuplet 3/2 {  b'4 r d'' } \undo \tupletsCurvedBrackets
  
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
