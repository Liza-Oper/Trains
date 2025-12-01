#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <unordered_map>
#include <chrono>
#include <string>
#include <sstream>
#include <iomanip>
#include <semaphore>

std::mutex printL;
std::mutex dataL;
std::unordered_map<std::string, std::unique_ptr<std::counting_semaphore<2>>> st_acs;


std::string clock_tim()
{
    using namespace std::chrono;
    auto t = system_clock::to_time_t(system_clock::now());
    std::tm pc_time = *std::localtime(&t);
    std::ostringstream os;
    os << std::put_time(&pc_time, "%H:%M:%S");
    return os.str();
}

void waitMs(int b, int n)
{
    const int d = b + (rand() % n);
    std::this_thread::sleep_for(std::chrono::milliseconds(d));
}

void station_g(const std::string& st, int tr_num, int plt)
{
    std::counting_semaphore<2>* sem;

    {
        std::lock_guard<std::mutex> lock(dataL);
        if (!st_acs.count(st))
        { st_acs[st] = std::make_unique<std::counting_semaphore<2>>(2); }
        sem = st_acs[st].get(); }
    sem->acquire();

    {
        std::lock_guard<std::mutex> lock(printL);
        std::cout << "Train #" << tr_num<< " is coming to " << st<< " (platform #" << plt << ") at "  << clock_tim() << std::endl; }

    waitMs(800, 1200);
    {
        std::lock_guard<std::mutex> lock(printL);
        std::cout << "Train #" << tr_num << " arrived at " << st<< " (platform #" << plt << ") at "  << clock_tim() <<std::endl;
    }

    waitMs(3000, 1500);
    {
        std::lock_guard<std::mutex> lock(printL);
        std::cout << "Train #" << tr_num  << " departed from " << st<<" (platform #" << plt << ") at "<< clock_tim() <<std::endl;
    }  sem->release();
}

int main()
{
    std::cout<<"_______________________________$$$$$\n $$$$$$$$$$$$$$________$$$______$$$$$\n _$$$$$$$$$$$$$_______$$$$$______$$$\n __$$____$___$$_______$$$$$______$$$__$$\n __$$____$___$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n __$$$$$$$$$$$$_$$$$$_$$$$$$$$$_$$$$$$_$$$\n __$$$$$$$$$$$$_$$$$$__$$$$$$$$__$$$$$$_$$\n __$$$$$$$$$$$$_$$$$$$_$$$$$$$$$_$$$$$$_$\n ___$$$$$$$$$$$_$$$$$__$$$$$$$$__$$$$$_$$\n ___$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n _____$__$$$$$___$__$$$$$$___$$__$$$$__$$$$\n ______$$$$$$_____$$$$$$$_____$$$$$\n";

    srand(time(NULL));
    std::vector<std::string> stations = {
        "Kulyab", "Dangara", "Sangtuda", "Bokhtar",
        "Vakhsh", "Yavan", "Yangi-Bazar", "Rohaty", "Dushanbe"
    };

    const int N= 8;
    std::vector<std::thread> trains;
    trains.reserve(N);

    for (int i = 0; i < N; ++i)
    {
        int start = i % stations.size();
        int train_num = i + 1;
        trains.emplace_back([start, train_num, &stations]()
        {   int i = start; int dir = 1;
            // 1 Klb-Dsh, -1 Dsh-Klb

            {
                std::lock_guard<std::mutex> lock(printL);
                std::cout << "Train #"<< train_num<<" starts at "<<stations[i]<<" at"<<clock_tim()<<std::endl;
            }

            while (true) {
                int plt = (dir == 1 ? 2 : 1);
                station_g(stations[i], train_num, plt);
                i += dir;
                if (i== (int)stations.size() - 1) dir = -1;
                else if (i == 0) dir = 1;
            }});
    }
    std::this_thread::sleep_for(std::chrono::hours(24));



    return 0;
}
