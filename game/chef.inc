if !def(CHEF_INC)
def CHEF_RGBINC equ 1

include "hardware.inc"
include "utils.inc"

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

; pizza topping retrieval location ranges
def FIREPLACE_X_POS     equ (40)
def FIREPLACE_Y_POS     equ (40)
def FIREPALCE_WIDTH     equ (32)
def FIREPLACE_HEIGHT    equ (8)

def PEPPERONI_X_POS     equ (88)
def PEPPERONI_Y_POS     equ (40)
def PEPPERONI_WIDTH     equ (24)
def PEPPERONI_HEIGHT    equ (8)

def MUSHROOM_X_POS      equ (24)
def MUSHROOM_Y_POS      equ (48)
def MUSHROOM_WIDTH      equ (16)
def MUSHROOM_HEIGHT     equ (8)

def ONION_X_POS         equ (16)
def ONION_Y_POS         equ (90)
def ONION_WIDTH         equ (32)
def ONION_HEIGHT        equ (8)

def SARDINE_X_POS       equ (132)
def SARDINE_Y_POS       equ (40)
def SARDINE_WIDTH       equ (24)
def SARDINE_HEIGHT      equ (8)

; locations to move topping sprites on pizza
def PIZZA_LEFT_X_POS    equ (96)
def PIZZA_LEFT_Y_POS    equ (136)

def PIZZA_RIGHT_X_POS   equ (104)
def PIZZA_RIGHT_Y_POS   equ (136)

; locations for sauce moving
def PIZZA_TILE_LINE_1  equ ($11)
def PIZZA_TILE_LINE_2  equ ($20)
def PIZZA_TILE_LINE_3  equ ($30)
def PIZZA_TILE_LINE_4  equ ($41)

def PIZZA_TILE_OFFSET   equ ($4)

def PIZZA_LINE_1        equ ($9C0B)
def PIZZA_LINE_2        equ ($9C2A)
def PIZZA_LINE_3        equ ($9C4A)
def PIZZA_LINE_4        equ ($9C6B)

; number of sprites used by the chef
def NUM_CHEF_SPRITES    equ (5)

def CHEF_START_POS_X    equ (16)
def CHEF_START_POS_Y    equ (102)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;sprite tile addresses

def CHEF_TILE_ADDRESS       equ (4)
def PEPPERONI_TILE_ADDRESS  equ (34)
def MUSHROOM_TILE_ADDRESS   equ (36)
def ONION_TILE_ADDRESS      equ (38)
def SARDINE_TILE_ADRESS      equ (40)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


macro InitChefSprites
    rsset _OAMRAM
    def CHEF_SPRITE_ADDRESS         rl 1
    def PEPPERONI_SPRITE_ADDRESS    rl 1
    def MUSHROOM_SPRITE_ADDRESS     rl 1
    def ONION_SPRITE_ADDRESS        rl 1
    def SARDINE_SPRITE_ADDRESS      rl 1

    copy [CHEF_SPRITE_ADDRESS + OAMA_Y], CHEF_START_POS_Y
    copy [CHEF_SPRITE_ADDRESS + OAMA_X], CHEF_START_POS_X
    copy [CHEF_SPRITE_ADDRESS + OAMA_TILEID], CHEF_TILE_ADDRESS
    copy [CHEF_SPRITE_ADDRESS + OAMA_FLAGS], OAMF_PAL0

    copy [PEPPERONI_SPRITE_ADDRESS + OAMA_Y], 0
    copy [PEPPERONI_SPRITE_ADDRESS + OAMA_X], 0
    copy [PEPPERONI_SPRITE_ADDRESS + OAMA_TILEID], PEPPERONI_TILE_ADDRESS
    copy [PEPPERONI_SPRITE_ADDRESS + OAMA_FLAGS], OAMF_PAL0

    copy [MUSHROOM_SPRITE_ADDRESS + OAMA_Y], 0
    copy [MUSHROOM_SPRITE_ADDRESS + OAMA_X], 0
    copy [MUSHROOM_SPRITE_ADDRESS + OAMA_TILEID], MUSHROOM_TILE_ADDRESS
    copy [MUSHROOM_SPRITE_ADDRESS + OAMA_FLAGS], OAMF_PAL0

    copy [ONION_SPRITE_ADDRESS + OAMA_Y], 0
    copy [ONION_SPRITE_ADDRESS + OAMA_X], 0
    copy [ONION_SPRITE_ADDRESS + OAMA_TILEID], ONION_TILE_ADDRESS
    copy [ONION_SPRITE_ADDRESS + OAMA_FLAGS], OAMF_PAL0

    copy [SARDINE_SPRITE_ADDRESS + OAMA_Y], 0
    copy [SARDINE_SPRITE_ADDRESS + OAMA_X], 0
    copy [SARDINE_SPRITE_ADDRESS + OAMA_TILEID], SARDINE_TILE_ADRESS
    copy [SARDINE_SPRITE_ADDRESS + OAMA_FLAGS], OAMF_PAL0

