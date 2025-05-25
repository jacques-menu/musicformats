\version "2.24.4"

\header {
  workCreditTypeTitle = "Staff Group"
  encodingDate        = "2024-04-16"
  software            = "Sibelius 20231.1"
  software            = "Dolet 6.6 for Sibelius"
  title               = "Staff Group"
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
  \numericTimeSignature \time 2/4
  \stemDown  < a' e'' > 2
  \bar "|."
  
}

Part_PTwo_Staff_One_Voice_One = \absolute {
  \language "nederlands"
  
  
  \clef "treble"
  \key c \major
  \numericTimeSignature \time 2/4
  \stemUp  < g d' > 2
  \bar "|."
  
}

\book {

  \score {
    <<
      
      <<
      
        \new StaffGroup
        
        <<
        
          \new Staff  = "Part_POne_Staff_One"
          \with {
            instrumentName = "Violine I"
            shortInstrumentName = "Vl. I"
          }
          <<
            \context Voice = "Part_POne_Staff_One_Voice_One" <<
              \Part_POne_Staff_One_Voice_One
            >>
          >>
          \new Staff  = "Part_PTwo_Staff_One"
          \with {
            instrumentName = "Violine II"
            shortInstrumentName = "Vl. II"
          }
          <<
            \context Voice = "Part_PTwo_Staff_One_Voice_One" <<
              \Part_PTwo_Staff_One_Voice_One
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
