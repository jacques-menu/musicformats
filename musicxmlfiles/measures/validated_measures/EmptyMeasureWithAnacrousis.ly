\version "2.24.4"

\header {
  encodingDate        = "2025-05-05"
  software            = "Sibelius 20250.4"
  software            = "Dolet 6.6 for Sibelius"
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
  \partial 4
  
  \clef "treble"
  \key c \major
  \numericTimeSignature \time 3/4
  \stemUp g'4  | % 1
  \barNumberCheck #1
  \stemDown c''4 b' \stemUp a'  | % 2
  \barNumberCheck #2
  g'4 d' g'  | % 3
  \barNumberCheck #3
  f'4 c' f'  | % 4
  \barNumberCheck #4
  e'2.
  \bar "|."
   | % 1
  \barNumberCheck #5
}

Part_POne_Staff_One_Voice_Two = \absolute {
  \language "nederlands"
  
  \clef "treble"
  \key c \major
  \numericTimeSignature \time 3/4
  s2. s2.  | % 3
  \barNumberCheck #3
  \stemDown b2 bes!4  | % 4
  \barNumberCheck #4
  a2 b!4  | % 5
  \barNumberCheck #5
  c'2.
  \bar "|."
   | % 6
  \barNumberCheck #6
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
