//
// Created by 杨云飞 on 2023/2/26.
//

#ifndef SKIPLIST_COMPARATOR_H
#define SKIPLIST_COMPARATOR_H
#include "./typeTraits.h"
#include "cstring"
#include "string"

namespace MySkipList{
    template <class typeKey>
    int _isPODCompare(typeKey key1, typeKey key2, _true_type){
        return strcmp(key1, key2);
    }

    template <class typeKey>
    int _isPODCompare(typeKey key1, typeKey key2, _false_type){
        return key1.compare(key2);
    }

    template <class typeKey>
    int _compare(typeKey key1, typeKey key2, _true_type){
        typedef typename _type_traits<typeKey>::is_POD_type is_POD_type;
        int temp = _isPODCompare(key1, key2, is_POD_type());
        return temp == 0 ? 0 : (temp > 0 ? 1 : -1);
    }

    template <class typeKey>
    int _compare(typeKey key1, typeKey key2, _false_type){
        if (key1 == key2) return 0;
        else if (key1 < key2) return -1;
        else return 1;
    }

    template <class typeKey>
    int compareKey(typeKey key1, typeKey key2){
        typedef typename _type_traits<typeKey>::has_bytes_comparator has_bytes_comparator;
        return _compare(key1, key2, has_bytes_comparator());
    }
}
/*
 * compare的测试
 int main() {
    struct test{
    private:
        int a;
    public:
        test(int n): a(n){};
        ~test() {}
        int operator< (test t){
            if (a < t.a) return 1;
            else return 0;
        }

        int operator == (test t){
            if (a == t.a) return 1;
            else return 0;
        }
    };

    string s1 = "abc";
    string s2 = "ab";
    string s3 = "ab";

    cout << MySkipList::compareKey(s1, s2) << endl;
    cout << MySkipList::compareKey(s2, s3) << endl;

    const char *c1 = "abc";
    const char *c2 = "abd";
    char c3[4] = "aaa";
    char c4[4] = "aaa";

    cout << MySkipList::compareKey(c1, c2) << endl;
    cout << MySkipList::compareKey(c3, c4) << endl;

    test t1(2);
    test t2(3);
    cout << MySkipList::compareKey(t1, t2) << endl;

    int a = 0;
    int b = 0;
    int c = 2;
    cout << MySkipList::compareKey(a, b) << endl;
    cout << MySkipList::compareKey(a, c) << endl;
    return 0;
}
 */
#endif //SKIPLIST_COMPARATOR_H
