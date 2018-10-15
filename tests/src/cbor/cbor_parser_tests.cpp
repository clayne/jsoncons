// Copyright 2016 Daniel Parker
// Distributed under Boost license

#include <catch/catch.hpp>
#include <jsoncons/json.hpp>
#include <jsoncons_ext/cbor/cbor.hpp>
#include <jsoncons_ext/cbor/cbor_parser.hpp>
#include <sstream>
#include <vector>
#include <utility>
#include <ctime>
#include <limits>

using namespace jsoncons;
using namespace jsoncons::cbor;

void check_parsing(const std::vector<uint8_t>& v, const json& expected)
{
    try
    {
        std::error_code ec;

        jsoncons::json_decoder<json> decoder;
        cbor_parser parser(decoder);
        parser.update(v.data(),v.size());
        parser.parse_some(ec);

        json result = decoder.get_result();

        if (!(expected == result))
        {
            std::cout << "v: ";
            for (auto b : v)
            {
                std::cout << "0x" << std::hex << (int)b;
            }
            std::cout << "\n";
            std::cout << "expected: " << expected << "\n";
            std::cout << "result: " << result << "\n";
        }

        REQUIRE(expected == result);
    }
    catch (const std::exception& e)
    {
        std::cout << e.what() << std::endl;
        std::cout << expected.to_string() << std::endl;
    }
}

