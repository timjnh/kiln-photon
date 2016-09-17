#include "http_client.h"

class QuoteService {
public:
  QuoteService() {}

  String getQuote(String topic, int maxLength) {
    String quote;
    String movie;

    http_request_t _request;
    http_response_t _response;

    http_header_t headers[] = {
      { "Content-Type", "application/json" },
      { "Accept" , "application/json" },
      { NULL, NULL } // NOTE: Always terminate headers will NULL
    };

    _request.hostname = "api.quodb.com";
    _request.port = 80;
    _request.path = String::format("/search/%s?titles_per_page=1&phrases_per_title=1&page=%d", topic.c_str(), random(30000));

    _httpClient.get(_request, _response, headers);

    quote = findJsonValue(_response.body, "phrase");
    movie = findJsonValue(_response.body, "title");

    // make sure we're not going to be two long. include two "s, 4 spaces and a dash
    int overflow = maxLength - (quote.length() + movie.length() + 7);
    if(overflow < 0) {
      quote = quote.substring(0, quote.length() + overflow);
    }

    return "\"" + quote + "\" - " + movie + "  ";
  }

private:
  HttpClient _httpClient;

  String findJsonValue(String json, String key) {
    int keyLength = key.length() + 3;
    int keyPosition = json.indexOf("\"" + key + "\":\"");
    int closeQuotePosition = json.indexOf("\"", keyPosition + keyLength + 1);

    return json.substring(keyPosition + keyLength + 1, closeQuotePosition);
  }
};
