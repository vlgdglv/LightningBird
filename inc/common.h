#ifndef _COMMON_H_
#define _COMMON_H_

#include <vector>
#include <iostream>

struct Query{
    std::vector<int> ids;
    std::vector<float> values;
    unsigned int query_length, qid;
};

struct PostingList{
    std::vector<int> ids;
    std::vector<float> values;
    int key;
};

struct GroundtruthItem{
    unsigned int qid;
    std::vector<int> gts;
};

struct Item{
    int doc_id = -1;
    double scores = 0;
};

void load_query(const std::string& filename, std::vector<Query>& query, bool has_value);

void load_groundtruth(const std::string& filename, std::vector<GroundtruthItem>& groundtruth);

#endif