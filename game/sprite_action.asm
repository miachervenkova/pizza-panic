; Project 6
; @file sprite_action.asm
; @author Mia Chervenkova and Charlie Beard
; @date Nov 17 2024

; move sprite and alternate tile indices
update_sprite:
    ; timer for smooth motion
    inc c
    ld a, c
    and %00000111
    ld c, a
    jp nz, .do_nothing

    ; alternate between tile indices
    ld a, b
    xor 2
    ld b, a
    copy [SPRITE_0_ADDRESS + OAMA_TILEID], a

    ; change abscissa of sprite
    ld a, [SPRITE_0_ADDRESS + OAMA_X]
    inc a
    ld [SPRITE_0_ADDRESS + OAMA_X], a

    .do_nothing
        ret