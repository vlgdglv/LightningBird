# !/bin/bash

cd build

BASE_DIR=/datacosmos/User/baoht/onesparse2/marcov2
./LightningBird \
    --splade_index_path $BASE_DIR/index/bm25_test_cut0.1/bm25_inverted_index.bin \
    --spann_index_path $BASE_DIR/spann/bs_SimANS_8_invertedIndex/inverted_index.bin \
    --query_path $BASE_DIR/index/bm25_test/query_ids.bin \
    --posting_path $BASE_DIR/spann/bs_SimANS_8_invertedIndex/spann_query.bin \
    --query_embedding_path $BASE_DIR/embeddings/bs_SimANS_36k/query_embeddings_rerange.fbin \
    --corpus_embedding_path $BASE_DIR/embeddings/bs_SimANS_36k/test_corpus.bin \
    --query_lookup_path $BASE_DIR/embeddings/bs_SimANS_36k/qlookup_rerange.ibin \
    --corpus_lookup_path $BASE_DIR/embeddings/bs_SimANS_36k/test_corpus.ibin \
    --sptag_time_list_path $BASE_DIR/spann/bs_SimANS_8_invertedIndex/time_list.bin \
    --gt_path $BASE_DIR/data/qrels_test.bin   \
    --splade_weight 1.0 \
    --spann_weight 10000.0 \
    --topk 100 --distance_func inner_product