/* Michael Kochura */

#include "Pool.cpp"
#include "httplib.h"
#include <iostream>

int main() {
    httplib::Server server;
    Pool pool(/*tokens*/ 5000, /*dollars*/ 10000);

    server.Get("/", [&pool](const httplib::Request &, httplib::Response &res) {
        res.set_content("Hello!\nIt is a pool of token MSHP!\n" + pool.show_stat(), "text/plain");
    });

    server.Get("/stat", [&pool](const httplib::Request &, httplib::Response &res) {
        res.set_content(pool.show_stat(), "text/plain");
    });

    server.Post("/buy/(.*)", [&pool](const httplib::Request &req, httplib::Response &res) {
        double dollars = std::stod(req.matches[1]);
        std::string tokens = std::to_string(dollars * pool.get_token_price());
        std::string token_price = std::to_string(pool.get_token_price());

        Status st = pool.buy_tokens(dollars); // Да, stod() мне тоже нейронка подсказала
        switch (st) {
            case Ok: { std::string text = "Successful! You have bought " + tokens + " MSHP for " + std::to_string(dollars) + " $ at a price of " + token_price + " per."; 
                       res.set_content(text, "text/plain");
                       break;
                     }

            case TooMuch: res.set_content("Unsuccessful! You can not buy more 60% of pull.", "text/plain");
                          break;

            case TooLittle: res.set_content("Unsuccessful! You can not buy a negativ and a zero amount.", "text/plain");
                            break;
        }
    });

    server.Post("/sell/(.*)", [&pool](const httplib::Request &req, httplib::Response &res) {
        double tokens = std::stod(req.matches[1]);
        std::string dollars = std::to_string(tokens * pool.get_token_price());
        std::string token_price = std::to_string(pool.get_token_price());
        
        Status st = pool.sell_tokens(tokens);
        switch (st) {
            case Ok: { std::string text = "Successful! You have sold " + std::to_string(tokens) + " MSHP for " + dollars + " $ at a price of " + token_price + " per.";
                       res.set_content( text, "text/plain");
                       break;
                     }

            case TooMuch: res.set_content("Unsuccessful! You can not sell more 60% of pull.", "text/plain");
                          break;

            case TooLittle: res.set_content("Unsuccessful! You can not sell a negativ and a zero amount.", "text/plain");
                            break;
        }
    });

    std::cout << "Listening on http://localhost:8080\n";

    server.listen("0.0.0.0", 8080);
}
