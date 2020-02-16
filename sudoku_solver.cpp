
#include <stdio.h>
#include <iostream>
#include <strstream>

struct sudoku_row
{
    sudoku_row() = default;
    sudoku_row(int i0, int i1, int i2, int i3, int i4, int i5, int i6, int i7, int i8) :
        cells{i0, i1, i2, i3, i4, i5, i6, i7, i8}
    {}

    int cells[9] = { 0 };
    int operator[](int i) const
    {
        return cells[i];
    }

    int& operator[](int i)
    {
        return cells[i];
    }

    void log_nr(std::ostream& stream, int i) const
    {
        int nr = cells[i];
        if(nr == 0)
            stream << "  ";
        else
            stream << " " << nr;
    }
};


std::ostream& operator<<(std::ostream& stream, const sudoku_row& row)
{
    for(int i = 0; i < 3; i++)
    {
        stream << " |";
        for(int j=0; j < 3; j++)
            row.log_nr(stream, i*3+j);
    }
            
    stream << " |\n";
    return stream;
}

struct sudoku_sheet
{
    sudoku_sheet() = default;
    sudoku_sheet(sudoku_row i0, sudoku_row i1, sudoku_row i2, sudoku_row i3, sudoku_row i4, sudoku_row i5, sudoku_row i6, sudoku_row i7, sudoku_row i8) :
        rows{i0, i1, i2, i3, i4, i5, i6, i7, i8}
    {}
    sudoku_row rows[9];

    sudoku_row& operator[](int i)
    {
        return rows[i];
    }

    const sudoku_row& operator[](int i) const
    {
        return rows[i];
    }

    bool available(int x, int y, int n)
    {
        // check self cell
        if((*this)[y][x]!=0)
        {
            return false;
        }
        // check row
        for(int i : (*this)[y].cells)
        {
            if(i == n)
                return false;
        }
        // check column
        for(const sudoku_row& row : rows)
        {
            if(row[x] == n)
                return false;
        }
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

    for(int i = 0; i < 3; i++)
    {
        stream << " -------------------------\n";
        for(int j=0; j < 3; j++)
            stream << sheet.rows[i*3+j];
    }
    stream << " -------------------------\n";
    return stream;
}

/**
 * @returns if solved
 */
bool solve(sudoku_sheet& sheet, int& nsolutions, int maxnsolutions)
{
    for(int x = 0; x < 9; x++)
    {
        for(int y = 0; y < 9; y++)
        {
            if(sheet[y][x] != 0)
                continue;
            
            bool bFilledIn = false;
            for(int n = 1; n <= 9; n++)
            {
                if(sheet.available(x, y, n))
                {
                    //std::cout << "solve step...\n" <<  sheet;
                    sheet[y][x] = n;
                    if(solve(sheet, nsolutions, maxnsolutions))
                    {
                        if(nsolutions == maxnsolutions)
                            return true;
                        bFilledIn = true;
                    }
                    else
                    {
                        sheet[y][x] = 0;
                    }
                }
            }
            if(!bFilledIn)
            {
                return false;
            }
        }
    }
    nsolutions++;
    std::cout << "solution #" << nsolutions << ":\n" << sheet;
    return true;
}

int main(int argc, char** argv)
{
    /*
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
    */
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
    std::cout << "pre-solve:\n";
    std::cout << sheet;
    int nsolutions = 0;
    int maxnsolutions = 3;
    if(solve(sheet, nsolutions, maxnsolutions))
    {
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

