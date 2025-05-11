\version "2.24.4"

\header {
  movementTitle       = "Chords In Tuplet With Tuplet Stop Not On The Chord Last Note"
  encodingDate        = "2024-01-19"
  software            = "MuseScore 4.1.1"
  title               = "Chords In Tuplet With Tuplet Stop Not On The Chord Last Note"
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
  \time 4/4
  \stemDown  < f'' d'' > 4  < b' g' >   < e' c' >   < f'' d'' >   < b' g' >   < e' c' >  
  \once\override TupletBracket.bracket-visibility = ##t
  \tuplet 3/2 {   < f'' d'' > 4  < b' g' >   < e' c' >  } r2
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
