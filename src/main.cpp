// #include "spann.h"
#include "index.h"
#include "common.h"
#include <boost/program_options.hpp>

#include <iostream>


namespace po = boost::program_options;

int main(int argc, char* argv[]){
    std::string index_file_path;
    std::string query_file_path;
    std::string gt_file_path;
    
    try {
        po::options_description desc("Allowed options");
        desc.add_options()
            ("help", "produce help message")
            ("this_index", po::value<std::string>(&index_file_path), "input index file path")    
            ("query_path", po::value<std::string>(&query_file_path), "query file path")    
            ("gt_path", po::value<std::string>(&gt_file_path), "ground truth file path")    
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
    std::cout << "Index file path: " << index_file_path << std::endl;
    std::cout << "Query file path: " << query_file_path << std::endl;
    std::cout << "Ground truth file path: " << gt_file_path << std::endl;

    std::vector<Query> query_list = std::vector<Query>();
    load_query(query_file_path, query_list, false);

    // std::cout << "Query check: " << std::endl;
    // for (int i=0;i<10;++i) {    
    //     std::cout << query_list[i].query_length << std::endl;
    //     for (int j=0;j<query_list[i].query_length;++j) {
    //         std::cout << query_list[i].ids[j] << " ";
    //     }
    //     std::cout << std::endl;
    // }

    std::vector<GroundtruthItem> groundtruth = std::vector<GroundtruthItem>();
    load_groundtruth(gt_file_path, groundtruth);

    std::cout << "Ground truth check: " << std::endl;
    for (int i=0;i<10;++i) {    
        std::cout << "Qid:" << groundtruth[i].qid << " ";
        for (int j=0;j<groundtruth[i].gts.size();++j) {
            std::cout << groundtruth[i].gts[j] << " ";
        }
        std::cout << std::endl;
    }
    // InvertedIndex *index = new InvertedIndex();
    // index->load_posting_lists(this_index_file_path);



    return 0;
}