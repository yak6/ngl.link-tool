//  █▄░█ █▀▀ █░░ ▄▄ █▀ █▀█ ▄▀█ █▀▄▀█  
// █░▀█ █▄█ █▄▄ ░░ ▄█ █▀▀ █▀█ █░▀░█
// -> https://github.com/yak6/ngl-spam <-

#include <iostream>
#include <thread>
#include <vector>
#include <string>
#include <list>
#include <fstream>
#include <unistd.h>

#include "ngl.h"
#include "json.hpp"

using json = nlohmann::json;

json read_json_file(const std::string& file_path) {
    std::ifstream file(file_path);
    if (!file.is_open()) {
        throw std::runtime_error("[E] Couldn't open file: " + file_path);
    }
    json j;
    file >> j;  
    return j;
}
std::list<std::string> stl(std::string str) { 
    std::list<std::string> ls; 
    ls.push_back(str);
    return ls; 
}
int main(int argc, char* argv[]) {
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--help") {
            std::cout << "Usage:\n"
                      << "\t--help                    Show this help message.\n"
                      << "\t--create                  Create a sample .json config file.\n"
                      << "\t--load <config>           Run spammer using config file.\n" 
                      << "\t--ask <target> <question> Ask user a single question.\n"
                      << "\t--fuck <target>           Send 50 messages with random characters.\n"
                      << "Config file:\n"
                      << "\ttarget - Targets, leave one target or more. Remember, it must be in an array.\n"
                      << "\tmethod - There is two options - normal (slower) and thread (faster).\n"
                      << "\tquestion - Messages, must be in array too. Program picks random question from herre when sending.\n"
                      << "\ttimes - How many messages should be sent for every single user.\n" 
                      << "\tuseragents - Path to useragents file. (each useragent every new line)"
                      << std::endl;
            return 0; 
        }
        else if (arg == "--ask") { 
            if (argc < 4) { 
                std::cerr << "[Error] Wrong usage. Use --help" << std::endl;
                return -1;
            }
            std::string target = argv[2];
            std::string message = argv[3];

            send(target, message, random_deviceId(), "Mozilla/5.0 (Windows NT 6.1) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/40.0.2214.115 Safari/537.36");
            std::cout << "[Ask] Asked " << argv[2] << ": " << argv[3] << " [🗸]"<<std::endl;
            return 0;
        }
        else if (arg == "--fuck") { 
            std::string target = argv[2];
            if (argc < 3) {
                std::cerr << "[Error] Missing target's username." << std::endl;
                return -1;
            }
            for (int i = 0; i < 50; i++) { 
                send(target, random_string(100), random_deviceId(), fast_useragent());
                std::cout << "[Fuck] " << i+1 << "/" << 50 << " (" << (static_cast<double>(i+1)/50)*100 << "%)" << " [🗸]" <<std::endl;
            }
            return 0;
        }
        else if (arg == "--create") { 
            json config;
            
            config["target"] = {"target1", "target2"};
            config["question"] = {"hi", "how are you", "whats up!?"};
            config["method"] = "thread";
            config["useragents"] = "useragents.txt";
            config["times"] = 50;

            std::ofstream config_file("config.json");
            if (config_file.is_open()) {
                config_file << config.dump(4); 
                config_file.close();
                std::cout << "[Create] Configuration file has been created! [🗸]" << std::endl;
                return 0;
            } else {
                std::cerr << "[Error] Couldn't open file! [✗]" << std::endl;
                return -1;
            }
        }
        else if (arg == "--load") {
            if (argc < 3) {
                std::cerr << "[Error] Missing config path! [✗]" << std::endl;
                return -1;
            }
            const std::string config = argv[2];
            json j = read_json_file(config);

            std::list<std::string> targets = j["target"];
            std::list<std::string> questions = j["question"];

            std::string method = j["method"];
            std::string useragents = j["useragents"];

            const int times = j["times"];

            int counter = 0;

            std::size_t lentargets = targets.size();

            for (const auto& target : targets) {
                ++counter; 
                spam(target, questions, useragents, times, method, true);
                std::cout << "[Spam] " << counter << "/" << lentargets << " (" << (static_cast<double>(counter)/lentargets)*100 << "%) [🗸]" << std::endl; 
            }
            return 0;
        } 
    }

    std::cout << "Wrong usage. Use --help to get more information." << std::endl;
    return -1;
}
