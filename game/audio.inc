; Project 6
; @file audio.inc
; @author Mia Chervenkova and Charlie Beard
; @date Nov 17 2024

if !def(AUDIO_INC)
def AUDIO_RGBINC equ 1

macro PlayCoinSound
    copy [rNR10], $00
    copy [rNR11], $80
    copy [rNR12], $F3
    copy [rNR13], $83
    copy [rNR14], $87
endm

endc