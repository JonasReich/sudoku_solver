import numpy
import copy

global grid
# This is the same as the 9x9_3.sudoku file
grid_1_solution = [
    [0,0,0,0,0,6,0,0,0],
    [0,0,0,1,0,9,5,0,0],
    [6,8,0,0,0,0,0,9,0],
    [0,0,2,7,0,0,0,8,0],
    [0,9,0,0,0,0,0,0,4],
    [0,0,4,0,8,5,0,0,0],
    [0,0,0,0,0,0,4,0,3],
    [0,0,0,0,6,0,0,0,0],
    [8,7,0,0,5,3,9,0,0]
]
# copy with just [8][6] changed from 9 to 0
grid_15_solutions = copy.deepcopy(grid_1_solution)
grid_15_solutions[8][6]=0
grid = grid_1_solution

global solutions
solutions = []

def print_grid():
    print(numpy.matrix(grid))

def available(y, x, n):
    if grid[y][x] != 0:
        return False
    for i in range(9):
        if grid[y][i] == n:
            return False
        if grid[i][x] == n:
            return False
    basex = (x//3)*3
    basey = (y//3)*3
    for xs in range(basex, basex+3):
        for ys in range(basey, basey + 3):
            if grid[ys][xs] == n:
                return False
    return True

def solve():
    for x in range(9):
        for y in range(9):
            if grid[y][x] == 0:
                foundMinOneSolution = False
                for n in range(1, 10):
                    if available(y, x, n):
                        grid[y][x] = n
                        if solve():
                            foundMinOneSolution = True
                        grid[y][x] = 0
                return foundMinOneSolution
    print("solution:")
    print_grid()
    solutions.append(copy.deepcopy(grid))
    return True

print("before solving")
print_grid()

if solve():
    print("\n---- found " + str(len(solutions)) + " solution(s) ----")
    # If solved, confirm we have no duplicate results
    seen = []
    duplicates = []
    for s in solutions:
        if s in seen:
            print("duplicate result:")
            print(numpy.matrix(s))
        else:
            seen.append(s)
    if len(duplicates) == 0:
        print("no duplicate results!")
else:
    print("no solutions")
