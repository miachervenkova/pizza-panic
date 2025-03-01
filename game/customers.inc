; Project 6
; @file customers.inc
; @author Mia Chervenkova and Charlie Beard
; @date Nov 17 2024

if !def(CUSTOMERS_INC)
def CUSTOMERS_RGBINC equ 1

include "hardware.inc"
include "utils.inc"

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

def LEVEL_INFO_RAM_OFFSET  equ ($F)
def LEVEL_INFO_LOCATION     equ (_RAM + LEVEL_INFO_RAM_OFFSET)

def FIRST_ORDER_NUMBER      equ (1)
def EMPTY_PIZZA             equ (0)

def PEPPERONI_BIT           equ (0)
def MUSHROOM_BIT            equ (1)
def ONION_BIT               equ (2)
def SARDINE_BIT             equ (3)
def SAUCE_BIT               equ (4)

; storage method for level info:
;   bit 0 –> pepperoni topping
;   bit 1 –> mushroom topping
;   bit 2 –> onion topping
;   bit 3 –> sardine topping
;   bit 4 –> sauce topping (always set to )
;   bit 5-7 –> unused

rsset LEVEL_INFO_LOCATION
def CURRENT_ORDER_INFO  rb 1 ; state of the current order 
def ORDER_NUMBER        rb 1 ; counter for the current order
def ORDER_SEQUENCE_1             rb 1 ; RENAME LEVEL 1 ORDER
def ORDER_SEQUENCE_2             rb 1
def ORDER_SEQUENCE_3             rb 1
def ORDER_SEQUENCE_4             rb 1
def ORDER_SEQUENCE_5             rb 1
def ORDER_SEQUENCE_6             rb 1
def ORDER_SEQUENCE_7             rb 1
def ORDER_SEQUENCE_8             rb 1
def ORDER_SEQUENCE_9             rb 1
def ORDER_SEQUENCE_10            rb 1
def ORDER_SEQUENCE_11            rb 1
def ORDER_SEQUENCE_12            rb 1
def ORDER_SEQUENCE_13            rb 1
def ORDER_SEQUENCE_14            rb 1
def ORDER_SEQUENCE_15            rb 1
def ORDER_SEQUENCE_16            rb 1
def LEVELS_COMPLETED    rb 1 ; counter of # of levels completed

def ORDER_1             equ (%00010000)
def ORDER_2             equ (%00010001)
def ORDER_3             equ (%00010100)
def ORDER_4             equ (%00010110)
def ORDER_5             equ (%00011001)
def ORDER_6             equ (%00011101)
def ORDER_7             equ (%00010111)
def ORDER_8             equ (%00011111)

def FINAL_ORDER         equ (16)

def TEMPLATE_PIZZA_OFFSET   equ (64)

def TEMPLATE_LEFT_X_POS     equ (PIZZA_LEFT_X_POS + TEMPLATE_PIZZA_OFFSET)
def TEMPLATE_LEFT_Y_POS     equ (PIZZA_LEFT_Y_POS)

def TEMPLATE_RIGHT_X_POS    equ (PIZZA_RIGHT_X_POS + TEMPLATE_PIZZA_OFFSET)
def TEMPLATE_RIGHT_Y_POS    equ (PIZZA_RIGHT_Y_POS)

def COUNTER_X_POS           equ (116)
def COUNTER_Y_POS           equ (80)

def SPRITE_MAX_BOTTOM       equ (160)
def SPRITE_MAX_RIGHT        equ (168)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

; customer tile positions
def CUSTOMER_1_TILE_ADDRESS equ (42)
def CUSTOMER_2_TILE_ADDRESS equ (10)
def CUSTOMER_3_TILE_ADDRESS equ (16)
def CUSTOMER_4_TILE_ADDRESS equ (22)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

