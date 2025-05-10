\version "2.24.4"

\header {
  workCreditTypeTitle = "SkipsAndKeyChange"
  movementTitle       = "Partie 1"
  encodingDate        = "2024-12-09"
  software            = "Dorico 5.1.51.2153"
  title               = "SkipsAndKeyChange"
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
  \stemUp g'4 b' c''2  | % 3
  \barNumberCheck #3
  b'4 g'  | % 4
  \barNumberCheck #4
  a'4 \key f \major
  bes' ~   | % 5
  \barNumberCheck #5
  bes'4 a'  | % 6
  \barNumberCheck #6
  g'2  | % 7
  \barNumberCheck #7
  f'2  | % 8
  \barNumberCheck #8
  R2
  \bar "|."
   | % 1
  \barNumberCheck #9
}

Part_POne_Staff_One_Voice_Two = \absolute {
  \language "nederlands"
  
  \clef "treble"
  \key g \major
  \numericTimeSignature \time 2/4
  s2 \stemDown e'4 fis'  | % 3
  \barNumberCheck #3
  g'4 e'  | % 4
  \barNumberCheck #4
  \key f \major
  s2  | % 5
  \barNumberCheck #5
  e'4 f'  | % 6
  \barNumberCheck #6
  f'4 e'  | % 7
  \barNumberCheck #7
  s2  | % 8
  \barNumberCheck #8
  
  \bar "|."
  s2  | % 9
  \barNumberCheck #9
}

\book {

  \score {
    <<
      
      
      
      <<
      
        \new Staff  = "Part_POne_Staff_One"
        \with {
          instrumentName = "Stimme"
          shortInstrumentName = "V."
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
