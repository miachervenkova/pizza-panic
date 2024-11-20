; Project 6
; @file game.asm
; @author Mia Chervenkova and Charlie Beard
; @date Nov 17 2024

include "utils.inc"
include "chef.inc"
include "customers.inc"
include "audio.inc"
include "joypad.inc"
include "timer_functions.asm"
include "graphics.asm"
include "collision.inc"

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

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

rsset _RAM

def WRAM_FRAME_COUNTER              rb 1

def WRAM_END                        rb 0

; game states
def CURRENT_ROUND                   equ ($FF83)
def FINAL_ROUND_SCORE               equ ($FF84)

; final round goal
def FINAL_GOAL                      equ (4)

; initializes timer for the round
macro InitRound
    ld a, [CURRENT_ROUND]

    cp 1
    jp z, .init_round_1
    
    cp 2
    jp z, .init_round_2

    cp 3
    jp z, .init_round_final

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

    .init_round_final
        ld a, TIMER_ROUND_FINAL           
        ld [COUNTDOWN_TIMER], a
        call UpdateTimerDisplay   
        ld a, 0
        ld [FINAL_ROUND_SCORE], a  
        jp .over

    .over:        
        endm

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

section "game", rom0

InitGame:    
    ; init the WRAM state
    copy [WRAM_FRAME_COUNTER], $FF

    ; init the sound
    copy [rNR52], AUDENA_ON
    copy [rNR50], $77
    copy [rNR51], $FF

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

UpdateOnVblank:
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
        CheckOrderDone
        UpdateCollisionStatus

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

export InitGame, UpdateOnVblank

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

section "graphics_data", rom0[GRAPHICS_DATA_ADDRESS_START]
incbin "tileset.chr"
incbin "start_screen.tlm"
incbin "window.tlm"
incbin "background.tlm"
