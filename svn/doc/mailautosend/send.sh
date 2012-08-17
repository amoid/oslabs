#!/bin/sh
userdata=$1
fileindex=$2
filelist=$3

echo $userdata
echo $fileindex
echo $filelist

#
#get file index to send
fileid=0
declare -a arrary1
arrary1=(`cat "$fileindex"`)
echo ${arrary1[0]}
let fileid=${arrary1[0]}
echo "fileid is $fileid"
#
#get file name to be sent by index
#
filetosend=""
declare -a arrary2
arrary2=(`cat "$filelist"`)
filetosend="${arrary2[$fileid]}"
echo "will send $filetosend to user list"
#
#get user list, sent max num once in bcc list and with amoid@sohu.com in cc list
#
maxcount=19
totusercount=0
count=0
userindex=0
cclist=""
bcclist=""

declare -a arrary3
arrary3=(`cat "$userdata"`)
let totusercount=${#arrary3[*]}
echo "user count is $totusercount"
echo "user index is $userindex"

while [ $userindex -lt $totusercount ]
do
	echo $count $maxcount
	if [ $count -lt $maxcount ];then
		echo $cclist
		echo $bcclist
		bcclist="${arrary3[$userindex]}"";""$bcclist"
		let userindex=userindex+1
		let count=count+1
		if [ $count -eq $maxcount ]; then
			echo "here 1"
			echo "$cclist"
			echo "$bcclist"\
			echo $filetosend 
			cat $filetosend | sendEmail \
                                          -f amoid@sohu.com \
                                          -t "$bcclist" \
                                          -s smtp.sohu.com \
                                          -xu amoid@sohu.com \
                                          -xp 19810519 \
                                          -u "$filetosend"
			let count=0
			bcclist=""
			continue
		fi
	fi
		if [ $userindex -eq $totusercount ]; then
			echo $here 2
			echo "$cclist"
			echo "$bcclist"
			echo $filetosend
			cat "$filetosend" | sendEmail \
                                            -f amoid@sohu.com \
                                            -t "$bcclist" \
                                            -s smtp.sohu.com \
                                            -xu amoid@sohu.com \
                                            -xp 19810519 \
                                            -u "$filetosend"
			break;
		fi
done

let fileid=fileid+1
echo $fileid > $fileindex

