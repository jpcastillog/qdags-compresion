cd queries/
echo -e "Building test compression..."
# g++ -std=c++11 -mpopcnt -m64 -frename-registers -O9 -msse4.2 -DNDEBUG -I ~/include -L ~/lib set_zipf_distribution.cpp -o zipf_set .out -lsdsl -pthread
g++ -std=c++11 -O3 -DNDEBUG -I ~/include -L ~/lib set_zipf_distribution.cpp -o zipf_set.out -lsdsl -ldivsufsort -ldivsufsort64 -pthread
g++ -std=c++11 -O3 -DNDEBUG -I ~/include -L ~/lib inverted_index.cpp -o inverted_index.out -lsdsl -ldivsufsort -ldivsufsort64 -pthread
echo -e "Done!"