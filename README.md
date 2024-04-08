# Counting-Binary-Search-Tree
Binary Search Tree for efficient counts of tree values less than, equal to, and greater than inserted number.

This program solves the CSES Nested Ranges [intervals] Problem for n random intervals [x_i, y_i].

There are two output arrays.

The first is the number of ranges the interval contains.

The second is the number of ranges contained by the interval.

[a,b] contains [c,d] iff ((c >= a) && (d <= b)).

Ranges are assumed to be distinct.

Author: Simon Goater Mar 2024.

Usage: ./CSESNestedRanges2.bin n

1 <= n <= 2*10^5

1 <= x_i < y_i <= 10^9

