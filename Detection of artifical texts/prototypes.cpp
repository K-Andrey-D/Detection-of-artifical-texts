#include "prototypes.h"

void prepare(std::string& st) {
    std::string::iterator remove = std::remove_if(st.begin(), st.end(), [](char& ch) -> bool {
        return (isalpha((unsigned char)ch) || ch == ' ') ? false : true;
        });
    st.erase(remove, st.end());
    st.erase(
        std::unique_copy(st.begin(), st.end(), st.begin(),
            [](char c1, char c2) { return c1 == ' ' && c2 == ' '; }),
        st.end());
    std::transform(st.begin(), st.end(), st.begin(), tolower);
}

std::map<std::string, int> getNgramm(const std::string& doc, size_t n) {
    std::vector<std::string> s;
    std::map<std::string, int> dict;

    for (size_t i = 0; i < (doc.size() - n + (size_t)1); ++i) {
        const std::string word(&(doc[i]), &(doc[i + n]));
        ++dict[word];
    }
    return dict;
}

void n_gram(const std::string& doc, int n)
{
    std::map<std::string, int> dict;
    int min = 2; 
    for (int i = 0; i < n - min + 1; ++i) {
        std::map<std::string, int> temp_dic(getNgramm(doc, (size_t)min + i));
        dict.insert(temp_dic.begin(), temp_dic.end());
    }
    std::set<std::string> unique;
    std::ofstream dictionary_file("dictionary.txt");
    for (std::map<std::string, int>::iterator it = dict.begin(); it != dict.end(); ++it) {
        if (it->second > 1000)
            unique.insert(it->first);
    }
    for (std::string str : unique)
        dictionary_file << str << '\n';
    dictionary_file.close();
}

std::vector<int> freq_in_chunk(const std::string& chunk, const std::vector<std::string>& dictionary)
{
    std::vector<int> freq_in_chunks;
    int quantity = 0;
    for (std::vector<std::string>::const_iterator it = dictionary.begin(); it != dictionary.end(); ++it) {
        size_t pos = 0;
        while ((pos = chunk.find(*it, pos)) != std::string::npos) {
            ++pos;
            ++quantity;
        }
        freq_in_chunks.push_back(quantity);
        quantity = 0;
    }
    return freq_in_chunks;
}

long double dzv_calc(int T, 
                    const std::vector<int> di, 
                    const std::vector<int> dj, 
                    int i, int j, 
                    const std::vector<std::vector<int>> freq_of_robot_ngramm_i, 
                    const std::vector<std::vector<int>> freq_of_robot_ngramm_j)
{
    return abs(zv_calc(T, di, i, freq_of_robot_ngramm_i) + 
                zv_calc(T, dj, j, freq_of_robot_ngramm_j) - 
                zv_calc(T, di, j, freq_of_robot_ngramm_j) - 
                zv_calc(T, dj, i, freq_of_robot_ngramm_i)); 
}

long double zv_calc(int T, 
                    const std::vector<int> di, 
                    int i, 
                    const std::vector<std::vector<int>> freq_of_robot_ngramm_i_j)
{
    long double total = 0.0;
    for (size_t m = 1; m <= T; ++m)
        total += spearman_calc(di, freq_of_robot_ngramm_i_j[(size_t)i - m]);
    return total / T;
}

long double spearman_calc(const std::vector<int> di, 
                        const std::vector<int> freq_of_robot_ngramm_k)
{
    long double total = 0.0;
    for (int i = 0; i < di.size(); ++i)
        total += pow(di[i] - freq_of_robot_ngramm_k[i], 2);
    total *= 6;
    int n = di.size();
    long int m = n * (pow(n, 2) - 1);
    total /= m;
    return 1 - total;
}