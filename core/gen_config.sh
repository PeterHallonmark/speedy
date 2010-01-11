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

config_file=$1
shift

path=$1
shift

daemons=$*

add_header()
{
    echo "#include \""$1"\"" >> $config_file
}


add_daemons()
{
    first=1
    add_daemon_struct()
    {
        daemon_name=$1
        if [ $first -eq 1 ] 
        then
            first=0
            echo "        {" >> $config_file	
        else
            echo "        }, {" >> $config_file	
        fi
        echo "            .init = &"$daemon_name"_init," >> $config_file
        echo "            .shutdown = &"$daemon_name"_shutdown" >> $config_file
    }
     
    echo "service_t "$path"[] = {" >> $config_file
    for daemon in $daemons
    do
        add_daemon_struct $daemon
    done
    if [ $first -eq 0 ] 
    then
        echo "        }" >> $config_file
    fi
    echo "    };" >> $config_file
}

echo "/* This is a generated file. */" > $config_file
echo  >> $config_file

for daemon in $daemons
do
    add_header "../"$path"/"$daemon".h"
done

echo >> $config_file
echo >> $config_file
add_daemons
