#include <iostream>
#include <map>
#include <string>
#include <optional>

// HttpRequest class using the Builder pattern.
class HttpRequest
{
public:
    // Type aliases for clarity and modern C++ style.
    using Headers = std::map<std::string, std::string>;
    using QueryParams = std::map<std::string, std::string>;

private:
    // Required
    std::string url;

    // Optional members, represented with std::optional for clarity and correctness.
    std::optional<std::string> method;
    std::optional<Headers> headers;
    std::optional<QueryParams> queryParams;
    std::optional<std::string> body;
    std::optional<int> timeout;

    // Private constructor. Only the Builder can create an instance.
    HttpRequest(std::string url,
                std::optional<std::string> method,
                std::optional<Headers> headers,
                std::optional<QueryParams> queryParams,
                std::optional<std::string> body,
                std::optional<int> timeout)
        : url(std::move(url)), method(std::move(method)), headers(std::move(headers)),
          queryParams(std::move(queryParams)), body(std::move(body)),
          timeout(std::move(timeout)) {}

public:
    // Output details
    void print() const
    {
        std::cout << "HttpRequest {\n";
        std::cout << "  url: " << url << "\n";
        std::cout << "  method: " << method.value_or("GET") << "\n";
        std::cout << "  headers: " << (headers.has_value() ? headers.value().size() : 0) << " item(s)\n";
        std::cout << "  queryParams: " << (queryParams.has_value() ? queryParams.value().size() : 0) << " item(s)\n";
        std::cout << "  body: " << (body.value_or("").empty() ? "<empty>" : body.value()) << "\n";
        std::cout << "  timeout: " << timeout.value_or(30000) << " ms\n";
        std::cout << "}\n";
    }

    // Builder nested class, responsible for constructing HttpRequest objects.
    class Builder
    {
    private:
        std::string url;
        std::optional<std::string> method;
        std::map<std::string, std::string> headers;
        std::map<std::string, std::string> queryParams;
        std::optional<std::string> body;
        std::optional<int> timeout;

    public:
        // Builder's constructor requires the essential parameter: url.
        explicit Builder(std::string url)
            : url(std::move(url)) {}

        // Setters for optional parameters. Each returns a reference to the Builder.
        Builder& setMethod(std::string m)
        {
            method = std::move(m);
            return *this;
        }

        Builder& addHeader(std::string key, std::string value)
        {
            // Use try_emplace for more efficient insertion.
            headers.try_emplace(std::move(key), std::move(value));
            return *this;
        }

        Builder& addQueryParam(std::string key, std::string value)
        {
            queryParams.try_emplace(std::move(key), std::move(value));
            return *this;
        }

        Builder& setBody(std::string b)
        {
            body = std::move(b);
            return *this;
        }

        Builder& setTimeout(int t)
        {
            timeout = t;
            return *this;
        }

        // The build method constructs and returns the final object.
        HttpRequest build() const
        {
            // Note: The move semantics here are important for efficiency.
            return HttpRequest(url, method, headers, queryParams, body, timeout);
        }
    };
};

int main()
{
    // Build a simple GET request with a timeout.
    const auto request1 = HttpRequest::Builder("https://www.example.com/api/data")
        .setTimeout(5000)
        .build();
    request1.print();

    std::cout << "\n";

    // Build a more complex POST request with headers, query params, and a body.
    const auto request2 = HttpRequest::Builder("https://www.example.com/api/users")
        .setMethod("POST")
        .addHeader("Content-Type", "application/json")
        .addHeader("Authorization", "Bearer token123")
        .addQueryParam("id", "456")
        .addQueryParam("action", "update")
        .setBody("{\"username\":\"testuser\"}")
        .build();
    request2.print();

    return 0;
}
