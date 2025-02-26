# Task: OGR (Gardens)

## Problem Description
The government of Bajtocja has decided to allocate a significant amount of money to create a magnificent garden. The project is supervised by Professor Makary. He has planned a garden consisting of a single long alley, along which trees will grow. The alley is divided into `n` segments, numbered from `1` to `n`. Initially, all trees in every segment have a height of `1`. Each day, Professor Makary fertilizes a certain number of adjacent segments with a special fertilizer he developed. As a result, trees in these segments grow immediately.

Previous research by Professor Makary has shown that visitors are most impressed by trees arranged in non-decreasing order of height. Therefore, he occasionally inspects different parts of the garden and calculates the length of the longest contiguous sequence of segments where tree heights form a non-decreasing sequence.

Your task is to write a program to assist him in this task.

## Input
The first line of input contains two integers: `n` and `m` (`1 ≤ n ≤ 500,000`, `1 ≤ m ≤ 500,000`), which represent the number of segments and the number of operations performed by the professor, respectively.

Each of the following `m` lines describes an operation. If a line starts with the letter `N`, it represents a fertilization operation in the format:

```
N ai bi ki
```

where `1 ≤ ai ≤ bi ≤ n` and `1 ≤ ki ≤ 1,000` are integers. This means that the professor fertilized segments from `ai` to `bi`, causing trees in those segments to grow by `ki` units.

If a line starts with the letter `C`, it represents a query operation in the format:

```
C ai bi
```

where `1 ≤ ai ≤ bi ≤ n` are integers. This means that the professor wants to calculate the length of the longest contiguous sequence of segments within `[ai, ..., bi]` where tree heights form a non-decreasing sequence.

## Output
For each query operation (`C`), your program should output a single integer representing the length of the longest contiguous sequence of non-decreasing tree heights within the specified range. You can assume that at least one query operation (`C`) will be performed.

## Example

### Input:
```
6 8
N 4 5 2
N 2 4 3
C 1 3
C 3 6
N 1 2 3
C 1 6
N 5 6 4
C 4 6
```

### Output:
```
3
2
2
2
```

## Explanation
After the first two fertilizations, the tree height sequence is:
```
1, 4, 4, 6, 3, 1
```

1. The first query (`C 1 3`) considers the sequence `[1, 4, 4]`, which is fully non-decreasing, so the answer is `3`.
2. The second query (`C 3 6`) considers `[4, 6, 3, 1]`, where the longest non-decreasing subsequence is `[4, 6]`, so the answer is `2`.
3. After the third fertilization, the tree height sequence becomes:
```
4, 7, 4, 6, 3, 1
```
4. The third query (`C 1 6`) considers `[4, 7, 4, 6, 3, 1]`, where the longest non-decreasing subsequence is `[4, 7]`, so the answer is `2`.
5. After the last fertilization, the sequence becomes:
```
4, 7, 4, 6, 7, 5
```
6. The final query (`C 4 6`) considers `[6, 7, 5]`, where the longest non-decreasing subsequence is `[6, 7]`, so the answer is `2`.

