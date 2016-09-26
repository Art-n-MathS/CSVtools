#include <string>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <fstream>
#include<sstream>

int main(int argc, char *argv[])
{
    std::string input;
    std::string output;
    std::string line;
    std::vector<std::string> labels;
    std::vector<unsigned int> columnsOfInterest;

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
       else if (std::string(argv[i])=="-c")
       {
          while(i+1<argc && std::string(argv[i+1]).at(0)!='-')
          {
            ++i;
            columnsOfInterest.push_back(atoi(argv[i]));
          }
       }
       else
       {
          std::cerr << "WARNING: unknown tag : " <<argv[i] << "\n";
       }
    }

    if(columnsOfInterest.size()==0 || input=="" || output=="")
    {
       std::cerr << "ERROR: Missing Arguments\n";
       return EXIT_FAILURE;
    }
    unsigned int maxColOFInterest=0;
    for(unsigned int i=0; i<columnsOfInterest.size();++i)
    {
       if(maxColOFInterest<columnsOfInterest[i])
       {
          maxColOFInterest=columnsOfInterest[i];
       }
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
    while(std::getline(inFile,line))
    {
      std::istringstream ss( line );
      while (ss)
      {
        std::string subS;
        if (!std::getline( ss, subS, ',' )) break;
        labels.push_back(subS);
      }
      if(labels.size()<=maxColOFInterest)
      {
         std::cerr << "ERROR: Line too short. Columns that don't exist have been requested\n";
         return EXIT_FAILURE;
      }
      else
      {
         outFile << labels[columnsOfInterest[0]];
         for(unsigned int i=1; i<columnsOfInterest.size(); ++i)
         {
            outFile << "," << labels[columnsOfInterest[i]];
         }
         outFile << "\n";
      }
      labels.clear();
    }

    inFile.close();
    outFile.close();
    std::cout << "   ***   EXIT   ***\n";
    return EXIT_SUCCESS;
}

