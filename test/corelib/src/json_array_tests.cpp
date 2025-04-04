// Copyright 2013-2025 Daniel Parker
// Distributed under Boost license

#include <ctime>
#include <list>
#include <sstream>
#include <utility>
#include <vector>

#include <catch/catch.hpp>

#include <jsoncons/json.hpp>
#include <jsoncons/json_encoder.hpp>

using namespace jsoncons;

TEST_CASE("test_initializer_list_of_integers")
{
    json doc(json_array_arg, {0,1,2,3});
    CHECK(doc.is_array());
    CHECK(4 == doc.size());
    for (std::size_t i = 0; i < doc.size(); ++i)
    {
        CHECK(i == doc[i].as<std::size_t>());
    }
}

TEST_CASE("test_assignment_to_initializer_list")
{
    json doc;

    doc = json(json_array_arg, {0,1,2,3});
    CHECK(doc.is_array());
    CHECK(4 == doc.size());
    for (std::size_t i = 0; i < doc.size(); ++i)
    {
        CHECK(i == doc[i].as<std::size_t>());
    }
}

TEST_CASE("test_assignment_to_initializer_list2")
{
    json val;
    val["data"]["id"] = json(json_array_arg, {0,1,2,3,4,5,6,7});
    val["data"]["item"] = json(json_array_arg,{json(json_array_arg, {2}),
                                      json(json_array_arg, {4,5,2,3}),
                                      json(json_array_arg, {4}),
                                      json(json_array_arg, {4,5,2,3}),
                                      json(json_array_arg, {2}),
                                      json(json_array_arg, {4,5,3}),
                                      json(json_array_arg, {2}),
                                      json(json_array_arg, {4,3})});

    CHECK(json(2) == val["data"]["item"][0][0]);
    CHECK(json(4) == val["data"]["item"][1][0]);
    CHECK(json(4) == val["data"]["item"][2][0]);
    CHECK(json(4) == val["data"]["item"][3][0]);
    CHECK(json(2) == val["data"]["item"][4][0]);
    CHECK(json(4) == val["data"]["item"][5][0]);
    CHECK(json(2) == val["data"]["item"][6][0]);
    CHECK(json(4) == val["data"]["item"][7][0]);
    CHECK(json(3) == val["data"]["item"][7][1]);
}

TEST_CASE("test_assignment_to_initializer_list3")
{
    json val;
    val["data"]["id"] = json(json_array_arg, {0,1,2,3,4,5,6,7});
    val["data"]["item"] = json(json_array_arg, {json(json_object_arg, {{"first",1},{"second",2}})});

    json expected_id = json::parse(R"(
[0,1,2,3,4,5,6,7]
    )");

    json expected_item = json::parse(R"(
    [{"first":1,"second":2}]
    )");

    CHECK(expected_id == val["data"]["id"]);
    CHECK(expected_item == val["data"]["item"]);
}

TEST_CASE("test_assign_initializer_list_of_object")
{
    json doc(json_array_arg);

    json transaction;
    transaction["Debit"] = 10000;

    doc = json(json_array_arg, {transaction});
    CHECK(doc.is_array());
    CHECK(1 == doc.size());
    CHECK(doc[0] == transaction);
}

TEST_CASE("test_initializer_list_of_objects")
{
    json book1;
    book1["author"] = "Smith";
    book1["title"] = "Old Bones";

    json book2;
    book2["author"] = "Jones";
    book2["title"] = "New Things";

    json doc(json_array_arg, {book1, book2});
    CHECK(doc.is_array());
    CHECK(2 == doc.size());

    CHECK(book1 == doc[0]);
    CHECK(book2 == doc[1]);
}

TEST_CASE("test_array_constructor")
{
    json doc(json_array_arg);
    doc.resize(10,10.0);
    CHECK(doc.is_array());
    CHECK(doc.size() == 10);
    CHECK(doc[0].as<double>() == Approx(10.0).epsilon(0.0000001));
}

TEST_CASE("test_make_array")
{
    json doc(json_array_arg);
    CHECK(0 == doc.size());
    doc.resize(10,10.0);
    CHECK(doc.is_array());
    CHECK(doc.size() == 10);
    CHECK(doc[0].as<double>() == Approx(10.0).epsilon(0.0000001));

}

