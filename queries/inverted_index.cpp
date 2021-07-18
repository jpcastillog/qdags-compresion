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


sdsl::int_vector<>* read_inverted_list(std::ifstream &input_stream, uint64_t n){
    uint64_t x;
    uint64_t i;
    // uint64_t n;
    // input_stream >> n;
    int_vector<>* inverted_list  = new int_vector<>(n);
    for (i = 0; i < n; i++){
        input_stream >> x;
        (*inverted_list)[i] = x;
    }

    return inverted_list;
}


std::vector<std::vector<uint64_t>>* to_vector_of_vectors(sdsl::int_vector<>* v){
    uint64_t i;
    std::vector<uint64_t> element;

    std::vector<std::vector<uint64_t>>* set = new std::vector<std::vector<uint64_t>>();
    for (i = 0; i < v -> size(); ++i){
        element.push_back((*v)[i]);
        set -> push_back(element);
        element.clear();
    }

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


int main(){
    uint8_t coder = 1;
    // const std::string input_file_name  = "ejemplo.txt";
    const std::string input_file_name  = "./../../../../data/bitvectors/ii/gov2/url/gov2_ii_nofreq_url_dif.txt.B";
    
    std::ifstream input_stream(input_file_name);
    if (!input_stream.is_open()){
        cout << "El archivo no existe\n";
        return 1;
    }

    std::ofstream output_stream;
    const std::string output_file_name = "./results.txt";
    output_stream.open(output_file_name);
    
    //     output_stream.open(output_file_name);
    // if (coder == 0) {
    //     const std::string output_file_name = "./outputs/qdags.txt";
    //     
    // }
    // else if (coder == 1){
    //     const std::string output_file_name = "./outputs/elias_delta.txt";
    //     output_stream.open(output_file_name);
    // }
    // else if (coder == 2){
    //     const std::string output_file_name = "./outputs/elias_gamma.txt";
    //     output_stream.open(output_file_name);
    // }
    // else if(coder == 3){
    //     const std::string output_file_name = "./outputs/fibonacci.txt";
    //     output_stream.open(output_file_name);
    // }
    

    uint64_t i;
    uint64_t n = 0;
    uint64_t bytes_elias_delta = 0;
    uint64_t bytes_elias_gamma = 0;
    uint64_t bytes_fibonacci = 0;
    uint64_t bytes_qdag = 0;

    uint64_t size;
    input_stream >> size;
    cout << "Universe: " << size << "\n";
    for(i = 0; i < 100; i++){
        cout << "i: " << i << "\n";
        uint64_t n_elements;
        input_stream >> n_elements;
        if (n_elements >= 100000){
            // Elias delta, gamma y fibonacci
            sdsl::int_vector<>* il = read_inverted_list(input_stream, n_elements);
            uint64_t n_i = (il -> size());
            n += n_i;
            cout << "***n_i: " << n_i << "\n";
            util::bit_compress((*il));
            // elias gamma
            sdsl::enc_vector<sdsl::coder::elias_gamma>ev_gamma((*il));
            uint64_t gamma_i = sdsl::size_in_bytes(ev_gamma);
            bytes_elias_gamma += gamma_i;
            cout << "size in bytes elias gamma: " << gamma_i << "\n";
            // elias delta
            sdsl::enc_vector<sdsl::coder::elias_delta>ev_delta((*il));
            uint64_t delta_i = sdsl::size_in_bytes(ev_delta);
            bytes_elias_delta += delta_i;
            cout << "size in bytes elias delta: " << delta_i << "\n";
            // fibonacci
            sdsl::enc_vector<sdsl::coder::fibonacci>ev_fibonacci((*il));
            uint64_t fibonacci_i = sdsl::size_in_bytes(ev_fibonacci);
            bytes_fibonacci += fibonacci_i;
            cout << "size in bytes fibonacci: " << fibonacci_i << "\n";

            // qdag implementation
            qdag::att_set att_R;
            att_R.push_back(AT_X);

            std::vector<std::vector<uint64_t>>* il_qdag = to_vector_of_vectors(il);
            uint64_t grid_side = (il_qdag -> back()).back();
            if (grid_side < n_i){
                grid_side = maximum_in_table(*il_qdag, att_R.size(), grid_side);
            }
            // grid_side = maximum_in_table(*il_qdag, att_R.size(), grid_side);
            grid_side++;
            cout << "grid_side: " << grid_side << "\n";
            qdag *q = new qdag(*il_qdag, att_R, grid_side, 2, att_R.size());
            uint64_t qdag_i = q -> size();
            bytes_qdag += qdag_i;
            cout << "size in bytes qdags: " << qdag_i << "\n";


            delete il;
            delete il_qdag;
            delete q;
        }
        else {
            input_stream.ignore(n_elements*3, '\n');
        }
        

        // if (coder == 0) {
        //     // uint64_t grid_side = 25138631;
        //     uint64_t grid_side;
        //     std::vector<std::vector<uint64_t>>* il = read_inverted_list_qdag(input_stream);
        //     grid_side = maximum_in_table(*il, att_R.size(), grid_side);
        //     grid_side++;
        //     cout << "grid_side:" << grid_side << "\n";
        //     qdag *qdag_il = new qdag(*il, att_R, grid_side, 2, att_R.size());
        //     uint64_t size_in_bits = (qdag_il->size())*8;
        //     uint64_t n = (il->size())*1;
        //     float avg = (float) size_in_bits/n;
        //     output_stream << n << " " << size_in_bits << " " << avg << "\n";
        //     cout << n << " " << size_in_bits << " " << avg << "\n";
        //     delete qdag_il;
        //     delete il;
            
        // }
        // else {
        //     int_vector<>* il = read_inverted_list(input_stream);
        //     uint64_t n =  il -> size();
        //     util::bit_compress(*il);

        //     // Elias delta
        //     if (coder == 1){
        //         sdsl::vlc_vector<sdsl::coder::elias_delta>ev(*il);
        //         uint64_t size_in_bits = (size_in_bytes(ev)*8);
        //         float avg = (float)(size_in_bits)/n;
        //         output_stream << n << " " << size_in_bits << " " << avg << "\n";
        //         cout << n << " " << size_in_bits << " " << avg << "\n";
        //     }
        //     // Elias gamma
        //     else if(coder==2) {
        //         sdsl::vlc_vector<sdsl::coder::elias_gamma>ev(*il);
        //         int64_t size_in_bits = (size_in_bytes(ev)*8);
        //         float avg = (float)(size_in_bits)/n;
        //         output_stream << n << " " << size_in_bits << " " << avg << "\n";
        //         cout << n << " " << size_in_bits << " " << avg << "\n";  
        //     }
        //     // Fibonacci
        //     else {
        //         sdsl::vlc_vector<sdsl::coder::fibonacci>ev(*il);
        //         int64_t size_in_bits = (size_in_bytes(ev)*8);
        //         float avg = (float)(size_in_bits)/n;
        //         output_stream << n << " " << size_in_bits << " " << avg << "\n";
        //         cout << n << " " << size_in_bits << " " << avg << "\n";
        //     }
        //     delete il;
        // }
    }
    output_stream << "avg bits elias gamma: " << (float)(bytes_elias_gamma*8)/n << "\n";
    output_stream << "avg bits elias delta: " << (float)(bytes_elias_delta*8)/n << "\n";
    output_stream << "avg bits fibonacci: " << (float)(bytes_fibonacci*8)/n << "\n";
    output_stream << "avg bits qdag: " << (float)(bytes_qdag*8)/n << "\n";
    output_stream.close();
    
    cout << "avg bits elias gamma: " << (float)(bytes_elias_gamma*8)/n << "\n";
    cout << "avg bits elias delta: " << (float)(bytes_elias_delta*8)/n << "\n";
    cout << "avg bits fibonacci: " << (float)(bytes_fibonacci*8)/n << "\n";
    cout << "avg bits qdag: " << (float)(bytes_qdag*8)/n << "\n";
    input_stream.close();
    return 0;
}