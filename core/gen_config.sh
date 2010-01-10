#!/bin/bash

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
     
    echo "daemon_t "$path"[] = {" >> $config_file
    for daemon in $daemons
    do
        add_daemon_struct $daemon
    done
    echo "        }" >> $config_file
    echo "    };" >> $config_file
}

echo "/* This is a generated file. */" > $config_file
echo  >> $config_file

add_header "core_type.h"

for daemon in $daemons
do
    add_header "../"$path"/"$daemon".h"
done

echo >> $config_file
echo >> $config_file
add_daemons
