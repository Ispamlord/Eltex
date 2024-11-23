#!/bin/bash

# Запрос параметров у пользователя
read -p "Введите базовую директорию: " base_dir
read -p "Введите количество папок: " num_folders
read -p "Введите количество подпапок: " num_subfolders
read -p "Введите количество файлов: " num_files
read -p "Введите префикс для папок: " folder_prefix
read -p "Введите префикс для подпапок: " subfolder_prefix
read -p "Введите префикс для файлов: " file_prefix
read -p "Введите расширение для файлов (например, .txt): " file_extension

# Проверка и создание базовой директории, если она не существует
if [ ! -d "$base_dir" ]; then
    mkdir -p "$base_dir"
    echo "Создана базовая директория: $base_dir"
fi

# Основной цикл по созданию папок, подпапок и файлов
for ((j=1; j<=num_folders; j++))
do
    folder_name="${folder_prefix}${j}"
    mkdir -p "$base_dir/$folder_name"

    for ((f=1; f<=num_subfolders; f++))
    do
        subfolder_name="${subfolder_prefix}${f}"
        mkdir -p "$base_dir/$folder_name/$subfolder_name"

        for ((i=1; i<=num_files; i++))
        do
            file_name="${file_prefix}${i}${file_extension}"
            touch "$base_dir/$folder_name/$subfolder_name/$file_name"
        done
    done
done

echo "Создание структуры директорий завершено!"
