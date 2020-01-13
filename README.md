# Sudoku.C

Sudoku.C is a command line based program that solves Sudoku boards. The program first attemps to solve the Sudoku board first with the constrain satisfaction algorithm [AC-3](https://en.wikipedia.org/wiki/AC-3_algorithm), which uses the rules of Sudoku to fill in spaces where a single possible value is left given the other starting values. If this algorithm is not enough to complete the board, the rest will be completed with [Back Tracking Search](https://en.wikipedia.org/wiki/Backtracking) (BTS). BTS is implemented here as a recurssive depth-first search.

## Compilation

Makefile

```
> make clean
> make all
```

Manual

```
> gcc driver.c queue.c hashmaps.c -o driver.x
```

## Usage

For all options

```
> driver.x help
```

Running

```
> driver.x file < inputfile.txt
```

```
> driver.x terminal
Row 1: 000000000
Row 2: 302540000
Row 3: 050301070
Row 4: 000000004
Row 5: 409006005
Row 6: 023054790
Row 7: 000000050
Row 8: 700810000
Row 9: 080060009
```

```
> driver.x line 000000000302540000050301070000000004409006005023054790000000050700810000080060009
```

Output

```
solved with Back Tracking Search:
|1|4|8|6|9|7|5|2|3|
|3|7|2|5|4|8|9|6|1|
|9|5|6|3|2|1|4|7|8|
|5|6|7|9|8|3|2|1|4|
|4|1|9|2|7|6|3|8|5|
|8|2|3|1|5|4|7|9|6|
|6|9|1|4|3|2|8|5|7|
|7|3|5|8|1|9|6|4|2|
|2|8|4|7|6|5|1|3|9|
```

## Contributing

Pull requests are welcome. For major changes, please open an issue first to discuss what you would like to change.

## License

[MIT](https://choosealicense.com/licenses/mit/)
