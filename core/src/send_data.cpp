#include "send_data.h"
#include "map_canid.h"
#include <QDebug>

Send_Data::Send_Data()
{
    send_1 = new PeakCanSender(1, 500);
    send_2 = new PeakCanFdSender(2, 5000);
    send_3 = new PeakCanFdSender(3, 5000);
    send_4 = new PeakCanFdSender(4, 5000);
    send_5 = new PeakCanFdSender(5, 2000);

    sender_fc = new UdpSender(FC_IP, FC_PORT);
    sender_ac = new UdpSender(AC_IP, AC_PORT);
    sender_fr = new UdpSender(FR_IP, FR_PORT);

    replayer = new DataReplay();

    ofs_.open("0x161.txt");
}

Send_Data ::~Send_Data()
{
    delete (send_1);
    delete (send_2);
    delete (send_3);
    delete (send_4);
    delete (send_5);
    delete (sender_fc);
    delete (sender_ac);
    delete (sender_fr);
    delete (replayer);
    ofs_.close();
}

QStringList Send_Data::Set_File_Name(const string &file)
{
    Clear();
    read_file(file);
    QStringList tmp;
    tmp << "start time : " + QString::number(start_time, 10, 6);
    tmp << "end time : " + QString::number(end_time, 10, 6);
    tmp << "total :" + QString::number(end_time - start_time, 10, 6);
    if (end_time - start_time > 300)
        tmp << "Reminder: The duration of the current reloading dat file exceeds 300s, please pay attention to whether the data is correct! ! !";
    tmp << "bodycan size:" + QString::number(channel_1.size());
    tmp << "bcan size:" + QString::number(channel_2.size());
    tmp << "ccan size:" + QString::number(channel_3.size());
    tmp << "dcan size:" + QString::number(channel_4.size());
    tmp << "ecan size:" + QString::number(channel_5.size());

    tmp << "ac size:" + QString::number(data_ac.size());
    tmp << "fc size:" + QString::number(data_fc.size());
    tmp << "fr size:" + QString::number(data_fr.size());

    replayer->createIndexInfo(file);

    return tmp;
}

QStringList Send_Data::Send()
{
    std::thread td_1(&Send_Data::send_function_<AbstractCanSender *, const map<double, CanData> &>, this, send_1, channel_1);
    std::thread td_2(&Send_Data::send_function_<AbstractCanSender *, const map<double, CanData> &>, this, send_2, channel_2);
    std::thread td_3(&Send_Data::send_function_<AbstractCanSender *, const map<double, CanData> &>, this, send_3, channel_3);
    std::thread td_4(&Send_Data::send_function_ch4, this, send_4, channel_4);
    std::thread td_5(&Send_Data::send_function_<AbstractCanSender *, const map<double, CanData> &>, this, send_5, channel_5);

    std::thread td_ac(&Send_Data::send_function_<UdpSender *, const map<double, std::string> &>, this, sender_ac, data_ac);
    std::thread td_fc(&Send_Data::send_function_<UdpSender *, const map<double, std::string> &>, this, sender_fc, data_fc);
    std::thread td_fr(&Send_Data::send_function_<UdpSender *, const map<double, std::string> &>, this, sender_fr, data_fr);

    std::thread td_usbcam(&DataReplay::beginReplay, replayer);

    td_5.join();
    td_2.join();
    td_3.join();
    td_4.join();
    td_1.join();

    td_ac.join();
    td_fc.join();
    td_fr.join();
    td_usbcam.join();

    QStringList tmp;
    tmp << "this file inject over !!!";
    return tmp;
}

void Send_Data::read_file(const std::string &file_)
{
    std::ifstream ifs_;
    ifs_.open(file_);
    if (!ifs_.is_open())
    {
        std::cout << "open file error: " << file_ << std::endl;
    }
    ifs_.seekg(0);
    KDataFileHead fileHead;
    ifs_.read((char *)&fileHead, sizeof(fileHead));
    ifs_.seekg(sizeof(fileHead) + fileHead.protoContentSize);
    ifs_.seekg(fileHead.commentLength, ios::cur);

    cReplayData replayData;
    MetaData mdata;
    while (ifs_.read((char *)&replayData.head, sizeof(replayData.head)))
    {
        size_t size = replayData.head.size;
        mdata.resize(size);
        ifs_.read(&mdata[0], size);
        replayData.mdata = mdata;
        string temp = replayData.head.name;
        if (temp.find("OriginDataTrans.OriginCanData") != -1)
        {
            parse_proto(mdata);
        }
        else if (temp.find("OriginDataTrans.OriginUdpData_ac") != -1)
        {
            parse_eth(mdata, "ac");
        }
        else if (temp.find("OriginDataTrans.OriginUdpData_fc") != -1)
        {
            parse_eth(mdata, "fc");
        }
        else if (temp.find("OriginDataTrans.OriginUdpData_fr") != -1)
        {
            parse_eth(mdata, "fr");
        }
    }
}

