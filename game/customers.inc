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
def LEVEL_1             rb 1
def LEVEL_2             rb 1
def LEVEL_3             rb 1
def LEVEL_4             rb 1
def LEVEL_5             rb 1
def LEVEL_6             rb 1
def LEVEL_7             rb 1
def LEVEL_8             rb 1
def LEVEL_9             rb 1
def LEVEL_10            rb 1
def LEVEL_11            rb 1
def LEVEL_12            rb 1
def LEVEL_13            rb 1
def LEVEL_14            rb 1
def LEVEL_15            rb 1
def LEVEL_16            rb 1
def LEVELS_COMPLETED    rb 1

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

def CUSTOMER_1_TILE_ADDRESS equ (0)
def CUSTOMER_2_TILE_ADDRESS equ (16)
def CUSTOMER_3_TILE_ADDRESS equ (18)
def CUSTOMER_4_TILE_ADDRESS equ (20)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

; populating order info in ram
macro InitGameOrders
    ld a, ORDER_1
    ld [LEVEL_1], a
    ld a, ORDER_2
    ld [LEVEL_2], a
    ld a, ORDER_3
    ld [LEVEL_3], a
    ld a, ORDER_1
    ld [LEVEL_4], a
    ld a, ORDER_4
    ld [LEVEL_5], a
    ld a, ORDER_3
    ld [LEVEL_6], a
    ld a, ORDER_2
    ld [LEVEL_7], a
    ld a, ORDER_5
    ld [LEVEL_8], a
    ld a, ORDER_1
    ld [LEVEL_9], a
    ld a, ORDER_7
    ld [LEVEL_10], a
    ld a, ORDER_5
    ld [LEVEL_11], a
    ld a, ORDER_6
    ld [LEVEL_12], a
    ld a, ORDER_4
    ld [LEVEL_13], a
    ld a, ORDER_3
    ld [LEVEL_14], a
    ld a, ORDER_2
    ld [LEVEL_15], a
    ld a, ORDER_8
    ld [LEVEL_16], a

    ld a, FIRST_ORDER_NUMBER
    ld [ORDER_NUMBER], a
    ld a, EMPTY_PIZZA
    ld [CURRENT_ORDER_INFO], a
    ld a, 0
    ld [LEVELS_COMPLETED], a
endm

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

macro SendCustomer
    copy [CUSTOMER_SPRITE_ADDRESS + OAMA_Y], SPRITE_MAX_BOTTOM
    copy [CUSTOMER_SPRITE_ADDRESS + OAMA_X], COUNTER_X_POS

    ld a, COUNTER_Y_POS
    ld b, a
    ld a, [CUSTOMER_SPRITE_ADDRESS + OAMA_Y]
    .loop_enter_sprite
        halt
        halt
        dec a
        copy [CUSTOMER_SPRITE_ADDRESS + OAMA_Y], a
        cp a, b
        jp nz, .loop_enter_sprite

endm

macro DepartCustomer
    ld a, SPRITE_MAX_RIGHT
    ld b, a
    ld a, [CUSTOMER_SPRITE_ADDRESS + OAMA_X]
    .loop_exit_sprite
        halt
        halt
        inc a
        copy [CUSTOMER_SPRITE_ADDRESS + OAMA_X], a
        cp a, b
        jp nz, .loop_exit_sprite

    copy [CUSTOMER_SPRITE_ADDRESS + OAMA_Y], 0
    copy [CUSTOMER_SPRITE_ADDRESS + OAMA_X], 0
endm

macro UpdateTemplatePizza
    ld hl, ORDER_NUMBER
    ld a, [ORDER_NUMBER]
    ld c, a
    ld b, 0
    add hl, bc

    .temp_pepperoni
        ld a, [hl]
        bit PEPPERONI_BIT, a
        jp z, .temp_no_pepperoni
        copy [CUSTOMER_PEPPERONI_SPRITE_ADDRESS + OAMA_Y], TEMPLATE_LEFT_Y_POS
        copy [CUSTOMER_PEPPERONI_SPRITE_ADDRESS + OAMA_X], TEMPLATE_LEFT_X_POS
        jp .temp_mushroom
    .temp_no_pepperoni
        copy [CUSTOMER_PEPPERONI_SPRITE_ADDRESS + OAMA_Y], 0
        copy [CUSTOMER_PEPPERONI_SPRITE_ADDRESS + OAMA_X], 0

    .temp_mushroom
        ld a, [hl]
        bit MUSHROOM_BIT, a
        jp z, .temp_no_mushroom
        copy [CUSTOMER_MUSHROOM_SPRITE_ADDRESS + OAMA_Y], TEMPLATE_LEFT_Y_POS
        copy [CUSTOMER_MUSHROOM_SPRITE_ADDRESS + OAMA_X], TEMPLATE_LEFT_X_POS
        jp .temp_onion
    .temp_no_mushroom
        copy [CUSTOMER_MUSHROOM_SPRITE_ADDRESS + OAMA_Y], 0
        copy [CUSTOMER_MUSHROOM_SPRITE_ADDRESS + OAMA_X], 0

    .temp_onion
        ld a, [hl]
        bit ONION_BIT, a
        jp z, .temp_no_onion
        copy [CUSTOMER_ONION_SPRITE_ADDRESS + OAMA_Y], TEMPLATE_RIGHT_Y_POS
        copy [CUSTOMER_ONION_SPRITE_ADDRESS + OAMA_X], TEMPLATE_RIGHT_X_POS
        jp .temp_sardine
    .temp_no_onion
        copy [CUSTOMER_ONION_SPRITE_ADDRESS + OAMA_Y], 0
        copy [CUSTOMER_ONION_SPRITE_ADDRESS + OAMA_X], 0

    .temp_sardine    
        ld a, [hl]
        bit SARDINE_BIT, a
        jp z, .temp_no_sardine
        copy [CUSTOMER_SARDINE_SPRITE_ADDRESS + OAMA_Y], TEMPLATE_RIGHT_Y_POS
        copy [CUSTOMER_SARDINE_SPRITE_ADDRESS + OAMA_X], TEMPLATE_RIGHT_X_POS
        jp .temp_done
    .temp_no_sardine
        copy [CUSTOMER_SARDINE_SPRITE_ADDRESS + OAMA_Y], 0
        copy [CUSTOMER_SARDINE_SPRITE_ADDRESS + OAMA_X], 0

    .temp_done
endm

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
    jp nz, .continue

    DepartCustomer
    StartNextOrder

    .continue
endm

macro StartNextOrder
    ld a, [ORDER_NUMBER]
    ld b, a
    cp a, FINAL_ORDER
    jp nz, .skip_reset

    ld a, [LEVELS_COMPLETED]
    add a, b
    ld [LEVELS_COMPLETED], a

    ld a, 0
    
    .skip_reset
        inc a
        ld [ORDER_NUMBER], a

    SendCustomer

    ld a, 0
    ld [CURRENT_ORDER_INFO], a

    UpdateTemplatePizza
endm

macro UpdateGameState
    CheckOrderDone
endm


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

endc