TEST_CASE("test_add_element_to_array")
{
    json doc(json_array_arg);
    CHECK(doc.is_array());
    CHECK(doc.is_array());
    doc.push_back("Toronto");
    doc.push_back("Vancouver");
    doc.insert(doc.array_range().begin(),"Montreal");

    CHECK(3 == doc.size());

    CHECK(doc[0].as<std::string>() == std::string("Montreal"));
    CHECK(doc[1].as<std::string>() == std::string("Toronto"));
    CHECK(doc[2].as<std::string>() == std::string("Vancouver"));
}

TEST_CASE("test_emplace_element_to_array")
{
    json a(json_array_arg);
    CHECK(a.is_array());
    CHECK(a.is_array());
    a.emplace_back("Toronto");
    a.emplace_back("Vancouver");
    a.emplace(a.array_range().begin(),"Montreal");

    CHECK(3 == a.size());

    CHECK(a[0].as<std::string>() == std::string("Montreal"));
    CHECK(a[1].as<std::string>() == std::string("Toronto"));
    CHECK(a[2].as<std::string>() == std::string("Vancouver"));
}

TEST_CASE("test_array_add_pos")
{
    json arr(json_array_arg);
    CHECK(arr.is_array());
    CHECK(arr.is_array());
    arr.push_back("Toronto");
    arr.push_back("Vancouver");
    arr.insert(arr.array_range().begin(),"Montreal");

    CHECK(3 == arr.size());

    CHECK(arr[0].as<std::string>() == std::string("Montreal"));
    CHECK(arr[1].as<std::string>() == std::string("Toronto"));
    CHECK(arr[2].as<std::string>() == std::string("Vancouver"));
}

TEST_CASE("test_array_erase_range")
{
    json arr(json_array_arg);
    CHECK(arr.is_array());
    CHECK(arr.is_array());
    arr.push_back("Toronto");
    arr.push_back("Vancouver");
    arr.insert(arr.array_range().begin(),"Montreal");

    CHECK(3 == arr.size());

    arr.erase(arr.array_range().begin()+1,arr.array_range().end());

    CHECK(1 == arr.size());
    CHECK(arr[0].as<std::string>() == std::string("Montreal"));
}

TEST_CASE("test_reserve_array_capacity")
{
    json cities(json_array_arg);
    CHECK(cities.is_array());
    CHECK(cities.is_array());
    cities.reserve(10);  // storage is allocated
    CHECK(cities.capacity() == 10);
    CHECK(0 == cities.size());

    cities.push_back("Toronto");
    CHECK(cities.is_array());
    CHECK(cities.is_array());
    CHECK(cities.capacity() == 10);
    CHECK(1 == cities.size());
    cities.push_back("Vancouver");
    cities.insert(cities.array_range().begin(),"Montreal");
    CHECK(cities.capacity() == 10);
    CHECK(3 == cities.size());
}

TEST_CASE("test make_array()")
{
    json doc = json::make_array();
    CHECK(doc.is_array());
    CHECK(0 == doc.size());
    doc.emplace_back("Toronto");
    doc.emplace_back("Vancouver");
    doc.emplace(doc.array_range().begin(),"Montreal");
    CHECK(doc[0].as<std::string>() == std::string("Montreal"));
    CHECK(doc[1].as<std::string>() == std::string("Toronto"));
    CHECK(doc[2].as<std::string>() == std::string("Vancouver"));
}

TEST_CASE("test_one_dim_array")
{
    basic_json<char,sorted_policy,std::allocator<char>> a = basic_json<char,sorted_policy,std::allocator<char>>::make_array<1>(10,0);
    CHECK(a.size() == 10);
    CHECK(0 == a[0].as<int64_t>());
    a[1] = 1;
    a[2] = 2;
    CHECK(1 == a[1].as<int64_t>());
    CHECK(2 == a[2].as<int64_t>());
    CHECK(0 == a[9].as<int64_t>());

    CHECK(1 == a[1].as<long long>());
    CHECK(2 == a[2].as<long long>());
    CHECK(0 == a[9].as<long long>());
}

