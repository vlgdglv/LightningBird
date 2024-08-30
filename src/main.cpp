// #include "spann.h"
#include "index.h"

#include <boost/program_options.hpp>

#include <iostream>


namespace po = boost::program_options;

int main(int argc, char* argv[]){
    std::string this_index_file_path;
    try {
        po::options_description desc("Allowed options");
        desc.add_options()
            ("help", "produce help message")
            ("this_index", po::value<std::string>(&this_index_file_path), "input index file path")    
        ;
        po::variables_map vm;
        po::store(po::parse_command_line(argc, argv, desc), vm);
        po::notify(vm);
        if (vm.count("help")) {
            std::cout << desc << std::endl;
            return 0;
        }
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
        return 1;
    } catch (...) {
        std::cerr << "Exception of unknown type!\n";
        return 1;
    }

    std::cout << "Test main" << std::endl;
    std::cout << "this_index_file_path: " << this_index_file_path << std::endl;
    
    std::vector<std::vector<int>> index_ids = std::vector<std::vector<int>>();
    std::vector<std::vector<float>> index_values = std::vector<std::vector<float>>();
    read_index_file(this_index_file_path, index_ids, index_values);
    return 0;
}