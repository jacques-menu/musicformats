\version "2.24.4"

\header {
  workCreditTypeTitle = "Simple Stems Directions"
  movementTitle       = "Simple Stems Directions"
  encodingDate        = "2025-02-27"
  software            = "Dorico 5.1.81.2225"
  title               = "Simple Stems Directions"
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
  \key f \major
  \numericTimeSignature \time 1/4
  \stemDown c''8. [
  \tempo \markup {
    \concat {
      ( \smaller \general-align #Y #DOWN \note {4} #UP
      " = "
      109)
    
    } % concat
  }
  e''16 ]
  
  \bar "|."
}

Part_POne_Staff_Two_Voice_Two = \absolute {
  \language "nederlands"
  
  \clef "bass"
  \key f \major
  \numericTimeSignature \time 1/4
  \tempo \markup {
    \concat {
      ( \smaller \general-align #Y #DOWN \note {4} #UP
      " = "
      109)
    
    } % concat
  }
  R4
  \bar "|."
}

\book {

  \score {
    <<
      
      
      
      <<
      
        \new PianoStaff
        \with {
          instrumentName = "Part_POne"
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
            \context Voice = "Part_POne_Staff_Two_Voice_Two" <<
              \Part_POne_Staff_Two_Voice_Two
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
