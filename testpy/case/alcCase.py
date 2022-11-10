import threading
import time
from common import EvalCase
from common import udp_sender

odc_1text={
    0:"无故障",
    1:"控制器硬件故障",
    2:"控制器通讯故障",
    3:"FR故障",
    4:"左侧前角雷达故障",
    5:"右侧前角雷达故障",
    6:"左侧后角雷达故障",
    7:"右侧后角雷达故障",
    8:"前区12U故障",
    9:"后区12U故障",
    10:"前摄像头故障",
    11:"VSM故障",
    12:"4V前摄像头故障",
    13:"定位传感器故障",
    14:"按键故障",
    15:"IACC按键故障",
    16:"显示系统故障",
    17:"转向系统故障",
    18:"行车制动系统故障",
    19:"动力系统故障",
    20:"动力系统通讯故障",
    21:"驻车制动系统故障",
    22:"车辆传感器故障",
    23:"门盖状态不确定",
    24:"转向灯开关故障",
    25:"灯光系统故障",
    26:"配置未打开",
    27:"非安全状态",
    28:"FR未标定",
    29:"左侧前角雷达未标定",
    30:"右侧前角雷达未标定",
    31:"左侧后角雷达未标定",
    32:"右侧后角雷达未标定",
    33:"FC未标定",
    34:"VSM未标定",
    35:"4V未标定",
    36:"续航里程不足",
    37:"轮胎异常",
    38:"轮胎状态不确认",
    39:"制动盘过热",
    40:"大雨",
    41:"路面颠簸",
    42:"FR被遮挡",
    43:"左侧前角雷达被遮挡",
    44:"右侧前角雷达被遮挡",
    45:"左侧后角雷达被遮挡",
    46:"右侧后角雷达被遮挡",
    47:"前区12U被遮挡",
    48:"后区12U被遮挡",
    49:"FC被遮挡",
    50:"VSM被遮挡",
    51:"4V被遮挡",
    52:"IMS被遮挡",
    53:"检测不到驾驶员",
    54:"疲劳",
    55:"注意力分散",
    56:"脱手",
    57:"无法检测驾驶员状态",
    58:"车门未全关闭",
    59:"未系安全带",
    60:"未启动车辆",
    61:"未开启ESP",
    62:"IACC未启用",
    63:"UDLC未启用",
    64:"NDA未启用",
    65:"无导航信息",
    66:"手力矩过大",
    67:"车速超出范围",
    68:"未处于前进档",
    69:"方向盘转角过大",
    70:"EPB未释放",
    71:"静止未踩刹车",
    72:"运动时踩刹车",
    73:"CANCEL按键未释放",
    74:"4V后摄像头故障",
    75:"4V侧摄像头故障",
    76:"左前门状态不确定",
    77:"安全气囊控制器故障",
    78:"4V炫目",
    79:"中雨",
    80:"温度超过范围",
    81:"温度异常",
    82:"左前门未关闭",
    83:"转向灯未关闭",
    85:"AEB接口响应抑制",
    88:"前溜",
    89:"后溜",
    90:"路面湿滑",
    91:" TJP未启用",
    92:" InfoCAN通讯故障",
    93:" BodyCAN通讯故障",
    94:" 冗余电池电量过低",
    95:" DDSA未启用",
    96:"周视左前摄像头故障",
    97:"周视后向中距离摄像头故障",
    98:"周视右后摄像头故障",
    99:"周视右前摄像头故障",
    100:"周视左后摄像头故障",
    101:" ACC方向盘转角过大",
    102:" IACC方向盘转角过大",
    103:" NDA方向盘转角过大",
    104:" TJP方向盘转角过大",
    105:" DDSA方向盘转角过大",
    106:" ACC方向盘转速过大",
    107:" IACC方向盘转速过大",
    108:" NDA方向盘转速过大",
    109:" TJP方向盘转速过大",
    110:" DDSA方向盘转速过大",
    111:"驾驶员接管方向盘",
    112:"FR工作模式不匹配",
    113:"FC工作模式不匹配",
    114:"后角雷达工作模式不匹配",
    115:"LVSM工作模式不匹配",
    116:"前角雷达工作模式不匹配",
    117:"FC场景模式不匹配",
    118:"周视场景模式不匹配"
}

