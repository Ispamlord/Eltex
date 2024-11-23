#!/bin/bash

cut -d "|-" -f 2 logs.txt > temp.txt

read -p "Введите имя процесса для поиска в логах: " process_name
read -p "Введите дату начала (в формате ГГГГ-ММ-ДД ЧЧ:ММ:СС): " start_time
read -p "Введите дату окончания (в формате ГГГГ-ММ-ДД ЧЧ:ММ:СС): " end_time

journalctl --since "$start_time" --until "$end_time" | grep $process_name
