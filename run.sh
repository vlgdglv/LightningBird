# !/bin/bash

cd build

DATA_DIR=../data/bs4lb
./LightningBird \
    --splade_index_path $DATA_DIR/bm25_inverted_index.bin \
    --spann_index_path $DATA_DIR/spann_inverted_index.bin \
    --query_path $DATA_DIR/query_ids.fbin \
    --posting_path $DATA_DIR/spann_query.bin \
    --query_embedding_path $DATA_DIR/marco_query.fbin \
    --corpus_embedding_path $DATA_DIR/marco_doc.fbin