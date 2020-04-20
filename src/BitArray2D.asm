
        global  set_bit_elem
        global  get_bit_elem
        section .text


    ; Set bit to true at row and column
    ; @param row
    ; @param column
set_bit_elem:
        push rbp            ; save the base pointer on the stack (at rsp+8)
        mov rbp, rsp        ; set up the rbp for the bottom of this frame

        ; rdi contains array pointer
        ; rsi contains row width
        ; rdx contains row
        ; rcx contains col

        ; add your code here

		;get to index
        imul rsi, rdx
        add rsi, rcx
        push rsi

        ;byte offset
        shr rsi, 3
        push rsi

        ;bit offset
        imul rsi, 8
        pop rcx
        pop rdx
        push rcx
        sub rdx, rsi

        ;create mask in rsi
        mov rsi, 1
        mov rcx, 7
        sub rcx, rdx

        ;loop shifts left by 1
        .loop:
        cmp rcx, 0
        je .continue
		dec rcx
        sal rsi, 1
        jnz .loop

        ;or data and mask together
        .continue:
		pop rcx
		mov rdx, [rdi + rcx]
		;or flips the bit
		or rdx, rsi
		mov [rdi + rcx], rdx

        mov rsp, rbp        ; restore stack pointer to before we pushed parameters onto the stack
        pop rbp             ; remove rbp from the stack to restore rsp to initial value
        ret                 ; return value in rax




    ; Get bit at row and column
    ; @param row
    ; @param column
    ; @return bit at row and column as bool
get_bit_elem:
        push rbp            ; save the base pointer on the stack (at rsp+8)
        mov rbp, rsp        ; set up the rbp for the bottom of this frame

        ; rdi contains array pointer
        ; rsi contains row width
        ; rdx contains row
        ; rcx contains col

        ; add your code here - for now returning 0

		;get to index
        imul rsi, rdx
        add rsi, rcx
        push rsi

        ;byte offset
        shr rsi, 3
        push rsi

        ;bit offset
        imul rsi, 8
        pop rcx
        pop rdx
        push rcx
        sub rdx, rsi

        ;create mask in rsi
        mov rsi, 1
        mov rcx, 7
        sub rcx, rdx

        ;loop shifts left by 1
        .loop:
        cmp rcx, 0
        je .continue
		dec rcx
        sal rsi, 1
        jnz .loop

        ;and data and mask to get bit
        .continue:
		pop rcx
		mov rdx, [rdi + rcx]
        and rdx, rsi
		cmp rdx, 0
        setg al
        movsx rax, al

        mov rsp, rbp        ; restore stack pointer to before we pushed parameters onto the stack
        pop rbp             ; remove rbp from the stack to restore rsp to initial value
        ret                 ; return value in rax
