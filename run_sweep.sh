#!/bin/bash

cd build

BASE_DIR=/datacosmos/User/baoht/onesparse2/marcov2

splade_weights=(0 0.2 0.4 0.5 0.6 0.8 1.0)

for splade_weight in "${splade_weights[@]}"; do
    spann_weight=$(awk "BEGIN {print 1 - $splade_weight}")

    echo "Running experiment with splade_weight=$splade_weight, spann_weight=$spann_weight"

    DATA_DIR=/datacosmos/User/baoht/onesparse2/hybridsearch/lightningbrids/bs4lb
    ./LightningBird \
        --splade_index_path /datacosmos/User/baoht/onesparse2/hybridsearch/bm25/index/marco/marco_bm25.bin \
        --spann_index_path $DATA_DIR/spann_inverted_index.bin \
        --query_path $DATA_DIR/query_ids.fbin \
        --posting_path $DATA_DIR/spann_query.bin \
        --query_embedding_path $DATA_DIR/marco_query.fbin \
        --corpus_embedding_path $DATA_DIR/marco_doc_with_id769.fbin \
        --query_lookup_path $DATA_DIR/marco_query.ibin \
        --corpus_lookup_path $DATA_DIR/marco_doc_with_id769.ibin \
        --sptag_time_list_path $DATA_DIR/time_list.bin \
        --gt_path /datacosmos/User/baoht/onesparse2/hybridsearch/data/msmarco/qrels.bin    \
        --splade_weight $splade_weight --spann_weight $spann_weight \
        --topk 100 --distance_func euclidean_reciprocal

    echo "Experiment completed for splade_weight=$splade_weight, spann_weight=$spann_weight"
    echo "--------------------------------------"
done

