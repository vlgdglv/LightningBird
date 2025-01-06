# !/bin/bash

cd build

DATA_DIR=../data/bs4lb
./LightningBird \
    --splade_index_path $DATA_DIR/bm25_inverted_index.bin \
    --spann_index_path $DATA_DIR/spann_inverted_index.bin \
    --query_path $DATA_DIR/query_ids.fbin \
    --posting_path $DATA_DIR/spann_query.bin \
    --query_embedding_path $DATA_DIR/marco_query.fbin \
    --corpus_embedding_path $DATA_DIR/marco_doc.fbin \
    --query_lookup_path $DATA_DIR/marco_query.ibin \
    --corpus_lookup_path $DATA_DIR/marco_doc.ibin \
    --sptag_time_list_path $DATA_DIR/time_list.bin \
    --gt_path ../data/qrels.bin   \
    --splade_weight 1.0 \
    --spann_weight 10000.0 \
    --topk 100