TEST_CASE("test_two_dim_array")
{
    json a = json::make_array<2>(3,4,0);
    CHECK(3 == a.size());
    a[0][0] = "Tenor";
    a[0][1] = "ATM vol";
    a[0][2] = "25-d-MS";
    a[0][3] = "25-d-RR";
    a[1][0] = "1Y";
    a[1][1] = 0.20;
    a[1][2] = 0.009;
    a[1][3] = -0.006;
    a[2][0] = "2Y";
    a[2][1] = 0.18;
    a[2][2] = 0.009;
    a[2][3] = -0.005;

    CHECK(a[0][0].as<std::string>() ==std::string("Tenor"));
    CHECK(a[2][3].as<double>() == Approx(-0.005).epsilon(0.00000001));

    CHECK(a[0][0].as<std::string>() ==std::string("Tenor"));
    CHECK(a[2][3].as<double>() == Approx(-0.005).epsilon(0.00000001));
}

TEST_CASE("test_three_dim_array")
{
    json a = json::make_array<3>(4,3,2,0);
    CHECK(4 == a.size());
    a[0][2][0] = 2;
    a[0][2][1] = 3;

    CHECK(2 == a[0][2][0].as<int64_t>());
    CHECK(3 == a[0][2][1].as<int64_t>());
    CHECK(0 == a[3][2][1].as<int64_t>());

    CHECK(2 == a[0][2][0].as<long long>());
    CHECK(3 == a[0][2][1].as<long long>());
    CHECK(0 == a[3][2][1].as<long long>());
}

TEST_CASE("test_array_assign_vector")
{
    std::vector<std::string> vec;
    vec.push_back("Toronto");
    vec.push_back("Vancouver");
    vec.push_back("Montreal");

    json val;
    val = vec;

    CHECK(3 == val.size());
    CHECK(val[0].as<std::string>() ==std::string("Toronto"));
    CHECK(val[1].as<std::string>() ==std::string("Vancouver"));
    CHECK(val[2].as<std::string>() ==std::string("Montreal"));

}

TEST_CASE("test_array_assign_vector_of_bool")
{
    std::vector<bool> vec;
    vec.push_back(true);
    vec.push_back(false);
    vec.push_back(true);

    json val;
    val = vec;

    CHECK(3 == val.size());
    CHECK(val[0].as<bool>() == true);
    CHECK(val[1].as<bool>() == false);
    CHECK(val[2].as<bool>() == true);

}

TEST_CASE("test_array_add_null")
{
    json a(json_array_arg);
    a.push_back(jsoncons::null_type());
    a.push_back(json::null());
    CHECK(a[0].is_null());
    CHECK(a[1].is_null());
}

TEST_CASE("test_array_from_container")
{
    std::vector<int> vec;
    vec.push_back(10);
    vec.push_back(20);
    vec.push_back(30);

    json val1 = vec;
    REQUIRE(3 == vec.size());
    CHECK(vec[0] == 10);
    CHECK(vec[1] == 20);
    CHECK(vec[2] == 30);

    std::list<double> list;
    list.push_back(10.5);
    list.push_back(20.5);
    list.push_back(30.5);

    json val2 = list;
    REQUIRE(3 == val2.size());
    CHECK(val2[0].as<double>() == Approx(10.5).epsilon(0.000001));
    CHECK(val2[1].as<double>() == Approx(20.5).epsilon(0.000001));
    CHECK(val2[2].as<double>() == Approx(30.5).epsilon(0.000001));
}

TEST_CASE("test_array_as_vector_of_double")
{
    std::string s("[0,1.1,2,3.1]");
    json val = json::parse(s);

    std::vector<double> v = val.as<std::vector<double>>(); 
    CHECK(4 == v.size());
    CHECK(v[0] == Approx(0.0).epsilon(0.0000000001));
    CHECK(v[1] == Approx(1.1).epsilon(0.0000000001));
    CHECK(v[2] == Approx(2.0).epsilon(0.0000000001));
    CHECK(v[3] == Approx(3.1).epsilon(0.0000000001));
}