; populating order info in ram
macro InitGameOrders
    ld a, ORDER_1
    ld [ORDER_SEQUENCE_1], a
    ld a, ORDER_2
    ld [ORDER_SEQUENCE_2], a
    ld a, ORDER_3
    ld [ORDER_SEQUENCE_3], a
    ld a, ORDER_1
    ld [ORDER_SEQUENCE_4], a
    ld a, ORDER_4
    ld [ORDER_SEQUENCE_5], a
    ld a, ORDER_3
    ld [ORDER_SEQUENCE_6], a
    ld a, ORDER_2
    ld [ORDER_SEQUENCE_7], a
    ld a, ORDER_5
    ld [ORDER_SEQUENCE_8], a
    ld a, ORDER_1
    ld [ORDER_SEQUENCE_9], a
    ld a, ORDER_7
    ld [ORDER_SEQUENCE_10], a
    ld a, ORDER_5
    ld [ORDER_SEQUENCE_11], a
    ld a, ORDER_6
    ld [ORDER_SEQUENCE_12], a
    ld a, ORDER_4
    ld [ORDER_SEQUENCE_13], a
    ld a, ORDER_3
    ld [ORDER_SEQUENCE_14], a
    ld a, ORDER_2
    ld [ORDER_SEQUENCE_15], a
    ld a, ORDER_8
    ld [ORDER_SEQUENCE_16], a

    ld a, FIRST_ORDER_NUMBER
    ld [ORDER_NUMBER], a
    ld a, EMPTY_PIZZA
    ld [CURRENT_ORDER_INFO], a
    ld a, 0
    ld [LEVELS_COMPLETED], a
endm

; sets initial positions and oam info for customer sprites, as well as template pizza
macro InitCustomerSprites
    rsset (_OAMRAM + (NUM_CHEF_SPRITES * sizeof_OAM_ATTRS))
    def CUSTOMER_SPRITE_ADDRESS             rl 1
    def CUSTOMER_PEPPERONI_SPRITE_ADDRESS   rl 1
    def CUSTOMER_MUSHROOM_SPRITE_ADDRESS    rl 1
    def CUSTOMER_ONION_SPRITE_ADDRESS       rl 1
    def CUSTOMER_SARDINE_SPRITE_ADDRESS     rl 1

    copy [CUSTOMER_SPRITE_ADDRESS + OAMA_Y], COUNTER_Y_POS
    copy [CUSTOMER_SPRITE_ADDRESS + OAMA_X], COUNTER_X_POS
    copy [CUSTOMER_SPRITE_ADDRESS + OAMA_TILEID], CUSTOMER_2_TILE_ADDRESS
    copy [CUSTOMER_SPRITE_ADDRESS + OAMA_FLAGS], OAMF_PAL0

    copy [CUSTOMER_PEPPERONI_SPRITE_ADDRESS + OAMA_Y], 0
    copy [CUSTOMER_PEPPERONI_SPRITE_ADDRESS + OAMA_X], 0
    copy [CUSTOMER_PEPPERONI_SPRITE_ADDRESS + OAMA_TILEID], PEPPERONI_TILE_ADDRESS
    copy [CUSTOMER_PEPPERONI_SPRITE_ADDRESS + OAMA_FLAGS], OAMF_PAL0

    copy [CUSTOMER_MUSHROOM_SPRITE_ADDRESS + OAMA_Y], 0
    copy [CUSTOMER_MUSHROOM_SPRITE_ADDRESS + OAMA_X], 0
    copy [CUSTOMER_MUSHROOM_SPRITE_ADDRESS + OAMA_TILEID], MUSHROOM_TILE_ADDRESS
    copy [CUSTOMER_MUSHROOM_SPRITE_ADDRESS + OAMA_FLAGS], OAMF_PAL0

    copy [CUSTOMER_ONION_SPRITE_ADDRESS + OAMA_Y], 0
    copy [CUSTOMER_ONION_SPRITE_ADDRESS + OAMA_X], 0
    copy [CUSTOMER_ONION_SPRITE_ADDRESS + OAMA_TILEID], ONION_TILE_ADDRESS
    copy [CUSTOMER_ONION_SPRITE_ADDRESS + OAMA_FLAGS], OAMF_PAL0

    copy [CUSTOMER_SARDINE_SPRITE_ADDRESS + OAMA_Y], 0
    copy [CUSTOMER_SARDINE_SPRITE_ADDRESS + OAMA_X], 0
    copy [CUSTOMER_SARDINE_SPRITE_ADDRESS + OAMA_TILEID], SARDINE_TILE_ADRESS
    copy [CUSTOMER_SARDINE_SPRITE_ADDRESS + OAMA_FLAGS], OAMF_PAL0

    UpdateTemplatePizza
