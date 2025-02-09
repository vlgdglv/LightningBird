#ifndef _COMMON_H_
#define _COMMON_H_

#include <vector>
#include <iostream>
#include <map>
#include <functional> 
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
    bool operator<(const Item& other) const {
        return scores < other.scores;
    }
};

class Embedding{
public:
    Embedding(std::vector<float> *data): m_data(data) {};
    virtual ~Embedding() = default;

    static double cosine_similarity(Embedding *e1, Embedding *e2);
    static double euclidean_distance(Embedding *e1, Embedding *e2);
    static double inner_product(Embedding *e1, Embedding *e2);
// private:
    std::vector<float> *m_data;
};



double euclidean_distance_score_reciprocal(Embedding *e1, Embedding *e2);

double euclidean_distance_score_opposite(Embedding *e1, Embedding *e2);

double inner_product_score(Embedding *e1, Embedding *e2);

std::function<double(Embedding*, Embedding*)> select_distance_function(const std::string& dis_func);


class VectorSet {
public:
    VectorSet() = default;
    VectorSet(const std::string& filename);
    virtual ~VectorSet() = default;
    Embedding* get(unsigned int idx) { return &m_vectors[idx]; }
private:
    unsigned int m_vector_num;
    unsigned int m_vector_dim;
    std::vector<Embedding> m_vectors;
};

class MaxHeap {
private:
    std::vector<Item> heap;

    // Heapify up: Maintain the heap property after insertion
    void heapify_up(int index) {
        while (index > 0) {
            int parent = (index - 1) / 2;
            if (heap[index].scores > heap[parent].scores) {
                std::swap(heap[index], heap[parent]);
                index = parent;
            } else {
                break;
            }
        }
    }

    // Heapify down: Maintain the heap property after deletion
    void heapify_down(int index) {
        int size = heap.size();
        while (true) {
            int left = 2 * index + 1;
            int right = 2 * index + 2;
            int largest = index;

            if (left < size && heap[left].scores > heap[largest].scores) {
                largest = left;
            }
            if (right < size && heap[right].scores > heap[largest].scores) {
                largest = right;
            }
            if (largest != index) {
                std::swap(heap[index], heap[largest]);
                index = largest;
            } else {
                break;
            }
        }
    }

public:
    // Insert a new item into the heap
    void insert(Item& item) {
        heap.push_back(item);
        heapify_up(heap.size() - 1);
    }

    // Remove and return the top item (maximum score)
    Item extract_max() {
        if (heap.empty()) {
            throw std::runtime_error("Heap is empty");
        }
        Item max_item = heap[0];
        heap[0] = heap.back();
        heap.pop_back();
        heapify_down(0);
        return max_item;
    }

    // Peek at the top item without removing it
    const Item& peek() const {
        if (heap.empty()) {
            throw std::runtime_error("Heap is empty");
        }
        return heap[0];
    }

    // Check if the heap is empty
    bool empty() const {
        return heap.empty();
    }

    // Get the size of the heap
    size_t size() const {
        return heap.size();
    }

    std::vector<Item>* get_data() { return &heap; }

    std::vector<Item>* get_data(int top) {
        std::vector<Item>* data = new std::vector<Item>();
        for (int i=0; i<std::min(top, (int)heap.size()); ++i) {
            data->push_back(extract_max());
        }
        return data;
    }

    Item& operator [](int idx) { return heap[idx]; }
};

bool load_query(const std::string& filename, std::vector<Query>& query, bool has_value);

bool load_groundtruth(const std::string& filename, std::map<int, std::vector<int>*>& groundtruth);

bool load_lookup(const std::string& filename, std::map<int, int>& lookup);

void evaluation_and_print(std::map<int, std::vector<int>*> groundtruth, std::vector<std::vector<Item>*> result_list, std::map<int, int>& qlookup);

bool check_is_in(std::vector<int> vec, int id);

double load_sptag_time(const std::string& filename);
// void merge_posting_lists(std::vector<PostingList*>& pl1, Query& q1, 
//                         std::vector<PostingList*>& pl2, Query& q2);

void output(const std::string msg, bool flg);
#endif