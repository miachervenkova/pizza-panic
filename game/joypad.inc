if !def(JOYPAD_INC)
def JOYPAD_INC equ 1

macro isStartPressed
    UpdateJoypad

    ld a, [JOYPAD_CURRENT_ADDRESS]
    bit PADB_START, a 
    jp z, .nothing

    ld a, 0
    ld [START_SCREEN_STATE], a

    LoadBackgroundToVRAM

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