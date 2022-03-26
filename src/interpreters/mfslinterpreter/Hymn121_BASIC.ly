\version "2.22.0"

% Pick your choice from the next two lines as needed
%myBreak = { \break }
myBreak = {}

% Pick your choice from the next two lines as needed
%myPageBreak = { \pageBreak }
myPageBreak = {}

\header {
  title                = "121. ANGELS FROM THE REALMS OF GLORY"
  workTitle            = "121. ANGELS FROM THE REALMS OF GLORY"
  composer             = "Flemish"
  poet                 = ""
  software             = "Neuratron PhotoScore"
  right                = ""
  title                = "121. ANGELS FROM THE REALMS OF GLORY"
}

\paper {
  % page-count = -1
  % system-count = -1
  ragged-bottom = ##f
  ragged-last-bottom = ##f
}

\layout {
  \context {
    \Score
    autoBeaming = ##f % to display tuplets brackets
  }
  \context {
    \Voice
  }
}

Part_POne_Staff_One_Voice_One = \absolute {
  \language "nederlands"
  \key f \major
  \numericTimeSignature \time 4/4
  
  \clef "treble"
  a'4 \mp a' a' a'8 [ ( c'' ] ) | % 2
  \barNumberCheck #2
  | % 2
  \barNumberCheck #2
  c''4. bes'8 a'4 f' | % 3
  \barNumberCheck #3
  | % 3
  \barNumberCheck #3
  a'4 a'8 [ ^\< ( g' ] \! ) a'4 a'8 [ c'' ] | % 4
  \barNumberCheck #4
  | % 4
  \barNumberCheck #4
  c''4. ^\> bes'8 \! a'2 | % 5
  \barNumberCheck #5
  | % 5
  \barNumberCheck #5
  a'4 a' a' a'8 [ ( c'' ] ) | % 6
  \barNumberCheck #6
  | % 6
  \barNumberCheck #6
  c''4. bes'8 a'4 f' | % 7
  \barNumberCheck #7
  | % 7
  \barNumberCheck #7
  a'4 a'8 [ ^\< ( g' ] \! ) a'4 a'8 [ ( c'' ] ) | % 8
  \barNumberCheck #8
  | % 8
  \barNumberCheck #8
  c''4. ^\> ( bes'8 \! a'2 ) | % 9
  \barNumberCheck #9
  | % 9
  \barNumberCheck #9
  \bar "||"
  c''2 \f d''8 [ ( c'' bes' a' ] ) | % 10
  \barNumberCheck #10
  | % 10
  \barNumberCheck #10
  bes'2 c''8 [ ( bes' a' g' ] ) | % 11
  \barNumberCheck #11
  | % 11
  \barNumberCheck #11
  \break | % 11\myLineBreak
  
  a'2 bes'8 [ ( a' g' f' ] ) | % 12
  \barNumberCheck #12
  | % 12
  \barNumberCheck #12
  g'2 c' | % 13
  \barNumberCheck #13
  | % 13
  \barNumberCheck #13
  f'4 g' a' bes' | % 14
  \barNumberCheck #14
  | % 14
  \barNumberCheck #14
  a'2 ( g' ) | % 15
  \barNumberCheck #15
  | % 15
  \barNumberCheck #15
  c''2 d''8 [ ( c'' bes' a' ] ) | % 16
  \barNumberCheck #16
  | % 16
  \barNumberCheck #16
  bes'2 c''8 [ ( bes' a' g' ] ) | % 17
  \barNumberCheck #17
  | % 17
  \barNumberCheck #17
  a'2 bes'8 [ ( a' g' f' ] ) | % 18
  \barNumberCheck #18
  | % 18
  \barNumberCheck #18
  g'2 c' ^\< | % 19
  \barNumberCheck #19
  | % 19
  \barNumberCheck #19
  f'4 \ff \! g' a' bes' | % 20
  \barNumberCheck #20
  | % 20
  \barNumberCheck #20
  a'2 -> g' -> | % 21
  \barNumberCheck #21
  | % 21
  \barNumberCheck #21
  f'1 -> | % 1
  \barNumberCheck #22
  | % 1
  \barNumberCheck #22
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
  f'4 f'8 [ ^\< ( <e' \! g' > ] ] ] ] ] ] ) \! <f' a' >4 e'8 [ ( c' ] ) | % 4
  \barNumberCheck #4
  | % 4
  \barNumberCheck #4
  <d' ^\> g' >4 \> e' \! <c' f' >2 | % 5
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
  f'4 f'8 [ ^\< ( <e' \! g' > ] ] ] ] ] ] ) \! <f' a' >4 f'8 [ ( a' ] ) | % 8
  \barNumberCheck #8
  | % 8
  \barNumberCheck #8
  <d' ^\> g' >4 \> e' \! <c' f' >2 | % 9
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
  c'4 ( b! ) c'2 ^\< | % 19
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
  c'1 -> | % 1
  \barNumberCheck #22
  | % 1
  \barNumberCheck #22
}

