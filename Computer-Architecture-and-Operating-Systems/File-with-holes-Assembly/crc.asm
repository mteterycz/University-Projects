global _start

section .bss
    buffer resb 65535               ; Bufor na dane
    poly resb 16                    ; Bufor na wielomian
    crc_result resb 16              ; Bufor na wynik
    crc_poly_lenght resq 1          ; Długość wielomianu      

section .text
_start:
    ; Sprawdź ilość parametrów
    mov rdi, [rsp]                  ; Ładuj do rdi liczbę parametrów
    cmp rdi, 3                      ; Sprawdź czy liczba parametrów równa 3
    jne .error_exit                 ; Jeśli rdi != 3, wykonaj error_exit

    ; Otwórz plik
    mov rdi, [rsp + 16]             ; rdi = 1. argument = nazwa pliku
    mov rsi, 0                      ; Tryb otwarcia
    mov rdx, 0                      ; Domyślne uprawnienia
    mov rax, 2                      ; sys_open
    syscall

    test rax, rax
    js .error_exit                  ; Jeśli rax < 0, wykonaj error_exit

    mov rdi, rax                    ; rdi = deskryptor pliku

    ; Wczytaj wielomian
    mov rsi, [rsp + 24]             ; rsi = 2. argument = wielomian CRC
    call .wczytaj_wielomian

.main_loop:
    ; Wczytaj długość fragmentu (2 bajty)
    mov rsi, buffer                 ; rsi = buffer
    mov rdx, 2                      ; rdx = 2 (liczba bajtów do wczytania)
    xor rax, rax                    ; sys_read
    syscall
    
    test rax, rax
    js .error_exit                  ; Jeśli rax < 0, wykonaj error_exit
    cmp rax, 2
    jne .eof_exit                   ; Jeśli rax < 2, koniec pliku
    
    movzx r10, word [buffer]        ; r10 = długość fragmentu
    cmp r10, 0                      
    je .wczytaj_przesuniecie        ; Jeśli r10 = 0, wczytaj przesunięcie

    ; Wczytaj fragment
    mov rsi, buffer                 ; rsi = buffer
    mov rdx, r10                    ; rdx = długość fragmentu
    xor rax, rax                    ; sys_read
    syscall

    test rax, rax
    js .error_exit                  ; Jeśli rax < 0, wykonaj error_exit
    cmp rax, r10
    jne .error_exit                 ; Jeśli wczytano za mało, error_exit

    ; Zaktualizuj wynik
    mov rcx, r10
    call .update_crc

.wczytaj_przesuniecie:
    mov rsi, buffer                 ; rsi = buffer
    mov rdx, 4                      ; rdx = 4 (4-bajtowe przesunięcie)
    xor rax, rax                    ; sys_read
    syscall

    test rax, rax
    js .error_exit                  ; Jeśli rax < 0, wykonaj error_exit
    cmp rax, 4
    jne .error_exit                 ; Jeśli wczytano za mało, error_exit
    
    mov esi, dword [buffer]         ; esi = przesuniecie
    test esi, esi
    jns .wykonaj_przesuniecie       ; Jeśli esi >= 0

    add r10, 6                      ; r10 = dlugosc fragmentu (dlugosc + dane + przesuniecie)
    neg esi                         ; esi = -esi

    cmp r10, rsi                    ; Jeśli rsi = r10, koniec danych
    je .zakonczenie

    neg rsi                         ; rsi = -rsi
    
.wykonaj_przesuniecie:
    mov rax, 8                      ; sys_lseek
    mov rdx, 1                      ; wzgledem aktualnej pozycji
    syscall

    test rax, rax
    js .error_exit                  ; Jeśli rax < 0, wykonaj error_exit

    jmp .main_loop                  ; Wczytaj kolejny fragment

.zakonczenie:
    mov r9, 120                     ; r9 - licznik .liczenie_xor
    call .liczenie_xor              

