#!/bin/sh
mailist=""
i=0
id=0
j=0
lesson="lesson.dat"
while read lessonid
do
while
	true
do

if [ $lessonid -ne $id ]; then
	echo "big $lesson"
	let id=id+1
	continue
else
	echo "less $lesson"
	echo $id
 	break 
fi
done

done<$lesson

let i=0
datatosend="filelist.dat"
while read line
do
if [ $i -eq $id ]; then
	break;
else
	let i=i+1
	continue
fi	
done<$datatosend

let j=0
userlist="user.dat"
while read userline
do
if [ $j -eq 0 ]; then
	maillist="$userline"
else
	maillist="$userline"" ""$maillist"
fi
let j=j+1
done<$userlist

echo send $line to $maillist < $line
mail -s "$line" $maillist < $line
let id=id+1
echo $id > lesson.dat

