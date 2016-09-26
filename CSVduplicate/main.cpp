#include <string>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <fstream>
#include<sstream>
#include "Line.h"
#include <iomanip>

int main(int argc, char *argv[])
{
    std::string input;
    std::string output;
    std::string line;
    std::vector<std::string> LineVector;
    int columnX = -1;
    int columnY = -1;
    std::vector<Line *> m_lines;
    std::string labelsStr;
    double precision(0.001);

    for( int i=1; i<argc; ++i)
    {
       if(std::string(argv[i])=="-i" && i+1<=argc)
       {
          ++i;
          input = argv[i];
       }
       else if( std::string(argv[i]) == "-o" && i+1<argc)
       {
          ++i;
          output = argv[i];
       }
       else if( std::string(argv[i]) == "-p" && i+1<argc)
       {
          ++i;
          precision = atof(argv[i]);
       }
       else
       {
          std::cerr << "WARNING: unknown tag : " <<argv[i] << "\n";
       }
    }

    if(input=="" || output=="")
    {
       std::cerr << "ERROR: Missing Arguments\n";
       return EXIT_FAILURE;
    }



    std::cout << "Input: " << input << "\n";
    std::cout << "Output: " << output << "\n";

    std::ifstream inFile;
    inFile.open(input.c_str());
    if(!inFile)
    {
       std::cerr << "ERROR: File doesn't exist\n";
       return EXIT_FAILURE;
    }
    std::ofstream outFile;
    outFile.open(output.c_str());
    if(!outFile)
    {
       std::cerr << "ERROR: couldn't create file\n";
       return EXIT_FAILURE;
    }
    if(std::getline(inFile,labelsStr))
    {
       // read labels
       std::istringstream ss( labelsStr );
       while (ss)
       {
          std::string subS;
          if (!std::getline( ss, subS, ',' )) break;
          LineVector.push_back(subS);
       }
       for(unsigned int i=0; i<LineVector.size(); ++i)
       {
          if(LineVector[i]=="x" || LineVector[i] == "X")
          {
             columnX = i;
          }
          else if(LineVector[i]=="y" || LineVector[i] == "Y")
          {
             columnY = i;
          }
       }
       if(columnX<0 || columnY<0)
       {
          std::cout << "ERROR: missing X or Y column\n";
          return EXIT_FAILURE;
       }
       outFile << labelsStr << "\n";
    }
    else
    {
       std::cerr << "ERROR: empty file\n";
       return EXIT_FAILURE;
    }
    LineVector.clear();

    while(std::getline(inFile,line))
    {
      std::istringstream ss( line );
      while (ss)
      {
        std::string subS;
        if (!std::getline( ss, subS, ',' )) break;
        LineVector.push_back(subS);
      }
      m_lines.push_back(new Line(atof(LineVector[columnX].c_str()),
                                 atof(LineVector[columnY].c_str()),
                                 line));
      LineVector.clear();
    }

    std::cout << "number of trees = " << m_lines.size()-1 << "\n";

    // sort lines
     unsigned int len = m_lines.size();
      // allocate memory for temporarly saved values
      std::vector<Line *> tempValues;
      tempValues.resize(len);

      // sort y axis
      for(int step=2; step/2 < len; step*=2)
      {
         for(unsigned int i=0; i < len; i+=step)
         {
            int endOfT2 = i+step;
            if(i+ step/2 >= len)
            {
               continue;
            }
            else if (i+step >= len)
            {
               endOfT2 = len;
            }
            // both sets have step/2 items.
            // t1 points to the first set of values
            int t1 = i;
            // t2 points to the second set of values
            int t2 = i+step/2;
            // here we save all the values that have been overridden from the first set
            unsigned int tempIndex=0;
            while(t1 < i+step/2 && t2 < endOfT2)
            {
               if(m_lines[t1]->m_y>m_lines[t2]->m_y)
               {
                  tempValues[tempIndex]=m_lines[t1];
                  t1++;
               }
               else
               {
                  tempValues[tempIndex]=m_lines[t2];
                  t2++;
               }
               tempIndex++;
            }
            while(t1 < i+step/2)
            {
               tempValues[tempIndex]=m_lines[t1];
               t1++;
               tempIndex++;
            }
            // write values back to the array
            for(unsigned int t=0; t < tempIndex; ++t)
            {
                m_lines[i+t]=tempValues[t];
            }
         }
      }

     // sort x axis
      // start sorting 2 elements each time, then merge them with the two next to them etc
      for(int step=2; step/2 < len; step*=2)
      {
         for(unsigned int i=0; i < len; i+=step)
         {
            int endOfT2 = i+step;
            if(i+ step/2 >= len)
            {
               continue;
            }
            else if (i+step >= len)
            {
               endOfT2 = len;
            }
            // both sets have step/2 items.
            // t1 points to the first set of values
            int t1 = i;
            // t2 points to the second set of values
            int t2 = i+step/2;
            // here we save all the values that have been overridden from the first set
            unsigned int tempIndex=0;
            while(t1 < i+step/2 && t2 < endOfT2)
            {
               if(m_lines[t1]->m_x>m_lines[t2]->m_x)
               {
                  tempValues[tempIndex]=m_lines[t1];
                  t1++;
               }
               else
               {
                  tempValues[tempIndex]=m_lines[t2];
                  t2++;
               }
               tempIndex++;
            }
            while(t1 < i+step/2)
            {
               tempValues[tempIndex]=m_lines[t1];
               t1++;
               tempIndex++;
            }
            // write values back to the array
            for(unsigned int t=0; t < tempIndex; ++t)
            {
                m_lines[i+t]=tempValues[t];
            }
         }
      }

      // find dublicates and save to file
      unsigned int previous = 0;
      outFile << m_lines[previous]->m_line << "\n";
      for(unsigned int i=1; i < m_lines.size(); ++i)
      {
          if(m_lines[i]->m_x <= m_lines[previous]->m_x+precision &&
             m_lines[i]->m_x >= m_lines[previous]->m_x-precision &&
             m_lines[i]->m_y <= m_lines[previous]->m_y+precision &&
             m_lines[i]->m_y >= m_lines[previous]->m_y-precision )
          {
              // same as previous one
              std::cout << std::fixed << std::setprecision(2);
              std::cout << "Duplicating at " << m_lines[i]->m_x << " " << m_lines[i]->m_y << " - Removing item\n";
          }
          else
          {
             outFile << m_lines[i]->m_line << "\n";
             previous = i;
          }
      }
      std::cout << "\n";



    for(unsigned int i=0; i<m_lines.size(); ++i)
    {
       delete m_lines[i];
    }
    inFile.close();
    outFile.close();
    std::cout << "   ***   EXIT   ***\n";
    return EXIT_SUCCESS;
}

