#!/bin/bash

# Функция для отображения инструкции по использованию
function show_help() {
    echo "Использование: $0 [-s источник] [-d директория_назначения] [-t время_запуска_в_минутах]"
    echo "Параметры:"
    echo "  -s  Исходная директория для резервного копирования"
    echo "  -d  Директория для сохранения резервных копий"
    echo "  -t  Периодичность в минутах для создания резервных копий"
    exit 1
}

# Получение аргументов командной строки
while getopts ":s:d:t:" opt; do
    case $opt in
        s) source_dir="$OPTARG" ;;
        d) dest_dir="$OPTARG" ;;
        t) interval="$OPTARG" ;;
        *) show_help ;;
    esac
done

# Проверка на наличие обязательных аргументов
if [ -z "$source_dir" ] || [ -z "$dest_dir" ] || [ -z "$interval" ]; then
    show_help
fi

# Создание директории назначения, если она не существует
mkdir -p "$dest_dir"

# Функция для создания резервной копии
backup() {
    timestamp=$(date +%Y%m%d_%H%M%S)
    backup_file="$dest_dir/backup_$timestamp.tar.gz"
    echo "Создание резервной копии $source_dir в $backup_file..."
    tar -czf "$backup_file" -C "$source_dir" .
    echo "Резервное копирование завершено."
}

# Установка периодичности выполнения
while true; do
    backup
    echo "Следующее резервное копирование произойдет через $interval минут."
    sleep "${interval}m"
done
