#ifndef _COMMON_H_
#define _COMMON_H_

#include <vector>
#include <iostream>
#define INF 999999999

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

class Embedding{
public:
    Embedding(std::vector<float> *data): m_data(data) {};
    virtual ~Embedding() = default;

private:
    std::vector<float> *m_data;
};

class VectorSet {
public:
    VectorSet() = default;
    VectorSet(const std::string& filename);
    virtual ~VectorSet() = default;
    
private:
    unsigned int m_vector_num;
    unsigned int m_vector_dim;
    std::vector<Embedding> m_vectors;
};


void load_query(const std::string& filename, std::vector<Query>& query, bool has_value);

void load_groundtruth(const std::string& filename, std::vector<GroundtruthItem>& groundtruth);

// void merge_posting_lists(std::vector<PostingList*>& pl1, Query& q1, 
//                         std::vector<PostingList*>& pl2, Query& q2);

#endif