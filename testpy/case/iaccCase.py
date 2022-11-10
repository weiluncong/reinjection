import logging
import threading
import time
from common import EvalCase
from common import udp_sender

# 每接收到一个信号就会执行一遍case

# 获取指定信号最新的数据：
# data = self.getLastData("xxxx") 
# xxxx为信号名
# xxxx信号的值 data["value"]
# xxxx信号的接收时间 data["timestamp"]

# case还未出结果：
# return 0 

# case已经出结果：
# return self.setResult(result="",reason="") 设置case结果和原因



class iaccCase(EvalCase.EvalCase):
    # 初始化需要的变量
    counter = 0
    
    iaccSwitchFirstTime = 0
    errorFlag_lng = False
    errorFlag_lat = False
    errorFlag_odc1 = False
    errorFlag_odc2 = False
    reason = []

    ## 日志记录
    # def __init__(self):
    #     EvalCase.EvalCase.__init__(self)
    #     self.logger = logging.getLogger(self.__class__.__name__)
    #     self.logger.setLevel(logging.INFO)
    #     console_hander  = logging.FileHandler('log/'+self.__class__.__name__+'.log','w')      # 日志文件名：类名.log
    #     formatter       = logging.Formatter('%(asctime)s %(filename)s[line:%(lineno)d] %(levelname)s %(message)s')
    #     console_hander.setFormatter(formatter)
    #     self.logger.addHandler(console_hander)

    #     self.logger.info("start log")     # self.logger.info()调用接口写入日志信息

    def case(self):
        # d_grdIaccSwitchSts = self.getLastData("transCalc-FromgrdProto.BusButtonProcs~grdIaccSwitchSts")
        d_grdIaccSwitchSts = self.getLastData(self.getSignal(0))
        d_sysLatstate      = self.getLastData(self.getSignal(1))
        d_sysLngstate      = self.getLastData(self.getSignal(2))
        d_pdIaccStatus     = self.getLastData(self.getSignal(3))
        d_pdStatus         = self.getLastData(self.getSignal(4))
        d_lngAvailableWord = self.getLastData(self.getSignal(5))
        d_odcCruiseFeaturerescond1sts  = self.getLastData(self.getSignal(6))
        d_odcCruiseFeaturerescond2sts  = self.getLastData(self.getSignal(7))
        d_odcCruiseFeaturerescond1text = self.getLastData(self.getSignal(8))
        d_odcCruiseFeaturerescond2text = self.getLastData(self.getSignal(9))
        d_latAvailableWord          = self.getLastData(self.getSignal(10))
        d_odcLccFeaturerescond1sts  = self.getLastData(self.getSignal(11))
        d_odcLccFeaturerescond2sts  = self.getLastData(self.getSignal(12))
        d_odcLccFeaturerescond1text = self.getLastData(self.getSignal(13))
        d_odcLccFeaturerescond2text = self.getLastData(self.getSignal(14))
        d_odcIaccFunrescond1sts     = self.getLastData(self.getSignal(15))
        d_odcIaccFunrescond2sts     = self.getLastData(self.getSignal(16))
        d_odcIaccIaccrescond1text   = self.getLastData(self.getSignal(17))
        d_odcIaccIaccrescond2text   = self.getLastData(self.getSignal(18))

        # self.sender.send(str(self.getData("transCalc-StmProto.SysStateMng~sysLatstate",100)))
        # self.logger.info("iacc active")

        if (d_grdIaccSwitchSts["value"] == 1) and (self.iaccSwitchFirstTime == 0):
            self.iaccSwitchFirstTime = d_grdIaccSwitchSts["timestamp"]
        if self.iaccSwitchFirstTime !=0:
            if ((self.currentTime - self.iaccSwitchFirstTime)>4 and (self.currentTime - self.iaccSwitchFirstTime)<8):
                if (d_pdStatus["value"] != 2 and d_pdIaccStatus["value"] !=3):
                    if (d_pdIaccStatus["value"] ==1):
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
                                    self.reason.append("ODC_对中Feature抑制条件1不满足")
                                if (d_odcLccFeaturerescond2sts["value"] ==0x1):
                                    self.reason.append("ODC_对中Feature抑制条件2不满足")
                            
                        if ((d_odcIaccFunrescond1sts["value"] !=0) and (not self.errorFlag_odc1)):   # ODC_IACC功能抑制条件1状态
                            self.errorFlag_odc1 = True
                            self.reason.append("ODC_IACC功能抑制条件1不满足")
                        if ((d_odcIaccFunrescond2sts["value"] !=0) and (not self.errorFlag_odc2)):   # ODC_IACC功能抑制条件1状态
                            self.errorFlag_odc2 = True
                            self.reason.append("ODC_IACC功能抑制条件2不满足")
            elif (self.currentTime - self.iaccSwitchFirstTime)>8:
                if(not(self.errorFlag_lat or self.errorFlag_lng or self.errorFlag_odc1 or self.errorFlag_odc2)):
                    if(d_sysLatstate["value"] !=3 or d_sysLngstate["value"] !=3):
                        return self.setResult("failed","none") # case已经出结果，调用setResult()设置结果
                    else:
                        return self.setResult("success","none")
                else:
                    return self.setResult("failed",(",".join(self.reason)))
        return 0 



def main():
    mycase = iaccCase()
    i=0
    # mycase.run()
    while i<10000:
        i=i+1
        mycase.addData("transCalc-StmProto.SysStateMng~sysLngstate",i,0)
        # print(mycase.getCaseInfo())
        time.sleep(1)

if __name__ == "__main__":
    main()