void Send_Data::parse_proto(const std::string &msg)
{
    for (size_t i = 0; i < 10; i++)
    {
        ids_[i];
    }

    OriginDataTrans::OriginCanData can_data;
    can_data.ParseFromString(msg);
    CanData data;
    data.id = can_data.id();
    if (!ids_[can_data.bus_id()].contains(can_data.id()))
    {
        ids_[can_data.bus_id()].push_back(can_data.id());
    }
    data.len = can_data.data_len();
    if (can_data.time_stamp() == 0)
        return;
    end_time = can_data.time_stamp();
    if (start_time == 0)
        start_time = can_data.time_stamp();
    std::string tmp = can_data.can_data();
    memcpy(data.data, &tmp[0], can_data.can_data().length());

    if (can_data.bus_id() == 2)
    {
        channel_2[can_data.time_stamp()] = data;
    }
    else if (can_data.bus_id() == 3)
    {
        channel_3[can_data.time_stamp()] = data;
    }
    else if (can_data.bus_id() == 4)
    {
        // if (dcan_id_dlc.find(can_data.id()) == dcan_id_dlc.end())
        //     return;
        if (can_data.id() != 0x278)
            channel_4[can_data.time_stamp()] = data;
        if (can_data.id() == 0x161)
        {
            static double st = can_data.time_stamp();
            ofs_ << "time : " << std::fixed << can_data.time_stamp() - st << "   ";
            // for (auto char_ : tmp)
            // {
            //     ofs_ << int(char_) << "\t";
            // }
            ofs_ << int(tmp[14]) << "\t";
            ofs_ << "\n";
        }
    }
    else if (can_data.bus_id() == 5)
    {
        channel_5[can_data.time_stamp()] = data;
    }
    else if (can_data.bus_id() == 7)
    {
        channel_1[can_data.time_stamp()] = data;
    }
}

void Send_Data::parse_eth(const std::string &msg, const std::string &tocpic_name)
{
    OriginDataTrans::OriginUdpData data;
    data.ParseFromString(msg);
    if (data.time_stamp() == 0)
        return;
    end_time = data.time_stamp();
    if (start_time == 0)
        start_time = data.time_stamp();
    if (tocpic_name.find("ac") != -1)
    {
        data_ac[data.time_stamp()] = data.udp_data();
    }
    else if (tocpic_name.find("fr") != -1)
    {
        data_fr[data.time_stamp()] = data.udp_data();
    }
    else if (tocpic_name.find("fc") != -1)
    {
        data_fc[data.time_stamp()] = data.udp_data();
    }
}

