; Project 6
; @file main.asm
; @author Mia Chervenkova and Charlie Beard
; @date Nov 6 2024

include "hardware.inc"

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

section "header", rom0[$0100]
entrypoint:
    di
    jp main
    ds ($0150 - @), 0

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

macro DisableLCD
    ; wait for the vblank
    .wait_vblank\@
        ld a, [rLY]
        cp a, SCRN_Y
        jr nz, .wait_vblank\@

    ; turn the LCD off
    xor a
    ld [rLCDC], a
endm

section "main", rom0
main:
    DisableLCD
    
    call InitGame

    .loop
        call UpdateOnVblank
        jr .loop