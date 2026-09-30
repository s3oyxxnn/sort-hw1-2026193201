/* 퀵 정렬 — 피벗 하나를 골라 작은 쪽·큰 쪽으로 나눈 뒤 양쪽을 다시 정렬한다.
 *
 * 평균 O(n log n)이고 실전에서 가장 빠른 편이다. 대신 피벗을 잘못 고르면
 * O(n^2)까지 떨어진다. 그래서 두 가지를 넣었다.
 *   1) 세 값의 중앙값(median-of-three)을 피벗으로 쓴다.
 *      이미 정렬된 입력이나 역순 입력에서도 가운데 값이 피벗이 된다.
 *   2) 작은 쪽만 재귀하고 큰 쪽은 반복문으로 돈다.
 *      재귀 깊이가 최악에도 log2(n) 근처를 넘지 않는다.
 * 분할은 Hoare 방식이다. 같은 값에서도 양쪽이 멈춰 교환하므로 중복이 많아도
 * 한쪽으로 쏠리지 않는다. 멀리 떨어진 원소끼리 교환하므로 안정 정렬은 아니다.
 */
#include "sort.h"

#include <stdlib.h>

#include "sortctx.h"

/* a[i]와 피벗 값을 비교한다. 비교 횟수도 함께 센다. */
static int comparePivot(SortCtx *c, size_t i, const void *pivot) {
    if (c->stats != NULL) {
        c->stats->compares++;
    }
    return c->cmp(sortElemAt(c, i), pivot);
}

/* a[lo], a[mid], a[hi]를 정렬해 두면 a[mid]가 세 값의 중앙값이 된다. */
static void medianOfThree(SortCtx *c, size_t lo, size_t mid, size_t hi) {
    if (sortCompareAt(c, mid, lo) < 0) {
        sortSwap(c, mid, lo);
    }
    if (sortCompareAt(c, hi, lo) < 0) {
        sortSwap(c, hi, lo);
    }
    if (sortCompareAt(c, hi, mid) < 0) {
        sortSwap(c, hi, mid);
    }
}

/* a[lo..hi]를 Hoare 방식으로 나눈다. 돌려준 p에 대해
 * a[lo..p]는 모두 피벗 이하, a[p+1..hi]는 모두 피벗 이상이다. */
static size_t partition(SortCtx *c, size_t lo, size_t hi, void *pivot) {
    size_t mid = lo + (hi - lo) / 2;
    medianOfThree(c, lo, mid, hi);
    /* 피벗 값을 따로 복사해 둔다. 교환 중에 자리가 바뀌어도 값은 그대로다.
     * (tmp는 sortSwap이 쓰므로 피벗 자리는 따로 둔다.) */
    sortMove(c, pivot, sortElemAt(c, mid));

    size_t i = lo;
    size_t j = hi;
    for (;;) {
        while (comparePivot(c, i, pivot) < 0) {
            i++;
        }
        while (comparePivot(c, j, pivot) > 0) {
            j--;
        }
        if (i >= j) {
            return j;
        }
        sortSwap(c, i, j);
        i++;
        j--;
    }
}

static void quickSortRange(SortCtx *c, size_t lo, size_t hi, size_t depth, void *pivot) {
    if (c->stats != NULL && depth > c->stats->maxDepth) {
        c->stats->maxDepth = depth;
    }
    while (lo < hi) {
        size_t p = partition(c, lo, hi, pivot);
        /* 작은 쪽을 재귀하고 큰 쪽은 반복한다 → 스택이 log2(n) 정도로 묶인다 */
        if (p - lo < hi - p) {
            quickSortRange(c, lo, p, depth + 1, pivot);
            lo = p + 1;
        } else {
            quickSortRange(c, p + 1, hi, depth + 1, pivot);
            hi = p;
        }
    }
}

void quickSort(void *base, size_t n, size_t size, SortCompare cmp, SortStats *stats) {
    SortCtx c;
    if (!sortBegin(&c, base, n, size, cmp, stats)) {
        return;
    }
    void *pivot = malloc(size);
    if (pivot == NULL) {
        sortEnd(&c);
        return;
    }
    if (stats != NULL) {
        stats->extraBytes = 2 * size; /* tmp 한 칸 + 피벗 한 칸 (재귀 스택은 maxDepth로 본다) */
    }
    quickSortRange(&c, 0, n - 1, 1, pivot);
    free(pivot);
    sortEnd(&c);
}
