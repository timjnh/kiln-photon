class UrlEncode {
  public:

  static String encode(String value) {
    String escaped = "";

    for(int i = 0; i < value.length(); ++i) {
        char c = value.charAt(i);

        // keep alphanumeric and other accepted characters intact
        if(isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
            escaped += c;
            continue;
        }

        escaped += "%" + String::format("%02x", (int)c);
    }

    return escaped;
  }
};
