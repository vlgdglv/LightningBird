#!/bin/bash

cd build

BASE_DIR=/datacosmos/User/baoht/onesparse2/marcov2

splade_weights=(0 0.2 0.4 0.5 0.6 0.8 1.0) #

for splade_weight in "${splade_weights[@]}"; do
    spann_weight=$(awk "BEGIN {print 1 - $splade_weight}")

    echo "Running experiment with splade_weight=$splade_weight, spann_weight=$spann_weight"

    NAME=exp1218_hnret_osreg_osdisce
    DATA_DIR=/datacosmos/User/baoht/onesparse2/hybridsearch/lightningbrids/$NAME
    ./LightningBird \
        --splade_index_path $DATA_DIR/splade_index.bin \
        --spann_index_path $DATA_DIR/spann_invert_index.bin \
        --query_path $DATA_DIR/splade_query.bin \
        --posting_path $DATA_DIR/spann_query.bin \
        --query_embedding_path $DATA_DIR/query_769.fbin \
        --corpus_embedding_path $DATA_DIR/corpus_sent_769.fbin \
        --query_lookup_path $DATA_DIR/query_769.ibin \
        --corpus_lookup_path $DATA_DIR/corpus_sent_769.ibin \
        --sptag_time_list_path $DATA_DIR/time_list.bin \
        --gt_path /datacosmos/User/baoht/onesparse2/hybridsearch/data/msmarco/qrels.bin   \
        --splade_weight $splade_weight --spann_weight $spann_weight \
        --distance_func euclidean_opposite \
        --topk 100 

    echo "Experiment completed for splade_weight=$splade_weight, spann_weight=$spann_weight"
    echo "--------------------------------------"
done
