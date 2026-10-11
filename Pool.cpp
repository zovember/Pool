/* Michael Kochura */

#include <iostream>

enum Status {
    Ok,
    TooMuch,
    TooLittle
};


class Pool {
  private:
    double tokens;
    double dollars;
    double token_price;

    void upd_token_price();

  public:
    Pool(const double tokens, const double dollars);

    std::string show_stat();
    double get_token_price() { return this->token_price; }
    Status buy_tokens(const double);
    Status sell_tokens(const double);
};

Pool::Pool(const double tokens, const double dollars) {
    this->tokens = tokens;
    this->dollars = dollars;
    this->token_price = dollars / tokens;
}

void Pool::upd_token_price() {
    this->token_price = this->dollars / this->tokens;
}

std::string Pool::show_stat() {
    return "MSHP in pull: " + std::to_string(this->tokens) +
    "\nDollars in pull: " + std::to_string(this->dollars) +
    "\n1 MSHP = " + std::to_string(this->token_price) + " $";
}

Status Pool::buy_tokens(const double dollars) {
    if (dollars <= 0) return TooLittle;
    if (dollars * 100 / this->dollars > 60) return TooMuch; // Обычный математический "крестик". Нельзя купить больше 60% пула (да, это мне подсказала нейронка)
    else {
        this->dollars += dollars;
        this->tokens -= dollars / token_price;
        this->upd_token_price();
    }
    return Ok;
}

Status Pool::sell_tokens(const double tokens) {
    if (tokens <= 0) return TooLittle;
    if (tokens * 100 / this->tokens > 60) return TooMuch;
    else {
        this->tokens += tokens;
        this->dollars -= tokens * token_price;
        this->upd_token_price();
    }
    return Ok;
}
