# DBSCAN
Simple C++ implementation of DBSCAN for n-th dimensional points.

## How to build 
Standard `cmake` + `make` build. No major dependencies.

What you should typically want to do:
* Create a build subdirectory `mkdir build` and `cd` to it;
* Run `cmake -B . -S ../.`;
* Run `make`.

## How to run
Usage: `./dbscan -f inputfile [-m minpts] [-e epsilon] [-d distopt]`, where:
* `-f` input file name in the required format
* `-m` minimum number of points for clustering (see DBSCAN reference)
* `-e` maximum radius for neighbors search (see DBSCAN reference)
* `-d` distance metric: Euclid (default), Manhattan, Chebyshev

### Input
The input file should contain the n-dimensional points as
```
   pnt1_x1 pnt1_x2 .... pnt1_xn
   pnt2_x1 pnt2_x2 .... pnt2_xn
   ...
   pntN_x1 pntN_x2 .... pntN_xn
```

### Output
...

## References
* "A density-based algorithm for discovering clusters in large spatial databases with noise". Proceedings of the Second International Conference on Knowledge Discovery and Data Mining (KDD-96). AAAI Press. pp. 226–231, https://www.aaai.org/Papers/KDD/1996/KDD96-037.pdf
* "DBSCAN Revisited, Revisited: Why and How You Should (Still) Use DBSCAN", ACM Transactions on Database Systems, Volume 42, Issue 3, September 2017, Article No.: 19, pp 1–21, https://doi.org/10.1145/3068335