TEST_CASE("test_cbor_parsing")
{
    // unsigned integer
    check_parsing({0x00},json(0U));
    check_parsing({0x01},json(1U));
    check_parsing({0x0a},json(10U));
    check_parsing({0x17},json(23U));
    check_parsing({0x18,0x18},json(24U));
    check_parsing({0x18,0xff},json(255U));
    check_parsing({0x19,0x01,0x00},json(256U));
    check_parsing({0x19,0xff,0xff},json(65535U));
    check_parsing({0x1a,0,1,0x00,0x00},json(65536U));
    check_parsing({0x1a,0xff,0xff,0xff,0xff},json(4294967295U));
    check_parsing({0x1b,0,0,0,1,0,0,0,0},json(4294967296U));
    check_parsing({0x1b,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff},json(std::numeric_limits<uint64_t>::max()));

    // positive signed integer
    check_parsing({0x00},json(0));
    check_parsing({0x01},json(1));
    check_parsing({0x0a},json(10));
    check_parsing({0x17},json(23));
    check_parsing({0x18,0x18},json(24));
    check_parsing({0x18,0xff},json(255));
    check_parsing({0x19,0x01,0x00},json(256));
    check_parsing({0x19,0xff,0xff},json(65535));
    check_parsing({0x1a,0,1,0x00,0x00},json(65536));
    check_parsing({0x1a,0xff,0xff,0xff,0xff},json(4294967295));
    check_parsing({0x1b,0,0,0,1,0,0,0,0},json(4294967296));
    check_parsing({0x1b,0x7f,0xff,0xff,0xff,0xff,0xff,0xff,0xff},json(std::numeric_limits<int64_t>::max()));
    // negative integers
    check_parsing({0x20},json(-1));
    check_parsing({0x21},json(-2));
    check_parsing({0x37},json(-24));
    check_parsing({0x38,0x18},json(-25));
    check_parsing({0x38,0xff},json(-256));
    check_parsing({0x39,0x01,0x00},json(-257));
    check_parsing({0x39,0xff,0xff},json(-65536));
    check_parsing({0x3a,0,1,0x00,0x00},json(-65537));

    check_parsing({0x3a,0xff,0xff,0xff,0xff},json(-4294967296));
    check_parsing({0x3b,0,0,0,1,0,0,0,0},json(-4294967297));

    // null, true, false
    check_parsing({0xf6},json::null());

    check_parsing({0xf5},json(true));
    check_parsing({0xf4},json(false));

    // floating point
    check_parsing({0xfb,0,0,0,0,0,0,0,0},json(0.0));
    check_parsing({0xfb,0xbf,0xf0,0,0,0,0,0,0},json(-1.0));
    check_parsing({0xfb,0xc1,0x6f,0xff,0xff,0xe0,0,0,0},json(-16777215.0));

    // byte string
    std::vector<uint8_t> v;
    check_parsing({0x40},json(byte_string_view(v.data(),v.size())));
    v = {' '};
    check_parsing({0x41,' '},json(byte_string_view(v.data(),v.size())));
    v = {0};
    check_parsing({0x41,0},json(byte_string_view(v.data(),v.size())));
    v = {'H','e','l','l','o'};
    check_parsing({0x45,'H','e','l','l','o'},json(byte_string_view(v.data(),v.size())));
    v = {'1','2','3','4','5','6','7','8','9','0','1','2','3','4','5','6','7','8','9','0','1','2','3','4'};
    check_parsing({0x58,0x18,'1','2','3','4','5','6','7','8','9','0','1','2','3','4','5','6','7','8','9','0','1','2','3','4'},
                 json(byte_string_view(v.data(),v.size())));

    // string
    check_parsing({0x60},json(""));
    check_parsing({0x61,' '},json(" "));
    check_parsing({0x78,0x18,'1','2','3','4','5','6','7','8','9','0','1','2','3','4','5','6','7','8','9','0','1','2','3','4'},
                 json("123456789012345678901234"));

    // byte strings with undefined length
    check_parsing({0x5f,0xff}, json(byte_string()));
    check_parsing({0x5f,0x40,0xff}, json(byte_string()));
    check_parsing({0x5f,0x40,0x40,0xff}, json(byte_string()));

    check_parsing({0x5f,0x43,'H','e','l',0x42,'l','o',0xff}, json(byte_string("Hello")));
    check_parsing({0x5f,0x41,'H',0x41,'e',0x41,'l',0x41,'l',0x41,'o',0xff}, json(byte_string("Hello")));
    check_parsing({0x5f,0x41,'H',0x41,'e',0x40,0x41,'l',0x41,'l',0x41,'o',0xff}, json(byte_string("Hello")));

    // text strings with undefined length

    check_parsing({0x7f,0xff}, json(""));

    check_parsing({0x7f,0x60,0xff}, json(""));
    check_parsing({0x7f,0x60,0x60,0xff}, json(""));
    check_parsing({0x7f,0x63,'H','e','l',0x62,'l','o',0xff}, json("Hello"));
    check_parsing({0x7f,0x61,'H',0x61,'e',0x61,'l',0x61,'l',0x61,'o',0xff}, json("Hello"));
    check_parsing({0x7f,0x61,'H',0x61,'e',0x61,'l',0x60,0x61,'l',0x61,'o',0xff}, json("Hello"));

    // arrays
    check_parsing({0x80},json::array());
    check_parsing({0x81,'\0'},json::parse("[0]"));
    check_parsing({0x82,'\0','\0'},json::array({0,0}));
    check_parsing({0x82,0x81,'\0','\0'}, json::parse("[[0],0]"));
    check_parsing({0x81,0x65,'H','e','l','l','o'},json::parse("[\"Hello\"]"));

    // indefinite length arrays
    check_parsing({0x9f,0xff},json::array());
    check_parsing({0x9f,0x9f,0xff,0xff},json::parse("[[]]"));

    // maps
    check_parsing({0xa0},json::object());
    check_parsing({0xa1,0x62,'o','c',0x81,'\0'}, json::parse("{\"oc\": [0]}"));
    check_parsing({0xa1,0x62,'o','c',0x84,'\0','\1','\2','\3'}, json::parse("{\"oc\": [0, 1, 2, 3]}"));

    // indefinite length maps
    check_parsing({0xbf,0xff},json::object());
    check_parsing({0xbf,0x64,'N','a','m','e',0xbf,0xff,0xff},json::parse("{\"Name\":{}}"));

    // bignum
    check_parsing({0xc2,0x49,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
                  json(bignum(1,{0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00})));

    // date_time
    check_parsing({0xc0,0x78,0x19,'2','0','1','5','-','0','5','-','0','7',' ','1','2',':','4','1',':','0','7','-','0','7',':','0','0'},
                  json("2015-05-07 12:41:07-07:00", semantic_tag_type::date_time));

    // epoch_time
    check_parsing({0xc1,0x1a,0x55,0x4b,0xbf,0xd3},
                  json(1431027667, semantic_tag_type::epoch_time));

    // decimal fraction
    check_parsing({0xc4, // Tag 4
                   0x82, // Array of length 2
                   0x21, // -2
                   0x19,0x6a,0xb3 // 27315
                   },
                  json("273.15", semantic_tag_type::decimal));
}

TEST_CASE("test_decimal_as_string")
{
    SECTION("-2 27315")
    {
        std::vector<uint8_t> v = {0xc4, // Tag 4
                                  0x82, // Array of length 2
                                  0x21, // -2
                                  0x19,0x6a,0xb3 // 27315
                                  };

        const uint8_t* endp = nullptr;
        std::string s = cbor::detail::get_decimal_as_string(v.data(),v.data()+v.size(),&endp);
        REQUIRE_FALSE(endp == v.data());
        REQUIRE(endp == (v.data()+v.size()));
        CHECK(std::string("273.15") == s);
    }
    SECTION("-6 27315")
    {
        std::vector<uint8_t> v = {0xc4, // Tag 4
                                  0x82, // Array of length 2
                                  0x25, // -6
                                  0x19,0x6a,0xb3 // 27315
                                  };

        const uint8_t* endp = nullptr;
        std::string s = cbor::detail::get_decimal_as_string(v.data(),v.data()+v.size(),&endp);
        REQUIRE_FALSE(endp == v.data());
        REQUIRE(endp == (v.data()+v.size()));
        CHECK(std::string("0.027315") == s);
    }
    SECTION("-5 27315")
    {
        std::vector<uint8_t> v = {0xc4, // Tag 4
                                  0x82, // Array of length 2
                                  0x24, // -5
                                  0x19,0x6a,0xb3 // 27315
                                  };

        const uint8_t* endp = nullptr;
        std::string s = cbor::detail::get_decimal_as_string(v.data(),v.data()+v.size(),&endp);
        REQUIRE_FALSE(endp == v.data());
        REQUIRE(endp == (v.data()+v.size()));
        CHECK(std::string("0.27315") == s);
    }
    SECTION("0 27315")
    {
        std::vector<uint8_t> v = {0xc4, // Tag 4
                                  0x82, // Array of length 2
                                  0x00, // 0
                                  0x19,0x6a,0xb3 // 27315
                                  };

        const uint8_t* endp = nullptr;
        std::string s = cbor::detail::get_decimal_as_string(v.data(),v.data()+v.size(),&endp);
        REQUIRE_FALSE(endp == v.data());
        REQUIRE(endp == (v.data()+v.size()));
        CHECK(std::string("27315.0") == s);
    }
    SECTION("2 27315")
    {
        std::vector<uint8_t> v = {0xc4, // Tag 4
                                  0x82, // Array of length 2
                                  0x02, // 2
                                  0x19,0x6a,0xb3 // 27315
                                  };

        const uint8_t* endp = nullptr;
        std::string s = cbor::detail::get_decimal_as_string(v.data(),v.data()+v.size(),&endp);
        REQUIRE_FALSE(endp == v.data());
        REQUIRE(endp == (v.data()+v.size()));
        CHECK(std::string("2731500.0") == s);
    }
    SECTION("-2 18446744073709551616")
    {
        std::vector<uint8_t> v = {0xc4, // Tag 4
                                  0x82, // Array of length 2
                                  0x21, // -2
                                  0xc2,0x49,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 // 18446744073709551616
                                  };

        const uint8_t* endp = nullptr;
        std::string s = cbor::detail::get_decimal_as_string(v.data(),v.data()+v.size(),&endp);
        REQUIRE_FALSE(endp == v.data());
        REQUIRE(endp == (v.data()+v.size()));
        CHECK(std::string("184467440737095516.16") == s);
    }
}
