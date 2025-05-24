\version "2.24.4"

\header {
  workCreditTypeTitle = "TupletTest4"
  title               = "TupletTest4"
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
  \key e \minor
  \numericTimeSignature \time 2/4
  \once \omit TupletBracket
  \once \omit TupletNumber
  \tuplet 3/2 {  \stemUp fis''8 [
  ( e'' ) c'' ]
  \< } \once \omit TupletBracket
  \once \omit TupletNumber
  \tuplet 3/2 {  fis''8 [
  ( e'' ) c'' ]
  }
  
  \once \omit TupletBracket
  \once \omit TupletNumber
  \tuplet 3/2 {  fis''8 [
  ( e'' \! ) c'' ]
  } \once \omit TupletBracket
  \once \omit TupletNumber
  \tuplet 3/2 {  fis''8 [
  ( e'' ) c'' ]
  }
  \bar "|."
  
}

Part_POne_Staff_One_Voice_Two = \absolute {
  \language "nederlands"
  
  
  \clef "treble"
  \key e \minor
  \numericTimeSignature \time 2/4
  \stemDown  < e' ~  ais'! ~  > 2
  
   < e' ais' > 2
  \bar "|."
  
}

\book {

  \score {
    <<
      
      <<
      
        \new Staff  = "Part_POne_Staff_One"
        \with {
          instrumentName = "Piano"
          shortInstrumentName = "Pno."
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
