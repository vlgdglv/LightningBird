// #include "spann.h"
#include "index.h"
#include "common.h"
#include <boost/program_options.hpp>

#include <iostream>
#include <chrono>

namespace po = boost::program_options;

int main(int argc, char* argv[]){
    std::string index_file_path;
    std::string splade_index_file_path, spann_index_file_path;
    std::string query_file_path, query_embedding_path, posting_file_path;
    std::string corpus_embedding_path;
    std::string gt_file_path;
    
    try {
        po::options_description desc("Allowed options");
        desc.add_options()
            ("help", "produce help message")
            ("index_file_path", po::value<std::string>(&index_file_path), "input index file path")  
            ("splade_index_path", po::value<std::string>(&splade_index_file_path), "input index file path")    
            ("spann_index_path", po::value<std::string>(&spann_index_file_path), "input index file path")
            ("query_path", po::value<std::string>(&query_file_path), "query file path")    
            ("query_embedding_path", po::value<std::string>(&query_embedding_path), "query file path")
            ("corpus_embedding_path", po::value<std::string>(&corpus_embedding_path), "query file path")
            
            ("posting_path", po::value<std::string>(&posting_file_path), "query file path")    
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

    // std::cout << "Test main" << std::endl;
    // std::cout << "Index file path: " << index_file_path << std::endl;
    // std::cout << "Query file path: " << query_file_path << std::endl;
    // std::cout << "Ground truth file path: " << gt_file_path << std::endl;

    std::cout << "Load query ids..." << std::endl;
    std::vector<Query> query_list = std::vector<Query>();
    load_query(query_file_path, query_list, true);

    std::cout << "Load posting list..." << std::endl;
    std::vector<Query> posting_list = std::vector<Query>();
    load_query(posting_file_path, posting_list, true);

    // std::cout << "Query check: " << std::endl;
    // for (int i=0;i<1;++i) {    
    //     std::cout << query_list[i].query_length << std::endl;
    //     for (int j=0;j<query_list[i].query_length;++j) {
    //         std::cout << query_list[i].ids[j] << ", " << query_list[i].values[j] << "; ";
    //     }
    //     std::cout << std::endl;
    // }
    // std::cout << "Posting check: " << std::endl;
    // for (int i=0;i<1;++i) {    
    //     std::cout << posting_list[i].query_length << std::endl;
    //     for (int j=0;j<posting_list[i].query_length;++j) {
    //         std::cout << posting_list[i].ids[j] << ", " << posting_list[i].values[j] << "; ";
    //     }
    //     std::cout << std::endl;
    // }

    std::cout << "Load splade index" << std::endl;
    InvertedIndex *splade_index = new InvertedIndex();
    splade_index->load_posting_lists(splade_index_file_path);

    std::cout << "Load spann index" << std::endl;
    InvertedIndex *spann_index = new InvertedIndex();
    spann_index->load_posting_lists(spann_index_file_path);

    // std::vector<GroundtruthItem> groundtruth = std::vector<GroundtruthItem>();
    // load_groundtruth(gt_file_path, groundtruth);

    // std::cout << "Ground truth check: " << std::endl;
    // for (int i=0;i<10;++i) {    
    //     std::cout << "Qid:" << groundtruth[i].qid << " ";
    //     for (int j=0;j<groundtruth[i].gts.size();++j) {
    //         std::cout << groundtruth[i].gts[j] << " ";
    //     }
    //     std::cout << std::endl;
    // }
    
    // VectorSet *query_embedding = new VectorSet(query_embedding_path);
    // VectorSet *corpus_embedding = new VectorSet(corpus_embedding_path);

    auto batchstart = std::chrono::high_resolution_clock::now();
    for (int i=0;i<query_list.size();++i) {
        std::vector<PostingList*> query_posting_lists = splade_index->retrieve_posting_lists(query_list[i]);
        std::vector<PostingList*> spann_posting_lists = spann_index->retrieve_posting_lists(posting_list[i]);
        
        int current_docid = -1;

        std::vector<int> splade_cursors = std::vector<int>(query_posting_lists.size(), 0);
        std::vector<int> spann_cursors = std::vector<int>(spann_posting_lists.size(), 0);

        std::vector<int> result = std::vector<int>();
        while(1){
            int splade_min_id = INF, splade_min_value = 0;
            std::vector<int> splade_posting_ids = std::vector< int>();
            int length = query_posting_lists.size();
            for(int j=0; j<length; ++j){
                int cursor = splade_cursors[j], posting_list_size = query_posting_lists[j]->ids.size();
                while (cursor < posting_list_size && query_posting_lists[j]->ids[cursor] < current_docid)
                    ++cursor;
                splade_cursors[j] = cursor;
                if (cursor < posting_list_size){
                    if (query_posting_lists[j]->ids[cursor] < splade_min_id){
                        splade_min_id = query_posting_lists[j]->ids[cursor];
                        splade_min_value = query_posting_lists[j]->values[cursor] * query_list[i].values[j];
                        splade_posting_ids.clear();
                        splade_posting_ids.push_back(j);
                    }else if (query_posting_lists[j]->ids[cursor] == splade_min_id){
                        splade_min_value += query_posting_lists[j]->values[cursor] * query_list[i].values[j];
                        splade_posting_ids.push_back(j);
                    }
                }
            }

            int spann_min_id = INF, spann_min_value = 0;
            std::vector<int> spann_posting_ids = std::vector< int>();
            length = spann_posting_lists.size();
            for(int j=0; j<length; ++j){
                int cursor = spann_cursors[j], posting_list_size = spann_posting_lists[j]->ids.size();
                while (cursor < posting_list_size && spann_posting_lists[j]->ids[cursor] < current_docid)
                    ++cursor;
                spann_cursors[j] = cursor;
                if (cursor < posting_list_size){
                    if (spann_posting_lists[j]->ids[cursor] < spann_min_id){
                        spann_min_id = spann_posting_lists[j]->ids[cursor];
                        // spann_min_value = posting_posting_lists[j]->values[cursor];
                        spann_posting_ids.clear();
                        spann_posting_ids.push_back(j);
                    }else if (spann_posting_lists[j]->ids[cursor] == spann_min_id){
                        // spann_min_value += posting_posting_lists[j]->values[cursor];
                        spann_posting_ids.push_back(j);
                    }
                }
            }

            if (splade_min_id == INF && spann_min_id == INF){
                break;
            }

            if (splade_min_id == spann_min_id){
                // add candidate
                result.push_back(splade_min_id);
                current_docid = splade_min_id;
                for (int j=0;j<splade_posting_ids.size();++j)  splade_cursors[splade_posting_ids[j]] += 1;
                for (int j=0;j<spann_posting_ids.size();++j)  spann_cursors[spann_posting_ids[j]] += 1;
            }else{
                current_docid = std::max(splade_min_id, spann_min_id);
            }
        }

        // for (int i=0;i < 20; ++i) {
        //     std::cout << result[i] << " ";
        // }
        // std::cout << std::endl;
    }
    auto batchend = std::chrono::high_resolution_clock::now();
    auto batchtime = std::chrono::duration_cast<std::chrono::milliseconds>(batchend - batchstart);
    std::cout << "Batch time: " << batchtime.count() << " ms, Average time: " << 1.0 * batchtime.count() / query_list.size() << " ms" << std::endl;
    return 0;
}