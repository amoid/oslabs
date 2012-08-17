#!/bin/sh
cd /mnt/lindows/work/branch/cliofetion-210/build
rm -rf log.msg
./cliofetion -f 13472798633 -p amoid123456 -u user.dat -r record.dat >log.msg
mail -s "log message" amoid@163.com < log.msg
echo successfully
