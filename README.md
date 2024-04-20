# Dinic's Algorithm

This repository contains an implementation of Dinic's Algorithm, a powerful algorithm for finding the maximum flow in a flow network. The maximum flow problem is fundamental in various domains such as transportation, computer networking, and logistics.

## Overview

Dinic's Algorithm is an efficient method for solving the maximum flow problem in a directed graph. It builds upon the Ford-Fulkerson method but improves its time complexity to O(V^2 * E) in general graphs and O(E * sqrt(V)) in graphs with unit capacities. 

## Features

- Efficient implementation in C.
- Reads graph data from an input file.
- Outputs the maximum flow for each graph in the input file.

## Usage

1. Clone this repository to your local machine.
2. Compile the source code using a C compiler.
3. Prepare an input file with graph data.
4. Run the compiled executable, passing the input file as an argument.

```bash
./dinics_algorithm input.txt