.wypisz_wynik:
    mov r10, 1
    mov r9, [crc_poly_lenght]
    mov rax, [crc_result]
    xor rsi, rsi                    ; Wyzeruj rsi

.petla_pisanie:
    mov sil, '0'                    
    bt rax, 63                      ; Sprawdź kolejny bit
    jnc .dodaj_cyfre_do_bufora      ; Jeśli jest równy 1 to sil = '1'

    mov sil, '1'

.dodaj_cyfre_do_bufora:
    mov [buffer + r10 - 1], sil     ; Dodaj sil do bufora
    shl rax, 1
    inc r10
    cmp r10, r9
    jne .petla_pisanie
    
.wypisz_wynik1:
    mov [buffer + r10 - 1], byte `\n`; Dodaj koniec linii do bufora
    mov rax, 1
    mov rdi, 1
    mov rsi, buffer
    mov rdx, r10
    syscall

; Zakończ program
.eof_exit:
    mov eax, 60                     ; sys_exit
    xor edi, edi                    ; Exit code 0
    syscall
    ret

.error_exit:
    mov eax, 60                     ; sys_exit
    mov edi, 1                      ; Exit code 1
    syscall

.wczytaj_wielomian:
    xor rcx, rcx
    mov r8, [poly]
    inc r8

.wczytaj_wielomian_petla:
    mov al, [rsi + rcx]             ; Load byte from rsi to al
    inc rcx

    test al, al
    jz .wczytaj_wielomian_koniec    ; If end of string, done 
    
    shl r8, 1                 

    cmp al, '0'                     ; znak = '0'
    je .wczytaj_wielomian_petla

    cmp al, '1'         
    jne .error_exit                 ; znak != '0' oraz znak != '1'
    
    inc r8
    jmp .wczytaj_wielomian_petla
    
.wczytaj_wielomian_koniec:
    cmp rcx, 1
    je .error_exit

    mov [crc_poly_lenght], rcx

    cmp rcx, 65
    je .stopien_65

    mov rcx, 64
    sub rcx, [crc_poly_lenght]
    shl r8, cl
    mov [poly], r8                  ; Zapisz wielomian
    xor rax, rax
    mov [poly + 8], rax
    
    mov rax, 1
    ret

.stopien_65:
    xor rcx, rcx
    bt r8, 0
    jnc .bit_0

    bts rcx, 63

.bit_0:
    shr r8, 1
    bts r8, 63
    mov [poly], r8
    mov [poly + 8], rcx
    mov rax, 1
    ret

.update_crc:
    ; rcx = length of fragment

    mov rsi, [poly]  
    mov rax, [crc_result + 8]       ; Load current CRC value
    xor r8, r8                      ; licznik petli przetwarzania

.przetwarzanie_bajtow_we_fragmencie:   
    xor rdx, rdx                                            
    mov dl, byte [buffer + r8]
    mov rax, [crc_result + 8]
    add rax, rdx                    ; dodaj kolejne 8 bitow do crc_result+8
    mov [crc_result + 8], rax

    mov r9, 8                       ; r9 licznik petli xor  
    call .liczenie_xor

    inc r8
    cmp r8, rcx                     ; jesli r8=rcx to koniec fragmentu         
    jne .przetwarzanie_bajtow_we_fragmencie

    ret

.liczenie_xor:
    ; r9 - ile razy ma wykonać się pętla
    
    mov rax, [crc_result]
    mov r11, [crc_result + 8]
    mov rsi, [poly]
    mov rdx, [poly + 8]

    bt rax, 63
    jnc .zerowy_bit

    xor rax, rsi
    xor r11, rdx
    
.zerowy_bit:
    shl rax, 1
    shl r11, 1
    jnc .zmniejsz_licznik
    
    inc rax   
    
.zmniejsz_licznik:
    mov [crc_result + 8], r11
    mov [crc_result], rax
    dec r9
    cmp r9, 0
    jne .liczenie_xor
    ret