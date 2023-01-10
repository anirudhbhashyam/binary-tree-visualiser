# Binary Tree Visualiser

A simple binary tree visualiser written in C.

`Under development`

# Usage
```sh
$ git clone https://github.com/anirudhbhashyam/binary-tree-visualiser
$ ./pack.sh binary_tree.c
$ ./binary_tree <data_file>
$ dot -T<format> bst.dot > bst.<format>
```
`data_file` is a txt file with `\n` separated nodes. See [here](https://graphviz.org/pdf/dot.1.pdf) for available `format`s.

# Dependencies
- `gcc`
- `dot (graphviz)`

# TODO 
- Support char nodes.