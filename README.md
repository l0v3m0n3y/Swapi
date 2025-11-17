# Swapi
api for swapi.dev star wars data site
# main
```cpp
#include "Swapi.h"
#include <iostream>

int main() {
   Swapi api;

    auto film = api.get_films_by_id(1).then([](json::value result) {
        std::cout << "Search results: " << result.serialize() << std::endl;
    });
    film.wait();
    
    return 0;
}
```

# Launch (your script)
```
g++ -std=c++11 -o main main.cpp -lcpprest -lssl -lcrypto -lpthread -lboost_system -lboost_chrono -lboost_thread
./main
```
