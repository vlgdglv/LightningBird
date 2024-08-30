#ifndef _COMMON_H_
#define _COMMON_H_

#include <vector>

struct Query{
    std::vector<int> ids;
    std::vector<float> values;
    int query_length;
};

struct PostingList{
    std::vector<int> ids;
    std::vector<float> values;
    int key;
};

struct Item{
    int doc_id = -1;
    double scores = 0;
};

void load_query(const std::string& filename, std::vector<Query>& query)  {
    query.clear();
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Unable to open file\n";
        return;
    }

    unsigned int num_queries;
    file.read(reinterpret_cast<char*>(&num_queries), sizeof(num_queries));

    for (unsigned int i = 0; i < num_queries; ++i) {
        unsigned int query_length, ids_size, values_size;
        file.read(reinterpret_cast<char*>(&query_length), sizeof(query_length));
        file.read(reinterpret_cast<char*>(&ids_size), sizeof(ids_size));
        std::vector<int> ids(ids_size / sizeof(int));
        file.read(reinterpret_cast<char*>(ids.data()), ids_size);

        file.read(reinterpret_cast<char*>(&values_size), sizeof(values_size));
        std::vector<float> values(values_size / sizeof(float));
        file.read(reinterpret_cast<char*>(values.data()), values_size);

        Query query_item;
        query_item.query_length = query_length;
        query_item.ids = ids;
        query_item.values = values;
        query.push_back(query_item);
    }
    std::cout << "Total number of queries: " << num_queries << std::endl;
}
#endif