import time
import threading
from . import udp_sender
import logging
import os

#返回list中小于或等于value的值中最大的值
def Bisect(list=[],value=0):
    pos=0
    start = 0
    end = len(list) -1

    if len(list)< 1 :
        print("警告：输入列表为空")
        return -1
    elif len(list) >= 1 :
        if list[end]<value:
            # print("警告：大于最大值")
            return end
        if list[end]==value:
            return end
        if list[0]>value:
            # print("警告：小于最小值")
            return 0
        if list[0]==value:
            return 0
        else:
            while 1:
                if end-start==1:
                    pos = start
                    return pos
                mid = int((start + end)/2)
                if list[mid]>value:
                    end = mid
                elif list[mid]<value:
                    start = mid
                else :
                    pos = mid
                    return pos

class EvalCase():
    result_ = ""
    reason_ = ""
    # dataDict_ = {}

    signalList=[]
    allDataDict_ = {"":{}}
    lastDataDict_ = {}
    currentTime = 0.0
    startTime = 0.0

    sender = udp_sender.udpSender()

    if not os.path.exists("./log"):
        os.mkdir("./log")


    def __init__(self):
        self.result_ = ""
        self.reason_ = ""
        # self.dataDict_ = {}

        self.allDataDict_ = {"":{}}
        self.lastDataDict_ = {}
        self.currentTime = 0.0
        self.startTime = 0.0
        self.signalList.clear()

        self.sender = udp_sender.udpSender()

        self.dataLogger = logging.getLogger(self.__class__.__name__ + "data")
        self.dataLogger.setLevel(logging.INFO)
        console_hander  = logging.FileHandler('log/'+self.__class__.__name__+'_data.log','w')
        formatter       = logging.Formatter('%(asctime)s %(filename)s[line:%(lineno)d] %(levelname)s %(message)s')
        console_hander.setFormatter(formatter)
        self.dataLogger.addHandler(console_hander)
        self.dataLogger.info("start dataLogger")

    def case(self):
        pass

    def addData(self,sglName="",value=0.0,timestamp=0.0):
        if self.startTime == 0:
            self.startTime = timestamp
        self.currentTime = timestamp - self.startTime
        # self.dataDict_[sglName] = [value,self.currentTime]
        self.lastDataDict_[sglName] = {"value":value,"timestamp":self.currentTime}


        if sglName in self.allDataDict_:
            self.allDataDict_[sglName].update({self.currentTime:value})
        else:
            self.allDataDict_.update({sglName:{self.currentTime:value}})
        
        # print (str(self.allDataDict_[sglName]))

        self.dataLogger.info(str("{0} {1}: {2}").format('%.5f' % self.currentTime,sglName,value) )
        
        return self.case()

    def setResult(self,result="",reason=""):
        self.result_ = result
        self.reason_ = reason
        return 1

    def getResult(self):
        s = str(self.result_) + ";" + str(self.reason_)
        return s

    def getData(self,sglName="",timestamp=0.0):
        if sglName in self.allDataDict_.keys():
            dict = {}
            dict = self.allDataDict_[sglName]
            timelist = list(dict.keys())
            pos = Bisect(timelist,timestamp)
            return [timelist[pos],dict[timelist[pos]]]
        else:
            return "none"
    
    # 返回最后收到的数据和时间
    def getLastData(self,sglName=""):
        if sglName in self.lastDataDict_.keys():
            return self.lastDataDict_[sglName]
        else:
            return {"value":0,"timestamp":0}

    def setSignals(self,sglName=""):
        self.signalList.append(sglName)
        # self.sender.send(str(sglName)+str(len(self.signalList)))

    def getSignal(self,pos=0):
        if pos<len(self.signalList) and pos >=0:
            return self.signalList[pos]
        else:
            return "none"