endm

; sends a customer to the counter
macro SendCustomer

    UpdateCustomerAppearance

    ld a, [CUSTOMER_SPRITE_ADDRESS + OAMA_TILEID]
    add a, BACK_SPRITE_OFFSET
    ld [CUSTOMER_SPRITE_ADDRESS + OAMA_TILEID], a

    copy [CUSTOMER_SPRITE_ADDRESS + OAMA_Y], SPRITE_MAX_BOTTOM
    copy [CUSTOMER_SPRITE_ADDRESS + OAMA_X], COUNTER_X_POS

    ld a, COUNTER_Y_POS
    ld b, a
    ld a, [CUSTOMER_SPRITE_ADDRESS + OAMA_Y]
    .loop_enter_sprite\@
        halt
        halt
        dec a
        copy [CUSTOMER_SPRITE_ADDRESS + OAMA_Y], a
        cp a, b
        jp nz, .loop_enter_sprite\@

    ld a, [CUSTOMER_SPRITE_ADDRESS + OAMA_TILEID]
    sub a, BACK_SPRITE_OFFSET
    ld [CUSTOMER_SPRITE_ADDRESS + OAMA_TILEID], a
endm

; makes the customer exit the pizza shop
macro DepartCustomer

    ld a, [CUSTOMER_SPRITE_ADDRESS + OAMA_TILEID]
    add a, RIGHT_SPRITE_OFFSET
    ld [CUSTOMER_SPRITE_ADDRESS + OAMA_TILEID], a

    ld a, SPRITE_MAX_RIGHT
    ld b, a
    ld a, [CUSTOMER_SPRITE_ADDRESS + OAMA_X]
    .loop_exit_sprite\@
        halt
        halt
        inc a
        copy [CUSTOMER_SPRITE_ADDRESS + OAMA_X], a
        cp a, b
        jp nz, .loop_exit_sprite\@

    copy [CUSTOMER_SPRITE_ADDRESS + OAMA_Y], 0
    copy [CUSTOMER_SPRITE_ADDRESS + OAMA_X], 0

    ld a, [CUSTOMER_SPRITE_ADDRESS + OAMA_TILEID]
    sub a, RIGHT_SPRITE_OFFSET
    ld [CUSTOMER_SPRITE_ADDRESS + OAMA_TILEID], a
endm

; changes the tileid of the sprite between orders, so customers cycle through
macro UpdateCustomerAppearance
   ld a, [CUSTOMER_SPRITE_ADDRESS + OAMA_TILEID]
   .check_first\@
       cp a, CUSTOMER_1_TILE_ADDRESS
       jp nz, .check_second\@
       ld a, CUSTOMER_2_TILE_ADDRESS
       ld [CUSTOMER_SPRITE_ADDRESS + OAMA_TILEID], a
       jp .done_appearance\@
   .check_second\@
       cp a, CUSTOMER_2_TILE_ADDRESS
       jp nz, .check_third\@
       ld a, CUSTOMER_3_TILE_ADDRESS
       ld [CUSTOMER_SPRITE_ADDRESS + OAMA_TILEID], a
       jp .done_appearance\@
   .check_third\@
       cp a, CUSTOMER_3_TILE_ADDRESS
       jp nz, .check_fourth\@
       ld a, CUSTOMER_4_TILE_ADDRESS
       ld [CUSTOMER_SPRITE_ADDRESS + OAMA_TILEID], a
       jp .done_appearance\@
   .check_fourth\@
       cp a, CUSTOMER_4_TILE_ADDRESS
       jp nz, .done_appearance\@
       ld a, CUSTOMER_1_TILE_ADDRESS
       ld [CUSTOMER_SPRITE_ADDRESS + OAMA_TILEID], a
   .done_appearance\@
endm

