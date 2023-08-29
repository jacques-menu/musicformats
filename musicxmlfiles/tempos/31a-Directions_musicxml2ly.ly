\version "2.25.5"
% automatically converted by musicxml2ly from 31a-Directions_musicxml2ly.xml
\pointAndClickOff

%% additional definitions required by the score:
abcffz = #(make-dynamic-script "abc-ffz")
accRegHMML = \markup { \normalsize \combine
          \raise #2.5 \musicglyph #"accordion.dot"
          \combine
          \raise #1.5 \translate #(cons 0.5 0) \musicglyph #"accordion.dot"
          \combine
          \raise #1.5 \translate #(cons -0.5 0) \musicglyph #"accordion.dot"
          \combine
          \raise #0.5 \musicglyph #"accordion.dot"
          \musicglyph #"accordion.discant" }

fffff = #(make-dynamic-script "fffff")
ffffff = #(make-dynamic-script "ffffff")
fz = #(make-dynamic-script "fz")
pppppp = #(make-dynamic-script "pppppp")
rf = #(make-dynamic-script "rf")
sffz = #(make-dynamic-script "sffz")
sfp = #(make-dynamic-script "sfp")
sfpp = #(make-dynamic-script "sfpp")

\header {
    title =  "MusicXML directions (attached to staff)"
    texidoc = 
    "All <direction> elements 
          defined in MusicXML. The lyrics for each note describes the direction
          element assigned to that note."
    }

\layout {
    \context { \Score
        autoBeaming = ##f
        }
    }
PartPOneVoiceOne =  \relative c' {
    \clef "treble" \time 4/4 \key c \major | % 1
    \mark \markup { \box { A } } c4 \mark \markup { B } c4 \mark \markup
    { \box { Test } } c4 \mark \markup { \circle { Crc } } c4 | % 2
    \mark \markup { \musicglyph "scripts.segno" } c4 \mark \markup {
        \musicglyph "scripts.coda" } c4 c4 - "words" \mark \markup {
        \eyeglasses } c4 | % 3
    c4 -\p c4 -\pp c4 -\ppp c4 -\pppp | % 4
    c4 -\ppppp c4 -\pppppp c4 -\f c4 -\ff | % 5
    c4 -\fff c4 -\ffff c4 -\fffff c4 -\ffffff | % 6
    c4 -\mp c4 -\mf c4 -\sf c4 -\sfp | % 7
    c4 -\sfpp c4 -\fp c4 -\rf c4 -\rfz | % 8
    c4 -\sfz c4 -\sffz c4 -\fz c4 -\abcffz | % 9
    c4 -\< c4 -\! c4 \startTrillSpan c4 \stopTrillSpan | \barNumberCheck
    #10
    c4 c4 \ottava #-1 c4 \ottava #0 c4 | % 11
    c4 \sustainOn c4 \sustainOff\sustainOn c4 c4 \sustainOff | % 12
    \tempo 4=60 c4 c4 -\markup { \harp-pedal #"-^^|--v^" } c4 c4 | % 13
    c4 \mark \accRegHMML c4 r2 \bar "||"
    c4 _\markup{ \small\italic {subito} } _\markup{ \small { } } _\p c4
    _\ppp _\< c4 _\! _\fff r4 \bar "|."
    }

PartPOneVoiceOneLyricsOne =  \lyricmode {\set ignoreMelismata = ##t
    "reh.A (def=sq.)" "reh.B (none)" "reh.Test (sq.)" "reh.Crc (crc.)"
    Segno Coda Words "Eyegl." p pp ppp pppp ppppp pppppp f ff fff ffff
    fffff ffffff mp mf sf sfp sfpp fp rf rfz sfz sffz fz
    "abc-ffz (oth.)" hairpin -- cresc dash -- es bra -- cket "oct." --
    shift pedal -- change --\skip1 mark "Metr." "Harp ped." Damp
    "Damp all" "Scord." "Accordion reg." subp "ppp cresc" -- "to fff"
    subp
    }


% The score definition
\score {
    <<
        
        \new Staff
        <<
            
            \context Staff << 
                \mergeDifferentlyDottedOn\mergeDifferentlyHeadedOn
                \context Voice = "PartPOneVoiceOne" {  \PartPOneVoiceOne }
                \new Lyrics \lyricsto "PartPOneVoiceOne" { \set stanza = "1." \PartPOneVoiceOneLyricsOne }
                >>
            >>
        
        >>
    \layout {}
    % To create MIDI output, uncomment the following line:
    %  \midi {\tempo 4 = 100 }
    }

