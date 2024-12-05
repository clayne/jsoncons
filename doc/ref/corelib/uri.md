### jsoncons::uri

```cpp
#include <jsoncons/utility/uri.hpp>

```
The class `uri` represents a Uniform Resource Identifier (URI) reference.

#### Constructor

    uri();

    explicit uri(const std::string& str);
Constructs a `uri` by parsing the given string.

    uri(const std::string& uri);
Constructs a `uri` by parsing the given string.

    uri(jsoncons::string_view scheme,
        jsoncons::string_view userinfo,
        jsoncons::string_view host,
        jsoncons::string_view port,
        jsoncons::string_view path,
        jsoncons::string_view query,
        jsoncons::string_view fragment)
Constructs a `uri` from the given non-encoded parts.
 
    uri(const uri& other, uri_fragment_part_t, jsoncons::string_view fragment);
Constructs a `uri` from `other`, replacing it's fragment part with (non-encoded) `fragment`.

    uri(const uri& other);
Copy constructor.

    uri(uri&& other) noexcept;
Move constructor.

#### Assignment
    
    uri& operator=(const uri& other);
    
    uri& operator=(uri&& other) noexcept;

#### Member functions

    jsoncons::string_view scheme() const noexcept;
Returns the scheme part of this URI. The scheme is the first part of the URI, before the : character.

    std::string userinfo() const;
Returns the decoded userinfo part of this URI.

    jsoncons::string_view encoded_userinfo() const noexcept;
Returns the encoded userinfo part of this URI.

    jsoncons::string_view host() const noexcept;

    jsoncons::string_view encoded_host() const noexcept;

    jsoncons::string_view port() const noexcept;

    jsoncons::string_view encoded_port() const noexcept;

    std::string authority() const;
Returns the decoded authority part of this URI.

    jsoncons::string_view encoded_authority() const noexcept;
Returns the encoded authority part of this URI.

    std::string path() const;
Returns the decoded path part of this URI.

    jsoncons::string_view encoded_path() const noexcept;

    std::string query() const;

    jsoncons::string_view encoded_query() const noexcept;

    std::string query() const;

    jsoncons::string_view encoded_query() const noexcept;

    std::string fragment() const;

    jsoncons::string_view encoded_fragment() const noexcept;

    uri resolve(const uri& base) const;
Resolves a uri reference against a base URI.

    const std::string& string() const;
Returns a URI string.

    static uri parse(const std::string& str, std::error_code& ec);
Creates a `uri` by parsing the given string.

    const std::string& string() const;

    static uri parse(const std::string& str, std::error_code& ec);
Creates a `uri` by parsing the given string.

### Examples
  
