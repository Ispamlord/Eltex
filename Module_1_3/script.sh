#!/bin/bash
for ((i=1;i<4;i++))
    do
    if [ $i -eq 1 ];then
        touch file$i
        tar -cf archive.tar file$i
        tar -tf archive.tar
        echo "-----"
    else
        touch file$i
        tar -rf archive.tar file$i
        tar -tf archive.tar
        echo "----"
    fi
done
tar --delete --file archive.tar file2
tar -tf archive.tar
echo "----"
mkdir folder_for_archive
tar -xf archive.tar -C folder_for_archive
