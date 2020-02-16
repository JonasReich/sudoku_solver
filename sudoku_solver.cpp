
#include <stdio.h>
#include <iostream>
#include <strstream>
#include <array>
#include <vector>

struct sudoku_sheet
{
    typedef std::array<int, 9> row;

    sudoku_sheet() = default;
    sudoku_sheet(row r0, row r1, row r2, row r3, row r4, row r5, row r6, row r7, row r8)
    {
        cells[0] = r0;
        cells[1] = r1;
        cells[2] = r2;
        cells[3] = r3;
        cells[4] = r4;
        cells[5] = r5;
        cells[6] = r6;
        cells[7] = r7;
        cells[8] = r8;
    }

    std::array<row, 9> cells;

          row& operator[](int i)       { return cells[i]; }
    const row& operator[](int i) const { return cells[i]; }

    bool available(int x, int y, int n)
    {
        // check self cell
        if((*this)[y][x]!=0)
            return false;
        
        // check row
        for(int i = 0; i < 9; i++)
            if((*this)[y][i] == n)
                return false;
        
        // check column
        for(int i = 0; i < 9; i++)
            if((*this)[i][x] == n)
                return false;
        
        // check 9-block
        const int base_x = x - x%3;
        const int base_y = y - y%3;
        for(int block_x = base_x; block_x < base_x + 3; block_x++)
            for(int block_y = base_y; block_y < base_y + 3; block_y++)
                if((*this)[block_y][block_x] == n)
                    return false;
        
        return true;
    }
};

std::ostream& operator<<(std::ostream& stream, const sudoku_sheet& sheet)
{
    for(int yblock = 0; yblock < 3; yblock++)
    {
        stream << " -------------------------\n";
        for(int ycol=0; ycol < 3; ycol++)
            {
                auto& row = sheet.cells[yblock*3+ycol];
                for(int xblock = 0; xblock < 3; xblock++)
                {
                    stream << " |";
                    for(int xcol=0; xcol < 3; xcol++)
                    {
                        int nr = row[xblock*3+xcol];
                        if(nr == 0)
                            stream << "  ";
                        else
                            stream << " " << nr;
                    }
                }
                        
                stream << " |\n";
            }
    }
    stream << " -------------------------\n";
    return stream;
}

/**
 * @param sheet working copy of the sheet to operate on (will likekly not contain a valid answer at the end)
 * @param solutions found solutions
 * @param maxnsolutions maximum number of solutions after which to stop the search
 * @returns if at least 1 solution was found
 */
bool solve(sudoku_sheet& sheet, std::vector<sudoku_sheet>& solutions, int maxnsolutions)
{
    for(int x = 0; x < 9; x++)
    {
        for(int y = 0; y < 9; y++)
        {
            if(sheet[y][x] != 0)
                continue;
            
            for(int n = 1; n <= 9; n++)
            {
                if(sheet.available(x, y, n))
                {
                    //std::cout << "solve step...\n" <<  sheet;
                    sheet[y][x] = n;
                    if(solve(sheet, solutions, maxnsolutions))
                    {
                        if(solutions.size() == maxnsolutions)
                            return true;
                    }
                    
                    sheet[y][x] = 0;
                }
            }
            return false;
        }
    }
    solutions.push_back(sheet);
    std::cout << "solution #" << solutions.size() << ":\n" << sheet;
    return true;
}

int main(int argc, char** argv)
{
    
    sudoku_sheet sheet = {
        {1, 2, 3, 4, 5, 6, 7, 8, 9},
        {9, 8, 7, 1, 2, 3, 4, 5, 6},
        {4, 5, 6, 7, 8, 9, 1, 2, 3},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0}
    };
    
    //sudoku_sheet sheet;
   /*
    sudoku_sheet sheet = {
        {1, 2, 3, 4, 5, 6, 7, 8, 9},
        {9, 8, 7, 1, 2, 3, 4, 5, 6},
        {4, 5, 6, 7, 8, 9, 1, 2, 3},
        {0, 5, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0}
    };
    */
   /*
    sudoku_sheet sheet = {
        {1, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 2, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 3, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 4, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 5, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 6, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 7, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 8, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 9}
    };
    */
    std::cout << "pre-solve:\n";
    std::cout << sheet;
    int maxnsolutions = 0;
    std::vector<sudoku_sheet> solutions;
    if(solve(sheet, solutions, maxnsolutions))
    {
        int nsolutions = solutions.size();
        bool bLimitReached = (nsolutions == maxnsolutions);
        std::cout << "found " << nsolutions << (bLimitReached ? "+" : "") << " solutions (see above)!\n";
        if(bLimitReached)
        {
            std::cout << "(limit of " << maxnsolutions << " reached)\n";
        }
    }
    else
    {
        std::cout << "no solution found!\n";
    }
    return 0;
}