TEST_CASE("test_array_as_vector_of_bool")
{
    std::string s("[true,false,true]");
    json val = json::parse(s);

    std::vector<bool> v = val.as<std::vector<bool>>(); 
    CHECK(3 == v.size());
    CHECK(v[0] == true);
    CHECK(v[1] == false);
    CHECK(v[2] == true);
}

TEST_CASE("test_array_as_vector_of_string")
{
    std::string s("[\"Hello\",\"World\"]");
    json val = json::parse(s);

    std::vector<std::string> v = val.as<std::vector<std::string>>(); 
    CHECK(2 == v.size());
    CHECK(v[0] == "Hello");
    CHECK(v[1] == "World");
}

TEST_CASE("test_array_as_vector_of_char")
{
    std::string s("[20,30]");
    json val = json::parse(s);

    std::vector<char> v = val.as<std::vector<char>>(); 
    CHECK(2 == v.size());
    CHECK(v[0] == 20);
    CHECK(v[1] == 30);
}

TEST_CASE("test_array_as_vector_of_int")
{
    std::string s("[0,1,2,3]");
    json val = json::parse(s);

    std::vector<int> v = val.as<std::vector<int>>(); 
    CHECK(4 == v.size());
    CHECK(0 == v[0]);
    CHECK(1 == v[1]);
    CHECK(2 == v[2]);
    CHECK(3 == v[3]);

    std::vector<unsigned int> v1 = val.as<std::vector<unsigned int>>(); 
    CHECK(4 == v1.size());
    CHECK(0 == v1[0]);
    CHECK(1 == v1[1]);
    CHECK(2 == v1[2]);
    CHECK(3 == v1[3]);

    std::vector<long> v2 = val.as<std::vector<long>>(); 
    CHECK(4 == v2.size());
    CHECK(0 == v2[0]);
    CHECK(1 == v2[1]);
    CHECK(2 == v2[2]);
    CHECK(3 == v2[3]);

    std::vector<unsigned long> v3 = val.as<std::vector<unsigned long>>(); 
    CHECK(4 == v3.size());
    CHECK(0 == v3[0]);
    CHECK(1 == v3[1]);
    CHECK(2 == v3[2]);
    CHECK(3 == v3[3]);

    std::vector<long long> v4 = val.as<std::vector<long long>>(); 
    CHECK(4 == v4.size());
    CHECK(0 == v4[0]);
    CHECK(1 == v4[1]);
    CHECK(2 == v4[2]);
    CHECK(3 == v4[3]);

    std::vector<unsigned long long> v5 = val.as<std::vector<unsigned long long>>(); 
    CHECK(4 == v5.size());
    CHECK(0 == v5[0]);
    CHECK(1 == v5[1]);
    CHECK(2 == v5[2]);
    CHECK(3 == v5[3]);
}

TEST_CASE("test_array_as_vector_of_int_on_proxy")
{
    std::string s("[0,1,2,3]");
    json val = json::parse(s);
    json root;
    root["val"] = val;
    std::vector<int> v = root["val"].as<std::vector<int>>();
    CHECK(4 == v.size());
    CHECK(0 == v[0]);
    CHECK(1 == v[1]);
    CHECK(2 == v[2]);
    CHECK(3 == v[3]);
}

TEST_CASE("test json_array erase with iterator")
{
    SECTION("json erase with iterator")
    {
        json doc(jsoncons::json_array_arg);

        doc.push_back("a");
        doc.push_back("b");
        doc.push_back("c");

        auto it = doc.array_range().begin();
        while (it != doc.array_range().end())
        {
            if (*it == "a" || *it == "c")
            {
                it = doc.erase(it);
            }
            else
            {
                it++;
            }
        }

        CHECK(1 == doc.size());
        CHECK(doc[0] == "b");
    }

    SECTION("json erase with iterator 2")
    {
        json doc(jsoncons::json_array_arg);

        doc.push_back("a");
        doc.push_back("b");
        doc.push_back("c");

        auto it = doc.array_range().begin();
        while (it != doc.array_range().end())
        {
            if (*it == "a")
            {
                it = doc.erase(it,it+2);
            }
            else
            {
                it++;
            }
        }

        CHECK(1 == doc.size());
        CHECK(doc[0] == "c");
    }
}
