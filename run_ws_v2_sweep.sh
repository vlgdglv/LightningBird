#!/bin/bash

cd build

BASE_DIR=/datacosmos/local/User/baoht/onesparse2/marcov2

# 定义 splade_weight 取值
splade_weights=(0 0.2 0.4 0.5 0.6 0.8 1.0)

# 遍历不同的权重值
for splade_weight in "${splade_weights[@]}"; do
    spann_weight=$(awk "BEGIN {print 1 - $splade_weight}")  # 使用 awk 计算浮点数

    echo "Running experiment with splade_weight=$splade_weight, spann_weight=$spann_weight"

    ./LightningBird \
        --splade_index_path $BASE_DIR/warehouse/splade_index/cotrain_exp0117/splade_full.bin \
        --spann_index_path $BASE_DIR/spann/bs_SimANS_8_invertedIndex/inverted_index.bin \
        --query_path $BASE_DIR/warehouse/splade_index/cotrain_exp0117/splade_query.bin \
        --posting_path $BASE_DIR/spann/bs_SimANS_8_invertedIndex/spann_query.bin \
        --query_embedding_path $BASE_DIR/embeddings/bs_SimANS_36k/query_embeddings_rerange.fbin \
        --corpus_embedding_path $BASE_DIR/embeddings/bs_SimANS_36k/test_corpus.bin \
        --query_lookup_path $BASE_DIR/embeddings/bs_SimANS_36k/qlookup_rerange.ibin \
        --corpus_lookup_path $BASE_DIR/embeddings/bs_SimANS_36k/test_corpus.ibin \
        --sptag_time_list_path $BASE_DIR/spann/bs_SimANS_8_invertedIndex/time_list.bin \
        --gt_path $BASE_DIR/data/qrels_test.bin \
        --splade_weight $splade_weight --spann_weight $spann_weight \
        --distance_func inner_product \
        --topk 100 

    echo "Experiment completed for splade_weight=$splade_weight, spann_weight=$spann_weight"
    echo "--------------------------------------"
done