Part_PThree_Staff_One_Voice_One = \absolute {
  \language "nederlands"
  \key f \major
  \numericTimeSignature \time 4/4
  
  \clef "bass"
  
  \bar "|."
  c'4 \mp c' c' a | % 2
  \barNumberCheck #2
  | % 2
  \barNumberCheck #2
  a4. bes8 c'4 a | % 3
  \barNumberCheck #3
  | % 3
  \barNumberCheck #3
  d'4 d' ^\< d' \! c' | % 4
  \barNumberCheck #4
  | % 4
  \barNumberCheck #4
  g4 ^\> g \! f2 | % 5
  \barNumberCheck #5
  | % 5
  \barNumberCheck #5
  c'4 c' d' a | % 6
  \barNumberCheck #6
  | % 6
  \barNumberCheck #6
  f4 c' c' a | % 7
  \barNumberCheck #7
  | % 7
  \barNumberCheck #7
  c'4 c' ^\< c' \! a | % 8
  \barNumberCheck #8
  | % 8
  \barNumberCheck #8
  g4 ^\> ( g \! f2 ) | % 9
  \barNumberCheck #9
  | % 9
  \barNumberCheck #9
  \bar "||"
  a2. \f d'4  ~ | % 10
  \barNumberCheck #10
  | % 10
  \barNumberCheck #10
  d'2 c'  ~ | % 11
  \barNumberCheck #11
  | % 11
  \barNumberCheck #11
  \break | % 11\myLineBreak
  
  c'2 bes4 f | % 12
  \barNumberCheck #12
  | % 12
  \barNumberCheck #12
  d2 e | % 13
  \barNumberCheck #13
  | % 13
  \barNumberCheck #13
  f4 e f d' | % 14
  \barNumberCheck #14
  | % 14
  \barNumberCheck #14
  c'2. bes4 | % 15
  \barNumberCheck #15
  | % 15
  \barNumberCheck #15
  a2. d'4  ~ | % 16
  \barNumberCheck #16
  | % 16
  \barNumberCheck #16
  d'2 c'  ~ | % 17
  \barNumberCheck #17
  | % 17
  \barNumberCheck #17
  c'2 bes4 f | % 18
  \barNumberCheck #18
  | % 18
  \barNumberCheck #18
  d2 e ^\< | % 19
  \barNumberCheck #19
  | % 19
  \barNumberCheck #19
  f4 \ff \! e f d' | % 20
  \barNumberCheck #20
  | % 20
  \barNumberCheck #20
  c'2 -> c'4 ( -> bes ) | % 21
  \barNumberCheck #21
  | % 21
  \barNumberCheck #21
  a1 -> | % 1
  \barNumberCheck #22
  | % 1
  \barNumberCheck #22
}

