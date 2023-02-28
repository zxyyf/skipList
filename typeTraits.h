//
// Created by 杨云飞 on 2023/2/27.
//

#ifndef SKIPLIST_TYPETRAITS_H
#define SKIPLIST_TYPETRAITS_H
namespace MySkipList{
    struct _false_type{};
    struct _true_type{};

    template<class T>
    struct _type_traits{
        typedef _false_type has_bytes_comparator;
        typedef _false_type is_POD_type;
    };

    template<> struct _type_traits<std::string>{
        typedef _true_type has_bytes_comparator;
        typedef _false_type is_POD_type;
    };

    template<> struct _type_traits<char *>{
        typedef _true_type has_bytes_comparator;
        typedef _true_type is_POD_type;
    };

    template<> struct _type_traits<const char *>{
        typedef _true_type has_bytes_comparator;
        typedef _true_type is_POD_type;
    };
}
#endif //SKIPLIST_TYPETRAITS_H
