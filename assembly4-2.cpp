#include <iostream>
using namespace std;

#pragma once

int main()
{
    const int N = 16;
    int C[N]{};
    int C_asm[2 * N]{};
    int q;
    int cnt = 0;
    short int cnt_asm = 0;
    int A[N]{}, B[N]{};
    int count = 0;
    bool bl = true;
cycle:
    cout << "Tests(0), input(1) or quit the program(2)?";
    cin >> q;
    if (q == 0)
        goto tests;
    else if (q == 1)
        goto input;
    else return 0;

tests:
    memset(A, 0, N);
    memset(B, 0, N);
    memset(C, 0, N);
    memset(C_asm, 0, 2 * N);
    cnt = 0;
    cnt_asm = 0;

    switch (count)
    {
    case 0:
        A[0] = 1; A[1] = 2; A[2] = 3; A[3] = 4; A[4] = 5; A[5] = 6; A[6] = 7; A[7] = 8; A[8] = 9; A[9] = 1; A[10] = 2; A[11] = 3; A[12] = 4; A[13] = 5; A[14] = 6; A[15] = 7;
        B[0] = 1; B[1] = 2; B[2] = 3; B[3] = 4; B[4] = 5; B[5] = 6; B[6] = 7; B[7] = 8; B[8] = 9; B[9] = 1; B[10] = 2; B[11] = 3; B[12] = 4; B[13] = 5; B[14] = 6; B[15] = 7;
        break;
    case 1:
        A[0] = 0; A[1] = 1; A[2] = 2; A[3] = 4; A[4] = 3; A[5] = 4; A[6] = 5; A[7] = 8; A[8] = 7; A[9] = 8; A[10] = 9; A[11] = 10; A[12] = 11; A[13] = 12; A[14] = 13; A[15] = 7;
        B[0] = 1; B[1] = 2; B[2] = 3; B[3] = 4; B[4] = 5; B[5] = 6; B[6] = 7; B[7] = 8; B[8] = 9; B[9] = 1; B[10] = 2; B[11] = 3; B[12] = 4; B[13] = 5; B[14] = 6; B[15] = 7;
        break;
    case 2:
        A[0] = 0; A[1] = 0; A[2] = 0; A[3] = 0; A[4] = 0; A[5] = 0; A[6] = 0; A[7] = 0; A[8] = 0; A[9] = 0; A[10] = 0; A[11] = 0; A[12] = 0; A[13] = 0; A[14] = 0; A[15] = 0;
        B[0] = 1; B[1] = 2; B[2] = 3; B[3] = 4; B[4] = 5; B[5] = 6; B[6] = 7; B[7] = 8; B[8] = 9; B[9] = 1; B[10] = 2; B[11] = 3; B[12] = 4; B[13] = 5; B[14] = 6; B[15] = 7;
        break;
    }

    goto main;
input:

    memset(A, 0, N);
    memset(B, 0, N);
    memset(C, 0, N);
    memset(C_asm, 0, 2 * N);
    count = -1;
    cnt = 0;
    cnt_asm = 0;

    for (int i = 0; i < N; i++) {
        cout << "A" << i << ": ";
        bl = true;
        while (bl)
        {
            cin >> A[i];
            if (cin.fail())
            {
                cin.clear();
                cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Error! Reinput value \n";
            }
            else bl = false;
        }
    }
    for (int i = 0; i < N; i++) {
        cout << "B" << i << ": ";
        bl = true;
        while (bl)
        {
            cin >> B[i];
            if (cin.fail())
            {
                cin.clear();
                cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Error! Reinput value \n";
            }
            else bl = false;
        }
    }
    goto main;

main:

    __asm {
        lea esi, A; // запись в регистр esi начального адреса массива A
        lea edi, B; // запись в регистр edi начального адреса массива B
        mov edx, 0; // счетчик
        mov cx, 16; // счетчик оставшихся слов
    ONE:
        mov al, byte ptr[esi]; // поместить в регистр al младший байт следующего слова A
        mov bl, byte ptr[edi];// поместить в регистр bl младший байт следующего слова B
        cmp al, bl; // compare
        jne TWO; // если не равны, перейти на TWO
        mov dword ptr C_asm[4 * edx], esi; // записывает 32-битное значение из регистра esi в память по адресу, вычисленному как C_asm + 4 * edx.
        mov eax, edx;
        add eax, 16;
        mov dword ptr C_asm[4 * eax], edi; // используем предыд. 3 строки для записи адресов слов из массива B с середины C_asm
        inc edx; // счетчик++
    TWO:
        add esi, 4; // перейти к следующему dw в A
        add edi, 4; // перейти к следующему dw в B
        dec cx; // убавить счетчик оставшихся слов
        jcxz END; // если регистр cx == 0, перейти на END
        jmp ONE; // цикл
    END:
        mov cnt_asm, dx; // сохранить в cnt_asm количество слов удовл условию
    }

    cout << "Assembler results: \n";
    cout << "Count: " << std::dec << cnt_asm << '\n';
    for (int i = 0; i < cnt_asm; i++)
    {
        if (C_asm[i] != 0)
        {
            cout << "&A = " << std::hex << C_asm[i] << ", &B = " << std::hex << C_asm[i + N] << endl;
        }

    }

    std::cout << "C++ results: \n";
    for (int i = 0; i < N; i++) {
        if ((A[i] & 0xFF) == (B[i] & 0xFF)) {
            C[cnt++] = i;
        }
    }

    cout << "Count: " << std::dec << cnt << endl;
    cout << "Addresses: \n";

    for (int i = 0; i < cnt; i++)
    {
        cout << "&A[" << std::dec << C[i] << "] = " << std::hex << &A[C[i]] << ", &B[" << std::dec << C[i] << "] = " << std::hex << &B[C[i]] << endl;
    }

    if ((count != -1) && (count < 2))
    {
        count += 1;
        cnt = 0;
        goto tests;
    }
    count = 0;
    goto cycle;
}
