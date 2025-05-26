\version "2.24.4"

\header {
  workCreditTypeTitle = "Acciaccatura In Slur"
  encodingDate        = "2025-05-26"
  software            = "Sibelius 20250.4"
  software            = "Dolet 6.6 for Sibelius"
  right               = "Copyright © "
  title               = "Acciaccatura In Slur"
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
  \acciaccatura { b'8 } \stemDown e''8. [
  ( fis''16 ]
  \acciaccatura { fis''8 } a''16 [
  gis''! g''! fis'' ]
  \acciaccatura { fis''8 } b''8 [
  ) -. b'' ]
  e''4 ->
  \bar "|."
   | % 3
  \barNumberCheck #3
}

\book {

  \score {
    <<
      
      <<
      
        \new Staff  = "Part_POne_Staff_One"
        \with {
          instrumentName = "[Piano]"
          shortInstrumentName = "Pno."
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
