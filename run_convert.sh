#! /bin/sh


function do_convert
{
    origin_dir=$1                       #源目录
    destination_dir=$2                  #目标目录
    if ! [ -d  $destination_dir ]       
    then
        mkdir $destination_dir
    fi
    for i in `ls $origin_dir`
    do
        if [ ${i:(-4)} = ".bmp" ]
        then
            printf "$i\n"
            ./bmp2mcp $origin_dir"/"$i > $destination_dir"/"$i".h"
        else
            if [ -d $origin_dir"/"$i ]
            then
                printf "$i is not bmp format\n"
            fi
        fi
    done
}

function main
{
    if test -n $1 
    then 
        param=$1
    fi

    if [ $# -lt 1 ]
    then
        echo "$0 -h"
        exit 0
    elif [ ${param} == "-h" ] 
    then
        echo "$0 <源目录 目标目录>"
        echo "$0 origin_dir destination_dir"
        echo "源目录需要有bmp文件用于转换"
        exit 0
    elif [ $# -gt 1 ]
    then
        if test -d $1
        then
        do_convert $1 $2
        fi
    else
        echo "输入参数不正确，请参考：$0 -h"
        exit -1
    fi
}


main $1 $2;