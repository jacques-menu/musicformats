\version "2.24.4"

\header {
  workCreditTypeTitle = "Several Offset Harmonies On A Given Note"
  encodingDate        = "2023-02-08"
  software            = "MuseScore 3.6.2"
  title               = "Several Offset Harmonies On A Given Note"
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
  
  \clef "treble"
  \key c \major
  \numericTimeSignature \time 4/4
  f4:5.3 g:7 a2:m c2:5.3 g:5.3  | % 3
  \barNumberCheck #3
  b1:5.3
  \bar "|."  %{ b333 visitStart (S_msrBarLine& elt) %}
   | % 1
  \barNumberCheck #4
}

Part_POne_Staff_One_Voice_One = \absolute {
  \language "nederlands"
  
  \clef "treble"
  \key c \major
  \numericTimeSignature \time 4/4
  R1 \stemUp e'2 r  | % 3
  \barNumberCheck #3
  c'1
  \bar "|."  %{ b333 visitStart (S_msrBarLine& elt) %}
   | % 1
  \barNumberCheck #4
}

Part_POne_Staff_One_Voice_One_Stanza_One = \lyricmode {
  \set associatedVoice = #"Part_POne_Staff_One_Voice_One"
  \set ignoreMelismata = ##t
    | % 5316
    mi  | % 5316
    do  | % 5316

} %{ FOFO FOFO %}

Part_PTwo_Staff_One_Voice_One = \absolute {
  \language "nederlands"
  
  \clef "bass"
  \key c \major
  \numericTimeSignature \time 4/4
  \transposition c'
  \stemUp c4 b, \stemDown d e R1  | % 3
  \barNumberCheck #3
  R1
  \bar "|."  %{ b333 visitStart (S_msrBarLine& elt) %}
   | % 4
  \barNumberCheck #4
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
            \context Voice = "Part_POne_Staff_One_Voice_One" <<
              \Part_POne_Staff_One_Voice_One
            >>
            \new Lyrics
              \with {
              }
              \lyricsto "Part_POne_Staff_One_Voice_One" { \Part_POne_Staff_One_Voice_One_Stanza_One }
          >>
          
        >>
        
        \new Staff  = "Part_PTwo_Staff_One"
        \with {
          instrumentName = "Contrabass"
          shortInstrumentName = "Cb."
        }
        <<
          \context Voice = "Part_PTwo_Staff_One_Voice_One" <<
            \Part_PTwo_Staff_One_Voice_One
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
