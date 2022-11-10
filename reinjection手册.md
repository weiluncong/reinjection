# reinjection使用说明

[TOC]

- **更新日期：2022年10月17日**

## 一、代码拉取

reinjection地址：https://iov.changan.com.cn/git/capilot/reinjection ,选择tags中V1.x.x最新的版本

## 二、运行

### 1.  运行组件

1.修改task.yaml

​	根据需要开启关闭模块，通过每个模块下的“activate”控制

​	如果只需要单个模块回注，如AEB模块，则将其他所有模块的“activate”改为false，AEB模块的“activate”改为true；

2.修改capilot_comVerRecord.sh文件

​	修改capilot运行命令，必须要有--online=false，其他命令可以根据需要更改：

```shell
capilot  --online=false --record=false  --task_path=task.yaml 
```

​	运行capilot_comVerRecord.sh

### 2.  运行回注工具

进入reinjection目录，运行build.sh编译成功后，运行run.sh

```shell
ca@ca:~/reinjection$ ./run.sh 
replayer bind ipc:///tmp/replayer.ipc
replayer connect tcp://127.0.0.1:8888
reveiveParserData init over
start running test cases
current case: aebCase 1 1
```

### 3.  日志

程序默认记录接收到的数据到log文件夹下，以脚本的类名+"_data.log"作为文件名，记录接收数据的currentTime,sglName,value等信息，内容格式如下：

```shell
2022-09-19 16:21:07,869 EvalCase.py[line:77] INFO start dataLogger
2022-09-19 16:21:07,876 EvalCase.py[line:97] INFO 0.00000 transCalc-StmProto.SysStateMng~sysLatstate: 1.0
2022-09-19 16:21:07,877 EvalCase.py[line:97] INFO 0.00000 transCalc-StmProto.SysStateMng~sysLngstate: 3.0
2022-09-19 16:21:07,877 EvalCase.py[line:97] INFO 0.00003 transCalc-StmProto.PdStsMng~pdIaccStatus: 1.0
2022-09-19 16:21:07,877 EvalCase.py[line:97] INFO 0.00003 transCalc-StmProto.PdStsMng~pdStatus: 1.0
2022-09-19 16:21:07,877 EvalCase.py[line:97] INFO 0.00004 transCalc-StmProto.DebugSTM~latAvailableWord: 4.0
```



### 4.  生成报告

程序运行结束生成report.yaml文件，如果在运行之前已经有report.yaml文件，则会将其进行覆盖

## 三、用例编写

### 1.  用例模板

ABCD.py  	*# 文件名和用例类名一致*

```python
import threading
import time
from common import EvalCase
from common import udp_sender

class ABCD(EvalCase.EvalCase):	# ABCD需要和文件名保持一致
    ### 添加自定义变量 ###
    
    def case(self):
        ### 添加代码 ###
```

### 2.  注意事项

- case每接收到一个新的信号就会执行一遍case

- 获取指定信号最新的数据：

  `data = self.getLastData("xxxx")`   	*# xxxx为信号名*

  xxxx信号的值 `data["value"]`

  xxxx信号的接收时间 `data["timestamp"]`

- case的返回值：

  还未出结果：`return 0` 

  已经出结果：`return self.setResult(result="",reason="") ` 	*# 设置case结果和原因*

- self.setResult（result="",reason=""）注意事项：

  1.两个参数均为字符串，禁止向其中写入 “ ; ”  **！！！**
  
  2.成功必须使用：**success**
  
  3.失败必须使用：**failed**
  
  

## 四、配置文件

### 1.  用例配置文件模板

ABCD.yaml  	*# 文件名和用例类名一致*

```yaml
Message :
    - transCalc-StmProto.Odc~odc397Stsinh~odcLccFeaturerescond1sts      # 0
    - transCalc-StmProto.Odc~odc821Stsinh~odcLccFeaturerescond2sts      # 1
    - transCalc-StmProto.Odc~odc397Textinh~odcLccFeaturerescond1text    # 2
    - transCalc-StmProto.Odc~odc821Textinh~odcLccFeaturerescond2text    # 3
    - transCalc-StmProto.Odc~odc397Stsinh~odcIaccFunrescond1sts         # 4

author : 202207766@any3.com
dataFile : "2022-08-08_19-16-02.dat"	# 2022/09/15修改：只写dat文件名，dat文件统一放在reinjection工程data目录下
IaccActive : -1 # 不小于0时有效			# 2022/09/15增加：iacc激活信号配置
```

### 2.  回注工具配置文件 ReinjectConfig.yaml

1."case"字段下填写评价脚本相关信息，”isReplay“字段控制是否运行此脚本

2.设置需要回注的数据：

​	a）“replayTopic ”字段下的所有topic都将直接回注到框架；

​	b）“replayTopic_TC397”字段下的所有topic都将组件名替换为“reinjection”后回注到框架，例如：

​		  "sab_c385-EmsProto.BusEmsMotionInfo" ----> “reinjection-EmsProto.BusEmsMotionInfo”

```yaml
case : 
- DTMNumber: "DTM123456789_1"  
  scriptPath : iaccCase		# 用例名
  scriptConfigPath :  "./testpy/config/iaccCase.yaml"
  isReplay : true			# 运行开关

- DTMNumber: "DTM123456789_2"  
  scriptPath : iaccCase1
  scriptConfigPath :  "./testpy/config/iaccCase1.yaml"
  isReplay : true

- DTMNumber: "DTM123456789_3"  
  scriptPath : iaccCase2
  scriptConfigPath :  "./testpy/config/iaccCase2.yaml"
  isReplay : false

replayTopic :
    - sab_c385-RawDataProto.RawData-fcr
    - sab_c385-RawDataProto.RawData-rcr
    - sab_c385-RawDataProto.RawData-rv
    # ......
replayTopic_TC397 :
    - sab_c385-EmsProto.BusEmsMotionInfo
    - sab_c385-EmsProto.BusEmsSts
```







## 五、退出

### 1.  超时退出

​	运行超过数据的时长用例将被强制退出，运行下一个用例

### 2.  提前退出

​	Ctrl+C 无法结束回注工具时，使用 Ctrl+Z 暂停进程，然后执行  killall -9 reinjection_  



## 六、上传

### 1.  脚本上传

上传git：https://iov.changan.com.cn/git/capilot/reinjection/-/tree/cicd

分支：cicd

### 2.  数据上传

1）、地址：[http://10.64.32.169/svn/智能驾驶所/99_Capilot/injectData/data](http://10.64.32.169/svn/智能驾驶所/99_Capilot/injectData/data)

2）、运行流水线同步至服务器：http://sdas.changan.com:88/jenkins-auto/view/intergrate-APA/job/cicd-update-SVN-pipeline/







