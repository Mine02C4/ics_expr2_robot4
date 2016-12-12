#!/bin/bash

#echo こんにちは | open_jtalk \
#-x /var/lib/mecab/dic/open-jtalk/naist-jdic \
#-m ~/work/voice/mei/mei_normal.htsvoice \
#-ow ~/open_jtalk.wav \
#aplay ~/open_jtalk.wav 
#rm -f ~/open_jtalk.wav


#global variables
dict=/var/lib/mecab/dic/open-jtalk/naist-jdic
num=1
function talk(){
    local voicefile=~/mei/mei_normal.htsvoice
    local outfile=~/open_jtalk.wav
    local input=$1
    makeAndPlay $voicefile $outfile $input
}
function makeAndPlay(){
    echo $1 $2 $3
    echo $3 | open_jtalk -x $dict -m $1 -ow $2
    if [ -f $2 ]; then
	aplay -D plughw:0,0 $2
	rm -f $2
    fi
}
    input=$1
    talk $input
