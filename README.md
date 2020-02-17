# C++ Sudoku Solver
This solver is based on a basic backtracing algorithm described in [this Computerphile video](https://www.youtube.com/watch?v=G_UYXzGuqvM).

I've extended the algorithm to support arbitrary grid dimensions. Where your typical sudoku puzzle is on base 3 - a 3x3 set of smaller 3x3 blocks, adding up to 9x9 cells in total - you can also create sudoku puzzles of base 2 resulting in 4x4 cells or base 6 resulting in 36x36 cells.

## Sudoku Sheet Files
The example_sheets folder contains four 9x9 sheets and two 4x4 sheets.
Sudoku sheet files must have exactly 1 character per grid cell with rows separated by line-breaks.
Empty cells in the sheets are represented either as zeros or spaces.

## Limitations
Because of the current sheet parsing and console printing functions, only base 2 and base 3 sudoku sheets are supported (we asume 1 char per cell). This needs to be extended to have a variable number of digits for grid cell numbers, requiring value separator characters or fixed multi-digit counts in the sudoku sheet files.

## Contributing
For the time being I will leave the implementation as is, but you can easily add proper support for more dimensions. Feel free to create a pull-request, if you do so! Please note that the actual algorithm supports any number of dimensions already and does not need to be modified.

## License
This program is released under the [MIT License](LICENSE.md).
