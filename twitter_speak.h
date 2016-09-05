#include "url_encode.h"

// Credit: https://community.particle.io/t/update-twitter-with-thingspeak/5587
class TwitterSpeak {
  public:

    void begin(String apiKey) {
      _apiKey = apiKey;
    }

    bool tweet(String tweet) {
      String tweetData;

      if(_client.connect("api.thingspeak.com", 80)) {
          tweetData = "api_key=" + _apiKey + "&status=" + UrlEncode::encode(tweet);

          _client.print("POST /apps/thingtweet/1/statuses/update HTTP/1.1\n");
          _client.print("Host: api.thingspeak.com\n");
          _client.print("Connection: close\n");
          _client.print("Content-Type: application/x-www-form-urlencoded\n");
          _client.print("Content-Length: ");
          _client.print(tweetData.length());
          _client.print("\n\n");

          _client.println(tweetData);

          // without this delay, the tcp _client may not have time to send
          // the complete message before it shuts down
          delay(200);
      }
      else {
          return false;
      }

      _client.flush();
      _client.stop();

      return true;
    }

  private:
    String _apiKey;

    TCPClient _client;
};
