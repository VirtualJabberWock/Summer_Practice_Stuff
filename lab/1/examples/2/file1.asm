.686P
.model flat

extrn _printf: proc
public _main

.data
msg db "Hello, World!", 13, 10, 0
format db "%s", 0
format2 db "%d", 13, 10, 0

.code

_main:

    ; printf (format, msg);
    push offset msg
    push offset format
    call _printf
    add esp, 8

    push 1
    push 2
    call fun
    add esp, 8
    
    push eax
    push offset format2
    call _printf
    add esp, 8
    
    ret
    
fun:
    sub esp, 4
    
    mov eax, [esp + 8]
    mov [esp], eax
    mov eax, [esp + 12]
    add [esp], eax
    mov eax, [esp]
    
    add esp, 4
    ret

end
