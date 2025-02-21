section .text
global mdiv

; int64_t mdiv(int64_t *x, size_t n, int64_t y);
; Wejście:
;   rdi - adres x
;   rsi - n
;   rdx - y
; Wyjście:
;   rax - x % y
;   rdi - adres x (po podzieleniu)

mdiv:
    test rdx, rdx                   ; sprawdz czy y = 0
    jz .nadmiar                     ; jesli tak, zglos wystapienie nadmiaru

    cmp rdx, 1                      ; sprawdz czy y = 1
    je .dzielenie_przez_1           ; jesli tak, uruchom dzielenie przez 1

    mov rcx, rsi                    ; zapis n w rcx (jako licznik petli)
    mov r8, rdx                     ; zapis y w r8
    mov r9, [rdi + rcx * 8 - 8]     ; zapis najbardziej znaczacego bitu w r9
    mov r10, rdx                    ; zapis y w r10 (pozniej r10 = |y|)

    test r9, r9                     ; sprawdz czy x < 0
    js .ujemna_dzielna              ; jesli tak, uruchom .ujemna_dzielna

.ustaw_r10:
    test r8, r8                     ; sprawdz czy y < 0
    jns .przygotowanie_do_dzielenia ; jesli nie, uruchom dzielenie
    neg r10                         ; jesli tak, ustaw r10 = -y

.przygotowanie_do_dzielenia:
    xor rdx, rdx

.dzielenie_w_petli:                 ; wykonaj dzielenie dla |x| i |y|
    mov rax, [rdi + rcx * 8 - 8]
    div r10                         ; po dzieleniu rax - iloraz, rdx - reszta
    mov [rdi + rcx * 8 - 8], rax

    dec rcx                         ; zmniejsz licznik petli
    jnz .dzielenie_w_petli          ; jesli rcx > 0, kontynuuj petle
                                    ; jesli nie:
    mov rax, rdx                    ; zapisz reszte w rax

    test r9, r9                     ; sprawdz znak x
    js .sprawdz_dzielnik            ; x < 0, sprawdzam znak y

    test r8, r8                     ; x > 0, sprawdzam znak y
    js .negacja_x                   ; x > 0, y < 0

    ret                             ; jesli x, y > 0, zakoncz program

.sprawdz_dzielnik:                  ; x < 0
    test r8, r8                     ; sprawdz znak y
    js .negacja_reszty              ; jesli x, y < 0, zaneguj reszte
                                    ; jesli nie to x < 0, y > 0, wiec
    call .negacja_x                 ; zaneguj x
                                    ; oraz
.negacja_reszty:                    ; zaneguj reszte
    neg rax
    ret                             ; zakoncz program

.ujemna_dzielna:                    ; jesli x < 0:
    xor rdx, rdx
    mov rax, 0x8000000000000000     ; sprawdz czy najbardziej znaczacy
    div qword [rdi + rcx * 8 - 8]   ; bit x jest rowny -2^64
    cmp rax, 1                      ; jesli tak to zglos nadmiar
    je .nadmiar

    call .negacja_x                 ; zaneguj x
    jmp .ustaw_r10                  ; powrot do odpowiedniego miejsca programu

.negacja_x:                         ; funkcja negujaca x
    mov rcx, rsi
    cmp rsi, 1                      ; jesli n = 1, wykonaj uproszczona negacje
    je .prosta_negacja
                                    
.negacja_x_petla:                   ; jesli nie, wykonuj negacje w petli
    not qword [rdi + rcx * 8 - 8]   ; odwroc bity
    dec rcx                         ; zmniejsz licznik petli
    jnz .negacja_x_petla            ; jesli licznik petli > 0, kontynuuj petle

.dodaj_1:                           ; dodaj 1 do najmniej znaczacego bitu
    cmp rcx, rsi                    ; jesli rcx = rsi, liczba za duza,
    je .nadmiar                     ; wiec zglos nadmiar
    
    inc rcx                         ; zwieksz licznik
    add qword [rdi + rcx * 8 - 8], 1; dodaj 1 do aktualnego bitu
    jc .dodaj_1                     ; jesli wystapilo przeniesienie to
                                    ; dodaj 1 do kolejnego bitu

    mov rcx, rsi                    ; przywroc wartosc rejestru rcx
    ret                             ; zakoncz negacje

.prosta_negacja:                    ; wykonaj prosta negacje
    mov rdx, [rdi]                  ; zapis x w rdx
    neg rdx                         ; negacja rdx
    mov [rdi], rdx                  ; zapis rdx w x
    ret                             ; zakoncz negacje

.nadmiar:
    xor rax, rax
    div rax                         ; zgloszenie nadmiaru

.dzielenie_przez_1:                 ; dzielenie przez 1, czyli
    xor rax, rax                    ; ustawienie reszty na 0
    ret                             ; koniec programu
