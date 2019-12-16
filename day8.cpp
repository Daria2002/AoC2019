#include <sstream>
#include <fstream>
#include <iostream>
#include <vector>

/**
 * image cosists of layers, each layer has matrix
 * matrix has defined number of pixels wide and tall
 * */
class Layer
{

    public:
        Layer(int pixels_wide, int pixels_tall,
            std::vector<int> elements) 
        {
            pixels_wide = pixels_wide;
            pixels_tall = pixels_tall;
        }

        int get_zero() {
            return calculate_occurence(0);
        }

        int get_one() {
            return calculate_occurence(1);
        }

        int get_two() {
            return calculate_occurence(2);
        }

    private:
        int pixels_wide;
        int pixels_tall;
        int **matrix;

        int calculate_occurence(int element) {
            int count = 0;
            for(int i = 0; i < pixels_tall; i++) {
                for(int j = 0; j < pixels_wide; j++) {
                    if(matrix[i][j] == 0) {
                        count++;
                    }
                }
            }

            return count;
        } 

        int** alloc_array(int **p) {
            // dynamically allocates memory for pixels_wide
            // number continuously of integer pointers and
            // returns pointer to the 1st element of the
            // sequence
            p = new int*[pixels_tall];
            // p[0] is first element, p[2] is second etc.
            // p is a pointer to first element
            for(int i = 0; i < pixels_tall; i++) {
                p[i] = new int[pixels_wide];
            }
        }

        int** build_matrix(std::vector<int> elements) {
            int **p = alloc_array(p);
            for(int i = 0; i < pixels_tall; i++) {
                for(int j = 0; j < pixels_wide; j++) {
                    **p = elements[i*pixels_wide+j];
                    (*p)++;
                }
                p++;
            }
        }
};

int main() {
    std::ifstream file("day8.txt");
    std::string str;
    std::getline(file, str);
    std::stringstream ss(str);

    std::vector<int> elements;
    while(std::getline(ss, str, ',')) {
        elements.push_back(std::stoi(str));
    }

    std::vector<Layer> Image;
}