//
// Created by 杨云飞 on 2023/2/26.
//

#ifndef SKIPLIST_SKIPLIST_H
#define SKIPLIST_SKIPLIST_H

#include <cstring>
#include <iostream>
#include <mutex>
#include <fstream>
#include <ctime>
#include "./comparator.h"
#define STORE_FILE "store/dumpFile"

std::string delimiter = ":";

namespace MySkipList{
    // 节点的构造
    template<class typeK, class typeV>
    class Node {
    private:
        typeK key;
        typeV value;
    public:
        int nodeLevelHeight;                // 该节点在跳表中的层高，也就是有几级索引，从0开始计数的
        Node<typeK, typeV> **forward;       // 这是个指针数组，存放的是该节点后面的节点的指针，下标表示是哪一层

    public:
        Node() {};
        Node(typeK k, typeV v, int level);
        ~Node();
        typeK getKey() const;
        typeV getValue() const;
        int nodeCompare(const Node &n);
        int nodeCompare(const typeK k);
    };

    template<class typeK, class typeV>
    Node<typeK, typeV>::Node(typeK k, typeV v, int level) {
        nodeLevelHeight = level;
        key = k;
        value = v;
        forward = new Node<typeK, typeV> *[nodeLevelHeight + 1];
        memset(forward, 0, sizeof(Node<typeK, typeV> *) * (nodeLevelHeight + 1));
    }

    template <class typeK, class typeV>
    Node<typeK, typeV>::~Node() {
        delete []forward;
    }

    template <class typeK, class typeV>
    typeK Node<typeK, typeV>::getKey() const {
        return key;
    }

    template <class typeK, class typeV>
    typeV Node<typeK, typeV>::getValue() const {
        return value;
    }

    template<class typeK, class typeV>
    int Node<typeK, typeV>::nodeCompare(const typeK k) {
        return compareKey(key, k);
    }

    template<class typeK, class typeV>
    int Node<typeK, typeV>::nodeCompare(const Node<typeK, typeV> &node) {
        return compareKey(key, node.key);
    }

    // 跳表类的构造
    template <class typeK, class typeV>
    class SkipList{
    private:
        int maxLevel;                   // 跳表的最大高度
        int curLevel;                   // 跳表的实际高度
        Node<typeK, typeV> *head;       // 跳表头节点
        int elementNum;                 // 跳表中元素个数

        std::ofstream fileWriter;       // 操作写文件
        std::ifstream fileReader;       // 操作读文件

        std::mutex mtx;             // 互斥锁支持并发

    public:
        SkipList(int maxListLevel);
        ~SkipList();
        int getRandomLevel();

        Node<typeK, typeV> * searchElement(typeK k);
        bool insertElement(typeK k, typeV v);
        bool deleteElement(typeK k);
        void displaySkipList();
        int size();

        void dumpFile();
        void loadFile();                // 从文件中获取key和value

    private:
        void getKeyValueFromString(const std::string& str, std::string *key, std::string *value);
        bool isValidString(const std::string &str);
    };

    template <class typeK, class typeV>
    SkipList<typeK, typeV>::SkipList(int maxListLevel) {
        maxLevel = maxListLevel;
        curLevel = 0;
        elementNum = 0;
        typeK k;
        typeV v;
        head = new Node<typeK, typeV>(k, v, maxLevel);
    }

    template <class typeK, class typeV>
    SkipList<typeK, typeV>::~SkipList() {
        if (fileReader.is_open()){
            fileReader.close();
        }
        if (fileWriter.is_open()){
            fileWriter.close();
        }
        delete head;
    }

    template <class typeK, class typeV>
    int SkipList<typeK, typeV>::size() {
        return elementNum;
    }

    template <class typeK, class typeV>
    int SkipList<typeK, typeV>::getRandomLevel() {
        int k = 0;                                  // 从0开始还是1？？
        while (rand() % 2) k++;
        k = (k < maxLevel) ? k : maxLevel;
        return k;
    }

    template <class typeK, class typeV>
    void SkipList<typeK, typeV>::displaySkipList() {
        std::cout << "\n***********Skip List**********\n";
        for (int i = 0; i <= curLevel; i++){
            Node<typeK, typeV> *temp = head->forward[i];
            std::cout << "Level-" << i << std::endl;
            while(temp){
                std::cout << temp->getKey() << ":" << temp->getValue() << "; ";
                temp = temp->forward[i];
            }
            std::cout << std::endl;
        }
    }

    template<class typeK, class typeV>
    Node<typeK, typeV> * SkipList<typeK, typeV>::searchElement(typeK key) {
        if (head->forward[0] == nullptr) return nullptr;
        Node<typeK, typeV> *current = head;              // 由于插入的时候会补高度，所以这个不会为null
        for (int i = curLevel; i >= 0; i--){
            while(current->forward[i] && current->forward[i]->nodeCompare(key) < 0){
                current = current->forward[i];
            }
            if (current->forward[i] && current->forward[i]->nodeCompare(key) == 0) return current->forward[i];
        }

        current = current->forward[0];
        if (current== nullptr || current->nodeCompare(key) != 0) return nullptr;
        else return current;
    }

