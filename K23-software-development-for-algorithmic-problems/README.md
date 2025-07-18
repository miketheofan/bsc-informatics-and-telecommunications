# K23-Software Development for Algorithmic Problems

Projects for the course "_K23:Software Development for Algorithmic Problems_" 2022 by professor [Ioannis Emiris](https://cgi.di.uoa.gr/~emiris/).

## Project 1

### Part 1

Given a input dataset with vectors and a query dataset, for every query vector find:

- The true nearest neighbor of the input dataset.
- The aproximate nearest neighbor of the input dataset.
- The aproximate N nearest neighbors of the input dataset.
- All vectors inside a given range R. (approximate search)

In order to find the aproximate nearest neighbor(s) we use:

- [Locality-sensitive hashing (LSH)](https://en.wikipedia.org/wiki/Locality-sensitive_hashing)
- Projection to Hypercube

### Part 2

Vector Clustering.

The initialization of the clusters is done using kMeans++.

The assignment to each cluster can be performed by each of the following:

- Lloyds assignment.
- LSH reverse assignment using Range search.
- Hypercube reverse assignment using Range search.

## Project 2

### Part 1 - Neighbor Search

In the first part of the project we implemented 3 different approaches:

    Time series are represented as vectors in the Euclidean space. The distance between two vectors is computed using L2 (Euclidean distance) metric. The methods used for the Nearest Neighbor Search are: LSH for vectors and Projection to Hypercube (same as Project 1).
    Time series are represented as a polygonal curves in R^2 space. The distance between two time-series is computed using Discrete Frechet metric (dynamic programming implemention). The method used for the Nearest Neighbor Search is LSH for discrete Time-Series, providing Nearest Neighbor Search, K Nearest Neighbors Search and Range Search. The hashing procedure of LSH algorithm is: snapping to 2D grid and elimination of dupicate snapped points followed by the Classic vector LSH procedures.
    Time series are represented as a polygonal curves in R space, by abolishing the time dimension. The distance between two time-series is computed using Continuous Frechet metric. The method used for the Nearest Neighbor Search is LSH for continuous Time-Series, providing Nearest Neighbor Search, K Nearest Neighbors Search and Range Search. Filtering is applied to time-series in order to reduce their complexity. The hashing procedure of LSH algorithm is: filtering, snapping to 1D grid, minima-maxima elimination followed by the Classic vector LSH procedures.

### Part 2 - Clustering

Implementation of clustering algorithms for time-series and vectors.

Implementation with **five** different approaches:

- **Vector clustering with Lloyds assignment and Kmeans++ initialization**. Distance metric: L2.
- **Vector clustering with LSH Range Search assignment (reverse assignment) and Kmeans++ initialization**. Distance metric: L2.
- **Vector clustering with Hypercube Range Search assignment (reverse assignment) and Kmeans++ initialization**. Distance metric: L2.
- **Discrete Time-series clustering with Lloyds assignment and Kmeans++ initialization**. Distance metric: Discrete Frechet.
- **Discrete Time-series clustering with LSH Range Search assignment (reverse assignment) and Kmeans++ initialization**. Distance metric: Discrete Frechet.

## Project 3

- **Time Series Forecasting**
- **Time Series Anomaly Detection**
- **Time Series Compression**
- **Time Series Clustering after compression**
