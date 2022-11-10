#include "can_structure.h"
#include <map>

std::map<int,int> dlcLenMap{
    {8,8},{12,9},{16,10},{20,11},{24,12},{32,13},{48,14},{64,15}
};



int canDataLenToDlc(int len)
{
    if(len<0||dlcLenMap.find(len)==dlcLenMap.end())
    {
        throw std::runtime_error("error can data len to convert to dlc "+std::to_string(len));
    }
    int dlc=0;
    if(len<=8)
    {
        dlc=len;
    }
    else
    {
        dlc=dlcLenMap.at(len);
    }
    return dlc;
}
