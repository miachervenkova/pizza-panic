; Project 6
; @file timer_functions.asm
; @author Mia Chervenkova and Charlie Beard
; @date Nov 17 2024

section "timer", rom0

; updates timer tiles on display
UpdateTimerDisplay:
    ld a, [COUNTDOWN_TIMER]

    ; calculate hundreds digit
    ld b, a                   
    call CalculateHundreds
    ld [WINDOW_TILE_HUNDREDS], a 

    ; calculate tens digit             
    ld a, b               
    call CalculateTens    
    ld [WINDOW_TILE_TENS], a 

    ; calculate ones digit/remainder
    ld a, b               
    add a, DIGIT_TILE_START 
    ld [WINDOW_TILE_ONES], a 
    
    ret

CalculateTotalPizzas:
    ld a, [ORDER_NUMBER]
    ld b, a
    ld a, [LEVELS_COMPLETED]
    add a, b
    dec a

    ; calculate tens digit             
    ld b, a       
    call CalculateTens    
    ld [PIZZAS_TILE_TENS], a 

    ; calculate ones digit/remainder
    ld a, b               
    add a, DIGIT_TILE_START 
    ld [PIZZAS_TILE_ONES], a 
    
    ret
    
; calculate the hundreds digit
CalculateHundreds:
    ld c, 0                
    .tens_loop:
        cp 100                 
        jr c, .done_hundreds  
        sub 100               
        inc c                 
        jr .tens_loop         

    .done_hundreds:
        ld b, a                
        ld a, c               
        add a, DIGIT_TILE_START 
        ret                  

; calculate the tens digit
CalculateTens:
    ld c, 0                
    .tens_loop:
        cp 10                  
        jr c, .done_tens        
        sub 10                  
        inc c                 
        jr .tens_loop
    .done_tens:
        ld b, a               
        ld a, c                 
        add a, DIGIT_TILE_START 
        ret