; updates template pizza between orders
macro UpdateTemplatePizza
    ld hl, ORDER_NUMBER
    ld a, [ORDER_NUMBER]
    ld c, a
    ld b, 0
    add hl, bc

    .temp_pepperoni\@
        ld a, [hl]
        bit PEPPERONI_BIT, a
        jp z, .temp_no_pepperoni\@
        copy [CUSTOMER_PEPPERONI_SPRITE_ADDRESS + OAMA_Y], TEMPLATE_LEFT_Y_POS
        copy [CUSTOMER_PEPPERONI_SPRITE_ADDRESS + OAMA_X], TEMPLATE_LEFT_X_POS
        jp .temp_mushroom\@
    .temp_no_pepperoni\@
        copy [CUSTOMER_PEPPERONI_SPRITE_ADDRESS + OAMA_Y], 0
        copy [CUSTOMER_PEPPERONI_SPRITE_ADDRESS + OAMA_X], 0

    .temp_mushroom\@
        ld a, [hl]
        bit MUSHROOM_BIT, a
        jp z, .temp_no_mushroom\@
        copy [CUSTOMER_MUSHROOM_SPRITE_ADDRESS + OAMA_Y], TEMPLATE_LEFT_Y_POS
        copy [CUSTOMER_MUSHROOM_SPRITE_ADDRESS + OAMA_X], TEMPLATE_LEFT_X_POS
        jp .temp_onion\@
    .temp_no_mushroom\@
        copy [CUSTOMER_MUSHROOM_SPRITE_ADDRESS + OAMA_Y], 0
        copy [CUSTOMER_MUSHROOM_SPRITE_ADDRESS + OAMA_X], 0

    .temp_onion\@
        ld a, [hl]
        bit ONION_BIT, a
        jp z, .temp_no_onion\@
        copy [CUSTOMER_ONION_SPRITE_ADDRESS + OAMA_Y], TEMPLATE_RIGHT_Y_POS
        copy [CUSTOMER_ONION_SPRITE_ADDRESS + OAMA_X], TEMPLATE_RIGHT_X_POS
        jp .temp_sardine\@
    .temp_no_onion\@
        copy [CUSTOMER_ONION_SPRITE_ADDRESS + OAMA_Y], 0
        copy [CUSTOMER_ONION_SPRITE_ADDRESS + OAMA_X], 0

    .temp_sardine\@ 
        ld a, [hl]
        bit SARDINE_BIT, a
        jp z, .temp_no_sardine\@
        copy [CUSTOMER_SARDINE_SPRITE_ADDRESS + OAMA_Y], TEMPLATE_RIGHT_Y_POS
        copy [CUSTOMER_SARDINE_SPRITE_ADDRESS + OAMA_X], TEMPLATE_RIGHT_X_POS
        jp .temp_done\@
    .temp_no_sardine\@
        copy [CUSTOMER_SARDINE_SPRITE_ADDRESS + OAMA_Y], 0
        copy [CUSTOMER_SARDINE_SPRITE_ADDRESS + OAMA_X], 0

    .temp_done\@
endm

; compares template pizza  with current order status to see if the pizza is done
macro CheckOrderDone
    ld a, [CURRENT_ORDER_INFO]
    ld d, a
    ld hl, ORDER_NUMBER
    ld a, [ORDER_NUMBER]
    ld c, a
    ld b, 0
    add hl, bc
    ld a, [hl]
    
    cp a, d
    jp nz, .continue\@

    PlayCoinSound
    DepartCustomer
    StartNextOrder

    .continue\@
endm

; updates game info related to the specific order
macro StartNextOrder
    ld a, [ORDER_NUMBER]
    ld b, a
    cp a, FINAL_ORDER
    jp nz, .skip_reset\@

    ld a, [LEVELS_COMPLETED]
    add a, b
    ld [LEVELS_COMPLETED], a

    ld a, 0
    
    .skip_reset\@
        inc a
        ld [ORDER_NUMBER], a

    SendCustomer

    ld a, 0
    ld [CURRENT_ORDER_INFO], a

    UpdateTemplatePizza
endm

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

endc