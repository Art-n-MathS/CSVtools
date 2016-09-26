#ifndef LINE_H
#define LINE_H

#include <iostream>
#include <vector>

class Line
{
public:
   //--------------------------------------------------------------------------
   /// @brief
   //--------------------------------------------------------------------------
   Line(double i_x,double i_y, std::string line);

   /// @brief destructor
   ~Line();

   double m_x;
   double m_y;

   std::string m_line;
};

#endif // LINE_H
