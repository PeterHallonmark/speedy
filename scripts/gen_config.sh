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

function_path=`echo "$0" | sed 's/gen_config.sh/functions.sh/g'`
. $function_path

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
    filename=$3
    path=$4
    end_char=$5
    
    function_name=$daemon_name"_"$function_type
    if grep -q $function_name $path"/"$daemon_name".h" ; then
        printf "%13s%s%s%s%s\n" "." $function_type " = &" $function_name $end_char >> $filename
    else
        printf "%13s%s%s%s\n" "." $function_type " = NULL" $end_char >> $filename
    fi
}

add_daemons()
{
    filename=$1
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
            printf "%10s\n" "{" >> $filename
        else
            printf "%13s\n" "}, {" >> $filename
        fi
        add_function "get_name" $daemon_name $filename $path ","
        add_function "provides" $daemon_name $filename $path ","
        add_function "get_dependency" $daemon_name $filename $path ","
        add_function "initialization" $daemon_name $filename $path ","
        add_function "shutdown" $daemon_name $filename $path ""
    }
     
    printf "service_t %s[] = {\n" $path >> $filename
    for daemon in $daemons
    do
        add_daemon_struct $daemon
    done
    if [ $first -eq 0 ] 
    then
        echo "         }" >> $filename
    fi
    echo "      };" >> $filename
}

create_file()
{
    filename=$1
    shift
    path=$1
    shift
    daemons=$*
    
    printf "/* This is a generated file. */\n\n" > $filename
    add_header "<stdlib.h>" $filename
    printf "\n"  >> $filename

    for daemon in $daemons
    do
        add_header "\"../"$path"/"$daemon".h\"" $filename
    done    

    printf "\n\n" >> $filename
    add_daemons $filename $path $daemons
} 
 
main()
{
    remove_file=$1
    shift
    config_file=$1
    config_file_tmp=$config_file".tmp"
    shift
    path=$1
    shift
    daemons=$*
    
    create_file $config_file_tmp $path $daemons
    update_file $config_file_tmp $config_file $remove_file
}

main $*
