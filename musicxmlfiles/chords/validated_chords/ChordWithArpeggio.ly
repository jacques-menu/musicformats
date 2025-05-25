\version "2.24.4"

\header {
  movementTitle       = "Chord With Arpeggio"
  encodingDate        = "2016-12-16"
  software            = "Frescobaldi 2.19.0"
  title               = "Chord With Arpeggio"
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
  \time 4/4
  
  \arpeggioNormal < e'' g'' > 4 -\arpeggio < d'' f'' >  < d'' f'' > 2
}

Part_POne_Staff_One_Voice_Two = \absolute {
  \language "nederlands"
  
  
  \clef "treble"
  \time 4/4
  < d' f' > 2 < g' b' >  
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
            \voiceOne % out of 2 regular voices
            \Part_POne_Staff_One_Voice_One
          >>
          \context Voice = "Part_POne_Staff_One_Voice_Two" <<
            \voiceTwo % out of 2 regular voices
            \Part_POne_Staff_One_Voice_Two
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
