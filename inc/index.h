#ifndef _INDEX_H_
#define _INDEX_H_

#include <fstream>
#include <iostream>
#include <vector>
#include <map>

#include "common.h"


class InvertedIndex {
public:
    InvertedIndex() = default;
    virtual ~InvertedIndex() = default;

    void load_posting_lists(const std::string& filename);
    std::vector<Item> retrieve(const Query& query, int topk = 100, double threshold = 0.0);
    void search_posting_lists();
    std::vector<PostingList*> retrieve_posting_lists(const Query& query, int ignore_thr=0);

private:
    std::vector<PostingList> m_posting_lists;
    std::map<int, int> m_key_to_idx;
    unsigned int m_total_docs;

};

#endif