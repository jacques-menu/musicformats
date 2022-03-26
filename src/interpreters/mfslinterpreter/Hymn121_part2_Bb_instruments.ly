\version "2.22.0"

% Pick your choice from the next two lines as needed
%myBreak = { \break }
myBreak = {}

% Pick your choice from the next two lines as needed
%myPageBreak = { \pageBreak }
myPageBreak = {}

\header {
  title                = "Hymn 121 - Angels From The Realms Of Glory - Harmony band"
  workTitle            = "121. ANGELS FROM THE REALMS OF GLORY"
  composer             = "Flemish"
  poet                 = ""
  software             = "Neuratron PhotoScore"
  right                = ""
  title                = "Hymn 121 - Angels From The Realms Of Glory - Harmony band"
  subtitle             = "Part 2 - Bb instruments"
}

\paper {
  % page-count = -1
  % system-count = -1
  
  
  
  
}

\layout {
  \context {
    \Score
    autoBeaming = ##f % to display tuplets brackets
  }
  \context {
    \Voice
    \consists "Ambitus_engraver"
  }
}

Part_PTwo_Staff_One_Voice_One = \absolute {
  \language "nederlands"
  \key f \major
  \numericTimeSignature \time 4/4
  
  \clef "treble"
  
  \bar "|."
  f'4 \mp f' f' f'8 [ ( a' ] ) | % 2
  \barNumberCheck #2
  | % 2
  \barNumberCheck #2
  <c' f' >4. e'8 f'4 <c' f' > | % 3
  \barNumberCheck #3
  | % 3
  \barNumberCheck #3
  f'4 f'8 [ _\< ( <e' \! g' > ] ] ] ] ] ] ) \! <f' a' >4 e'8 [ ( c' ] ) | % 4
  \barNumberCheck #4
  | % 4
  \barNumberCheck #4
  <d' _\> g' >4 \> e' \! <c' f' >2 | % 5
  \barNumberCheck #5
  | % 5
  \barNumberCheck #5
  f'4 f' f' e'8 [ ( a' ] ) | % 6
  \barNumberCheck #6
  | % 6
  \barNumberCheck #6
  <f' a' >4 g' f' <c' f' > | % 7
  \barNumberCheck #7
  | % 7
  \barNumberCheck #7
  f'4 f'8 [ _\< ( <e' \! g' > ] ] ] ] ] ] ) \! <f' a' >4 f'8 [ ( a' ] ) | % 8
  \barNumberCheck #8
  | % 8
  \barNumberCheck #8
  <d' _\> g' >4 \> e' \! <c' f' >2 | % 9
  \barNumberCheck #9
  | % 9
  \barNumberCheck #9
  \bar "||"
  a'2. \f bes'8 [ ( a' ] ) | % 10
  \barNumberCheck #10
  | % 10
  \barNumberCheck #10
  g'2. a'8 [ ( g' ] ) | % 11
  \barNumberCheck #11
  | % 11
  \barNumberCheck #11
  \break | % 11\myLineBreak
  
  f'2. g'8 [ ( f' ] ) | % 12
  \barNumberCheck #12
  | % 12
  \barNumberCheck #12
  c'4 ( b! ) c'2 | % 13
  \barNumberCheck #13
  | % 13
  \barNumberCheck #13
  c'4 d' c' f' | % 14
  \barNumberCheck #14
  | % 14
  \barNumberCheck #14
  f'2 ( e' ) | % 15
  \barNumberCheck #15
  | % 15
  \barNumberCheck #15
  a'2. bes'8 [ ( a' ] ) | % 16
  \barNumberCheck #16
  | % 16
  \barNumberCheck #16
  g'2. a'8 [ ( g' ] ) | % 17
  \barNumberCheck #17
  | % 17
  \barNumberCheck #17
  f'2. g'8 [ ( f' ] ) | % 18
  \barNumberCheck #18
  | % 18
  \barNumberCheck #18
  c'4 ( b! ) c'2 _\< | % 19
  \barNumberCheck #19
  | % 19
  \barNumberCheck #19
  c'4 \ff \! d' c' f' | % 20
  \barNumberCheck #20
  | % 20
  \barNumberCheck #20
  f'4 ( -> d' ) e'2 -> | % 21
  \barNumberCheck #21
  | % 21
  \barNumberCheck #21
  c'1 -> | % 22
  \barNumberCheck #22
  | % 22
  \barNumberCheck #22
}

\book {
  \score {
    <<
      
      \new Staff = "Part_PTwo_Staff_One"
      \with {
        instrumentName = "Voice 2"
      }
      <<
        \context Voice = "Part_PTwo_Staff_One_Voice_One" <<
          \transposition bes
          \transpose bes c'
          \Part_PTwo_Staff_One_Voice_One
        >>
      >>
      
    >>
    
    \layout {
      \context {
        \Score
        autoBeaming = ##f % to display tuplets brackets
      }
      \context {
        \Voice
        \consists "Ambitus_engraver"
      }
    }
    
    \midi {
      \tempo 16 = 360
    }
  }
  
}
