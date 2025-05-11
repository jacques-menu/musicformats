\version "2.24.4"

\header {
  workCreditTypeTitle = "Beams On Sixteenth Notes"
  movementTitle       = "Beams On Sixteenth Notes"
  encodingDate        = "2025-01-16"
  composer            = "Johann Christoph Pepusch"
  software            = "Dorico 5.1.70.2200"
  right               = "Copyright: Creative Commons Attribution Non-commercial 3.0"
  title               = "Beams On Sixteenth Notes"
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
  
  \clef "bass"
  \key f \major
  \time 4/4
  r16 ^\markup { \bold "Allegro" } \stemUp c [
  e c ]
  \stemDown f4 r16 g [
  bes d' ]
  e4
  \bar "|."
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
