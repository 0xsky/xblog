#!/bin/bash

# 管理应用程序通用脚本
# By: xSky
#

# 应用程序名
appname=xblog
#日志文件名标识
logflag=xblog

export LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH

cfont()
{
while (($#!=0))
do
        case $1 in
            -b)
                    echo -ne " ";
            ;;
            -t)
                    echo -ne "\t";
            ;;
            -n)     echo -ne "\n";
            ;;
            -black)
                    echo -ne "\033[30m";
            ;;
            -red)
                    echo -ne "\033[31m";
            ;;
            -green)
                    echo -ne "\033[32m";
            ;;
            -yellow)
                    echo -ne "\033[33m";
            ;;
            -blue)
                    echo -ne "\033[34m";
            ;;
            -purple)
                    echo -ne "\033[35m";
            ;;
            -cyan)
                    echo -ne "\033[36m";
            ;;
            -white|-gray) echo -ne "\033[37m";
            ;;
            -reset)
                    echo -ne "\033[0m";
            ;;
            -h|-help|--help)
                    echo "Usage: cfont -color1 message1 -color2 message2 ...";
                    echo "eg:       cfont -red [ -blue message1 message2 -red ]";
            ;;
            *)
            echo -ne "$1"
            ;;
        esac
        shift
done
}

RETVAL=0

start() {
    cfont -cyan "Now will start the " -yellow  "[" -green "$appname" -yellow "]."  -reset -n
    pid=$(pidof $appname)
    if [ -n "$pid" ]; then
        cfont -red "The " -yellow "[" -green "$appname" -yellow "]" -red " is running. pid is " -yellow  "[" -green "$pid" -yellow "]." -reset -n
        cfont -cyan "Do nothing...exit." -reset -n
        exit 0
    fi
    cfont -cyan "Now run the " -yellow  "[" -green "$appname" -yellow "]." -cyan " Please waite..." -reset -n
    nohup ./$appname &>/dev/null &
    sleep 2         #����Ҫ�ȼ���ȳ���������������Ȼȡ��������ID
    apppid=$(pidof $appname)
    cfont -cyan "The " -yellow  "[" -green "$appname" -yellow "]" -cyan " is started successful! The pid is " -yellow "[" -green "$apppid" -yellow "]." -reset -n
}

stop() {
    cfont -cyan "Now will stop the " -yellow "[" -green "$appname" -yellow "]" -cyan " ..."  -reset -n
    pid=$(pidof $appname)
    if [ -z "$pid" ]; then
        cfont -red "The " -yellow "[" -green "$appname" -yellow "]" -red " is not running." -reset -n
        cfont -cyan "Do nothing...exit" -reset -n
        exit
    fi
    cfont -cyan "the " -yellow "[" -green "$appname" -yellow "]" -cyan "is Running. the pid is " -yellow "[" -green "$pid" -yellow "]" -cyan " ..."  -reset -n
    kill -9 $pid
    sleep 2
    pid=$(pidof $appname)
    if [ -z "$pid" ]; then
        cfont -cyan "The " -yellow "[" -green "$appname" -yellow "]"  -cyan " be killed successful ...." -reset -n
    fi
}

view(){
    logname=$(ls -lt $logflag*.log | head -1 | awk '{print $9}')
    cfont -cyan "The logfile is " -yellow "[" -green "$logname" -yellow "]" -cyan ". Now open it..." -reset -n
    if [ -z "$logname" ]; then
        cfont -red "The " -yellow "[" -green "$logname" -yellow "]" -red " don't find  " -reset -n
        cfont -cyan "Do nothing...exit." -reset -n
        exit 0
    fi
    tail -f $logname
    exit 1
}

restart() {
    stop
    start
}    

case "$1" in
  start)
    start
    ;;
  stop)
    stop
    ;;
  restart)
    restart
    ;;
  view)
    view
    ;;
  *)
    echo $"Usage: $0 {start|stop|view|restart}"
    exit 1
esac

exit $?
