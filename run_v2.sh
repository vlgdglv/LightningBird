# !/bin/bash

cd build

DATA_DIR=../data/exp1127_bs
./LightningBird \
    --splade_index_path $DATA_DIR/splade_cut_invert_index.bin \
    --spann_index_path $DATA_DIR/spann_invert_index.bin \
    --query_path $DATA_DIR/query_ids.fbin \
    --posting_path $DATA_DIR/spann_query.bin \
    --query_embedding_path $DATA_DIR/query_769.fbin \
    --corpus_embedding_path $DATA_DIR/corpus_sent_769.fbin \
    --query_lookup_path $DATA_DIR/query_769.ibin \
    --corpus_lookup_path $DATA_DIR/corpus_sent_769.ibin \
    --sptag_time_list_path $DATA_DIR/time_list.bin \
    --gt_path ../data/qrels.bin   \
    --splade_weight 1.0 --spann_weight 1.0 \
    --distance_func euclidean_opposite \
    --topk 100 