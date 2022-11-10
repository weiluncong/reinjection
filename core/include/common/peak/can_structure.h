/**
 * @file can_structure.h
 * @author wang kuan (wangkuan@changan.com.cn)
 * @brief define can structures
 * @version 0.1
 * @date 2020-09-28
 * 
 * @copyright Copyright (c) 2020 Changan Automobile Co., Ltd. All rights reserved.
 * 
 */

#ifndef CAN_STRUCTURE_H
#define CAN_STRUCTURE_H

#include <cmath>
#include <string>
#include <vector>
#include <map>

/**
 * @brief protocol is intel or motorola
 */
enum class CanModel
{
    Intel,
    Motorola
};

/**
 * @brief describe can signal
 * contains information such as `factor`, `offset`, `start bit`
 */
struct CanSignal
{
    int start = 0;
    int length = 0;
    CanModel model = CanModel::Intel;
    double factor = 0;
    double offset = 0;
    bool isSign = 1;
    double min = 0;
    double max = 0;
    std::string name = "";
    std::string belongsCanmsgName = "";
    int belongsCanmsgId = 0;
    int belongsCanmsgLen = 0;
    int cycleTime = 0;

    CanSignal() {}
    CanSignal(int start_, int length_, CanModel model_, double factor_, double offset_,
              bool isSign_, double min_, double max_, const std::string &name_,
              const std::string &belongsCanmsgName_, int belongsCanmsgId_, int belongsCanmsgLen_, int cycleTime_)
        : start(start_),
          length(length_),
          model(model_),
          factor(factor_),
          offset(offset_),
          isSign(isSign_),
          min(min_),
          max(max_),
          name(name_),
          belongsCanmsgName(belongsCanmsgName_),
          belongsCanmsgId(belongsCanmsgId_),
          belongsCanmsgLen(belongsCanmsgLen_),
          cycleTime(cycleTime_)
    {
        if (min == 0 && max == 0) {
            if (!isSign) {
                min = 0;
                max = (pow(2, length) - 1) * factor + offset;
            } else {
                min = -pow(2, length - 1) * factor + offset;
                max = (pow(2, length - 1) - 1) * factor + offset;
            }
        }

        const int BYTE_LENGTH = 8;
        if (model == CanModel::Motorola) {
            int row = start_ / BYTE_LENGTH;
            int col = start_ % BYTE_LENGTH;

            int remainder = length_ - (col + 1);
            int spanRows = ceil(remainder / (double)BYTE_LENGTH);

            remainder %= BYTE_LENGTH;
            if (remainder <= 0) {
                remainder += BYTE_LENGTH;
            }

            start = (row + spanRows + 1) * BYTE_LENGTH - remainder;
        }
    }
};


typedef std::vector<std::pair<double, CanSignal>> CanVars; //the first of pair is the can signal value

/**
 * @brief describe the can message
 * contains the message id, the message name and signals
 */
struct CanMessage
{
    int id;
    std::string name;
    std::map<std::string, CanSignal> canSignals; //key is the name of signal
};

struct CanFdMessage
{
    int id;
    int Len;
    std::string name;
    std::map<std::string, CanSignal> canSignals; //key is the name of signal
};

#pragma pack(1)
struct CanData
{
    int id = 0;
    int len = 8;
    unsigned char data[64] = {0};
};
#pragma pack()

typedef std::map<int, CanData> CanDataMap; //key is msg id

struct CanDataPack
{
    int flag = 0; //data source
    int type = 0; //data type
    double timestamp = 0;
    CanDataMap datamap;
};

/**
 * @brief the corresponding number of bits
 */
const long long kBitsValue[9] = {
    0,
    1,
    3,
    7,
    0xf,
    0x1f,
    0x3f,
    0x7f,
    0xff};

int canDataLenToDlc(int len);

#endif //CAN_STRUCTURE_H
