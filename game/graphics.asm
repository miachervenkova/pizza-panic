; Project 6
; @file graphics.asm
; @author Mia Chervenkova and Charlie Beard
; @date Nov 20 2024

section "graphics", rom0

; Distance between ascii values and tileset index
def ALPHABET_OFFSET                 equ ($25)
def NUMBER_OFFSET                   equ ($26) 
def SPACE_OFFSET                    equ ($35)
def EXCLAMATION_OFFSET              equ ($42)
def COMMA_OFFSET                    equ ($36)
def DASH_OFFSET                     equ ($38)
def PERIOD_OFFSET                   equ ($33)
def QUESTION_OFFSET                 equ ($25)

; ascii values of used characters
def A_ASCII                         equ ($41) 
def EXCLAMATION_ASCII               equ ($21)
def SPACE_ASCII                     equ ($20)
def COMMA_ASCII                     equ ($2C)
def DASH_ASCII                      equ ($2D)
def PERIOD_ASCII                    equ ($2E)
def QUESTION_ASCII                  equ ($3F)

; the zero tile index in the tileset
def DIGIT_TILE_START                equ ($56)       

; tile locations for the window display
def WINDOW_TILE_THOUSANDS           equ ($9C43)    
def WINDOW_TILE_HUNDREDS            equ ($9C44)     
def WINDOW_TILE_TENS                equ ($9C45)   
def WINDOW_TILE_ONES                equ ($9C46)    

; tile locations for game over total count
def PIZZAS_TILE_TENS                equ ($9969)   
def PIZZAS_TILE_ONES                equ ($996A)

; window coordinates
def WINDOW_START_X                  equ (7)
def WINDOW_START_Y                  equ (112)

def WINDOW_LOSE_X                   equ (7)
def WINDOW_LOSE_Y                   equ (0)

; game over text
def YOU_MADE_TEXT_LINE              equ ($9926)
def NUMBER_LINE                     equ ($9948)
def PIZZAS_TEXT_LINE                equ ($9987)
def OUT_OF_TEXT_LINE                equ ($99A6)
def GAME_OVER_LINE                  equ ($99C3)

STRING_YOU_MADE:
    db "YOU MADE\0"

STRING_PIZZAS:
    db "PIZZAS\0"

STRING_CONGRATS:
    db "IN 14 SEC! YAY!\0"

STRING_TRY_AGAIN:
    db "OUT OF 4\0"

; load the graphics data from ROM to VRAM
macro LoadGraphicsDataIntoVRAM
    ld de, GRAPHICS_DATA_ADDRESS_START
    ld hl, _VRAM8000
    .load_data\@
        ld a, [de]
        inc de
        ld [hli], a

        ld a, d
        cp a, high(GRAPHICS_DATA_ADDRESS_END)
        jr nz, .load_data\@
endm

; clear the OAM
macro InitOAM
    ld c, OAM_COUNT
    ld hl, _OAMRAM + OAMA_Y
    ld de, sizeof_OAM_ATTRS
    .init_oam\@
        ld [hl], 0
        add hl, de
        dec c
        jr nz, .init_oam\@
endm

; replaces the start screen tilemap with the background one
macro LoadBackgroundToVRAM
    halt
    ld a, LCDCF_OFF
    ld [rLCDC], a

    ld de, BACKGROUND_TILEMAP_ROM_LOCATION
    ld hl, TILEMAP_VRAM_LOCATION

    .load_bg_tilemap
        ld a, [de]
        inc de
        ld [hli], a

        ld a, h
        cp a, high(TILEMAP_VRAM_END_LOCATION)
        jr nz, .load_bg_tilemap

        ld a, l
        cp a, low(TILEMAP_VRAM_END_LOCATION)
        jp nz, .load_bg_tilemap
    
    ld a, LCDCF_ON | LCDCF_WIN9C00 | LCDCF_WINON | LCDCF_BG8800 | LCDCF_BG9800 \
             | LCDCF_OBJ16 | LCDCF_OBJON | LCDCF_BGON
    ld [rLCDC], a
endm

; replaces the background tilemap with the start screen one
macro LoadStartScreenToVRAM
    halt
    ld a, LCDCF_OFF
    ld [rLCDC], a

    ld de, START_SCREEN_TILEMAP_ROM_LOCATION
    ld hl, TILEMAP_VRAM_LOCATION

    .load_start_tilemap
        ld a, [de]
        inc de
        ld [hli], a

        ld a, h
        cp a, high(TILEMAP_VRAM_END_LOCATION)
        jr nz, .load_start_tilemap

        ld a, l
        cp a, low(TILEMAP_VRAM_END_LOCATION)
        jp nz, .load_start_tilemap
    
        ld a, LCDCF_ON | LCDCF_BG9800 | LCDCF_BGON
        ld [rLCDC], a
endm

HandleEndGame:
    ld a, [ORDER_NUMBER]
    ld b, a
    ld a, [LEVELS_COMPLETED]
    add a, b
    dec a
    cp 4
    ; if we made >= 4 pizzas
    jp nc, .game_won
    
    ld bc, OUT_OF_TEXT_LINE
    ld hl, STRING_TRY_AGAIN
    call print_text

    ld a, [CURRENT_ROUND]
    dec a
    ld [CURRENT_ROUND], a 
    jp .handling_done
    
    .game_won
        ld a, 1
        ld [FINAL_ROUND_SCORE], a
        ld bc, GAME_OVER_LINE
        ld hl, STRING_CONGRATS
        call print_text

        ld a, 0         
        ld [CURRENT_ROUND], a

    .handling_done
        ret

TimerEnd:
    ld a, 1
    ld [START_SCREEN_STATE], a

    LoadStartScreenToVRAM

    halt
    ld bc, YOU_MADE_TEXT_LINE
    ld hl, STRING_YOU_MADE
    call print_text

    call CalculateTotalPizzas

    ld bc, PIZZAS_TEXT_LINE
    ld hl, STRING_PIZZAS
    call print_text

    ld a, [CURRENT_ROUND]
    cp 3
    jp z, .game_over
    jp .over

    .game_over
        call HandleEndGame

    .over
        ld a, 0
        ld [LEVELS_COMPLETED], a
        ld [ORDER_NUMBER], a
        ld [CURRENT_ORDER_INFO], a
    ret

; print text beginning at (hl) into window starting at location (bc)
print_text:
    .print_loop
        ld a, [hli]

        ; check for end of string (null)
        cp a, $00
        jp z, .done
        cp a, A_ASCII
        jp c, .non_letter

        add a, ALPHABET_OFFSET
        jp .update_bc

        .non_letter
            cp a, SPACE_ASCII
            jp z, .space

            cp a, EXCLAMATION_ASCII
            jp z, .exclamation

            cp a, COMMA_ASCII
            jp z, .comma

            cp a, DASH_ASCII
            jp z, .dash

            cp a, PERIOD_ASCII
            jp z, .period

            cp a, QUESTION_ASCII
            jp z, .question

            add a, NUMBER_OFFSET
            jp .update_bc

        .space
            add a, SPACE_OFFSET
            jp .update_bc

        .exclamation
            add a, EXCLAMATION_OFFSET
            jp .update_bc

        .comma
            add a, COMMA_OFFSET
            jp .update_bc

        .dash
            add a, DASH_OFFSET
            jp .update_bc

        .period
            add a, PERIOD_OFFSET
            jp .update_bc

        .question
            add a, QUESTION_OFFSET

        .update_bc
            ld [bc], a
            inc bc
            jp .print_loop

    .done
        ret