\version "2.24.4"

\header {
  workCreditTypeTitle = "Happy Birthday"
  encodingDate        = "2016-11-21"
  software            = "MuseScore 2.0.3.1"
  title               = "Happy Birthday"
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
  \numericTimeSignature \time 3/4
  s4 c:5.3 s s4 g2:7  | % 3
  \barNumberCheck #3
  s2.  | % 4
  \barNumberCheck #4
  s4 c2:5.3  | % 5
  \barNumberCheck #5
  s2.  | % 6
  \barNumberCheck #6
  \break | % 1333333 \myLineBreak
  
  s2.  | % 7
  \barNumberCheck #7
  s2.  | % 8
  \barNumberCheck #8
  
  s2.
  \bar "|."
  s
}

Part_POne_Staff_One_Voice_One = \absolute {
  \language "nederlands"
  
  \clef "treble"
  \key c \major
  \numericTimeSignature \time 3/4
  \stemUp g'8 [
  g' ]
  a'4 g' \stemDown c''4 b'2  | % 3
  \barNumberCheck #3
  \stemUp g'8 [
  g' ]
  a'4 g'  | % 4
  \barNumberCheck #4
  \stemDown d''4 c''2  | % 5
  \barNumberCheck #5
  \stemUp g'8 [
  g' ]
  \stemDown g''4 e''  | % 6
  \barNumberCheck #6
  \break | % 1333333 \myLineBreak
  
  c''4 b' \stemUp a'  | % 7
  \barNumberCheck #7
  \stemDown f''8 [
  f'' ]
  e''4 c''  | % 8
  \barNumberCheck #8
  d''4 c''2
  \bar "|."
   | % 1
  \barNumberCheck #9
}

Part_POne_Staff_Two_Voice_Five = \absolute {
  \language "nederlands"
  
  \clef "bass"
  \key c \major
  \numericTimeSignature \time 3/4
  r4 \stemDown  < c e g > 2 r4  < d f g > 2  | % 3
  \barNumberCheck #3
  r4  < b, f g > 2  | % 4
  \barNumberCheck #4
  r4  < c e g > 2  | % 5
  \barNumberCheck #5
  r4  < e ais! c' > 2  | % 6
  \barNumberCheck #6
  \break | % 1333333 \myLineBreak
  
  r4  < f a c' > 2  | % 7
  \barNumberCheck #7
  r4  < e g c' > 2  | % 8
  \barNumberCheck #8
   < d f g > 4  < c e g > 2
  \bar "|."
   | % 9
  \barNumberCheck #9
}

\book {

  \score {
    <<
      
      
      
      <<
      
        \new PianoStaff
        \with {
          instrumentName = "Part_POne"
          shortInstrumentName = "Pno."
        }
        
        <<
        
          \context ChordNames = "Part_POne_HARMONIES_Staff_HARMONIES_VoiceEleven"
          \Part_POne_HARMONIES_Staff_HARMONIES_VoiceEleven
          
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
            \context Voice = "Part_POne_Staff_Two_Voice_Five" <<
              \Part_POne_Staff_Two_Voice_Five
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