Part_PFour_Staff_One_Voice_One = \absolute {
  \language "nederlands"
  \key f \major
  \numericTimeSignature \time 4/4
  
  \clef "bass"
  
  \bar "|."
  f4 \mp f f f | % 2
  \barNumberCheck #2
  | % 2
  \barNumberCheck #2
  a4. g8 f4 f | % 3
  \barNumberCheck #3
  | % 3
  \barNumberCheck #3
  d4 d ^\< d \! a | % 4
  \barNumberCheck #4
  | % 4
  \barNumberCheck #4
  g4 ^\> c \! f2 | % 5
  \barNumberCheck #5
  | % 5
  \barNumberCheck #5
  f4. e8 d4 c | % 6
  \barNumberCheck #6
  | % 6
  \barNumberCheck #6
  d4 e f f | % 7
  \barNumberCheck #7
  | % 7
  \barNumberCheck #7
  f4 c' ^\< a \! f | % 8
  \barNumberCheck #8
  | % 8
  \barNumberCheck #8
  g4 ^\> c \! f2 | % 9
  \barNumberCheck #9
  | % 9
  \barNumberCheck #9
  \bar "||"
  f4 \f ( ees! d2 ) | % 10
  \barNumberCheck #10
  | % 10
  \barNumberCheck #10
  g8 [ ( f e! d ] ) e4 c | % 11
  \barNumberCheck #11
  | % 11
  \barNumberCheck #11
  \break | % 11\myLineBreak
  
  f8 [ ( e d c ] ) d4 <a, a > | % 12
  \barNumberCheck #12
  | % 12
  \barNumberCheck #12
  <g, g >2 <c c' >4 ( <bes, bes > ) | % 13
  \barNumberCheck #13
  | % 13
  \barNumberCheck #13
  <a, a >4 <g, g > <f, f > bes, | % 14
  \barNumberCheck #14
  | % 14
  \barNumberCheck #14
  c1 | % 15
  \barNumberCheck #15
  | % 15
  \barNumberCheck #15
  f4 ( ees! d2 ) | % 16
  \barNumberCheck #16
  | % 16
  \barNumberCheck #16
  g8 [ ( f e! d ] ) e4 c | % 17
  \barNumberCheck #17
  | % 17
  \barNumberCheck #17
  f8 [ ( e d c ] ) d4 <a, a > | % 18
  \barNumberCheck #18
  | % 18
  \barNumberCheck #18
  <g, g >2 <c ^\< c' >4 ( \< <bes, \! bes > ) \! | % 19
  \barNumberCheck #19
  | % 19
  \barNumberCheck #19
  <a, a >4 -\ff <g, g > <f, f > bes, | % 20
  \barNumberCheck #20
  | % 20
  \barNumberCheck #20
  <c c' >2 -> <c c' > -> | % 21
  \barNumberCheck #21
  | % 21
  \barNumberCheck #21
  <f, f >1 -> | % 22
  \barNumberCheck #22
  | % 22
  \barNumberCheck #22
}

Part_PFour_Staff_One_Voice_One_Stanza_One = \lyricmode {
  \set associatedVoice = #"Part_PFour_Staff_One_Voice_One"
  \set ignoreMelismata = ##t
    \skip4 \skip4 \skip4 \skip4 \skip4. \skip8 \skip4 \skip4 \skip4 \skip4 \skip4 \skip4 \skip4 \skip4 \skip2 \skip4. \skip8 \skip4 \skip4 \skip4 \skip4 \skip4 \skip4 \skip4 \skip4 \skip4 \skip4 \skip4 \skip4 \skip2 \skip4 \skip4 \skip2 \skip8 \skip8 \skip8 \skip8 \skip4 \skip4 %{ syllableLineBreak, line 3727 %} %| % -1
\skip8 \skip8 \skip8 \skip8 \skip4 \skip4 \skip2 \skip4 \skip4 \skip4 \skip4 \skip4 \skip4 \skip1 \skip4 \skip4 \skip2 \skip8 \skip8 \skip8 \skip8 \skip4 \skip4 \skip8 \skip8 \skip8 \skip8 \skip4 \skip4 \skip2 \skip4 \skip4 \skip4 \skip4 \skip4 \skip4 \skip2 \skip2 e1 --
}

\book {
  \score {
    <<
      
        \new Staff = "Part_POne_Staff_One"
        \with {
          instrumentName = "Voice"
        }
        <<
          \context Voice = "Part_POne_Staff_One_Voice_One" <<
            \Part_POne_Staff_One_Voice_One
          >>
        >>
        \new Staff = "Part_PTwo_Staff_One"
        \with {
          instrumentName = "Yoicel"
        }
        <<
          \context Voice = "Part_PTwo_Staff_One_Voice_One" <<
            \Part_PTwo_Staff_One_Voice_One
          >>
        >>
        
        \new Staff = "Part_PThree_Staff_One"
        \with {
          instrumentName = "Yoicel"
        }
        <<
          \context Voice = "Part_PThree_Staff_One_Voice_One" <<
            \Part_PThree_Staff_One_Voice_One
          >>
        >>
        
        \new Staff = "Part_PFour_Staff_One"
        \with {
          instrumentName = "Voice"
        }
        <<
          \context Voice = "Part_PFour_Staff_One_Voice_One" <<
            \Part_PFour_Staff_One_Voice_One
          >>
          \new Lyrics
            \with {
            }
            \Part_PFour_Staff_One_Voice_One_Stanza_One
        >>
        
      
    >>
    
    \layout {
      \context {
        \Score
        autoBeaming = ##f % to display tuplets brackets
      }
      \context {
        \Voice
      }
    }
    
    \midi {
      \tempo 16 = 360
    }
  }
  
}
