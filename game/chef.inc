; Project 6
; @file chef.inc
; @author Mia Chervenkova and Charlie Beard
; @date Nov 17 2024

if !def(CHEF_INC)
def CHEF_RGBINC equ 1

include "hardware.inc"
include "utils.inc"

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

; pizza topping retrieval location ranges
def FIREPLACE_X_POS_LEFT     equ (40)
def FIREPLACE_Y_POS_TOP      equ (40)
def FIREPLACE_X_POS_RIGHT    equ (FIREPLACE_X_POS_LEFT + 32)
def FIREPLACE_Y_POS_BOTTOM   equ (FIREPLACE_Y_POS_TOP + 8)

def PEPPERONI_X_POS_LEFT     equ (88)
def PEPPERONI_Y_POS_TOP      equ (40)
def PEPPERONI_X_POS_RIGHT    equ (PEPPERONI_X_POS_LEFT + 24)
def PEPPERONI_Y_POS_BOTTOM   equ (PEPPERONI_Y_POS_TOP + 8)

def MUSHROOM_X_POS_LEFT      equ (24)
def MUSHROOM_Y_POS_TOP       equ (48)
def MUSHROOM_X_POS_RIGHT     equ (MUSHROOM_X_POS_LEFT + 16)
def MUSHROOM_Y_POS_BOTTOM    equ (MUSHROOM_Y_POS_TOP + 8)

def ONION_X_POS_LEFT         equ (16)
def ONION_Y_POS_TOP          equ (90)
def ONION_X_POS_RIGHT        equ (ONION_X_POS_LEFT + 32)
def ONION_Y_POS_BOTTOM       equ (ONION_Y_POS_TOP + 8)

def SARDINE_X_POS_LEFT       equ (132)
def SARDINE_Y_POS_TOP        equ (40)
def SARDINE_X_POS_RIGHT      equ (SARDINE_X_POS_LEFT + 24)
def SARDINE_Y_POS_BOTTOM     equ (SARDINE_Y_POS_TOP + 8)

; locations to move topping sprites on pizza
def PIZZA_LEFT_X_POS    equ (80)
def PIZZA_LEFT_Y_POS    equ (136)

def PIZZA_RIGHT_X_POS   equ (88)
def PIZZA_RIGHT_Y_POS   equ (136)

; locations for sauce moving
def PIZZA_TILE_LINE_1  equ ($11)
def PIZZA_TILE_LINE_2  equ ($20)
def PIZZA_TILE_LINE_3  equ ($30)
def PIZZA_TILE_LINE_4  equ ($41)

def PIZZA_TILE_OFFSET   equ ($4)

def PIZZA_LINE_1        equ ($9C09)
def PIZZA_LINE_2        equ ($9C28)
def PIZZA_LINE_3        equ ($9C48)
def PIZZA_LINE_4        equ ($9C69)

; number of sprites used by the chef
def NUM_CHEF_SPRITES    equ (5)

def CHEF_START_POS_X    equ (16)
def CHEF_START_POS_Y    equ (102)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

; sprite OAM locations
rsset _OAMRAM
def CHEF_SPRITE_ADDRESS         rl 1
def PEPPERONI_SPRITE_ADDRESS    rl 1
def MUSHROOM_SPRITE_ADDRESS     rl 1
def ONION_SPRITE_ADDRESS        rl 1
def SARDINE_SPRITE_ADDRESS      rl 1

;sprite tile addresses
def CHEF_TILE_ADDRESS       equ (4)
def PEPPERONI_TILE_ADDRESS  equ (34)
def MUSHROOM_TILE_ADDRESS   equ (36)
def ONION_TILE_ADDRESS      equ (38)
def SARDINE_TILE_ADRESS      equ (40)

def RIGHT_SPRITE_OFFSET     equ (2)
def BACK_SPRITE_OFFSET      equ (4)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

; initializes OAM data for the sprites associated with the chef
macro InitChefSprites
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

; handles all functionality for the chef, each vblank
macro UpdateChef
    MoveChef
    CheckChefPosition
    UpdatePizza
endm

