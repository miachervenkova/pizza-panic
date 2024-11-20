; Project 6
; @file joypad.inc
; @author Mia Chervenkova and Charlie Beard
; @date Nov 17 2024

if !def(JOYPAD_INC)
def JOYPAD_INC equ 1

macro isStartPressed
    UpdateJoypad

    ld a, [JOYPAD_CURRENT_ADDRESS]
    bit PADB_START, a 
    jp z, .nothing

    ; check if game ended, then restart to start screen
    ld a, [FINAL_ROUND_SCORE]
    cp 1
    jp z, .restart_game

    LoadBackgroundToVRAM
    ld a, 0
    ld [START_SCREEN_STATE], a
    jp .nothing

    .restart_game
        LoadStartScreenToVRAM
        ld a, 0
        ld [FINAL_ROUND_SCORE], a
        halt 
        halt

    .nothing
endm

macro ReadMovement
    UpdateJoypad

    .move_down
        ld a, [JOYPAD_CURRENT_ADDRESS]
        bit PADB_DOWN, a
        jp z, .move_up

        ld a, [SPRITE_0_ADDRESS + OAMA_Y]
        inc a
        ld [SPRITE_0_ADDRESS + OAMA_Y], a

    .move_up
        ld a, [JOYPAD_CURRENT_ADDRESS]
        bit PADB_UP, a
        jp z, .move_left

        ld a, [SPRITE_0_ADDRESS + OAMA_Y]
        dec a
        ld [SPRITE_0_ADDRESS + OAMA_Y], a

    .move_left
        ld a, [JOYPAD_CURRENT_ADDRESS]
        bit PADB_LEFT, a
        jp z, .move_right

        ld a, [SPRITE_0_ADDRESS + OAMA_X]
        dec a
        ld [SPRITE_0_ADDRESS + OAMA_X], a

    .move_right
        ld a, [JOYPAD_CURRENT_ADDRESS]
        bit PADB_RIGHT, a
        jp z, .move_none

        ld a, [SPRITE_0_ADDRESS + OAMA_X]
        inc a
        ld [SPRITE_0_ADDRESS + OAMA_X], a

    .move_none
endm

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

endc