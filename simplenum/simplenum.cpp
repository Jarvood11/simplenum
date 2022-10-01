// simplenum.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#include <cstring>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctime>

#define Append(x) {\
    w[++w_end] = x; /* Append x to the ordered set W */\
    d[x] = false;\
}

void Extend(uint32_t w[], uint32_t& w_end, uint32_t& length, uint32_t n, uint32_t N, bool d[], uint32_t& w_end_max) {
    /* Rolls full wheel W up to n, and sets length=n */
    uint32_t i, x;
    i = 0;
    x = length + 1; /* length+w[0] */
    while (x <= n) {
        Append(x);
        x = length + w[++i];
    }
    length = n;
    if (length == N) Append(N + 1); /* sentinel */
    if (w_end > w_end_max) w_end_max = w_end;
}

void Delete(uint32_t w[], uint32_t length, uint32_t p, bool d[], uint32_t& imaxf) {
    /* Deletes multiples p*w[i] of p from W, and sets imaxf to last i deleted */
    uint32_t i, x;
    i = 0;
    x = p; /* p*w[0]=p*1 */
    while (x <= length) {
        d[x] = true; /* Remove x from W; */
        x = p * w[++i];
    }
    imaxf = i - 1;
}

void Compress(uint32_t w[], uint32_t length, uint32_t N, bool d[], uint32_t imax, uint32_t& w_end) {
    /* Removes deleted values in w[0..imax], and if length < N, updates w_end, otherwise pads with zeros on right */
    uint32_t i, j;
    j = 0;
    for (i = 1; i <= imax; i++) {
        if (!d[w[i]]) {
            w[++j] = w[i];
        }
    }
    if (length < N) {
        w_end = j;
    }
    else {
        for (uint32_t k = j + 1; k <= imax; k++) w[k] = 0;
    }
}

void Sift(uint32_t N, bool printPrimes, uint32_t& nrPrimes, uint32_t& vBound) {
    /* finds the nrPrimes primes up to N, printing them if printPrimes */
    uint32_t* w = new uint32_t[N / 4 + 5];
    bool* d = new bool[N + 1];
    uint32_t w_end, length;
    /* representation invariant (for the main loop): */
    /* if length < N (so W is a complete wheel), w[0..w_end] is the ordered set W; */
    /* otherwise, w[0..w_end], omitting zeros and values w with d[w] true, is the ordered set W, */
    /* and no values <= N/p are omitted */
    uint32_t w_end_max, p, imaxf, i;
    /* W,k,length = {1},1,2: */
    w_end = 0; w[0] = 1;
    w_end_max = 0;
    length = 2;
    /* Pr = {2}: */
    nrPrimes = 1;
    if (printPrimes) printf("%d", 2);
    p = 3;
    /* invariant: p = p_(k+1) and W = W_k inter {1,...,N} and length = min(P_k,N) and Pr = the first k primes */
    /* (where p_i denotes the i'th prime, W_i denotes the i'th wheel, P_i denotes the product of the first i primes) */
    while (p * p <= N) {
        /* Append p to Pr: */
        nrPrimes++;
        if (printPrimes) printf(" %d", p);
        if (length < N) {
            /* Extend W,length to minimum of p*length,N: */
            Extend(w, w_end, length, std::min(p * length, N), N, d, w_end_max);
        }
        Delete(w, length, p, d, imaxf);
        Compress(w, length, N, d, (length < N ? w_end : imaxf), w_end); /* (can be inefficient for last full wheel) */
        /* p = next(W, 1): */
        i = 1;
        while (w[i] == 0) i++;
        p = w[i];
        /* k++ */
    }
    if (length < N) {
        /* Extend full wheel W,length to N: */
        Extend(w, w_end, length, N, N, d, w_end_max);
    }
    if (N == 2) { Append(N + 1); w_end_max = w_end; } /* sentinel */
    /* gather remaining primes: */
    i = 0;
    while (w[++i] <= N) {
        if (w[i] == 0 || d[w[i]]) continue;
        if (printPrimes) printf(" %d", w[i]);
        nrPrimes++;
    }
    vBound = w_end_max + 1;
}

int main(int argc, char* argw[]) {
    uint32_t N, nrPrimes, vBound;
    N = 150;
    int start_s = clock();
    Sift(N, true, nrPrimes, vBound);
    int stop_s = clock();
    printf("\n%d primes up to %lu found in %.3f ms using array w[%d]\n", nrPrimes, (unsigned long)N, (stop_s - start_s) * 1000.0 / double(CLOCKS_PER_SEC), vBound);
}
// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
