\version "2.25.5"
% automatically converted by musicxml2ly from ExtendStartAndStop_musicxml2ly.xml


\header {
    title =  "Extend Start And Stop"
    }

#(set-global-staff-size 20.66257142857143)
\paper {

    }
\layout {
    \context { \Score
        autoBeaming = ##f
        }
    }
PartPOneVoiceOne =  \relative b' {
    \clef "treble" \time 2/4 \key c \major | % 1
    \stemDown b4 ^\markup{ \bold {Allegro} } \stemDown c8 \stemDown e8 | % 2
    \stemDown fis8 \stemDown gis4 \stemDown a8 }

PartPOneVoiceOneLyricsOne =  \lyricmode {\set ignoreMelismata = ##t
    "AB " __ _ \skip1 "　" "C " __ _
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
    %  \midi {\tempo 4 = 92 }
    }