; move the chef sprite based on joypad input
macro MoveChef
    ; storing previous location in memory for use with collisions
    ld a, [CHEF_SPRITE_ADDRESS + OAMA_X]
    ld [CHEF_PREV_X], a
    ld a, [CHEF_SPRITE_ADDRESS + OAMA_Y]
    ld [CHEF_PREV_Y], a

    .move_down\@
        ld a, [JOYPAD_CURRENT_ADDRESS]
        bit PADB_DOWN, a
        jp z, .move_up\@

        ld a, [CHEF_SPRITE_ADDRESS + OAMA_Y]
        inc a
        ld [CHEF_SPRITE_ADDRESS + OAMA_Y], a

    .move_up\@
        ld a, [JOYPAD_CURRENT_ADDRESS]
        bit PADB_UP, a
        jp z, .move_left\@

        ld a, [CHEF_SPRITE_ADDRESS + OAMA_Y]
        dec a
        ld [CHEF_SPRITE_ADDRESS + OAMA_Y], a

    .move_left\@
        ld a, [JOYPAD_CURRENT_ADDRESS]
        bit PADB_LEFT, a
        jp z, .move_right\@

        ld a, [CHEF_SPRITE_ADDRESS + OAMA_X]
        dec a
        ld [CHEF_SPRITE_ADDRESS + OAMA_X], a

    .move_right\@
        ld a, [JOYPAD_CURRENT_ADDRESS]
        bit PADB_RIGHT, a
        jp z, .move_none\@

        ld a, [CHEF_SPRITE_ADDRESS + OAMA_X]
        inc a
        ld [CHEF_SPRITE_ADDRESS + OAMA_X], a

    .move_none\@

    UpdateChefOrientation
endm

; change the chef sprite's orientation based on which buttons are held
macro UpdateChefOrientation
    .check_chef_up\@
        ld a, [JOYPAD_PRESSED_ADDRESS]
        bit PADB_UP, a
        jp z, .check_chef_down\@

        ld a, CHEF_TILE_ADDRESS + BACK_SPRITE_OFFSET
        ld [CHEF_SPRITE_ADDRESS + OAMA_TILEID], a
        jp .check_chef_left\@

    .check_chef_down\@
        ld a, [JOYPAD_PRESSED_ADDRESS]
        bit PADB_DOWN, a
        jp z, .check_chef_left\@

        ld a, CHEF_TILE_ADDRESS
        ld [CHEF_SPRITE_ADDRESS + OAMA_TILEID], a

    .check_chef_left\@
        ld a, [JOYPAD_PRESSED_ADDRESS]
        bit PADB_LEFT, a
        jp z, .check_chef_right\@

        ld a, [CHEF_SPRITE_ADDRESS + OAMA_FLAGS]
        set OAMB_XFLIP, a
        ld [CHEF_SPRITE_ADDRESS + OAMA_FLAGS], a
        ld a, CHEF_TILE_ADDRESS + RIGHT_SPRITE_OFFSET
        ld [CHEF_SPRITE_ADDRESS + OAMA_TILEID], a
        jp .orientation_done\@

    .check_chef_right\@
        ld a, [JOYPAD_PRESSED_ADDRESS]
        bit PADB_RIGHT, a
        jp z, .orientation_done\@

        ld a, [CHEF_SPRITE_ADDRESS + OAMA_FLAGS]
        res OAMB_XFLIP, a
        ld [CHEF_SPRITE_ADDRESS + OAMA_FLAGS], a
        ld a, CHEF_TILE_ADDRESS + RIGHT_SPRITE_OFFSET
        ld [CHEF_SPRITE_ADDRESS + OAMA_TILEID], a

    .orientation_done\@
endm

; comparing chef location with topping target locations
macro CheckChefPosition
    CheckFireplace
    CheckTopping PEPPERONI_X_POS_LEFT, PEPPERONI_X_POS_RIGHT, PEPPERONI_Y_POS_TOP, PEPPERONI_Y_POS_BOTTOM, PEPPERONI_BIT
    CheckTopping MUSHROOM_X_POS_LEFT, MUSHROOM_X_POS_RIGHT, MUSHROOM_Y_POS_TOP, MUSHROOM_Y_POS_BOTTOM, MUSHROOM_BIT
    CheckTopping ONION_X_POS_LEFT, ONION_X_POS_RIGHT, ONION_Y_POS_TOP, ONION_Y_POS_BOTTOM, ONION_BIT
    CheckTopping SARDINE_X_POS_LEFT, SARDINE_X_POS_RIGHT, SARDINE_Y_POS_TOP, SARDINE_Y_POS_BOTTOM, SARDINE_BIT
endm

; fireplace has to be separate because it moves tiles rather than sprites
macro CheckFireplace
    ld a, [CHEF_SPRITE_ADDRESS + OAMA_X]
    cp a, FIREPLACE_X_POS_LEFT
    jp c, .no_fireplace\@

    cp a, FIREPLACE_X_POS_RIGHT
    jp nc, .no_fireplace\@

    ld a, [CHEF_SPRITE_ADDRESS + OAMA_Y]
    cp a, FIREPLACE_Y_POS_TOP
    jp c, .no_fireplace\@

    cp a, FIREPLACE_Y_POS_BOTTOM
    jp nc, .no_fireplace\@

    ld a, [JOYPAD_PRESSED_ADDRESS]
    bit PADB_A, a
    jp z, .no_fireplace\@

    ld a, [CURRENT_ORDER_INFO]
    set SAUCE_BIT, a
    ld [CURRENT_ORDER_INFO], a

    .no_fireplace\@
