# Симуляция жд на C++ через потоки

Задача была решена в рамках следующего задания:
Взять одну из жд веток Таджикистана, например, Душанбе-Пахтаабад или Душанбе-Куляб или какие-то маршруты около Худжанда.
Написать Си++ программу, соответствующую следующим условиям:
- Поезда стартуют из различных пунктов по своему маршруту. 
- Каждый поезд - это поток, у него должен быть уникальный номер. Минимальное количество поездов - 8 (то есть 8 потоков)
- Одновременно на станции должен быть только один поезд (то есть это ресурс, который блокируется). Другой поезд в это время не может заехать и ждёт очереди
- Все события (отбытия-прибытия поездов) должны логироваться в консоль с использованием текущего времени
- Перемещение между станциями не должно быть мгновенным. Назначьте разумные временные промежутки в течение которых поезд проезжает от одной станции к другой

В рамках данного проекта была взята ветка Душанбе-Куляб, состоящая из следующих станций:
**Куляб, Дангара, Сангтуда, Бохтар, Вахш, Яван, Янги-Базар, Рохати, Душанбе**


Каждая станция имеет 2 платформы
Назначение платформ зависит от направления поезда:
- Платформа #1: Душанбе → Куляб
- Платформа #2: Куляб → Душанбе

Ниже ссылки на интересные источники, которыми я вдохновлялась (либо подсматривала код, но советую просмотреть их для ясности):
- [Habr: Многопоточность в C++](https://habr.com/ru/articles/182610/)
- [Habr: Семафоры и синхронизация](https://habr.com/ru/articles/182626/)
- [Как в c++ использовать chrono](https://blog.csdn.net/Kernel_Heart/article/details/129436679)


В результаты, мы получаем, что наш проект моделирует железнодорожную систему, где несколько поездов перемещаются между станциями. Каждый поезд представлен отдельным потоком и взаимодействует с общими ресурсами, такими как платформы на станциях.
Основные особенности:
1. Многопоточность с использованием thread;
2. Синхронизация платформ с помощью counting_semaphore;
3. Безопасный вывод в консоль через mutex;
4. Логирование времени прибытия, отправления и использования платформ;
5. Случайные интервалы ожидания для реалистичного поведения поездов;
6. Симуляция работает непрерывно, поезда движутся туда и обратно между станциями.


## Результаты работы ##

```_______________________________$$$$$
 $$$$$$$$$$$$$$________$$$______$$$$$
 _$$$$$$$$$$$$$_______$$$$$______$$$
 __$$____$___$$_______$$$$$______$$$__$$
 __$$____$___$$$$$$$$$$$$$$$$$$$$$$$$$$$$
 __$$$$$$$$$$$$_$$$$$_$$$$$$$$$_$$$$$$_$$$
 __$$$$$$$$$$$$_$$$$$__$$$$$$$$__$$$$$$_$$
 __$$$$$$$$$$$$_$$$$$$_$$$$$$$$$_$$$$$$_$
 ___$$$$$$$$$$$_$$$$$__$$$$$$$$__$$$$$_$$
 ___$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
 _____$__$$$$$___$__$$$$$$___$$__$$$$__$$$$
 ______$$$$$$_____$$$$$$$_____$$$$$
Train #1 starts at Kulyab at16:49:57
Train #2 starts at Dangara at16:49:57
Train #1 is coming to Kulyab (platform #2) at 16:49:57
Train #3 starts at Sangtuda at16:49:57
Train #3 is coming to Sangtuda (platform #2) at 16:49:57
Train #2 is coming to Dangara (platform #2) at 16:49:57
Train #5 starts at Vakhsh at16:49:57
Train #5 is coming to Vakhsh (platform #2) at 16:49:57
Train #7 starts at Yangi-Bazar at16:49:57
Train #8 starts at Rohaty at16:49:57
Train #8 is coming to Rohaty (platform #2) at 16:49:57
Train #6 starts at Yavan at16:49:57
Train #7 is coming to Yangi-Bazar (platform #2) at 16:49:57
Train #4 starts at Bokhtar at16:49:57
Train #6 is coming to Yavan (platform #2) at 16:49:57
Train #4 is coming to Bokhtar (platform #2) at 16:49:57
Train #2 arrived at Dangara (platform #2) at 16:49:58
Train #3 arrived at Sangtuda (platform #2) at 16:49:58
Train #1 arrived at Kulyab (platform #2) at 16:49:58
Train #8 arrived at Rohaty (platform #2) at 16:49:58
Train #5 arrived at Vakhsh (platform #2) at 16:49:58
Train #7 arrived at Yangi-Bazar (platform #2) at 16:49:58
Train #6 arrived at Yavan (platform #2) at 16:49:58
Train #4 arrived at Bokhtar (platform #2) at 16:49:58
Train #3 departed from Sangtuda (platform #2) at 16:50:01
Train #3 is coming to Bokhtar (platform #2) at 16:50:01
Train #1 departed from Kulyab (platform #2) at 16:50:01
Train #1 is coming to Dangara (platform #2) at 16:50:01
Train #2 departed from Dangara (platform #2) at 16:50:01
Train #2 is coming to Sangtuda (platform #2) at 16:50:01
```
Другой "кусок", где фигурирет платформа №1:
```Train #1 arrived at Sangtuda (platform #2) at 16:50:08
Train #3 arrived at Vakhsh (platform #2) at 16:50:08
Train #5 arrived at Yangi-Bazar (platform #2) at 16:50:08
Train #8 arrived at Rohaty (platform #1) at 16:50:08
Train #6 arrived at Rohaty (platform #2) at 16:50:08
Train #7 arrived at Dushanbe (platform #1) at 16:50:08
Train #4 arrived at Yavan (platform #2) at 16:50:08
Train #2 departed from Bokhtar (platform #2) at 16:50:12
Train #2 is coming to Vakhsh (platform #2) at 16:50:12
Train #1 departed from Sangtuda (platform #2) at 16:50:12
Train #1 is coming to Bokhtar (platform #2) at 16:50:12
Train #3 departed from Vakhsh (platform #2) at 16:50:12
Train #3 is coming to Yavan (platform #2) at 16:50:12
Train #5 departed from Yangi-Bazar (platform #2) at 16:50:12
Train #8 departed from Rohaty (platform #1) at 16:50:12
Train #8 is coming to Yangi-Bazar (platform #1) at 16:50:12
Train #5 is coming to Rohaty (platform #2) at 16:50:12
Train #6 departed from Rohaty (platform #2) at 16:50:12
....
Train #7 departed from Bokhtar (platform #2) at 16:55:13
Train #7 is coming to Vakhsh (platform #2) at 16:55:13
Train #6 is coming to Bokhtar (platform #2) at 16:55:13
Train #4 departed from Kulyab (platform #2) at 16:55:13
Train #4 is coming to Dangara (platform #2) at 16:55:13
Train #3 departed from Dangara (platform #1) at 16:55:13
Train #3 is coming to Kulyab (platform #2) at 16:55:13
Train #2 departed from Sangtuda (platform #1) at 16:55:13
Train #2 is coming to Dangara (platform #1) at 16:55:13
Train #1 departed from Bokhtar (platform #1) at 16:55:13
Train #1 is coming to Sangtuda (platform #1) at 16:55:13
Train #8 arrived at Yavan (platform #2) at 16:55:15
Train #5 arrived at Sangtuda (platform #2) at 16:55:15
Train #7 arrived at Vakhsh (platform #2) at 16:55:15
```
