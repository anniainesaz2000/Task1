#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include "../include/WareHouse.h"


// Split a string into a vector of substrings based on a delimiter
std::vector<std::string> split(const std::string& s, char delimiter) {
    std::vector<std::string> tokens;
    std::istringstream ss(s);
    std::string token;

    while (std::getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }

    return tokens;
}


class ConfigParser {
public:
    std::vector<Customer> customers;
    std::vector<Volunteer> volunteers;

    void parseFile(const std::string& filename) {
        std::ifstream file(filename);
        std::string line;

        while (std::getline(file, line)) {
            if (line.empty() || line[0] == '#') {
                // Skip empty lines and comments
                continue;
            }

            std::vector<std::string> tokens = split(line, ' ');

            if (tokens[0] == "customer") {
                parseCustomer(tokens);
            } else if (tokens[0] == "volunteer") {
                parseVolunteer(tokens);
            } else {
                std::cerr << "Unknown type: " << tokens[0] << std::endl;
            }
        }
    }

private:
    void parseCustomer(const std::vector<std::string>& tokens) {
        std::string name = tokens[1];
        std::string customerType = tokens[2];
        int distance = std::stoi(tokens[3]);
        int maxOrders = std::stoi(tokens[4]);

        customers.emplace_back(name, customerType, distance, maxOrders);
    }

    void parseVolunteer(const std::vector<std::string>& tokens) {
        std::string name = tokens[1];
        std::string role = tokens[2];
        int coolDown = std::stoi(tokens[3]);
        int maxDistance, distancePerStep, maxOrders;

        if (role == "driver") {
            maxDistance = std::stoi(tokens[4]);
            distancePerStep = std::stoi(tokens[5]);
            maxOrders = -1;  // Unlimited max orders for drivers
        } else {
            maxOrders = std::stoi(tokens[4]);
            maxDistance = -1;  // Unlimited max distance for collectors
            distancePerStep = -1;
        }

        volunteers.emplace_back(name, role, coolDown, maxDistance, distancePerStep, maxOrders);
    }
};
