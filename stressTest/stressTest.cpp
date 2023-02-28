//
// Created by 杨云飞 on 2023/2/27.
//

#include <iostream>
#include <chrono>
#include <cstdlib>
#include <vector>
#include "../skipList.h"

#define TEST_COUNT 1000000

void display(std::vector<double> &vec, const char *str){
    std::cout << str << std::endl;
    int i = 0;
    while(i < vec.size() - 1){
        std::cout << "Round " << i + 1 << "\t\t";
        i++;
    }
    std::cout << "Average" << std::endl;
    for (int i = 0; i < vec.size(); i++){
        std::cout << vec[i] << "\t\t";
    }
    std::cout << std::endl;
}

std::string randStr(const int len){
    std::string str;
    for (int i = 0; i < len; i++){
        char c = 'a' + rand()%26;
        str.push_back(c);
    }
    return str;
}

int main(){
    MySkipList::SkipList<std::string, std::string> skipList(18);
    std::vector<double> strInsertTimes;
    std::vector<double> strSearchTimes;
    std::vector<double> intInsertTimes;
    std::vector<double> intSearchTimes;
    double strInsertSum = 0;
    double strSearchSum = 0;
    double intInsertSum = 0;
    double intSearchSum = 0;

    for (int i = 0; i < 5; i++) {
        srand(time(0));
        {
            auto start = std::chrono::high_resolution_clock::now();
            for (int i = 0; i < TEST_COUNT; i++) {
                skipList.insertElement(randStr(rand() % 20 + 1), randStr(rand() % 5 + 1));
            }
            auto finish = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> elapsed = finish - start;
            strInsertTimes.push_back(elapsed.count());
            strInsertSum += elapsed.count();
            std::cout << "insert elapsed: " << elapsed.count() << std::endl;

            //        skipList.dumpFile();

            start = std::chrono::high_resolution_clock::now();
            for (int i = 0; i < TEST_COUNT; i++) {
                skipList.searchElement(randStr(rand() % 20 + 1));
            }
            finish = std::chrono::high_resolution_clock::now();
            elapsed = finish - start;
            strSearchTimes.push_back(elapsed.count());
            strSearchSum += elapsed.count();
            std::cout << "get elapsed: " << elapsed.count() << std::endl;

            //        skipList.loadFile();
        }

        MySkipList::SkipList<int, std::string> skipList2(18);
        {
            auto start = std::chrono::high_resolution_clock::now();
            for (int i = 0; i < TEST_COUNT; i++) {
                skipList2.insertElement(rand() % TEST_COUNT, randStr(rand() % 5 + 1));
            }
            auto finish = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> elapsed = finish - start;
            intInsertTimes.push_back(elapsed.count());
            intInsertSum += elapsed.count();
            std::cout << "insert elapsed: " << elapsed.count() << std::endl;

            start = std::chrono::high_resolution_clock::now();
            for (int i = 0; i < TEST_COUNT; i++) {
                skipList2.searchElement(rand() % TEST_COUNT);
            }
            finish = std::chrono::high_resolution_clock::now();
            elapsed = finish - start;
            intSearchTimes.push_back(elapsed.count());
            intSearchSum += elapsed.count();
            std::cout << "get elapsed: " << elapsed.count() << std::endl;
        }
    }

    strInsertTimes.push_back(strInsertSum/strInsertTimes.size());
    strSearchTimes.push_back(strSearchSum/strSearchTimes.size());

    intInsertTimes.push_back(intInsertSum/intInsertTimes.size());
    intSearchTimes.push_back(intSearchSum/intSearchTimes.size());

    // 输出结果
    display(strInsertTimes, "**********String Insert**********");
    display(strSearchTimes, "**********String Search**********");
    display(intInsertTimes, "**********Int Insert**********");
    display(intSearchTimes, "**********Int Search**********");

    return 0;
}