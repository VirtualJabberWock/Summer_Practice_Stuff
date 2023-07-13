.686P
.model flat

extrn _printf: proc
;public _main
public _fun

.data
msg db "Hello, World!", 13, 10, 0
format db "%s", 0
format2 db "%d", 13, 10, 0

.code
   
_fun:
    sub esp, 4
    
    mov eax, [esp + 8]
    mov [esp], eax
    mov eax, [esp + 12]
    add [esp], eax
    mov eax, [esp]
    
    add esp, 4
    ret

end
