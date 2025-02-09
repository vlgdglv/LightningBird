# !/bin/bash

cd build

DATA_DIR=/datacosmos/local/User/baoht/onesparse2/hybridsearch/lightningbrids/bs4lb
./LightningBird \
    --splade_index_path /datacosmos/local/User/baoht/onesparse2/hybridsearch/bm25/index/marco/marco_bm25.bin\
    --spann_index_path $DATA_DIR/spann_inverted_index.bin \
    --query_path $DATA_DIR/query_ids.fbin \
    --posting_path $DATA_DIR/spann_query.bin \
    --query_embedding_path $DATA_DIR/marco_query.fbin \
    --corpus_embedding_path $DATA_DIR/marco_doc_with_id769.fbin \
    --query_lookup_path $DATA_DIR/marco_query.ibin \
    --corpus_lookup_path $DATA_DIR/marco_doc_with_id769.ibin \
    --sptag_time_list_path $DATA_DIR/time_list.bin \
    --gt_path /datacosmos/local/User/baoht/onesparse2/hybridsearch/data/msmarco/qrels.bin    \
    --splade_weight 1.0 \
    --spann_weight 10000.0 \
    --topk 100 --distance_func euclidean_reciprocal