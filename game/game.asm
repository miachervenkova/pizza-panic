; Project 5
; @file game.asm
; @author Mia Chervenkova and Charlie Beard
; @date Nov 6 2024

include "utils.inc"
include "chef.inc"
include "customers.inc"
include "joypad.inc"
include "timer_functions.asm"

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

section "vblank_interrupt", rom0[$0040]
    reti

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

def TILES_COUNT                     equ (384)
def BYTES_PER_TILE                  equ (16)
def TILES_BYTE_SIZE                 equ (TILES_COUNT * BYTES_PER_TILE)

def TILEMAPS_COUNT                  equ (3)
def BYTES_PER_TILEMAP               equ (1024)
def TILEMAPS_BYTE_SIZE              equ (TILEMAPS_COUNT * BYTES_PER_TILEMAP)

def GRAPHICS_DATA_SIZE              equ (TILES_BYTE_SIZE + TILEMAPS_BYTE_SIZE)
def GRAPHICS_DATA_ADDRESS_END       equ ($8000)
def GRAPHICS_DATA_ADDRESS_START \
    equ (GRAPHICS_DATA_ADDRESS_END - GRAPHICS_DATA_SIZE)

; Tilemap ROM locations
def BACKGROUND_ROM_LOCATION \         
    equ (GRAPHICS_DATA_ADDRESS_END - (TILES_BYTE_SIZE + (2 * BYTES_PER_TILEMAP)))

def START_SCREEN_TILEMAP_ROM_LOCATION \
    equ (GRAPHICS_DATA_ADDRESS_START + TILES_BYTE_SIZE)

def WINDOW_TILEMAP_ROM_LOCATION     equ (START_SCREEN_TILEMAP_ROM_LOCATION + BYTES_PER_TILEMAP)
def BACKGROUND_TILEMAP_ROM_LOCATION equ (WINDOW_TILEMAP_ROM_LOCATION + BYTES_PER_TILEMAP)

; VRAM location for the tilemap (background starts at $9800)
def TILEMAP_VRAM_LOCATION           equ ($9800)
def TILEMAP_VRAM_END_LOCATION       equ ($9BFF)

; Game state storage in HRAM
def START_SCREEN_STATE              equ ($FF80)

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

; timer location on window, value location
def TIMER_POSITION                  equ ($9C43)
def COUNTDOWN_TIMER                 equ ($FF81)
def FRAME_COUNTER                   equ ($FF82)
def FRAME_DELAY                     equ (60)  

; timers for each round
def TIMER_ROUND_1                   equ (60)
def TIMER_ROUND_2                   equ (40)

; game states
def ROUNDS_SCORES                   equ ($FF83)
def CURRENT_ROUND                   equ ($FF84)

; game over text
def YOU_MADE_TEXT_LINE              equ ($9926)
def NUMBER_LINE                     equ ($9948)
def PIZZAS_TEXT_LINE                equ ($9987)
def GAME_OVER_LINE                  equ ($99C4)

STRING_YOU_MADE:
    db "YOU MADE\0"

STRING_PIZZAS:
    db "PIZZAS\0"

STRING_GAME_OVER:
    db "GAME  OVER!!\0"

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

; initializes timer for the round
macro InitRound
    ld a, [CURRENT_ROUND]

    ; alternate between even and odd rounds
    and a, $01
    cp 0
    jp z, .init_round_2
    
    ; we will develop these more
    ; cp 2
    ; jp z, .init_round_2
    ; jp .invalid

    .init_round_1:
        ld a, TIMER_ROUND_1          
        ld [COUNTDOWN_TIMER], a
        call UpdateTimerDisplay      
        jp .over

    .init_round_2:
        ld a, TIMER_ROUND_2           
        ld [COUNTDOWN_TIMER], a
        call UpdateTimerDisplay     
        jp .over

    ; .invalid:
    ;     halt
    ;     ld bc, GAME_OVER_LINE
    ;     ld hl, STRING_GAME_OVER
    ;     call print_text
    ;     .loop_game_over
    ;         jp .loop_game_over

    .over:        
        endm

; show start screen when game over
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

    ld a, 0
    ld [LEVELS_COMPLETED], a
    ld [ORDER_NUMBER], a
    ld [CURRENT_ORDER_INFO], a

    ret

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

section "game", rom0

InitSample:
    LoadGraphicsDataIntoVRAM

    ld a, 1          
    ld [CURRENT_ROUND], a

    ; timer initialization
    InitRound
    ld a, FRAME_DELAY
    ld [FRAME_COUNTER], a

    ld a, 1
    ld [START_SCREEN_STATE], a

    ; init the palette
    ld a, %11100100
    ld [rBGP], a
    ld [rOBP0], a
    ld a, %00011011
    ld [rOBP1], a

    InitOAM

    ; enable the vblank interrupt
    ld a, IEF_VBLANK
    ld [rIE], a
    ei

    ; place the window at the bottom of the LCD
    ld a, WINDOW_START_X
    ld [rWX], a
    ld a, WINDOW_START_Y
    ld [rWY], a

    ; set the graphics parameters and turn back LCD on
    ld a, LCDCF_ON | LCDCF_BG9800 | LCDCF_BGON
    ld [rLCDC], a

    halt

    InitChefSprites
    InitGameOrders
    InitCustomerSprites
    
    ret

UpdateSample:
    halt

    ; if start screen -> state = 1
    ld a, [START_SCREEN_STATE]
    cp 0
    jp z, .game

    isStartPressed
    jp .end

    .game
        UpdateJoypad
        UpdateChef
        UpdateGameState

        ld a, [FRAME_COUNTER]
        dec a
        ld [FRAME_COUNTER], a
        jp nz, .skip_decrement

        ld a, FRAME_DELAY
        ld [FRAME_COUNTER], a

        ld a, [COUNTDOWN_TIMER]
        cp 0
        jp z, .handle_timer_end 

        dec a                  
        ld [COUNTDOWN_TIMER], a 
        call UpdateTimerDisplay
        jp .skip_decrement

    .handle_timer_end
        call TimerEnd

        ; check if round done
        ld a, [CURRENT_ROUND]
        inc a
        ld [CURRENT_ROUND], a 

        InitRound

    .skip_decrement:
        ret

    .end
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

export InitSample, UpdateSample

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

section "graphics_data", rom0[GRAPHICS_DATA_ADDRESS_START]
incbin "tileset.chr"
incbin "start_screen.tlm"
incbin "window.tlm"
incbin "background.tlm"
