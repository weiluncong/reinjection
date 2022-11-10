#include "abstract_can_receiver.h"

/**
 * @brief receive packed can data or canfd data
 * 
 * @tparam T CanData or CanFdData
 * @param[out] flag Indicate the source of received data 
 * @param[out] timestamp The time stamp of received data 
 * @return std::map<int, T> 
 */
CanDataPack AbstractCanReceiver::receive()
{
    CanDataPack data = packer_.getDataPack();

    // cout<<"data size:                 "<<data.datamap.size()<<endl;
    if (data.datamap.empty())
    {
        while (1)
        {
            packer_.addData(receiveFromDeviceBuffer(), KTime().getTime());
            data = packer_.getDataPack();
            if (data.datamap.empty())
            {
                // cout << "sleep in abstract can receiver" << endl;
                std::this_thread::sleep_for(std::chrono::milliseconds(2));
            }
            else
            {
                
                break;
            }
        }
    }
    return data;
}

/**
 * @brief calculate the AccCode to set the can hardware's receive range with AccMask 
 * 
 * @tparam T 
 * @param beginId the start id of the receiving range
 * @return unsigned int 
 */
unsigned int AbstractCanReceiver::calculateAccCode(int beginId) const
{
    return beginId << 21;
}

/**
 * @brief calculate the AccMask to set the can hardware's receive range with AccCode 
 * 
 * @tparam T 
 * @param beginId the start id of the receiving range
 * @param endId the end id of the receiving range
 * @return unsigned int 
 */
unsigned int AbstractCanReceiver::calculateAccMask(int beginId, int endId) const
{
    int bitDifference = beginId ^ endId;
    int bitsOfBitDifference = ceil(log(bitDifference) / log(2));
    int max = pow(2, bitsOfBitDifference) - 1;
    int mask = bitDifference | max;
    return (mask << 21) + 0x1fffff;
}