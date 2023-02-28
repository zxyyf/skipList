//#include <iostream>
#include <time.h>
#include "skipList.h"
#include "comparator.h"
using namespace std;
/*
 * 优化点：第一个插入第一个元素的时候限制其高度为0；
 * 将比较方法抽象化，可以使用多种类型的key，可以使用stl里面的traits思想
 */

int main() {
    srand(time(0));
//    struct test{
//    private:
//        int a;
//    public:
//        test(int n): a(n){};
//        ~test() {}
//        int operator< (test t){
//            if (a < t.a) return 1;
//            else return 0;
//        }
//
//        int operator == (test t){
//            if (a == t.a) return 1;
//            else return 0;
//        }
//    };
//
//    string s1 = "abc";
//    string s2 = "ab";
//    string s3 = "ab";
//
//    cout << MySkipList::compareKey(s1, s2) << endl;
//    cout << MySkipList::compareKey(s2, s3) << endl;
//
//    const char *c1 = "abc";
//    const char *c2 = "abd";
//    char c3[4] = "aaa";
//    char c4[4] = "aaa";
//
//    cout << MySkipList::compareKey(c1, c2) << endl;
//    cout << MySkipList::compareKey(c3, c4) << endl;
//
//    test t1(2);
//    test t2(3);
//    cout << MySkipList::compareKey(t1, t2) << endl;
//
//    int a = 0;
//    int b = 0;
//    int c = 2;
//    cout << MySkipList::compareKey(a, b) << endl;
//    cout << MySkipList::compareKey(a, c) << endl;
//    MySkipList::Node<string, int> n1("ab", 1, 2);
//    MySkipList::Node<string, int> n2("ac", 1, 2);
//    MySkipList::Node<string, int> n3("ab", 1, 2);
//    cout << n1.nodeCompare(n2) << endl;
//    cout << n2.nodeCompare(n3) << endl;
//    cout << n1.nodeCompare(n3) << endl;

    MySkipList::SkipList<string , string> skipList(4);
    skipList.insertElement("aaa", "a");
    skipList.insertElement("aab", "b");
    skipList.insertElement("hbf", "d");
    skipList.insertElement("abc", "e");

    skipList.displaySkipList();

    skipList.deleteElement("abc");
    skipList.displaySkipList();

    skipList.deleteElement("uhd");
    return 0;
}
