\version "2.24.4"

\header {
  movementTitle       = "Nested Tuplets With Stops Out Of Order"
  encodingDate        = "2016-12-04"
  software            = "Frescobaldi 2.18.1"
  title               = "Nested Tuplets With Stops Out Of Order"
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
  
  %{ begin kMeasureKindOverFlowing, measure 1, % measureCurrentPositionInMeasure: pim 13/10 %}
  
  \clef "treble"
  \time 4/4
  c''4
  \once\override TupletBracket.bracket-visibility = ##t
  \tuplet 5/4 {  %{ tupletNumber: 1, tupleFactor: 5/4, line 39 %}  d''8 e'' f''
  \once\override TupletBracket.bracket-visibility = ##t
  \tuplet 3/2 {  %{ tupletNumber: 2, tupleFactor: 3/2, line 81 %}  g''8 a'' b'' } } c'''4
  %{ end kMeasureKindOverFlowing, measure  %}
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
