\version "2.24.4"

\header {
  workCreditTypeTitle = "MaxBarockAmbitus_ORIGINAL"
  encodingDate        = "2022-05-09"
  software            = "soundnotation"
  software            = "Dolet 6.6"
  right               = ""
  title               = "MaxBarockAmbitus_ORIGINAL"
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

_Staff_One_Voice_One = \absolute {
  \language "nederlands"
  
  \clef "treble"
  \key f \major
  \numericTimeSignature \time 4/4
  \stemDown f'''8 [  %{ beam 1, line 126 %}
  e'''16 d''' ]  %{ beam 1, line 151 %}
  c''' [  %{ beam 1, line 164 %}
  bes'' a'' g'' ]  %{ beam 1, line 204 %}
  f''8 [  %{ beam 1, line 217 %}
  e''16 d'' ]  %{ beam 1, line 242 %}
  \stemUp c'' [  %{ beam 1, line 255 %}
  bes' a' g' ]  %{ beam 1, line 295 %}
  f'8 [  %{ beam 1, line 496 %}
  e'16 d' ]  %{ beam 1, line 521 %}
  c'8 [  %{ beam 1, line 534 %}
  d'16 e' ]  %{ beam 1, line 559 %}
  f'8 [  %{ beam 1, line 572 %}
  g'16 a' ]  %{ beam 1, line 597 %}
  \stemDown bes' [  %{ beam 1, line 611 %}
  c'' d'' e'' ]  %{ beam 1, line 650 %}
   | % 3
  \barNumberCheck #3
  \break | % 1333333 \myLineBreak
  
  f''8 [  %{ beam 1, line 838 %}
  g''16 a'' ]  %{ beam 1, line 863 %}
  bes'' [  %{ beam 1, line 877 %}
  c''' d''' e''' ]  %{ beam 1, line 916 %}
  f''' [  %{ beam 1, line 929 %}
  c''' a'' c''' ]  %{ beam 1, line 968 %}
  a'' [  %{ beam 1, line 981 %}
  f'' a'' f'' ]  %{ beam 1, line 1020 %}
   | % 4
  \barNumberCheck #4
  c''16 [  %{ beam 1, line 1234 %}
  f'' c'' a' ]  %{ beam 1, line 1273 %}
  \stemUp c'' [  %{ beam 1, line 1286 %}
  a' f' a' ]  %{ beam 1, line 1325 %}
  f' [  %{ beam 1, line 1338 %}
  c' f' c'' ]  %{ beam 1, line 1377 %}
  \stemDown f'' [  %{ beam 1, line 1390 %}
  c''' f'''8 ]  %{ beam 1, line 1416 %}
  
  \bar "|."  %{ b333 visitStart (S_msrBarLine& elt) %}
   | % 1
  \barNumberCheck #5
}

_Staff_Two_Voice_Five = \absolute {
  \language "nederlands"
  
  \clef "bass"
  \key f \major
  \numericTimeSignature \time 4/4
  \stemUp f,,8 [  %{ beam 1, line 311 %}
  g,,16 a,, ]  %{ beam 1, line 336 %}
  bes,, [  %{ beam 1, line 350 %}
  c, d, e, ]  %{ beam 1, line 389 %}
  f,8 [  %{ beam 1, line 402 %}
  g,16 a, ]  %{ beam 1, line 427 %}
  bes, [  %{ beam 1, line 441 %}
  c d e ]  %{ beam 1, line 480 %}
  \stemDown f8 [  %{ beam 1, line 666 %}
  g16 a ]  %{ beam 1, line 691 %}
  bes8 [  %{ beam 1, line 705 %}
  a16 g ]  %{ beam 1, line 730 %}
  f8 [  %{ beam 1, line 743 %}
  e16 d ]  %{ beam 1, line 768 %}
  \stemUp c [  %{ beam 1, line 781 %}
  bes, a, g, ]  %{ beam 1, line 821 %}
   | % 3
  \barNumberCheck #3
  \break | % 1333333 \myLineBreak
  
  f,8 [  %{ beam 1, line 1036 %}
  e,16 d, ]  %{ beam 1, line 1061 %}
  c, [  %{ beam 1, line 1074 %}
  bes,, a,, g,, ]  %{ beam 1, line 1114 %}
  f,, [  %{ beam 1, line 1127 %}
  a,, c, a,, ]  %{ beam 1, line 1166 %}
  c, [  %{ beam 1, line 1179 %}
  f, c, f, ]  %{ beam 1, line 1218 %}
   | % 4
  \barNumberCheck #4
  a,16 [  %{ beam 1, line 1431 %}
  f, a, c ]  %{ beam 1, line 1470 %}
  a, [  %{ beam 1, line 1483 %}
  c f c ]  %{ beam 1, line 1522 %}
  \stemDown f [  %{ beam 1, line 1535 %}
  a f c ]  %{ beam 1, line 1574 %}
  \stemUp f, [  %{ beam 1, line 1587 %}
  c, f,,8 ]  %{ beam 1, line 1613 %}
  
  \bar "|."  %{ b333 visitStart (S_msrBarLine& elt) %}
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
        
          \new Staff  = "_Staff_One"
          \with {
          }
          <<
            \context Voice = "_Staff_One_Voice_One" <<
              \_Staff_One_Voice_One
            >>
          >>
          
          \new Staff  = "_Staff_Two"
          \with {
          }
          <<
            \context Voice = "_Staff_Two_Voice_Five" <<
              \_Staff_Two_Voice_Five
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
