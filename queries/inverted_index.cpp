#include <fstream>
#include <ratio>
#include <bits/stdc++.h>
#include <chrono>
#include <ctime>
#include <sdsl/vectors.hpp>

using namespace std::chrono;
using namespace sdsl;

#include "../src/joins.cpp"

#define AT_X 0
#define AT_Y 1
#define AT_Z 2
#define AT_V 3
#define AT_U 4

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

sdsl::int_vector<64>* read_inverted_list(std::ifstream &input_stream){
    uint64_t x;
    uint64_t i;
    
    
    uint64_t n;
    input_stream >> n;
    int_vector<64>* inverted_list  = new int_vector<64>(n);
    for (i = 0; i < n; i++){
        input_stream >> x;
        (*inverted_list)[i] = x;
    }

    return inverted_list;
}

void write_output(std::string file_name){

}

int main(){
    uint8_t coder = 0;
    // const std::string input_file_name  = "ejemplo.txt";
    const std::string input_file_name  = "./../../../../data/bitvectors/ii/gov2/url/gov2_ii_nofreq_url_dif.txt.B";
    std::ifstream input_stream(input_file_name);
    if (!input_stream.is_open()){
        cout << "El archivo no existe\n";
        return 1;
    }
    // qdags parameters
    qdag::att_set att_R;
    att_R.push_back(AT_X);
    const uint64_t grid_side = 18;

    std::ofstream output_stream;
    if (coder == 0) {
        const std::string output_file_name = "./outputs/qdaqs.txt";
        output_stream.open(output_file_name);
    }
    else if (coder == 1){
        const std::string output_file_name = "./outputs/elias_delta.txt";
        output_stream.open(output_file_name);
    }
    else if (coder == 2){
        const std::string output_file_name = "./outputs/elias_gamma.txt";
        output_stream.open(output_file_name);
    }
    else if(coder == 3){
        const std::string output_file_name = "./outputs/fibonacci.txt";
        output_stream.open(output_file_name);
    }
    
    uint64_t size;
    uint64_t i;

    input_stream >> size;
    for(i = 0; i < size; i++){
        // qdag implementation
        if (coder == 0) {
            std::vector<std::vector<uint64_t>>* il = read_inverted_list_qdag(input_stream);
            qdag* qdag_il = new qdag(*il, att_R, grid_side, 1, att_R.size());
            uint64_t size_in_bits = (qdag_il->size())*8;
            uint64_t n = (il->size())*1;
            float avg = (float) size_in_bits/n;
            output_stream << n << " " << size_in_bits << " " << avg << "\n";
            cout << n << " " << size_in_bits << " " << avg << "\n";
            delete qdag_il;
            
        }
        else {
            int_vector<64>* il = read_inverted_list(input_stream);
            uint64_t n =  il -> size();
            util::bit_compress(*il);

            // Elias delta
            if (coder == 1){
                enc_vector<sdsl::coder::elias_delta>ev(*il);
                uint64_t size_in_bits = (size_in_bytes(ev)*8);
                float avg = (float)(size_in_bits)/n;
                output_stream << n << " " << size_in_bits << " " << avg << "\n";
                cout << n << " " << size_in_bits << " " << avg << "\n";
            }
            // Elias gamma
            else if(coder==2) {
                enc_vector<sdsl::coder::elias_gamma>ev(*il);
                int64_t size_in_bits = (size_in_bytes(ev)*8);
                float avg = (float)(size_in_bits)/n;
                output_stream << n << " " << size_in_bits << " " << avg << "\n";
                cout << n << " " << size_in_bits << " " << avg << "\n";  
            }
            // Fibonacci
            else {
                enc_vector<sdsl::coder::fibonacci>ev(*il);
                int64_t size_in_bits = (size_in_bytes(ev)*8);
                float avg = (float)(size_in_bits)/n;
                output_stream << n << " " << size_in_bits << " " << avg << "\n";
                cout << n << " " << size_in_bits << " " << avg << "\n";
            }
            delete il;
        }
    }
    output_stream.close();
    return 0;
}