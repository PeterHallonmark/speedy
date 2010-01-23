#!/bin/bash

# Copyright (c) 2010, Peter Johansson <zeronightfall@gmx.com>
#
# Permission to use, copy, modify, and/or distribute this software for any
# purpose with or without fee is hereby granted, provided that the above
# copyright notice and this permission notice appear in all copies.
#
# THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
# WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
# MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
# ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
# WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
# ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
# OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.


add_header()
{
    header_name=$1
    file_name=$2
    
    printf "#include %s\n" $header_name >> $file_name
}

add_function()
{
    function_type=$1
    daemon_name=$2
    file_name=$3
    path=$4
    end_char=$5
    
    function_name=$daemon_name"_"$function_type
    if grep -q $function_name $path"/"$daemon_name".h" ; then
        printf "%13s%s%s%s%s\n" "." $function_type " = &" $function_name $end_char >> $file_name
    else
        printf "%13s%s%s%s\n" "." $function_type " = NULL" $end_char >> $file_name
    fi
}

add_daemons()
{
    file_name=$1
    shift
    path=$1
    shift
    daemons=$*
        
    first=1
    add_daemon_struct()
    {
        daemon_name=$1
        if [ $first -eq 1 ] 
        then
            first=0
            printf "%10s\n" "{" >> $file_name
        else
            printf "%13s\n" "}, {" >> $file_name
        fi
        add_function "get_name" $daemon_name $file_name $path ","
        add_function "init" $daemon_name $file_name $path ","
        add_function "shutdown" $daemon_name $file_name $path ""
    }
     
    printf "service_t %s[] = {\n" $path >> $file_name
    for daemon in $daemons
    do
        add_daemon_struct $daemon
    done
    if [ $first -eq 0 ] 
    then
        echo "         }" >> $file_name
    fi
    echo "      };" >> $file_name
}

create_file()
{
    file_name=$1
    shift
    path=$1
    shift
    daemons=$*
    
    printf "/* This is a generated file. */\n\n" > $file_name
    add_header "<stdlib.h>" $file_name
    printf "\n"  >> $file_name

    for daemon in $daemons
    do
        add_header "\"../"$path"/"$daemon".h\"" $file_name
    done    

    printf "\n\n" >> $file_name
    add_daemons $file_name $path $daemons
}

get_md5sum()
{
    file=$1
    md5sum=$(md5sum $file 2> /dev/null)
    set -- $md5sum
    echo $1
}

update_file()
{
    old_file=$1
    new_file=$2
    
    md5sum_new=$(get_md5sum $new_file)
    md5sum_old=$(get_md5sum $old_file)
    
    if [ "$md5sum_new" != "$md5sum_old" ]; then
        mv -f $new_file $old_file
    fi
}    
    
main()
{
    config_file=$1
    config_file_tmp=$config_file".tmp"
    shift
    path=$1
    shift
    daemons=$*
    
    create_file $config_file_tmp $path $daemons
    update_file $config_file $config_file_tmp
}

main $*
