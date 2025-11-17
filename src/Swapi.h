#ifndef SWAPI_H
#define SWAPI_H

#include <cpprest/http_client.h>
#include <cpprest/json.h>
#include <pplx/pplx.h>
#include <iostream>
#include <string>
#include <map>

using namespace web;
using namespace web::http;
using namespace web::http::client;

class Swapi {
private:
    std::string api_base = "https://swapi.dev/api";
    http_client_config client_config;
    http_request create_request(const std::string& endpoint, const std::string& method, const std::string& data = "") {
        http_request request;
        
        if (method == "GET") {
            request.set_method(methods::GET);
        } else if (method == "POST") {
            request.set_method(methods::POST);
        } else if (method == "PUT") {
            request.set_method(methods::PUT);
        } else if (method == "DEL") {
            request.set_method(methods::DEL);
        }
        
        request.set_request_uri(utility::conversions::to_string_t(endpoint));
        
        // Set headers
        request.headers().add(U("Host"), U("swapi.dev"));
        request.headers().add(U("content-type"), U("application/json"));
        request.headers().add(U("User-Agent"), U("Mozilla/5.0 (X11; Linux x86_64; rv:109.0) Gecko/20100101 Firefox/115.0"));
        
        if (!data.empty() && (method == "POST" || method == "PUT")) {
            request.set_body(data);
        }
        
        return request;
    }
    
    pplx::task<json::value> make_api_call(const std::string& endpoint, const std::string& method, const std::string& data = "") {
        http_client client(utility::conversions::to_string_t(api_base), client_config);
        auto request = create_request(endpoint, method, data);

        return client.request(request)
            .then([](http_response response) {
                if (response.status_code() == status_codes::OK) {
                    return response.extract_json();
                } else {
                    json::value error_obj;
                    error_obj[U("error")] = json::value::string(
                        U("HTTP Error: ") + utility::conversions::to_string_t(std::to_string(response.status_code())));
                    error_obj[U("success")] = json::value::boolean(false);
                    return pplx::task_from_result(error_obj);
                }
            })
            .then([](pplx::task<json::value> previousTask) {
                try {
                    return previousTask.get();
                } catch (const std::exception& e) {
                    json::value error_obj;
                    error_obj[U("error")] = json::value::string(
                        U("Exception: ") + utility::conversions::to_string_t(e.what()));
                    error_obj[U("success")] = json::value::boolean(false);
                    return error_obj;
                }
            });
    }

    std::string build_query_params(const std::map<std::string, std::string>& params) {
        if (params.empty()) return "";
        
        std::string query = "?";
        bool first = true;
        for (const auto& param : params) {
            if (!param.second.empty()) {
                if (!first) query += "&";
                query += param.first + "=" + param.second;
                first = false;
            }
        }
        return query;
    }

public:
    Swapi() {
        client_config.set_validate_certificates(false);
    }

    pplx::task<json::value> get_films() {
        return make_api_call("/films","GET");
    }

    pplx::task<json::value> get_people() {
        return make_api_call("/people","GET");
    }

    pplx::task<json::value> get_planets() {
        return make_api_call("/planets","GET");
    }

    pplx::task<json::value> get_species() {
        return make_api_call("/species","GET");
    }

    pplx::task<json::value> get_starships() {
        return make_api_call("/starships","GET");
    }

    pplx::task<json::value> get_vehicles() {
        return make_api_call("/vehicles","GET");
    }

    pplx::task<json::value> get_films_by_id(int film_id) {
        return make_api_call("/films/" + std::to_string(film_id),"GET");
    }

    pplx::task<json::value> get_people_by_id(int people_id) {
        return make_api_call("/people/" + std::to_string(people_id),"GET");
    }

    pplx::task<json::value> get_planets_by_id(int planets_id) {
        return make_api_call("/planets/" + std::to_string(planets_id),"GET");
    }

    pplx::task<json::value> get_species(int species_id) {
        return make_api_call("/species/" + std::to_string(species_id),"GET");
    }

    pplx::task<json::value> get_starships(int starships_id) {
        return make_api_call("/starships/" + std::to_string(starships_id),"GET");
    }

    pplx::task<json::value> get_vehicles(int vehicles_id) {
        return make_api_call("/vehicles/" + std::to_string(vehicles_id),"GET");
    }
};

#endif
