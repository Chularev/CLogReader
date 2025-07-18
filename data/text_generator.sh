#!/bin/bash

for i in {1..10000}; do
    length=$((1 + RANDOM % 1000))  # Случайная длина строки от 1 до 1000 символов
    head -c $length /dev/urandom | base64 -w 0 >> testfile.txt
    echo >> testfile.txt  # Добавляем перевод строки
done
truncate -s 4M testfile.txt  # Обрезаем или дополняем до 2 МБ
