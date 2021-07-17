#include <fstream>
#include <ratio>
#include <bits/stdc++.h>
#include <chrono>
#include <ctime>
#include <sdsl/vectors.hpp>
#include <sdsl/enc_vector.hpp>

using namespace std::chrono;
using namespace sdsl;

#include "../src/joins.cpp"

#define AT_X 0
#define AT_Y 1
#define AT_Z 2
#define AT_V 3
#define AT_U 4

sdsl::int_vector<>* random_vector(uint64_t u, uint64_t n){
    std::random_device rd;
    std::mt19937_64 generator(rd());
    std::uniform_int_distribution<uint64_t> dist{1, u};
    std::set<uint64_t> r;
    sdsl::int_vector<>* rv = new sdsl::int_vector<>(n);

    while(r.size() != n){
        r.insert(dist(generator));
    }
    std::set<uint64_t>::iterator it;
    int i = 0;
    for (it = r.begin(); it != r.end(); ++it){
        // uint64_t value = *it;
        (*rv)[i] = *it;
        i++;
    }
    // std::vector<uint64_t>* rv = new std::vector<uint64_t>(r.begin(), r.end());
    cout << "Size of set generated: " << r.size() << "\n";
    
    return rv;

}


std::vector<uint64_t>* load_set(std::ifstream &input_stream){
    uint64_t n;
    uint64_t x;
    input_stream >> n;

    std::vector<uint64_t>* v = new std::vector<uint64_t>();
    uint64_t i = 0;
    while (!input_stream.eof() && i < n){
        input_stream >> x;
        v->push_back(x);
    }
    
    return v;
}


sdsl::int_vector<>* to_int_vector(vector<uint64_t>* v,  uint64_t samples){
    uint64_t i = 0;
    uint64_t n = v->size();

    std::set<uint64_t> r;
    std::random_device rd;
    std::mt19937_64 generator(rd());
    std::uniform_int_distribution<uint64_t> dist{0, n};
    for (int i = 0; i < samples; ++i){
        uint64_t index = dist(generator);
        r.insert((*v)[index]);
    }

    int_vector<>* set  = new sdsl::int_vector<>(r.size());
    
    std::set<uint64_t>::iterator it;
    uint64_t j = 0;
    for (it = r.begin(); it != r.end(); ++it){
        (*set)[j] = *it;
        j++;
    }
    return set;
}


std::vector<std::vector<uint64_t>>* to_vector_of_vectors(std::vector<uint64_t>* v, uint64_t samples){
    uint64_t i = 0;
    uint64_t n = v->size();
    std::vector<uint64_t> element;

    std::set<uint64_t> r;
    std::random_device rd;
    std::mt19937_64 generator(rd());
    std::uniform_int_distribution<uint64_t> dist{0, n};
    for (int i = 0; i < samples; ++i){
        uint64_t index = dist(generator);
        r.insert((*v)[index]);
    }

    // int_vector<>* set  = new sdsl::int_vector<>(r.size());
    std::vector<std::vector<uint64_t>>* set = new std::vector<std::vector<uint64_t>>(); 
    
    std::set<uint64_t>::iterator it;
    uint64_t j = 0;
    for (it = r.begin(); it != r.end(); ++it){
        element.push_back(*it);
        set->push_back(element);
        element.clear();       
    }
    return set;
}


std::vector<std::vector<uint64_t>>* read_inverted_list_qdag(std::ifstream &input_stream){
    uint64_t x;
    uint64_t i;
    std::vector<std::vector<uint64_t>>* inverted_list;
    std::vector<uint64_t> tuple;

    inverted_list = new std::vector<std::vector<uint64_t>>();

    uint64_t n;
    input_stream >> n;
    for (i = 0; i < n; i++){
        input_stream >> x;
        tuple.push_back(x);
        inverted_list->push_back(tuple);
        tuple.clear();
        // cout << x << "\t";
    }
    // cout << "\n";
    return inverted_list;
}


std::vector<std::vector<uint64_t>>* read_set_qdags(std::ifstream &input_stream, uint64_t n){
    uint64_t x;
    uint64_t i;
    std::vector<std::vector<uint64_t>>* set = new std::vector<std::vector<uint64_t>>();
    std::vector<uint64_t> element;
    // input_stream >> n;
    i = 0;
    while((!input_stream.eof()) && (i < n)){   
        input_stream >> x;
        element.push_back(x);
        set -> push_back(element);
        element.clear();
        i++;
    }
    return set;
}


