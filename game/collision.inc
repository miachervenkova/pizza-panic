if !def(COLLISION_INC)
def COLLISION_RGBINC equ 1

include "hardware.inc"
include "utils.inc"

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

def WRAM_COLLISION_OFFSET   equ ($200)
def WRAM_COLLISION_LOCATION equ (_RAM + WRAM_COLLISION_OFFSET)

rsset WRAM_COLLISION_LOCATION
def CHEF_PREV_X             rb 1
def CHEF_PREV_Y             rb 1

def SAFE_TILE_1    equ (199)
def SAFE_TILE_2    equ (200)
def SAFE_TILE_3    equ (201)

def SPRITE_X_OFFSET     equ (8)

def BACKGROUND_VRAM_ORIGIN equ ($9800)
def BACKGROUND_ROW_SIZE equ ($20)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

macro UpdateCollisionStatus
    ld a, [CHEF_SPRITE_ADDRESS + OAMA_X]
    sub a, SPRITE_X_OFFSET
    srl a
    srl a
    srl a
    ld e, a
    ld d, 0 ; de is the column

    ld a, [CHEF_SPRITE_ADDRESS + OAMA_Y]
    srl a
    srl a
    srl a 

    ld h, 0
    ld l, 0

    ld b, 0
    ld c, BACKGROUND_ROW_SIZE
    
    .multiply\@
        add hl, bc
        dec a
        jp nz, .multiply\@

    ld b, h
    ld c, l

    ; now bc contains row offset and de contains column offset 
    
    ld hl, BACKGROUND_VRAM_ORIGIN
    add hl, de
    add hl, bc

    ; now hl contains VRAM location of current tile

    ld a, [hl]

    cp a, SAFE_TILE_1
    jp z, .check_right\@

    cp a, SAFE_TILE_2
    jp z, .check_right\@

    cp a, SAFE_TILE_3
    jp z, .check_right\@  

    ; reset movememnt if in an illegal tile
    ld a, [CHEF_PREV_X]
    ld [CHEF_SPRITE_ADDRESS + OAMA_X], a
    ld a, [CHEF_PREV_Y]
    ld [CHEF_SPRITE_ADDRESS + OAMA_Y], a

    jp .collisions_over\@

    .check_right\@
        ld a, [CHEF_SPRITE_ADDRESS + OAMA_X]
        srl a
        srl a
        srl a
        ld e, a
        ld d, 0 ; de is the column

        ld hl, BACKGROUND_VRAM_ORIGIN
        add hl, de
        add hl, bc
        ; now hl contains VRAM location of current tile (of bottom right of sprite)

        ld a, [hl]

        cp a, SAFE_TILE_1
        jp z, .collisions_over\@

        cp a, SAFE_TILE_2
        jp z, .collisions_over\@

        cp a, SAFE_TILE_3
        jp z, .collisions_over\@  

        ; reset movememnt if in an illegal tile
        ld a, [CHEF_PREV_X]
        ld [CHEF_SPRITE_ADDRESS + OAMA_X], a
        ld a, [CHEF_PREV_Y]
        ld [CHEF_SPRITE_ADDRESS + OAMA_Y], a

    .collisions_over\@
         
endm

endc