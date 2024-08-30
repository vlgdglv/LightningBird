#include "index.h"
#include "common.h"

#include<algorithm>

InvertIndex::InvertIndex() {}

InvertIndex::~InvertIndex() {}

void InvertIndex::load_posting_lists(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Unable to open file\n";
        return;
    }

    unsigned int num_keys, total_docs;
    file.read(reinterpret_cast<char*>(&num_keys), sizeof(num_keys));
    file.read(reinterpret_cast<char*>(&total_docs), sizeof(total_docs));

    for (unsigned int i = 0; i < num_keys; ++i) {
        unsigned int key, ids_size, values_size;
    
        file.read(reinterpret_cast<char*>(&key), sizeof(key));
        file.read(reinterpret_cast<char*>(&ids_size), sizeof(ids_size));

        std::vector<int> ids(ids_size / sizeof(int));
        file.read(reinterpret_cast<char*>(ids.data()), ids_size);

        file.read(reinterpret_cast<char*>(&values_size), sizeof(values_size));
        std::vector<float> values(values_size / sizeof(float));
        file.read(reinterpret_cast<char*>(values.data()), values_size);

        // std::cout << "Key: " << key << "\nIDs: ";
        // for (int id : ids) {
        //     std::cout << id << " ";
        // }
        // std::cout << "\nValues: ";
        // for (float value : values) {
        //     std::cout << value << " ";
        // }
        // std::cout << std::endl;

        PostingList posting_list;
        posting_list.key = key;
        posting_list.ids = ids;
        posting_list.values = values;
        m_posting_lists.push_back(posting_list);
        m_key_to_idx[key] = i;
    }
    m_total_docs = total_docs;
    file.close();

    std::cout << "Total number of keys: " << num_keys << "\nTotal number of documents: " << total_docs << std::endl;
}

std::vector<Item> InvertIndex::retrieve(const Query& query, int topk = 100, double threshold = 0.0) {
    int query_len = query.query_length;
    int query_ids = 0; 
    double query_values = 0;
    PostingList tmp_posting_list;

    std::vector<Item> result;
    std::map<int, int> doc2idx;

    for (int i=0; i<query_len; ++i) {
        query_ids = query.ids[i];
        query_values = query.values[i];
        
        int posting_idx = m_key_to_idx[query_ids];

        tmp_posting_list = m_posting_lists[posting_idx];
        int tmp_posting_list_len = tmp_posting_list.ids.size();
        for (int j=0; j< tmp_posting_list_len; ++j) {
            int doc_idx = tmp_posting_list.ids[j];
            double doc_value = tmp_posting_list.values[j];

            if (doc2idx.find(doc_idx) == doc2idx.end()) {
                Item item;
                item.doc_id = doc_idx;
                item.scores = query_values * doc_value;
                doc2idx[doc_idx] = result.size();
                result.push_back(item);
            } else {
                result[doc2idx[doc_idx]].scores += query_values * doc_value;
            }
        }
    }

    std::sort(result.begin(), result.end(), [](const Item& a, const Item& b) { return a.scores > b.scores; });
    
    std::vector<Item> result_topk;
    int result_length = std::min(topk, (int)result.size());
    for (int i=0; i<result_length; ++i) {
        if (result[i].scores < threshold) break;
        result_topk.push_back(result[i]);
    }
    return result;
}


void InvertIndex::search_posting_lists() {

}