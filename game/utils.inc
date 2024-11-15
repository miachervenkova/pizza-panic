if !def(UTILS_RGBINC)
def UTILS_RGBINC equ 1

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

include "hardware.inc"

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

; copy \2 into \1 through (a)
; example: copy [$FF84], 10
macro copy
    ld a, \2
    ld \1, a
endm

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

rsset _RAM ; $C000
def JOYPAD_CURRENT_ADDRESS  rb 1
def JOYPAD_PREVIOUS_ADDRESS rb 1
def JOYPAD_PRESSED_ADDRESS  rb 1
def JOYPAD_RELEASED_ADDRESS rb 1

macro UpdateJoypad

    ld a, [JOYPAD_CURRENT_ADDRESS]
    ld [JOYPAD_PREVIOUS_ADDRESS], a
    ld c, a ; (c) contains previous joypad

    ld a, P1F_GET_BTN ; that's %00010000
    ld [rP1], a

    ; wait!
    ld a, [rP1]
    ld a, [rP1]
    ld a, [rP1]
    ld a, [rP1]
    ld a, [rP1]

    ; read button poll result
    ld a, [rP1]

    ; save button result in b
    and $0F
    ld b, a

    ld a, P1F_GET_DPAD
    ld [rP1], a

    ; wait
    ld a, [rP1]

    ; read dpad poll result
    ld a, [rP1]

    ; move dpad result and load button result into (a)
    and $0F
    swap a
    or a, b
    cpl

    ; store in our custom joypad byte
    ld [JOYPAD_CURRENT_ADDRESS], a
    ld b, a ; (b) contains current joypad byte

    ; update pressed
    ; for each bit: if 1 before, 0 now then it was pressed, set to 0

    ; load current joypad byte (from (b))
    ld a, b
    xor c ; xor with previous joypad byte
    and b
    
    ; now any that were previous held or currently not held are 1's
    ; and all presses are 0's
    ld [JOYPAD_PRESSED_ADDRESS], a

    ; update released
    ; for each bit, if 0 before, and 1 now, then it was released

    ; load current joypad byte (in (b))
    ld a, b
    xor c ; xor previous joypad byte
    and c

    ; now any that were previous not hold and current hold are 1's
    ; and all releases are 0's
    ld [JOYPAD_RELEASED_ADDRESS], a

    ld a, P1F_GET_NONE
    ld [rP1], a

endm

endc