template <typename T1, typename T2>
void Send_Data::send_function_(T1 sender, T2 data)
{
    if (data.begin() == data.end())
    {
        return;
    }
    double gap = KTime().getTime() - start_time;
    for (auto frame = data.begin(); frame != data.end(); frame++)
    {
        while (1)
        {
            if (stop_flag)
                return;
            if (KTime().getTime() >= frame->first + gap)
                break;
        }
        sender->send(frame->second);
    }
}
#define POLY_ 0x1021;
void Send_Data::send_function_ch4(AbstractCanSender *sender, const map<double, CanData> &data)
{
    ofstream ofs_161;
    ofs_161.open("ch4_0x161.txt");

    if (data.begin() == data.end())
    {
        return;
    }
    CanData data_tmp;
    double gap = KTime().getTime() - start_time;
    for (auto frame = data.begin(); frame != data.end(); frame++)
    {
        data_tmp = frame->second;
        while (1)
        {
            if (stop_flag)
                return;
            if (KTime().getTime() >= frame->first + gap)
                break;
        }
        if (active_iacc_flag && data_tmp.id == 0x161)
        {
            static uint count = 0;
            if (count < 6 || count > 23)
            {
                data_tmp.data[14] = 32;
            }
            if (count >= 6 && count <= 46)
            {
                data_tmp.data[14] = 64;
            }
            count++;
            if (count > 50)
            {
                active_iacc_flag = false;
                count = 0;
            }
        }

        if (data_tmp.id == 0x161)
        {
            static int counter_161 = 0;
            // std::cout << "0x161:";
            // ofs_161 << counter_161 << ":";
            // for (int i = 0; i < 64; i++)
            // {
            //     std::cout << (int)data_tmp.data[i] << " ";
            //     ofs_161 << (int)data_tmp.data[i] << " ";
            //     if (i == 29)
            //         i = 60;
            // }
            /*************CRC8*************/
            // int crcSigLine = 504 / 8;
            // int len = 7;
            // unsigned char dataBuf[7] = {0};
            // memcpy(dataBuf, data_tmp.data + crcSigLine - len, len);

            // unsigned long crc8Data = 0xff;
            // unsigned long crc8Divisor = 0x1d;
            // for (int i = 0; i < len; i++)
            // {
            //     crc8Data = crc8Data ^ static_cast<unsigned long>(dataBuf[i]);
            //     for (int j = 0; j < 8; j++)
            //     {
            //         if ((crc8Data & 0x80 != 0))
            //         {
            //             unsigned long dataTemp = std::fmod((crc8Data << 1), 256);
            //             crc8Data = std::fmod((dataTemp ^ crc8Divisor), 256);
            //         }
            //         else
            //         {
            //             crc8Data = std::fmod((crc8Data << 1), 256);
            //         }
            //     }
            // }
            // crc8Data = std::fmod((crc8Data ^ 0xff), 256);

            //-------------------------------------------------------------------------//

            /*************CRC64*************/
            int crcSigLine = 504 / 8;
            int len = 62;
            unsigned char dataBuf[len] = {0};
            memcpy(dataBuf, data_tmp.data, len);

            unsigned char i, k = 0;
            unsigned short CRC_acc = 0xffff;

            while (len--)
            {
                CRC_acc = CRC_acc ^ (dataBuf[k++] << 8);
                for (i = 0; i < 8; i++)
                {
                    if ((CRC_acc & 0x8000) == 0x8000)
                    {
                        CRC_acc = CRC_acc << 1;
                        CRC_acc ^= POLY_;
                    }
                    else
                    {
                        CRC_acc = CRC_acc << 1;
                    }
                }
            }

            //-------------------------------------------------------------------------//

            data_tmp.data[62] = 0;
            data_tmp.data[63] = 0;

            int fval = CRC_acc;
            int row = 504 / 8;
            int col = 504 % 8;
            int processedbits = 0;
            while (processedbits < 16)
            {
                int thisRowProcessBits = min(8 - col, 16 - processedbits);
                data_tmp.data[row] &= (0xff - (kBitsValue[thisRowProcessBits] << col));
                data_tmp.data[row] += (fval & kBitsValue[thisRowProcessBits]) << col;
                row += -1;
                col = 0;
                processedbits += thisRowProcessBits;
                fval >>= thisRowProcessBits;
            }

            // std::cout << "CRC_acc:" << CRC_acc << " " << (int)data_tmp.data[62] << " " << (int)data_tmp.data[63];
            // ofs_161 << "CRC_acc:" << CRC_acc << " " << (int)data_tmp.data[62] << " " << (int)data_tmp.data[63];

            // std::cout << std::endl;
            // ofs_161 << "\n";
            counter_161++;
        }
        ofs_161.flush();
        sender->send(data_tmp);
    }
    ofs_161.close();
}

void Send_Data::Clear()
{
    if (send_4 != nullptr)
    {
        delete (send_4);
        this_thread::sleep_for(chrono::milliseconds(500));
        send_4 = new PeakCanFdSender(4, 5000);
    }

    channel_1.clear();
    channel_2.clear();
    channel_3.clear();
    channel_4.clear();
    channel_5.clear();

    data_ac.clear();
    data_fr.clear();
    data_fc.clear();

    start_time = 0;
    end_time = 0;
}

void Send_Data::setIaccActive()
{
    active_iacc_flag = true;
}