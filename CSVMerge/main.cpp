#include<stdio.h>
#include<cstdlib>
#include<iostream>
#include<string.h>
#include<fstream>
#include<sstream>
#include<dirent.h>
#include <vector>


unsigned int count = 0;

// <directory of CSVs> <optional directory of exporting all.csv>
int main (int argc, char const* argv[])
{
   std::string dirStr;
   if(argc<2)
   {
      std::cerr << "Too few arguments. Please include directory of .csv files";
      dirStr=".";
   }
   else
   {
      dirStr=argv[1];
   }

   std::string extension(".csv");
   std::vector<std::string> labels;
   std::string allCSV(dirStr+"\\all.csv");
   if(argc==3)
   {
      allCSV = argv[2];
   }
   std::ofstream allCSVfile;
   std::cout << "Saving output at: " << allCSV << "\n";
   allCSVfile.open(allCSV.c_str());
   if(!allCSVfile.is_open())
   {
      std::cerr<<"ERROR: couldn't create file\n";
   }
   DIR *dir;
   struct dirent *ent;
   if ((dir = opendir (dirStr.c_str())) != NULL) {
     /* print all the files and directories within directory */
     while ((ent = readdir (dir)) != NULL)
     {
       std::string current(ent->d_name);
       if(current.size()>4)
       {
//          std::cout << "\n\n" << allCSV << "\n";
           std::string subStr;
           if(current.length()>dirStr.length())
           {
           subStr= allCSV.substr(dirStr.length(),current.length());
           }
           else
           {
              subStr = allCSV;
           }

          current = dirStr + "\\" + current ;
          std::size_t pos = current.find_last_of(extension);

          if(pos==current.size()-1 && current!=subStr)
          {
              std::cout << current <<"\n";

             std::ifstream nextCSV(current.c_str());
             if(!nextCSV.is_open())
             {
                std::cerr<<"ERROR: File was not loaded\n";
                continue;
             }
             std::string line;
             if(std::getline(nextCSV,line))
             {
                if(labels.size()==0)
                {
                   allCSVfile << line << "\n";
                   std::istringstream ss( line );
                   while (ss)
                   {
                      std::string subS;
                      if (!std::getline( ss, subS, ',' )) break;
                      labels.push_back(subS);
                   }
                   for(auto &l : labels) for (auto & c: l) c = std::toupper(c);
                }
                else
                {
                   std::vector<std::string> clabels;
                   std::istringstream ss( line );
                   while (ss)
                   {
                      std::string subS;
                      if (!std::getline( ss, subS, ',' )) break;
                      clabels.push_back(subS);
                   }
                   for(auto &l : clabels) for (auto & c: l) c = std::toupper(c);
                   if(labels.size()!=clabels.size())
                   {
                      std::cout << "ERROR: header of file do not match with the current header\n";
                      continue;
                   }
                   unsigned int countSame(0);
                   for(unsigned int i=0; i<labels.size(); ++i)
                   {
                      countSame+=(labels[i]==clabels[i]);
                   }
                   if(countSame!=labels.size())
                   {
                       std::cout << "ERROR: header of file do not match with the current header\n";
                       continue;
                   }
                }
                count++;
                while(std::getline(nextCSV,line))
                {
                    allCSVfile << line << "\n";
                }
                nextCSV.close();
             }
             else
             {
                std::cout << "WARNING: File is empty\n";
             }
          }
       }
     }
     closedir (dir);
   }
   else
   {
      /* could not open directory */
      perror ("");
      return EXIT_FAILURE;
   }
   allCSVfile.close();
   std::cout << "   ***   EXIT SUCCESS   ***\n";
   return EXIT_SUCCESS;
}