std::vector<std::vector<uint64_t>>* read_set_qdags2(std::ifstream &input_stream, uint64_t n, uint64_t samples){
    uint64_t x;
    uint64_t i;
    std::vector<std::vector<uint64_t>>* set = new std::vector<std::vector<uint64_t>>();
    std::vector<uint64_t> element;
    
    i = 0;
    vector<uint64_t> v;
    while(!input_stream.eof() && i < n){
        input_stream >> x;
        v.push_back(x);
        // (*set)[i] = x;
        i++;
    }
    
    // uint64_t size_final = 1000000;
    std::set<uint64_t> r;
    std::random_device rd;
    std::mt19937_64 generator(rd());
    std::uniform_int_distribution<uint64_t> dist{0, n};
    for (int i = 0; i < samples; ++i){
        uint64_t index = dist(generator);
        r.insert(v[index]);
    }

    std::set<uint64_t>::iterator it;
    uint64_t j = 0;
    for (it = r.begin(); it != r.end(); ++it){
        element.push_back(*it);
        set -> push_back(element);
        element.clear();
    }

    // i = 0;
    // while((!input_stream.eof()) && (i < n)){   
    //     input_stream >> x;
    //     element.push_back(x);
    //     set -> push_back(element);
    //     element.clear();
    //     i++;
    // }
    return set;
}


uint64_t maximum_in_table(std::vector<std::vector<uint64_t>> &table, uint16_t n_columns, uint64_t max_temp)
{
    uint64_t i, j;
    
    for (i = 0; i < table.size(); i++) 
        for (j = 0; j < n_columns; j++)
            if (table[i][j] > max_temp)
                max_temp = table[i][j];
    
    
    return max_temp;
}

int main(int argc, char** argv){
    // sdsl::int_vector<>* v = random_vector(100000, 1000);
    // sdsl::int_vector<>*v = read_set(input_stream, size);
    // cout << "Random vector size: " << v->size() << "\n";
    
    // uint64_t n = v->size();
    // util::bit_compress((*v));
    // cout << "width: " << (int)(*v).width() << "\n";
    // vlc_vector<sdsl::coder::elias_delta> vlc(*v);
    // uint64_t size_in_bits = sdsl::size_in_bytes(vlc)*8;
    // float avg = (float)size_in_bits/n;
    // cout << n << " " << size_in_bits << " " << avg << "\n";

    std::string input_file_name(argv[1]);
    std::string coder(argv[2]);
    // const std::string input_file_name  = "./../../../../data/bitvectors/ii/gov2/url/gov2_ii_nofreq_url_dif.txt.B";

    std::ifstream input_stream(input_file_name);
    if (!input_stream.is_open()){
        cout << "El archivo no existe\n";
        return 1;
    }

    std::vector<uint64_t>* v_zipf = load_set(input_stream);

    cout << "Size of vector v: " << v_zipf->size() << "\n";
    
    uint64_t samples = 1000000;
    
    // qdag::att_set att_R;
    // att_R.push_back(AT_X);
    
    // std::vector<std::vector<uint64_t>>* set = to_vector_of_vectors(v_zipf, samples);
    // uint_fast64_t grid_side = maximum_in_table(*set, att_R.size(), grid_side);
    // grid_side++;

    // qdag *qdag_set = new qdag(*set, att_R, grid_side, 2, att_R.size());
    // uint64_t size_in_bits = (qdag_set->size())*8;
    // uint64_t n = (set->size())*1;
    // float avg = (float) size_in_bits/n;
    // cout << n << " " << size_in_bits << " " << avg << "\n";
    
    if (coder.compare("qdags") == 0) {
        // qdags parameters
        qdag::att_set att_R;
        att_R.push_back(AT_X);
        
        std::vector<std::vector<uint64_t>>* set = to_vector_of_vectors(v_zipf, samples);
        uint_fast64_t grid_side = maximum_in_table(*set, att_R.size(), grid_side);
        grid_side++;

        qdag *qdag_set = new qdag(*set, att_R, grid_side, 2, att_R.size());
        uint64_t size_in_bits = (qdag_set->size())*8;
        uint64_t n = (set->size())*1;
        float avg = (float) size_in_bits/n;
        cout << n << " " << size_in_bits << " " << avg << "\n";
    }
    else {
        sdsl::int_vector<>* set = to_int_vector(v_zipf, samples);
        uint64_t n = set -> size();
        util::bit_compress((*set));
        // cout << "width: " << (int)(*set).width() << "\n";

        if (coder.compare("elias_delta") == 0){
            enc_vector<sdsl::coder::elias_delta> vlc(*set);
            uint64_t size_in_bits = sdsl::size_in_bytes(vlc)*8;
            float avg = (float)size_in_bits/n;
            cout << n << " " << size_in_bits << " " << avg << "\n";
        }
        else if (coder.compare("elias_gamma") == 0){
            enc_vector<sdsl::coder::elias_gamma>vlc(*set);
            uint64_t size_in_bits = (sdsl::size_in_bytes(vlc)*8);
            float avg = (float)(size_in_bits)/n;
            cout << n << " " << size_in_bits << " " << avg << "\n";
        }
        else if(coder.compare("fibonacci") == 0){
            enc_vector<sdsl::coder::fibonacci>vlc(*set);
            uint64_t size_in_bits = (size_in_bytes(vlc)*8);
            float avg = (float)(size_in_bits)/n;
            cout << n << " " << size_in_bits << " " << avg << "\n";
        }
    }
    return 0;
}