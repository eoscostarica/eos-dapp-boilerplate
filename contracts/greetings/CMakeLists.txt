cmake_minimum_required(VERSION 3.5)
project(greetings_example VERSION 1.0.0)

find_package(eosio.cdt)

### Only generate the wasm
#add_executable( greetings.wasm greetings.cpp )

### Generate the wasm and abi
add_contract( greetings greetings greetings.cpp )
