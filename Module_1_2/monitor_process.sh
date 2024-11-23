#!/bin/bash

# Запрос имени процесса
read -p "Введите имя процесса для мониторинга: " process_name

# Запрос периода проверки в секундах
read -p "Введите периодичность проверки (в секундах): " interval

# Функция для перезапуска процесса
restart_process() {
    echo "Процесс $process_name не работает. Перезапуск..."
    # В этом примере предполагается, что процесс можно запустить по его имени
    systemctl start $process_name
}

# Основной цикл для проверки процесса
while true; do
    # Проверяем, запущен ли процесс
    if ! pgrep -x "$process_name" > /dev/null; then
        restart_process
    else
        echo "Процесс $process_name работает."
    fi

    # Ждем указанный интервал
    sleep $interval
done
