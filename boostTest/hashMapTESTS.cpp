#define BOOST_TEST_MODULE hashMap_tests

#include <boost/test/included/unit_test.hpp>
#include "classes/hashMap.h"

BOOST_AUTO_TEST_CASE(test_create_hm) {
    const auto hashmap1 = new hashMap;
    BOOST_CHECK(hashmap1 != nullptr);
    const auto hashmap2 = new hashMap(4);
    BOOST_CHECK(hashmap2 != nullptr);
}


BOOST_AUTO_TEST_CASE(test_hm_insert) {
    hashMap hashmap(1);
    hashmap.insert("key1", "value1");
    hashmap.insert("key1", "value1");
    hashmap.insert("key2", "value2");
    hashmap.insert("key3", "value3");
    hashmap.insert("key4", "value4");
    hashmap.insert("key5", "value5");
    const Pair pair("key0","value0");
    hashmap.insert(pair);
    BOOST_CHECK(hashmap.get_size() == 6);
    BOOST_CHECK(hashmap.get_bucketsCount() == 3);
}


BOOST_AUTO_TEST_CASE(test_hm_get) {
    hashMap hashmap;
    const Pair nopair;
    const Pair pair1("key0","value0");
    const Pair pair2("key1", "value1");
    hashmap.insert(pair1);
    hashmap.insert(pair2);
    BOOST_CHECK(hashmap.Get("key0") == pair1);
    BOOST_CHECK(hashmap.Get() == pair1 || hashmap.Get() == pair2);
    BOOST_CHECK(hashmap.Get("123") == nopair);
}


BOOST_AUTO_TEST_CASE(test_hm_del) {
    hashMap hashmap;
    hashmap.insert("key1", "value1");
    hashmap.insert("key2", "value2");
    hashmap.insert("key3", "value3");
    hashmap.insert("key4", "value4");
    hashmap.insert("key5", "value5");
    hashmap.insert("key6", "value6");
    hashmap.insert("key7", "value7");
    hashmap.insert("key8", "value8");
    hashmap.insert("key9", "value9");
    hashmap.insert("key10", "value10");
    hashmap.insert("key11", "value11");
    hashmap.del("key5");
    BOOST_CHECK(hashmap.get_size() == 10);
    BOOST_CHECK(hashmap.Get("key5") == Pair{});
    hashmap.del("key7");
    BOOST_CHECK(hashmap.get_size() == 9);
    BOOST_CHECK(hashmap.Get("key7") == Pair{});
    hashmap.del("key3");
    BOOST_CHECK(hashmap.get_size() == 8);
    BOOST_CHECK(hashmap.Get("key3") == Pair{});
    hashmap.del("key10");
    BOOST_CHECK(hashmap.get_size() == 7);
    BOOST_CHECK(hashmap.Get("key10") == Pair{});
}


BOOST_AUTO_TEST_CASE(test_hm_operator) {
    hashMap hashmap;
    hashmap.insert("key1", "value1");
    hashmap.insert("key2", "value2");
    std::stringstream stream;
    stream << hashmap;
    BOOST_CHECK(stream.str() == "{{[key1, value1]}, \n{[key2, value2]}}\n");
}
