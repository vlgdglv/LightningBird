#include "common.h"

#include <fstream>  
#include <iostream>

void load_query(const std::string& filename, std::vector<Query>& query, bool has_value) {
    query.clear();
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Unable to open query file\n";
        return;
    }

    unsigned int num_queries;
    file.read(reinterpret_cast<char*>(&num_queries), sizeof(num_queries));
    std::cout << "Total number of queries: " << num_queries << std::endl;

    for (unsigned int i = 0; i < num_queries; ++i) {
        Query query_item;
        unsigned int query_length, ids_size, values_size;
        
        // file.read(reinterpret_cast<char*>(&qid), sizeof(qid));
        file.read(reinterpret_cast<char*>(&query_length), sizeof(query_length));
        file.read(reinterpret_cast<char*>(&ids_size), sizeof(ids_size));
        std::vector<int> ids(ids_size / sizeof(int));
        file.read(reinterpret_cast<char*>(ids.data()), ids_size);
        
        query_item.query_length = query_length;
        query_item.ids = ids;

        if (has_value) {
            file.read(reinterpret_cast<char*>(&values_size), sizeof(values_size));
            std::vector<float> values(values_size / sizeof(float));
            file.read(reinterpret_cast<char*>(values.data()), values_size);        
            query_item.values = values;
        }
        query.push_back(query_item);
    }
}

void load_groundtruth(const std::string& filename, std::vector<GroundtruthItem>& groundtruth) {
    groundtruth.clear();
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Unable to open groundtruth file\n";
        return;
    }

    unsigned int num_gts;
    file.read(reinterpret_cast<char*>(&num_gts), sizeof(num_gts));
    std::cout << "Number of groundtruths: " << num_gts << std::endl;

    for (unsigned int i = 0; i < num_gts; ++i) {
        GroundtruthItem gt_item;
        unsigned int gts_length, gts_size, qid;

        file.read(reinterpret_cast<char*>(&qid), sizeof(qid));
        file.read(reinterpret_cast<char*>(&gts_length), sizeof(gts_length));
        file.read(reinterpret_cast<char*>(&gts_size), sizeof(gts_size));
        std::vector<int> gt_ids(gts_size / sizeof(int));
        file.read(reinterpret_cast<char*>(gt_ids.data()), gts_size);
        gt_item.gts = gt_ids;
        gt_item.qid = qid;
        groundtruth.push_back(gt_item);
    }
    
}


VectorSet::VectorSet(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Unable to open file\n";
        return;
    }

    file.read(reinterpret_cast<char*>(&m_vector_num), sizeof(m_vector_num));
    file.read(reinterpret_cast<char*>(&m_vector_dim), sizeof(m_vector_dim));
    std::cout << "Number of vectors: " << m_vector_num << std::endl;
    std::cout << "Vector dimension: " << m_vector_dim << std::endl;

    for (unsigned int i = 0; i < m_vector_num; ++i) {
        std::vector<float> data(m_vector_dim);
        file.read(reinterpret_cast<char*>(data.data()), m_vector_dim * sizeof(float));
        m_vectors.push_back(Embedding(new std::vector<float>(data)));
    }
    std::cout << "Finished loading vectors\n";
}