endm

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

macro UpdateChef
    MoveChef
    CheckChefPosition
    UpdatePizza
endm

macro MoveChef
    .move_down
    ld a, [JOYPAD_CURRENT_ADDRESS]
    bit PADB_DOWN, a
    jp z, .move_up

    ld a, [CHEF_SPRITE_ADDRESS + OAMA_Y]
    inc a
    ld [CHEF_SPRITE_ADDRESS + OAMA_Y], a

    .move_up
    ld a, [JOYPAD_CURRENT_ADDRESS]
    bit PADB_UP, a
    jp z, .move_left

    ld a, [CHEF_SPRITE_ADDRESS + OAMA_Y]
    dec a
    ld [CHEF_SPRITE_ADDRESS + OAMA_Y], a

    .move_left
    ld a, [JOYPAD_CURRENT_ADDRESS]
    bit PADB_LEFT, a
    jp z, .move_right

    ld a, [CHEF_SPRITE_ADDRESS + OAMA_X]
    dec a
    ld [CHEF_SPRITE_ADDRESS + OAMA_X], a

    .move_right
    ld a, [JOYPAD_CURRENT_ADDRESS]
    bit PADB_RIGHT, a
    jp z, .move_none

    ld a, [CHEF_SPRITE_ADDRESS + OAMA_X]
    inc a
    ld [CHEF_SPRITE_ADDRESS + OAMA_X], a

    .move_none
endm

macro CheckChefPosition
    CheckFireplace
    CheckPepperoni
    CheckMushroom
    CheckOnion
    CheckSardine
endm

macro CheckFireplace
    ld a, [CHEF_SPRITE_ADDRESS + OAMA_X]
    cp a, FIREPLACE_X_POS
    jp c, .no_fireplace

    cp a, FIREPLACE_X_POS + FIREPALCE_WIDTH
    jp nc, .no_fireplace

    ld a, [CHEF_SPRITE_ADDRESS + OAMA_Y]
    cp a, FIREPLACE_Y_POS
    jp c, .no_fireplace

    cp a, FIREPLACE_Y_POS + FIREPLACE_HEIGHT
    jp nc, .no_fireplace

    ld a, [JOYPAD_PRESSED_ADDRESS]
    bit PADB_A, a
    jp z, .no_fireplace

    ld a, [CURRENT_ORDER_INFO]
    set SAUCE_BIT, a
    ld [CURRENT_ORDER_INFO], a

    .no_fireplace
endm

macro CheckPepperoni
    ld a, [CHEF_SPRITE_ADDRESS + OAMA_X]
    cp a, PEPPERONI_X_POS
    jp c, .no_pepperoni

    cp a, PEPPERONI_X_POS + PEPPERONI_WIDTH
    jp nc, .no_pepperoni

    ld a, [CHEF_SPRITE_ADDRESS + OAMA_Y]
    cp a, PEPPERONI_Y_POS
    jp c, .no_pepperoni

    cp a, PEPPERONI_Y_POS + PEPPERONI_HEIGHT
    jp nc, .no_pepperoni

    ld a, [JOYPAD_PRESSED_ADDRESS]
    bit 0, a
    jp z, .check_rm_pepperoni

    ld a, [CURRENT_ORDER_INFO]
    set PADB_A, a
    ld [CURRENT_ORDER_INFO], a

    .check_rm_pepperoni
        ld a, [JOYPAD_PRESSED_ADDRESS]
        bit PADB_B, a
        jp z, .no_pepperoni

        ld a, [CURRENT_ORDER_INFO]
        res PEPPERONI_BIT, a
        ld [CURRENT_ORDER_INFO], a

    .no_pepperoni
endm

