\version "2.24.4"

\header {
  movementTitle       = "TupletWithGraceNotes"
  encodingDate        = "2024-11-12"
  software            = "MuseScore 4.4.3"
  title               = "TupletWithGraceNotes"
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
  \key g \major
  \numericTimeSignature \time 2/4
  r8 r \once \omit TupletBracket
  \tuplet 3/2 {  \acciaccatura { \stemUp fis''16 } \stemDown b''16 \stemUp ( [
  ) \acciaccatura { fis''16 } \stemDown b''16 \stemUp ( ) \stemDown b'' } b''8 ]
  
  \bar "|."
  
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
