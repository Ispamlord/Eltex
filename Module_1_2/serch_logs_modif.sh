#!/bin/bash

# Чтение конфигурационного файла
CONFIG_FILE="config.ini"

if [ ! -f "$CONFIG_FILE" ]; then
    echo "Ошибка: Конфигурационный файл '$CONFIG_FILE' не найден!"
    exit 1
fi

# Извлечение значений из конфигурационного файла
process_name=$(awk -F' = ' '/process_name/ {print $2}' "$CONFIG_FILE")
start_time=$(awk -F' = ' '/start_time/ {print $2}' "$CONFIG_FILE")
end_time=$(awk -F' = ' '/end_time/ {print $2}' "$CONFIG_FILE")

# Проверка, что все параметры заданы
if [ -z "$process_name" ] || [ -z "$start_time" ] || [ -z "$end_time" ]; then
    echo "Ошибка: Пожалуйста, убедитесь, что в конфигурационном файле заданы все параметры."
    exit 1
fi

# Запуск поиска с использованием journalctl
journalctl --since "$start_time" --until "$end_time" | grep "$process_name"