macro CheckMushroom
    ld a, [CHEF_SPRITE_ADDRESS + OAMA_X]
    cp a, MUSHROOM_X_POS
    jp c, .no_mushroom

    cp a, MUSHROOM_X_POS + MUSHROOM_WIDTH
    jp nc, .no_mushroom

    ld a, [CHEF_SPRITE_ADDRESS + OAMA_Y]
    cp a, MUSHROOM_Y_POS
    jp c, .no_mushroom

    cp a, MUSHROOM_Y_POS + MUSHROOM_HEIGHT
    jp nc, .no_mushroom

    ld a, [JOYPAD_PRESSED_ADDRESS]
    bit PADB_A, a
    jp z, .check_rm_mushroom

    ld a, [CURRENT_ORDER_INFO]
    set MUSHROOM_BIT, a
    ld [CURRENT_ORDER_INFO], a

    ; if topping not needed, play the invalid sound
     ld a, [CURRENT_ORDER_INFO]
     bit MUSHROOM_BIT, a
     jp nz, .check_rm_mushroom
 
     InvalidToppingSound

    .check_rm_mushroom
        ld a, [JOYPAD_PRESSED_ADDRESS]
        bit PADB_B, a
        jp z, .no_mushroom

        ld a, [CURRENT_ORDER_INFO]
        res MUSHROOM_BIT, a
        ld [CURRENT_ORDER_INFO], a

    .no_mushroom
endm

macro CheckOnion
    ld a, [CHEF_SPRITE_ADDRESS + OAMA_X]
    cp a, ONION_X_POS
    jp c, .no_onion

    cp a, ONION_X_POS + ONION_WIDTH
    jp nc, .no_onion

    ld a, [CHEF_SPRITE_ADDRESS + OAMA_Y]
    cp a, ONION_Y_POS
    jp c, .no_onion

    cp a, ONION_Y_POS + ONION_HEIGHT
    jp nc, .no_onion

    ld a, [JOYPAD_PRESSED_ADDRESS]
    bit PADB_A, a
    jp z, .check_rm_onion

    ld a, [CURRENT_ORDER_INFO]
    set ONION_BIT, a
    ld [CURRENT_ORDER_INFO], a

    ; if topping not needed, play the invalid sound
    ld a, [CURRENT_ORDER_INFO]
    bit ONION_BIT, a
    jp nz, .check_rm_onion

    InvalidToppingSound

    .check_rm_onion
        ld a, [JOYPAD_PRESSED_ADDRESS]
        bit PADB_B, a
        jp z, .no_onion

        ld a, [CURRENT_ORDER_INFO]
        res ONION_BIT, a
        ld [CURRENT_ORDER_INFO], a

    .no_onion
endm

macro CheckSardine
    ld a, [CHEF_SPRITE_ADDRESS + OAMA_X]
    cp a, SARDINE_X_POS
    jp c, .no_sardine

    cp a, SARDINE_X_POS + SARDINE_WIDTH
    jp nc, .no_sardine

    ld a, [CHEF_SPRITE_ADDRESS + OAMA_Y]
    cp a, SARDINE_Y_POS
    jp c, .no_sardine

    cp a, SARDINE_Y_POS + SARDINE_HEIGHT
    jp nc, .no_sardine

    ld a, [JOYPAD_PRESSED_ADDRESS]
    bit PADB_A, a
    jp z, .check_rm_sardine

    ld a, [CURRENT_ORDER_INFO]
    set SARDINE_BIT, a
    ld [CURRENT_ORDER_INFO], a

    ; if topping not needed, play the invalid sound
    ld a, [CURRENT_ORDER_INFO]
    bit SARDINE_BIT, a
    jp nz, .check_rm_sardine

    InvalidToppingSound

    .check_rm_sardine
        ld a, [JOYPAD_PRESSED_ADDRESS]
        bit PADB_B, a
        jp z, .no_sardine

        ld a, [CURRENT_ORDER_INFO]
        res SARDINE_BIT, a
        ld [CURRENT_ORDER_INFO], a

    .no_sardine
endm

macro UpdatePizza
    .fireplace
        ld a, [CURRENT_ORDER_INFO]
        bit SAUCE_BIT, a
        jp z, .rm_fireplace
        AddFireplace
        jp .pepperoni
    .rm_fireplace
        RmFireplace
    
    .pepperoni
        ld a, [CURRENT_ORDER_INFO]
        bit PEPPERONI_BIT, a
        jp z, .rm_pepperoni
        AddPepperoni
        jp .mushroom
    .rm_pepperoni
        RmPepperoni

    .mushroom
        ld a, [CURRENT_ORDER_INFO]
        bit MUSHROOM_BIT, a
        jp z, .rm_mushroom
        AddMushroom
        jp .onion
    .rm_mushroom
        RmMushroom

    .onion
        ld a, [CURRENT_ORDER_INFO]
        bit ONION_BIT, a
        jp z, .rm_onion
        AddOnion
        jp .sardine
    .rm_onion
        RmOnion

    .sardine
        ld a, [CURRENT_ORDER_INFO]
        bit SARDINE_BIT, a
        jp z, .rm_sardine
        AddSardine
        jp .done
    .rm_sardine
        RmSardine
    .done
    
