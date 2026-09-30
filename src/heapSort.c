/* 힙 정렬 — 수업에서 다루지 않아 새로 학습한 정렬.
 *
 * 배열을 "최대 힙"으로 본다. 인덱스 i의 자식은 2i+1, 2i+2이고, 부모는 늘
 * 자식보다 크거나 같다. 그러면 가장 큰 값이 항상 a[0]에 있다.
 *
 *   1단계 (힙 만들기) : 마지막 부모 n/2-1부터 0까지 거꾸로 siftDown한다. O(n)
 *   2단계 (꺼내기)    : a[0](최댓값)을 끝으로 보내고, 힙 크기를 하나 줄인 뒤
 *                       a[0]을 다시 siftDown한다. 이것을 n-1번. O(n log n)
 *
 * 입력 모양과 상관없이 최악에도 O(n log n)이고, 추가 메모리는 원소 한 칸뿐이다.
 * 재귀도 없다. 대신 멀리 떨어진 원소끼리 바꾸므로 안정 정렬이 아니고,
 * 트리를 따라 멀리 뛰어다니므로 캐시 효율이 나빠 퀵 정렬보다 느린 편이다.
 */
#include "sort.h"

#include "sortctx.h"

/* a[root]를 자식과 견주며 제자리까지 내려보낸다. 힙 크기는 end(미포함).
 * 교환 대신 "구멍 밀기"를 쓴다: 내려갈 값을 tmp에 들고, 큰 자식을 위로
 * 한 칸씩 올린 뒤 마지막 구멍에 tmp를 넣는다. 이동이 교환의 1/3로 준다. */
static void siftDown(SortCtx *c, size_t root, size_t end) {
    sortMove(c, c->tmp, sortElemAt(c, root));
    size_t hole = root;
    for (;;) {
        size_t child = 2 * hole + 1;
        if (child >= end) {
            break;
        }
        /* 오른쪽 자식이 더 크면 그쪽으로 */
        if (child + 1 < end && sortCompareAt(c, child, child + 1) < 0) {
            child++;
        }
        /* 큰 자식이 내려갈 값보다 크지 않으면 여기가 제자리다 */
        if (sortCompareTmp(c, child) <= 0) {
            break;
        }
        sortMove(c, sortElemAt(c, hole), sortElemAt(c, child));
        hole = child;
    }
    sortMove(c, sortElemAt(c, hole), c->tmp);
}

void heapSort(void *base, size_t n, size_t size, SortCompare cmp, SortStats *stats) {
    SortCtx c;
    if (!sortBegin(&c, base, n, size, cmp, stats)) {
        return;
    }
    /* 1단계: 아래쪽 부모부터 힙으로 만든다 */
    for (size_t i = n / 2; i-- > 0;) {
        siftDown(&c, i, n);
    }
    /* 2단계: 최댓값을 뒤로 보내고 남은 힙을 고친다 */
    for (size_t end = n - 1; end > 0; end--) {
        sortSwap(&c, 0, end);
        siftDown(&c, 0, end);
    }
    sortEnd(&c);
}
