
#include <stdio.h>
#include <iostream>
#include <strstream>
#include <vector>
#include <fstream>

struct sudoku_sheet
{
    typedef std::vector<int> row;

    sudoku_sheet() : cells(9, row(9)) {}
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
            
            bool bAtLeastOneFound = false;
            for(int n = 1; n <= 9; n++)
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
            "SYNTAX: \tsudoku_solver <sudoku sheet path>\n"
            "EXAMPLE: \tsudoku_solver ./test_sheet.sudoku\n";
        return 1;
    }

    std::string file_path = argv[1];
    std::ifstream sudoku_file (file_path);
    if (!sudoku_file.is_open())
    {
        std::cerr << "Unable to open file '" << file_path << "'\n";
        return 2;
    }

    sudoku_sheet sheet;

    std::string line;
    int y = 0;
    while ( std::getline (sudoku_file,line) )
    {
        if(line.size() != 9)
        {
            std::cerr << "Sudoku file '" << file_path << "', line " << y << " does not have exactly 9 columns!\n";
            return 3;
        }
        for(int x = 0; x < 9; x++)
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
        if(y == 9) break;
    }
    if(y != 9)
    {
        std::cerr << "Sudoku file '" << file_path << "' does not have exactly 9 lines!\n";
        return 3;
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