    template <class typeK, class typeV>
    bool SkipList<typeK, typeV>::insertElement(typeK k, typeV v) {
            mtx.lock();
            Node<typeK, typeV> *current = head;
            Node<typeK, typeV> *needUpdate[maxLevel + 1];
            memset(needUpdate, 0, sizeof(Node<typeK, typeV> *) * (maxLevel + 1));

            for (int i = curLevel; i >= 0; i--){
                while(current->forward[i] && current->forward[i]->nodeCompare(k) < 0){
                    current = current->forward[i];
                }
                needUpdate[i] = current;
            }
            current = current->forward[0];
            if (current && current->nodeCompare(k) == 0){
    //            std::cout << "The key already exists!" << std::endl;
                mtx.unlock();
                return false;
            }else{
                // current为nullptr或者其key值比要插入的k的大
                int insertedNodeLevel = getRandomLevel();
                if (head->forward[0] == nullptr) insertedNodeLevel = 0;

                if (insertedNodeLevel > curLevel){
                    for(int i = curLevel + 1; i <= insertedNodeLevel; i++){
                        needUpdate[i] = head;
                        // 这是插到head后面的情况
                    }
                    curLevel = insertedNodeLevel;
                }
                Node<typeK, typeV> *temp = new Node<typeK, typeV>(k, v, insertedNodeLevel);

                for (int i = 0; i <= insertedNodeLevel; i++){
                    temp->forward[i] = needUpdate[i]->forward[i];
                    needUpdate[i]->forward[i] = temp;
                }
                elementNum++;
    //            std::cout << "Insert Successfully!" << std::endl;
            }
            mtx.unlock();
            return true;
        }

    template <class typeK, class typeV>
    bool SkipList<typeK, typeV>::deleteElement(typeK k) {
        mtx.lock();
        if (head->forward[0] == nullptr){
            std::cout << "The SkipList is empty!" << std::endl;
            return false;
        }

        Node<typeK, typeV> *needUpdate[maxLevel + 1];
        memset(needUpdate, 0, sizeof(Node<typeK, typeV> *) *(maxLevel + 1));
        Node<typeK, typeV> *current = head;

        for (int i = curLevel; i >= 0; i--){
            while(current->forward[i] && current->forward[i]->getKey() < k){
                current = current->forward[i];
            }
            needUpdate[i] = current;
        }

        // 找到current的下一个节点，判断其key和所要删除的k的关系
        current = current->forward[0];
        // 第一种下一个节点不存在或者是key大于k，那么说明需要删除的k在跳表中不存在
        // 第二种下一个节点的key等于k，那么就需要删除k的节点，并更新needUpdate里面的节点的forward
        if (current == nullptr || current->nodeCompare(k) > 0){
            std::cout << "The key need to delete dose not exist!" << std::endl;
            mtx.unlock();
            return false;
        }else{
            // 也就是节点存在并且其key和所要删除的k的值是一样的
            for(int i = 0; i <= curLevel; i++){
                // needUpdate里面的也不全部是需要修改的，如果其后面的那个节点根本就不是current，就不需要删除
                if (needUpdate[i]->forward[i] != current) break;
                // 这里break的原因是如果某一层的后一个节点不是current了，那么更高层的节点的下一个节点就不可能是current了
                needUpdate[i]->forward[i] = current->forward[i];
            }

            // 删除了之后，可能某一层上面没有节点了，需要把那一层删除
            while (curLevel > 0 && head->forward[curLevel] == nullptr){
                current--;
            }
            std::cout << "The node with key: " << k << " has been deleted!" << std::endl;
            elementNum--;
        }
        mtx.unlock();
        return true;
    }

    template<class typeK, class typeV>
    bool SkipList<typeK, typeV>::isValidString(const std::string &str) {
        if (str.empty()) return false;
        if (str.find(delimiter) == std::string::npos) return false;
        return true;
    }

    template<class typeK, class typeV>
    void SkipList<typeK, typeV>::getKeyValueFromString(const std::string &str, std::string *key, std::string *value) {
        if (!isValidString(str)) return;
        // 从字符串中分割出key和value
        *key = str.substr(0, str.find(delimiter));
        *value = str.substr(str.find(delimiter)+1, str.length());
    }

    template<class typeK, class typeV>
    void SkipList<typeK, typeV>::dumpFile() {
        // 将跳表中的数据写到文件中
        std::cout << "**********Dump File************" << std::endl;
        fileWriter.open(STORE_FILE);
        Node<typeK, typeV> *node = head->forward[0];

        while(node){
            fileWriter << node->getKey() << ": " << node->getValue() << std::endl;
            node = node->forward[0];
        }

        fileWriter.flush();
        fileWriter.close();
    }

    template<class typeK, class typeV>
    void SkipList<typeK, typeV>::loadFile() {
        fileReader.open(STORE_FILE);
        std::cout << "**********Load File**********" << std::endl;
        std::string line;
        std::string *key = new std::string();
        std::string *value = new std::string();

        while(getline(fileReader, line)){
            getKeyValueFromString(line, key, value);
            if (key->empty()) continue;
            insertElement(*key, *value);
            std::cout << "key: " << *key << "; value: " << *value << std::endl;
        }
        fileWriter.close();
    }
}

#endif //SKIPLIST_SKIPLIST_H
