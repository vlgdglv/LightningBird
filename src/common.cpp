#include "common.h"

#include <fstream>  
#include <iostream>
#include <math.h>

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

void load_groundtruth(const std::string& filename, std::map<int, std::vector<int>*>& groundtruth) {
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
        int gts_size, qid;

        file.read(reinterpret_cast<char*>(&qid), sizeof(qid));
        file.read(reinterpret_cast<char*>(&gts_size), sizeof(gts_size));
        std::vector<int> gt_ids(gts_size / sizeof(int));
        file.read(reinterpret_cast<char*>(gt_ids.data()), gts_size);
        groundtruth[qid] = new std::vector<int>(gt_ids);
    }
}

void load_lookup(const std::string& filename, std::map<int, int>& lookup) {
    lookup.clear();
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Unable to open lookup file\n";
        return;
    }

    unsigned int num_entries, dim;
    file.read(reinterpret_cast<char*>(&num_entries), sizeof(num_entries));
    file.read(reinterpret_cast<char*>(&dim), sizeof(dim));
    std::cout << "Number of lookup and dimension: " << num_entries  << ", " << dim << std::endl;

    int id;
    for (int i = 0; i < num_entries; ++i) {
        file.read(reinterpret_cast<char*>(&id), sizeof(id));
        lookup[i] = id;
    }
    std::cout << "Finished loading lookups\n";
}

double Embedding::cosine_similarity(Embedding *e1, Embedding *e2){
    double dot_product = 0;
    double norm1 = 0;
    double norm2 = 0;
    for (int i = 0; i < e1->m_data->size(); ++i) {
        dot_product += e1->m_data->at(i) * e2->m_data->at(i);
        norm1 += e1->m_data->at(i) * e1->m_data->at(i);
        norm2 += e2->m_data->at(i) * e2->m_data->at(i);
    }
    return dot_product / (sqrt(norm1) * sqrt(norm2));
}
double Embedding::euclidean_distance(Embedding *e1, Embedding *e2) {
    double distance = 0;
    for (int i = 0; i < e1->m_data->size(); ++i) {
        distance += (e1->m_data->at(i) - e2->m_data->at(i)) * (e1->m_data->at(i) - e2->m_data->at(i));
    }
    // ignore sqrt
    return distance;
}
double Embedding::inner_product(Embedding *e1, Embedding *e2) {
    double inner_product = 0;
    for (int i = 0; i < e1->m_data->size(); ++i) {
        inner_product += e1->m_data->at(i) * e2->m_data->at(i);
    }
    return inner_product;
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

void evaluation_and_print(std::map<int, std::vector<int>*> groundtruth, std::vector<std::vector<Item>*> result_list, std::map<int, int>& qlookup) {
    int MaxMRRRank = 10;
    std::vector<int> Recall1, Recall5, Recall10, Recall20, Recall100, MAP; 
    double MRR = 0.0;
    for (int i=0; i<result_list.size(); ++i) {
        int qid = qlookup[i];
        std::vector<int> gts = *groundtruth[qid];
        std::vector<Item> items = *result_list[i];

        int length = std::min(MaxMRRRank, (int)items.size());
        
        for (int j=0; j<length; ++j) {
            if (check_is_in(gts, items[j].doc_id)) {
                MRR += (1.0 / (j + 1));
                break;
            }
        }

        for (int j=0; j<items.size(); ++j) {
            if (check_is_in(gts, items[j].doc_id)) {
                if (j==0) Recall1.push_back(qid);
                if (j<5) Recall5.push_back(qid);
                if (j<10) Recall10.push_back(qid);
                if (j<20) Recall20.push_back(qid);
                if (j<100) Recall100.push_back(qid);
                break;             
            }
        }
    }
    double denominator = result_list.size();
    std::cout << "MRR: " << MRR / denominator << std::endl;
    std::cout << "Recall@1: " << Recall1.size() / denominator << std::endl;
    std::cout << "Recall@5: " << Recall5.size() / denominator << std::endl;
    std::cout << "Recall@10: " << Recall10.size() / denominator << std::endl;
    std::cout << "Recall@20: " << Recall20.size() / denominator << std::endl;
    std::cout << "Recall@100: " << Recall100.size() / denominator << std::endl;
}

bool check_is_in(std::vector<int> vec, int id) {
    for(int i=0; i<vec.size(); ++i) {
        if (vec[i] == id) {
            return true;
        }
    }
    return false;
}