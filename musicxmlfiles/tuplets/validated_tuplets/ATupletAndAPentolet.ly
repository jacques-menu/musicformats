\version "2.24.4"

\header {
  workCreditTypeTitle = "A Tuplet And A Pentolet"
  encodingDate        = "2016-09-24"
  software            = "Frescobaldi 2.18.1"
  title               = "A Tuplet And A Pentolet"
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
  
  %{ begin kMeasureKindOverFlowing, measure 1, % measureAccumulatedWholeNotesDuration: 3/2 whn %}
  
  \clef "treble"
  \time 4/4
  a'2
  \once\override TupletBracket.bracket-visibility = ##t
  \tuplet 3/2 {  %{ tupletNumber: 1, tupleFactor: 3/2, line 41 %}  b'4 r %{ sr333 %}   d'' }
  %{ end kMeasureKindOverFlowing, measure  %}
  
  %{ begin kMeasureKindOverFlowing, measure 2, % measureAccumulatedWholeNotesDuration: 3/2 whn %}
  a''2
  \once\override TupletBracket.bracket-visibility = ##t
  \tuplet 5/4 {  %{ tupletNumber: 1, tupleFactor: 5/4, line 94 %}  b''8 c''' d''' e''' f''' }
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
