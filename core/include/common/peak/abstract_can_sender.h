/**
 * @file Abstract_can_sender.h
 * @author wang kuan (wangkuan@changan.com.cn)
 * @brief the abstract base class to send can type message
 * @version 0.1
 * @date 2020-09-29
 * 
 * @copyright Copyright (c) 2020 Changan Automobile Co., Ltd. All rights reserved.
 * 
 */

#ifndef ABSTRACT_CAN_SENDER_H
#define ABSTRACT_CAN_SENDER_H

#include "can_structure.h"
#include <memory>

class AbstractCanSender
{
public:
    AbstractCanSender() {}
    virtual ~AbstractCanSender() {}

    virtual int send(const CanData &data) = 0;
    int send(const CanDataMap& datamap);

};


typedef std::shared_ptr<AbstractCanSender> AbstractCanSenderPtr;

#endif //ABSTRACT_CAN_SENDER_H
