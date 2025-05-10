\version "2.24.4"

\header {
  workCreditTypeTitle = "MaxBarockAmbitus"
  encodingDate        = "2022-05-09"
  software            = "soundnotation"
  software            = "Dolet 6.6"
  right               = ""
  title               = "MaxBarockAmbitus"
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
  \key f \major
  \numericTimeSignature \time 4/4
  \stemDown f'''8 [
  e'''16 d''' ]
  c''' [
  bes'' a'' g'' ]
  f''8 [
  e''16 d'' ]
  \stemUp c'' [
  bes' a' g' ]
  f'8 [
  e'16 d' ]
  c'8 [
  d'16 e' ]
  f'8 [
  g'16 a' ]
  \stemDown bes' [
  c'' d'' e'' ]
   | % 3
  \barNumberCheck #3
  \break | % 1333333 \myLineBreak
  
  f''8 [
  g''16 a'' ]
  bes'' [
  c''' d''' e''' ]
  f''' [
  c''' a'' c''' ]
  a'' [
  f'' a'' f'' ]
   | % 4
  \barNumberCheck #4
  c''16 [
  f'' c'' a' ]
  \stemUp c'' [
  a' f' a' ]
  f' [
  c' f' c'' ]
  \stemDown f'' [
  c''' f'''8 ]
  
  \bar "|."
   | % 1
  \barNumberCheck #5
}

Part_POne_Staff_Two_Voice_Five = \absolute {
  \language "nederlands"
  
  \clef "bass"
  \key f \major
  \numericTimeSignature \time 4/4
  \stemUp f,,8 [
  g,,16 a,, ]
  bes,, [
  c, d, e, ]
  f,8 [
  g,16 a, ]
  bes, [
  c d e ]
  \stemDown f8 [
  g16 a ]
  bes8 [
  a16 g ]
  f8 [
  e16 d ]
  \stemUp c [
  bes, a, g, ]
   | % 3
  \barNumberCheck #3
  \break | % 1333333 \myLineBreak
  
  f,8 [
  e,16 d, ]
  c, [
  bes,, a,, g,, ]
  f,, [
  a,, c, a,, ]
  c, [
  f, c, f, ]
   | % 4
  \barNumberCheck #4
  a,16 [
  f, a, c ]
  a, [
  c f c ]
  \stemDown f [
  a f c ]
  \stemUp f, [
  c, f,,8 ]
  
  \bar "|."
   | % 5
  \barNumberCheck #5
}

\book {

  \score {
    <<
      
      
      
      <<
      
        \new PianoStaff
        \with {
          instrumentName = "Part_POne"
          shortInstrumentName = "Klav."
        }
        
        <<
        
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
