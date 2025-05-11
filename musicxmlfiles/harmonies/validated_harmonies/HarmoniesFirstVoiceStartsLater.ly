\version "2.24.4"

\header {
  movementTitle       = "Harmonies First Voice Starts Later"
  encodingDate        = "2018-12-21"
  software            = "soundnotation"
  software            = "Dolet 6.6"
  right               = ""
  title               = "Harmonies First Voice Starts Later"
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

Part_POne_HARMONIES_Staff_HARMONIES_VoiceEleven = \chordmode {
  \language "nederlands"
  \key c \major
  \numericTimeSignature \time 4/4
  f8.:5.3 s16*13 \break | % 1333333 \myLineBreak
  
  s1  | % 1
  \barNumberCheck #3
}

Part_POne_Staff_One_Voice_Two = \absolute {
  \language "nederlands"
  
  \clef "treble"
  \key c \major
  \numericTimeSignature \time 4/4
  \stemUp a8. [
  e'16 ]
  g8 [
  g ]
  g [
  g g g ]
  \break | % 1333333 \myLineBreak
  
  \stemDown e'8 [
  e' f' f' ]
  e'4 c'  | % 1
  \barNumberCheck #3
}

Part_POne_Staff_One_Voice_One = \absolute {
  \language "nederlands"
  
  \clef "treble"
  \key c \major
  \numericTimeSignature \time 4/4
  s1 \break | % 1333333 \myLineBreak
  
  \stemUp g'8 [
  g' a' a' ]
  g'4 e'8 [
  c' ]
   | % 3
  \barNumberCheck #3
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
        
          \context ChordNames = "Part_POne_HARMONIES_Staff_HARMONIES_VoiceEleven"
          \Part_POne_HARMONIES_Staff_HARMONIES_VoiceEleven
          
          \new Staff  = "Part_POne_Staff_One"
          \with {
          }
          <<
            \context Voice = "Part_POne_Staff_One_Voice_Two" <<
              \voiceOne % out of 2 regular voices
              \Part_POne_Staff_One_Voice_Two
            >>
            \context Voice = "Part_POne_Staff_One_Voice_One" <<
              \voiceTwo % out of 2 regular voices
              \Part_POne_Staff_One_Voice_One
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