endm

macro AddFireplace
    ld a, PIZZA_TILE_LINE_1
    ld hl, PIZZA_LINE_1
    ld [hl], a
    inc a
    inc hl
    ld [hl], a

    ld a, PIZZA_TILE_LINE_2
    ld hl, PIZZA_LINE_2
    ld [hl], a
    inc hl
    inc a
    ld [hl], a
    inc hl
    inc a
    ld [hl], a
    inc hl
    inc a
    ld [hl], a
    
    ld a, PIZZA_TILE_LINE_3
    ld hl, PIZZA_LINE_3
    ld [hl], a
    inc hl
    inc a
    ld [hl], a
    inc hl
    inc a
    ld [hl], a
    inc hl
    inc a
    ld [hl], a

    ld a, PIZZA_TILE_LINE_4
    ld hl, PIZZA_LINE_4
    ld [hl], a
    inc hl
    inc a
    ld [hl], a
endm

macro RmFireplace
    ld a, PIZZA_TILE_LINE_1 + PIZZA_TILE_OFFSET
    ld hl, PIZZA_LINE_1
    ld [hl], a
    inc a
    inc hl
    ld [hl], a

    ld a, PIZZA_TILE_LINE_2 + PIZZA_TILE_OFFSET
    ld hl, PIZZA_LINE_2
    ld [hl], a
    inc hl
    inc a
    ld [hl], a
    inc hl
    inc a
    ld [hl], a
    inc hl
    inc a
    ld [hl], a
    
    ld a, PIZZA_TILE_LINE_3 + PIZZA_TILE_OFFSET
    ld hl, PIZZA_LINE_3
    ld [hl], a
    inc hl
    inc a
    ld [hl], a
    inc hl
    inc a
    ld [hl], a
    inc hl
    inc a
    ld [hl], a

    ld a, PIZZA_TILE_LINE_4 + PIZZA_TILE_OFFSET
    ld hl, PIZZA_LINE_4
    ld [hl], a
    inc hl
    inc a
    ld [hl], a
    
endm

macro AddPepperoni
    ld a, PIZZA_LEFT_X_POS
    ld [PEPPERONI_SPRITE_ADDRESS + OAMA_X], a
    ld a, PIZZA_LEFT_Y_POS
    ld [PEPPERONI_SPRITE_ADDRESS + OAMA_Y], a

    ; if topping not needed, play the invalid sound
    ld a, [CURRENT_ORDER_INFO]
    bit PEPPERONI_BIT, a
    jp z, .valid_pepperoni

    PlayCoinSound
    .valid_pepperoni
endm

macro AddMushroom
    ld a, PIZZA_LEFT_X_POS
    ld [MUSHROOM_SPRITE_ADDRESS + OAMA_X], a
    ld a, PIZZA_LEFT_Y_POS
    ld [MUSHROOM_SPRITE_ADDRESS + OAMA_Y], a
endm

macro AddOnion
    ld a, PIZZA_RIGHT_X_POS
    ld [ONION_SPRITE_ADDRESS + OAMA_X], a
    ld a, PIZZA_RIGHT_Y_POS
    ld [ONION_SPRITE_ADDRESS + OAMA_Y], a
endm

macro AddSardine
    ld a, PIZZA_RIGHT_X_POS
    ld [SARDINE_SPRITE_ADDRESS + OAMA_X], a
    ld a, PIZZA_RIGHT_Y_POS
    ld [SARDINE_SPRITE_ADDRESS + OAMA_Y], a
endm

macro RmPepperoni
    ld a, 0
    ld [PEPPERONI_SPRITE_ADDRESS + OAMA_X], a
    ld a, 0
    ld [PEPPERONI_SPRITE_ADDRESS + OAMA_Y], a
endm

macro RmMushroom
    ld a, 0
    ld [MUSHROOM_SPRITE_ADDRESS + OAMA_X], a
    ld a, 0
    ld [MUSHROOM_SPRITE_ADDRESS + OAMA_Y], a
endm

macro RmOnion
    ld a, 0
    ld [ONION_SPRITE_ADDRESS + OAMA_X], a
    ld a, 0
    ld [ONION_SPRITE_ADDRESS + OAMA_Y], a
endm

macro RmSardine
    ld a, 0
    ld [SARDINE_SPRITE_ADDRESS + OAMA_X], a
    ld a, 0
    ld [SARDINE_SPRITE_ADDRESS + OAMA_Y], a
endm

endc