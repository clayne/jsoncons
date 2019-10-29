// Copyright 2019 Daniel Parker
// Distributed under the Boost license, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// See https://github.com/danielaparker/jsoncons for latest version

#ifndef JSONCONS_CBOR_CBOR_OPTIONS_HPP
#define JSONCONS_CBOR_CBOR_OPTIONS_HPP

#include <string>
#include <limits> // std::numeric_limits
#include <cwchar>
#include <jsoncons/json_exception.hpp>
#include <jsoncons_ext/cbor/cbor_detail.hpp>

namespace jsoncons { namespace cbor {

class cbor_decode_options
{
public:
    virtual ~cbor_decode_options() = default;
};

class cbor_encode_options
{
public:
    virtual ~cbor_encode_options() = default;

    virtual bool use_stringref() const = 0;
     
    virtual bool use_typed_arrays() const = 0; 
};

class cbor_options : public virtual cbor_decode_options, 
                     public virtual cbor_encode_options
{
private:
    bool use_stringref_;
    bool use_typed_arrays_;
public:

    static const cbor_options& get_default_options()
    {
        static cbor_options options{};
        return options;
    }


//  Constructors

    cbor_options()
        : use_stringref_(false),
          use_typed_arrays_(false)
    {
    }

    bool use_stringref() const override
    {
        return use_stringref_;
    }

    cbor_options& use_stringref(bool value)
    {
        use_stringref_ = value;
        return *this;
    }

#if !defined(JSONCONS_NO_DEPRECATED)
    JSONCONS_DEPRECATED_MSG("instead, use use_stringref(bool)")
    cbor_options& pack_strings(bool value)
    {
        use_stringref_ = value;
        return *this;
    }
#endif

    bool use_typed_arrays() const override
    {
        return use_typed_arrays_;
    }

    cbor_options& use_typed_arrays(bool value)
    {
        use_typed_arrays_ = value;
        return *this;
    }
};

}}
#endif
