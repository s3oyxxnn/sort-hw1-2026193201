# 정렬 비교 과제 — 삽입 · 퀵 · 힙

2026-2 고급알고리즘(SIT2001-01) 과제 1. [lec-algorithm/algorithm-env](https://github.com/lec-algorithm/algorithm-env)를 **Use this template** 하여 만든 저장소입니다.

| 구분 | 정렬 | 파일 |
| --- | --- | --- |
| 수업에서 배운 정렬 1 | 삽입 정렬 (Insertion Sort) | `src/insertionSort.c` |
| 수업에서 배운 정렬 2 | 퀵 정렬 (Quick Sort) | `src/quickSort.c` |
| 배우지 않은 정렬 | 힙 정렬 (Heap Sort) | `src/heapSort.c` |

- 보고서: LearnUs에 PDF로 제출
- 측정값 원본: [report/results.csv](report/results.csv)
- 그래프: [n에 따른 비교 횟수](report/growth-compares-log.svg) · [입력 모양별 시간](report/input-shapes-time-log.svg) · 그 밖의 SVG는 `report/`

## 돌려보기

Codespaces(Code → Codespaces → Create codespace on main)를 열면 터미널이 곧 컨테이너 안입니다.
로컬이라면 `docker compose up -d && docker compose exec lab bash` 후 같은 명령을 씁니다.

| 명령 | 하는 일 |
| --- | --- |
| `make run` | 세 정렬을 같은 입력으로 재어 비교 표 출력 |
| `make test` | 유닛 테스트 (42 checks) |
| `make charts` | 비교 그래프(SVG)와 `results.csv`를 `report/` 아래에 다시 만든다 |
| `make debug` | 디버그 심볼을 넣어 빌드 (VS Code F5) |
| `make clean` | 빌드 산출물 정리 |

## 저장소 구조

```plaintext
.
├── .devcontainer/ · compose.yml · Dockerfile   # 실습 컨테이너 (template 그대로)
├── .vscode/                                    # 빌드·디버그 설정
├── Makefile                                    # run · test · charts · debug · clean
├── src/
│   ├── sort.h                 # 공통 인터페이스 (SortAlgorithm 구조체 + 함수 포인터)
│   ├── sortctx.h · sort.c     # 구현들이 함께 쓰는 도구 · 구현 표
│   ├── insertionSort.c        # 삽입 정렬
│   ├── quickSort.c            # 퀵 정렬 (median-of-three, Hoare 분할)
│   ├── heapSort.c             # 힙 정렬
│   ├── bench.h · bench.c      # 입력 생성, 시간·비교·이동·안정성 측정
│   └── main.c                 # 비교 결과 출력 (--csv 옵션)
├── tests/test_sort.c          # 유닛 테스트 (표준 C만 사용)
├── tools/plot.py · svgchart.py  # 그래프 그리기 (Python 표준 모듈만)
└── report/                    # 그래프 SVG, results.csv (make charts가 만든다)
```

측정 틀(`bench`, `main`, `tools`)은 수업 샘플 [hw1-sample-2026](https://github.com/lec-algorithm/hw1-sample-2026)의 구조를 참고했고, 비교 대상 정렬만 바꿨습니다.