endm

; updates "current order" with appropriate topping based on args:
; \1 is topping left x position
; \2 is topping right X position
; \3 is topping left y position
; \4 is topping right y position
; \5 is topping bit position
macro CheckTopping
    ld a, [CHEF_SPRITE_ADDRESS + OAMA_X]
    cp a, \1
    jp c, .no_topping\@

    cp a, \2
    jp nc, .no_topping\@

    ld a, [CHEF_SPRITE_ADDRESS + OAMA_Y]
    cp a, \3
    jp c, .no_topping\@

    cp a, \4
    jp nc, .no_topping\@

    ld a, [JOYPAD_PRESSED_ADDRESS]
    bit PADB_A, a
    jp z, .check_rm_topping\@

    ld a, [CURRENT_ORDER_INFO]
    set \5, a
    ld [CURRENT_ORDER_INFO], a

    .check_rm_topping\@
        ld a, [JOYPAD_PRESSED_ADDRESS]
        bit PADB_B, a
        jp z, .no_topping\@

        ld a, [CURRENT_ORDER_INFO]
        res \5, a
        ld [CURRENT_ORDER_INFO], a

    .no_topping\@
endm

; updates the chef's pizza visually with any changes made from check toppings
macro UpdatePizza
    .fireplace\@
        ld a, [CURRENT_ORDER_INFO]
        bit SAUCE_BIT, a
        jp z, .rm_fireplace\@
        AddFireplace
        jp .pepperoni\@
    .rm_fireplace\@
        RmFireplace
    
    .pepperoni\@
        ld a, [CURRENT_ORDER_INFO]
        bit PEPPERONI_BIT, a
        jp z, .rm_pepperoni\@
        AddTopping PIZZA_LEFT_X_POS, PIZZA_LEFT_Y_POS, [PEPPERONI_SPRITE_ADDRESS + OAMA_X], [PEPPERONI_SPRITE_ADDRESS + OAMA_Y]
        jp .mushroom\@
    .rm_pepperoni\@
        RmTopping [PEPPERONI_SPRITE_ADDRESS + OAMA_X], [PEPPERONI_SPRITE_ADDRESS + OAMA_Y]

    .mushroom\@
        ld a, [CURRENT_ORDER_INFO]
        bit MUSHROOM_BIT, a
        jp z, .rm_mushroom\@
        AddTopping PIZZA_LEFT_X_POS, PIZZA_LEFT_Y_POS, [MUSHROOM_SPRITE_ADDRESS + OAMA_X], [MUSHROOM_SPRITE_ADDRESS + OAMA_Y]
        jp .onion\@
    .rm_mushroom\@
        RmTopping [MUSHROOM_SPRITE_ADDRESS + OAMA_X], [MUSHROOM_SPRITE_ADDRESS + OAMA_Y]

    .onion\@
        ld a, [CURRENT_ORDER_INFO]
        bit ONION_BIT, a
        jp z, .rm_onion\@
        AddTopping PIZZA_RIGHT_X_POS, PIZZA_RIGHT_Y_POS, [ONION_SPRITE_ADDRESS + OAMA_X], [ONION_SPRITE_ADDRESS + OAMA_Y]
        jp .sardine\@
    .rm_onion\@
        RmTopping [ONION_SPRITE_ADDRESS + OAMA_X], [ONION_SPRITE_ADDRESS + OAMA_Y]

    .sardine\@
        ld a, [CURRENT_ORDER_INFO]
        bit SARDINE_BIT, a
        jp z, .rm_sardine\@
        AddTopping PIZZA_RIGHT_X_POS, PIZZA_RIGHT_Y_POS, [SARDINE_SPRITE_ADDRESS + OAMA_X], [SARDINE_SPRITE_ADDRESS + OAMA_Y]
        jp .done\@
    .rm_sardine\@
        RmTopping [SARDINE_SPRITE_ADDRESS + OAMA_X], [SARDINE_SPRITE_ADDRESS + OAMA_Y]
    .done\@ 
endm

; moving tiles to add sauce to pizza
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

; removes sauce from pizza
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

; adds toppings in appropriate location based on args:
; \1 is pizza x position
; \2 is pizza y position
; \3 is sprite x oam position
; \4 is sprite y oam position
macro AddTopping
    ld a, \1
    ld \3, a
    ld a, \2
    ld \4, a
endm

; removes topping based on args:
; \1 is sprite oam x position
; \2 is sprite oam y position
macro RmTopping
    ld a, 0
    ld \1, a
    ld a, 0
    ld \2, a
endm

endc