odc_2text = {
    0:"无故障",
    1:"地图不可用",
    2:"障碍物",
    3:"定位异常",
    4:"地图非最新状态",
    5:"ALC FIDA道路类型不适用",
    6:"UDLC道路类型不适用",
    7:"IACC道路类型不适用",
    8:"NDA道路类型不适用",
    9:" 不满足电子围栏要求",
    10:"陡坡",
    11:"急弯",
    12:"车道错位",
    13:"施工区域",
    14:"事故区域",
    15:"匝道区域",
    16:"收费站区域",
    17:"非可行驶车道",
    18:"道路拥堵",
    19:"车道线不稳定",
    20:"车道宽度不合适",
    21:"车辆超速",
    22:"APA已激活",
    23:"行人 ",
    24:"航向角过大",
    25:"TJP道路类型不适用",
    26:"车辆压线"
}

class alcCase(EvalCase.EvalCase):
    counter = 0
    
    iaccSwitchFirstTime = 0
    errorFlag_lng = False
    errorFlag_lat = False
    errorFlag_odc1 = False
    errorFlag_odc2 = False
    reason = []

    def case(self):
        # d_grdIaccSwitchSts = self.getLastData("transCalc-FromgrdProto.BusButtonProcs~grdIaccSwitchSts")
        d_grdIaccSwitchSts = self.getLastData(self.getSignal(0))
        d_sysLatstate = self.getLastData(self.getSignal(1))
        d_sysLngstate = self.getLastData(self.getSignal(2))
        d_pdNdaStatus = self.getLastData(self.getSignal(3))
        d_pdStatus = self.getLastData(self.getSignal(4))
        d_lngAvailableWord = self.getLastData(self.getSignal(5))
        d_odcCruiseFeaturerescond1sts = self.getLastData(self.getSignal(6))
        d_odcCruiseFeaturerescond2sts = self.getLastData(self.getSignal(7))
        d_odcCruiseFeaturerescond1text = self.getLastData(self.getSignal(8))
        d_odcCruiseFeaturerescond2text = self.getLastData(self.getSignal(9))
        d_latAvailableWord = self.getLastData(self.getSignal(10))
        d_odcLccFeaturerescond1sts = self.getLastData(self.getSignal(11))
        d_odcLccFeaturerescond2sts = self.getLastData(self.getSignal(12))
        d_odcLccFeaturerescond1text = self.getLastData(self.getSignal(13))
        d_odcLccFeaturerescond2text = self.getLastData(self.getSignal(14))
        d_odcNdaFunrescond1sts = self.getLastData(self.getSignal(15))
        d_odcNdaFunrescond2sts = self.getLastData(self.getSignal(16))
        d_odcNdaFunrescond1text = self.getLastData(self.getSignal(17))
        d_odcNdaFunrescond2text = self.getLastData(self.getSignal(18))

        # self.sender.send(str(self.getData("transCalc-StmProto.SysStateMng~sysLatstate",100)))

        if (d_grdIaccSwitchSts["value"] == 1) and (self.iaccSwitchFirstTime == 0):
            self.iaccSwitchFirstTime = d_grdIaccSwitchSts["timestamp"]
        if self.iaccSwitchFirstTime !=0:
            if ((self.currentTime - self.iaccSwitchFirstTime)>4 and (self.currentTime - self.iaccSwitchFirstTime)<8):
                if (d_pdStatus["value"] != 3 and d_pdNdaStatus["value"] !=3):
                    if (d_pdNdaStatus["value"] ==1):
                        if ((d_sysLngstate["value"] !=3) and (not self.errorFlag_lng)):  # 纵向可用条件不满足
                            lng_avaiable_word = int(d_lngAvailableWord["value"]) 
                            if(lng_avaiable_word!=0):
                            {
                                self.errorFlag_lng = True
                            }
                            if ((lng_avaiable_word & 0x01) ==0x01):
                                if (d_odcCruiseFeaturerescond1sts["value"] == 0x01):
                                    self.reason.append("ODC_巡航Feature抑制条件1存在抑制")
                                if (d_odcCruiseFeaturerescond2sts["value"] == 0x01):
                                    self.reason.append("ODC_巡航Feature抑制条件2存在抑制")
                            if (((lng_avaiable_word & 0x02)>>1) ==0x01):
                                self.reason.append("纵向规划功能异常，来自LNP")
                            if (((lng_avaiable_word & 0x04)>>2) ==0x01):
                                self.reason.append("LCM_纵向控制功能异常，来自VLC")

                        if ((d_sysLatstate["value"] !=3) and (not self.errorFlag_lat)):   # 横向可用条件不满足
                            lat_avaiable_word = int(d_latAvailableWord["value"])
                            if(lat_avaiable_word!=0):
                            {
                                self.errorFlag_lat = True
                            }
                            if ((lat_avaiable_word & 0x01)==0x01):
                                self.reason.append("驾驶员油门超越")
                            if (((lat_avaiable_word & 0x02)>>1)==0x01):
                                self.reason.append("LatValid不满足")
                            if (((lat_avaiable_word & 0x04)>>2)==0x01):
                                self.reason.append("Path_Valid 无路径")
                            if (((lat_avaiable_word & 0x08)>>3)==0x01):
                                self.reason.append("满足禁止规则")
                            if (((lat_avaiable_word & 0x10)>>4)==0x01):
                                self.reason.append("pathplanning_warningsts 状态异常")
                            if (((lat_avaiable_word & 0x20)>>5)==0x01):
                                if (d_odcLccFeaturerescond1sts["value"] ==0x1):
                                    self.reason.append("ODC_对中Feature抑制条件1不满足："+ str(odc_2text.get(int(d_odcLccFeaturerescond1text["value"]), "None")))  # str(d_odcLccFeaturerescond1text["value"])
                                if (d_odcLccFeaturerescond2sts["value"] ==0x1):
                                    self.reason.append("ODC_对中Feature抑制条件2不满足："+ str(odc_2text.get(int(d_odcLccFeaturerescond2text["value"]), "None")))  # str(d_odcLccFeaturerescond2text["value"])

                        if ((d_odcNdaFunrescond1sts["value"] !=0) and (not self.errorFlag_odc1)):   # ODC_NDA功能抑制条件1状态
                            self.errorFlag_odc1 = True
                            self.reason.append("ODC_NDA功能抑制条件1不满足："+ str(odc_2text.get(int(d_odcNdaFunrescond1text["value"]), "None")))  # str(d_odcNdaFunrescond1text["value"])
                        if ((d_odcNdaFunrescond2sts["value"] !=0) and (not self.errorFlag_odc2)):   # ODC_NDA功能抑制条件1状态
                            self.errorFlag_odc2 = True
                            self.reason.append("ODC_NDA功能抑制条件2不满足："+ str(odc_2text.get(int(d_odcNdaFunrescond2text["value"]), "None")))  # str(d_odcNdaFunrescond2text["value"])
            elif (self.currentTime - self.iaccSwitchFirstTime)>8:
                if(not(self.errorFlag_lat or self.errorFlag_lng or self.errorFlag_odc1 or self.errorFlag_odc2)):
                    if(d_sysLatstate["value"] !=3 or d_sysLngstate["value"] !=3):
                        return self.setResult("failed","none") # case已经出结果，调用setResult()设置结果
                    else:
                        # print(d_odcNdaFunrescond2sts["value"])
                        return self.setResult("success","none")
                else:
                    return self.setResult("failed",(",".join(self.reason)))
        return 0 # case未出结果，返回0



def main():
    mycase = alcCase()
    i=0
    # mycase.run()
    while i<10000:
        i=i+1
        mycase.addData("transCalc-StmProto.SysStateMng~sysLngstate",i,0)
        # mycase.setSignals("transCalc-StmProto.SysStateMng~sysLngstate")
        # print(mycase.getCaseInfo())
        time.sleep(1)

if __name__ == "__main__":
    main()