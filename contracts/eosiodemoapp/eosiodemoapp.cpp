#include <eosiolib/eosio.hpp>
#include <eosiolib/print.hpp>

using namespace eosio;
using namespace std;

CONTRACT eosiodemoapp : public eosio::contract {

  public:
    using contract::contract;

    ACTION greet(name user, string msg ) {
      require_auth(user);
      greetings_table _greetings(_code, _code.value);

      _greetings.emplace(user, [&](auto &row) {
        row.id = _greetings.available_primary_key();
        row.user = user;
        row.msg = msg;
      });
    }

  private:
    TABLE greeting {
      uint64_t id;
      name user;
      std::string msg;

      uint64_t primary_key() const { return id; }
    };

    typedef eosio::multi_index<"greetings"_n, eosiodemoapp::greeting> greetings_table;
};
EOSIO_DISPATCH( eosiodemoapp, (greet) );
