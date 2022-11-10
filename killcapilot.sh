#!/bin/bash

help_str="参数说明:
    无参数:		    杀掉所有capilot相关进程
    -h, --help:             打印帮助信息
    -s, --show:             显示所有capilot相关进程
    -p, --pid:              仅显示所有capilot相关进程的pid
"
file_name=$(basename $0) # 文件名
app_name=${file_name%\.*} # 文件名无后缀名

# -q:禁止getopt输出错误报告 -o:定义短选项 -l:定义长选项
# --:后面为参数,前面为选项
getopt_cmd=$(getopt  -o h,s,p -l help,show,pid -- "$@")

[ $? -ne 0 ] && exit 1  # 若执行getopt错误,需要终止脚本

# set --: 将getopt的输出作为新的命令行选项和参数
# eval: 将字符设为变量(处理getopt输出的参数的转义字符)
eval set -- "$getopt_cmd" 

if [ -n "$1" ]
then
	case "$1" in
        -h|--help)
		echo -e "$help_str" 
		exit 0 ;;
        -s|--show)
		ps -ef | grep capilot | grep -v grep | grep -v killcapilot
		exit 0 ;;
        -p|--pid)
		ps -ef | grep capilot | grep -v grep | grep -v killcapilot| awk '{print $2}'
		exit 0 ;;
	--)			
		shift ;;
	*)
		echo "$(basename $0): '$1' 不是有效命令. 查看 '$(basename $0) --help'."
		exit 0 ;;
        esac
        shift
fi

ps -ef | grep capilot | grep -v grep  | grep -v killcapilot | awk '{print $2}'|xargs kill -2


