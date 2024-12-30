# !/bin/bash

cd build

DATA_DIR=../data/exp1127_bs
./LightningBird \
    --splade_index_path $DATA_DIR/splade_invert_index.bin \
    --spann_index_path $DATA_DIR/spann_invert_index.bin \
    --query_path $DATA_DIR/query_ids.fbin \
    --posting_path $DATA_DIR/postings.bin \
    --query_embedding_path $DATA_DIR/query_769.fbin \
    --corpus_embedding_path $DATA_DIR/corpus_sent_769.fbin