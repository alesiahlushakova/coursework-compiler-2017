.586
.model FLAT, C , stdcall
includelib libucrt.lib 
includelib STD.lib 
includelib kernel32.lib 
ExitProcess PROTO : DWORD 
.stack 4096
showstr PROTO : DWORD
showrc PROTO : DWORD
shownum PROTO : DWORD
issubstr PROTO : DWORD, : DWORD
getstrlen PROTO : DWORD
arsum PROTO : DWORD, : DWORD, : DWORD
geomsum PROTO : DWORD, : DWORD, : DWORD
.CONST
L1 DWORD 2
L2 DWORD 0
L3 DWORD 1
L4 DWORD 3
L5 DWORD 5
L6 DWORD 8
L7 DB "hello ", 0
L8 DWORD 100
L9 DB "hello world ", 0
L10 DWORD 7
L11 DB "контрольный пример", 0
L12 DB " cумма арифм прогресссии a 3 d 5 n 8 ", 0
L13 DB " cумма геом прогрессии b 3 q 5 n 7 умноженная на 2 ", 0
L14 DB " 3*(5+3)/2 равно ", 0
L15 DB " является ли hello подстрокой hello world?", 0
L16 DB " длина строки hello ", 0
L17 DWORD 0
.data
fcheckz DWORD 0
mainx DWORD 0
mainy DWORD 0
mainisstr DWORD 0
mainz DWORD 0
maina DWORD 0
mainasum DWORD 0
maingsum DWORD 0
mainsa DB 255 dup(0)
mainsb DB 255 dup(0)
mainsc DWORD 0
mainqwert DWORD 0
mainc DWORD 0
.CODE
fcheck PROC fcheckx : DWORD, fchecky : DWORD
PUSH fcheckx
PUSH fcheckx
PUSH fchecky
POP EDX
POP EBX
ADD EBX, EDX
PUSH EBX
POP EBX
POP EAX
MOV EDX, 0
MUL EBX
MOV EBX, EAX
PUSH EBX
PUSH L1
POP EBX
POP EAX
MOV EDX, 0
DIV EBX
MOV EBX, EAX
PUSH EBX
POP fcheckz
MOV EAX, fcheckz
ret
fcheck ENDP
main PROC
PUSH L2
POP mainqwert
PUSH mainqwert
CALL showrc 
PUSH L3
CALL showrc 
PUSH L4
POP mainx
PUSH L5
POP mainy
PUSH L6
POP maina
PUSH mainy
PUSH mainx
CALL fcheck
PUSH EAX
POP mainz
MOV ESI, OFFSET L7
MOV EDI, OFFSET mainsa
MOV ECX, 7
REP MOVSB
PUSH OFFSET mainsa
CALL getstrlen
PUSH EAX
PUSH L8
POP EDX
POP EBX
SUB EBX, EDX
PUSH EBX
POP mainisstr
MOV ESI, OFFSET L9
MOV EDI, OFFSET mainsb
MOV ECX, 13
REP MOVSB
PUSH OFFSET mainsa
PUSH OFFSET mainsb
CALL issubstr
PUSH EAX
POP mainc
PUSH maina
PUSH mainy
PUSH mainx
CALL arsum
PUSH EAX
POP mainasum
PUSH L10
PUSH L5
PUSH L4
CALL geomsum
PUSH EAX
PUSH L1
POP EBX
POP EAX
MOV EDX, 0
MUL EBX
MOV EBX, EAX
PUSH EBX
POP maingsum
PUSH OFFSET L11
CALL showstr 
PUSH OFFSET L12
CALL showstr 
PUSH mainasum
CALL shownum 
PUSH OFFSET L13
CALL showstr 
PUSH maingsum
CALL shownum 
PUSH OFFSET L14
CALL showstr 
PUSH mainz
CALL shownum 
PUSH OFFSET L15
CALL showstr 
PUSH mainc
CALL showrc 
PUSH OFFSET L16
CALL showstr 
PUSH mainisstr
CALL shownum 
MOV EAX, L17
PUSH 0
CALL ExitProcess
MAIN ENDP
end main
check PROC checkx : DWORD, checky : DWORD
PUSH L4
POP mainx
PUSH L5
POP mainy
PUSH L6
POP maina
PUSH mainy
PUSH mainx
CALL fcheck
PUSH EAX
POP mainz
MOV ESI, OFFSET L7
MOV EDI, OFFSET mainsa
MOV ECX, 7
REP MOVSB
PUSH OFFSET mainsa
CALL getstrlen
PUSH EAX
PUSH L8
POP EDX
POP EBX
SUB EBX, EDX
PUSH EBX
POP mainisstr
MOV ESI, OFFSET L9
MOV EDI, OFFSET mainsb
MOV ECX, 13
REP MOVSB
PUSH OFFSET mainsa
PUSH OFFSET mainsb
CALL issubstr
PUSH EAX
POP mainc
PUSH maina
PUSH mainy
PUSH mainx
CALL arsum
PUSH EAX
POP mainasum
PUSH L10
PUSH L5
PUSH L4
CALL geomsum
PUSH EAX
PUSH L1
POP EBX
POP EAX
MOV EDX, 0
MUL EBX
MOV EBX, EAX
PUSH EBX
POP maingsum
PUSH OFFSET L11
CALL showstr 
PUSH OFFSET L12
CALL showstr 
PUSH mainasum
CALL shownum 
PUSH OFFSET L13
CALL showstr 
PUSH maingsum
CALL shownum 
PUSH OFFSET L14
CALL showstr 
PUSH mainz
CALL shownum 
PUSH OFFSET L15
CALL showstr 
PUSH mainc
CALL showrc 
PUSH OFFSET L16
CALL showstr 
PUSH mainisstr
CALL shownum 
MOV EAX, L17
PUSH 0
CALL ExitProcess
check ENDP
