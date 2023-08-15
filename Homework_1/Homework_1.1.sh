#!/bin/bash

for (( i = 1; i <= 20; i++ ))
do
    curl https://loremflickr.com/800/400 -L > /home/svishneviy/Synergy-oreparting-system-architectures/Homework_1/attachments/photo_$i.png
done