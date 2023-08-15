#!/bin/bash

path=/home/svishneviy/Synergy-oreparting-system-architectures/Bash/attachments
for (( i = 1; i <= 20; i++ ))
do
    curl https://loremflickr.com/800/400 -L > $path/photo_$i.png
done
echo "Загрузка завершена."