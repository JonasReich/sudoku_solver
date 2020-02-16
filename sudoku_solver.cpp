// Copyright (c) 2020 Jonas Reich

#include <stdio.h>
#include <iostream>
#include <strstream>
#include <vector>
#include <fstream>

// default initialize with 0 so we must call set_sudoku_base() in any case
uint SUDOKU_BASE = 0;
uint SUDOKU_DIMENSIONS = 0;

void set_sudoku_base(int base)
{
    SUDOKU_BASE = base;
    SUDOKU_DIMENSIONS = base * base;
}

struct sudoku_sheet
{
    typedef std::vector<int> row;

    sudoku_sheet() : cells(SUDOKU_DIMENSIONS, row(SUDOKU_DIMENSIONS)) {}
    sudoku_sheet(std::vector<row> in_cells) : cells(in_cells) {}

    std::vector<row> cells;

          row& operator[](int i)       { return cells[i]; }
    const row& operator[](int i) const { return cells[i]; }

    bool available(int x, int y, int n)
    {
        // check self cell
        if((*this)[y][x]!=0)
            return false;
        
        // check row
        for(int i = 0; i < SUDOKU_DIMENSIONS; i++)
            if((*this)[y][i] == n)
                return false;
        
        // check column
        for(int i = 0; i < SUDOKU_DIMENSIONS; i++)
            if((*this)[i][x] == n)
                return false;
        
        // check SUDOKU_DIMENSIONS-block
        const int base_x = x - x % SUDOKU_BASE;
        const int base_y = y - y % SUDOKU_BASE;
        for(int block_x = base_x; block_x < base_x + SUDOKU_BASE; block_x++)
            for(int block_y = base_y; block_y < base_y + SUDOKU_BASE; block_y++)
                if((*this)[block_y][block_x] == n)
                    return false;
        
        return true;
    }
};

std::ostream& operator<<(std::ostream& stream, const sudoku_sheet& sheet)
{
    auto print_horizontal_line = [&]()
    {
        stream << " -";
        for (int i = 0; i < SUDOKU_DIMENSIONS + SUDOKU_BASE; i++)
        {
            stream << "--";
        }
        stream << "\n";
    };

    for(int yblock = 0; yblock < SUDOKU_BASE; yblock++)
    {
        print_horizontal_line();
        for(int ycol=0; ycol < SUDOKU_BASE; ycol++)
            {
                auto& row = sheet.cells[(yblock * SUDOKU_BASE) + ycol];
                for(int xblock = 0; xblock < SUDOKU_BASE; xblock++)
                {
                    stream << " |";
                    for(int xcol=0; xcol < SUDOKU_BASE; xcol++)
                    {
                        int nr = row[(xblock * SUDOKU_BASE) + xcol];
                        if(nr == 0)
                            stream << "  ";
                        else
                            stream << " " << nr;
                    }
                }
                        
                stream << " |\n";
            }
    }
    print_horizontal_line();
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
    for(int x = 0; x < SUDOKU_DIMENSIONS; x++)
    {
        for(int y = 0; y < SUDOKU_DIMENSIONS; y++)
        {
            if(sheet[y][x] != 0)
                continue;
            
            bool bAtLeastOneFound = false;
            for(int n = 1; n <= SUDOKU_DIMENSIONS; n++)
            {
                if(sheet.available(x, y, n))
                {
                    //std::cout << "solve step...\n" <<  sheet;
                    sheet[y][x] = n;
                    if(solve(sheet, solutions, maxnsolutions))
                    {
                        bAtLeastOneFound = true;
                        if(solutions.size() == maxnsolutions)
                            return true;
                    }
                    
                    sheet[y][x] = 0;
                }
            }
            return bAtLeastOneFound;
        }
    }
    solutions.push_back(sheet);
    std::cout << "solution #" << solutions.size() << ":\n" << sheet;
    return true;
}

int main(int argc, char** argv)
{
    if(argc < 2)
    {
        std::cerr << "Missing parameter!\n"
            "\tSYNTAX: \tsudoku_solver <sudoku_sheet_path> [<sudoku_base>]\n"
            "\t\t<sudoku_sheet_path> \tPath to a sudoku sheet text file. Must be formatted as 1 digit per sheet cell. No surrounding whitespace. (see example_sheets/ directory)\n"
            "\t\t<sudoku_base> \t\t(optional) Numeric base for sudoku sheet. Classic sudoku puzzles are on base 3 (3x3 blocks of 3x3 cells).\n"
            "\tEXAMPLE:\n"
            "\t\tsudoku_solver ./test_sheet.sudoku\n"
            "\t\tsudoku_solver 4 ./test_sheet.sudoku\n";
        return 1;
    }

    std::string file_path = argv[1];
    std::ifstream sudoku_file (file_path);
    if (!sudoku_file.is_open())
    {
        std::cerr << "Unable to open file '" << file_path << "'\n";
        return 2;
    }

    if(argc > 2)
    {
        std::string sudoku_base_str = argv[2];
        try
        {
            int base = std::stoi(sudoku_base_str);
            set_sudoku_base(base);
        }
        catch(std::invalid_argument e)
        {
            std::cerr << "cannot convert argument 1 '" << sudoku_base_str << "' to an integer!\n";
            return 4;
        }
    }
    else
    {
        set_sudoku_base(3);
    }


    sudoku_sheet sheet;

    std::string line;
    int y = 0;
    while ( std::getline (sudoku_file,line) )
    {
        if(line.size() != SUDOKU_DIMENSIONS)
        {
            std::cerr << "Sudoku file '" << file_path << "', line " << y << " does not have exactly " << SUDOKU_DIMENSIONS << " columns!\n";
            return SUDOKU_BASE;
        }
        for(int x = 0; x < SUDOKU_DIMENSIONS; x++)
        {   
            std::string ns = line.substr(x, 1);
            int n = 0;
            if(ns != " ")
            {
                try
                {
                     n = std::stoi(ns);
                }
                catch(std::invalid_argument e)
                {
                    std::cerr << "cannot convert '" << line[x] << "' (line " << y << ", col " << x << ") to an integer!\n";
                    return 4;
                }
            }
            sheet[y][x] = n;
            
        }
        y++;
        if(y == SUDOKU_DIMENSIONS) break;
    }
    if(y != SUDOKU_DIMENSIONS)
    {
        std::cerr << "Sudoku file '" << file_path << "' does not have exactly " << SUDOKU_DIMENSIONS << " lines!\n";
        return SUDOKU_BASE;
    }
    sudoku_file.close();
    
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

