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
                printf "***$i\n"
            fi
        fi
    done
}

function main
{
    do_convert $1 $2
}


main $1 $2;