# similarity_search

## make, then run:
similarity_search <filename> <#lines(sets) to find common integers(tokens,words)> <jaccard-threshold(0..1)>

## demo with threshold 0.9 (almost ident sets/lines)
./similarity_search <filename> dblp_first500.txt 50 0.9

## demo for original implementation
./set_sim_join --timings --statistics --whitespace '/home/liquid/similarity_search/enron.format' allpairs 0.9
