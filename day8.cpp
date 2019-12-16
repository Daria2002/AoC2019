#include <sstream>
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

#define PIXELS_WIDE 25
#define PIXELS_TALL 6
#define ASCII_ZERO 48

template <class T>
std::vector<T> extract_subvector(std::vector<T> v, int begin_index, int end_index) {
    auto begin = v.begin() + begin_index;
    auto end = v.begin() + end_index;
    std::vector<T> subvector(begin, end);
    return subvector;
}

int** alloc_array(int tall, int wide) {
    // dynamically allocates memory for pixels_wide
    // number continuously of integer pointers and
    // returns pointer to the 1st element of the
    // sequence
    int** p = new int*[tall];
    // p[0] is first element, p[2] is second etc.
    // p is a pointer to first element
    for(int i = 0; i < tall; i++) {
        p[i] = new int[wide];
    }
    return p;
}

int** ini_to_negative_value(int tall, int wide) {
    // dynamically allocates memory for pixels_wide
    // number continuously of integer pointers and
    // returns pointer to the 1st element of the
    // sequence
    int** p = new int*[tall];
    // p[0] is first element, p[2] is second etc.
    // p is a pointer to first element
    for(int i = 0; i < tall; i++) {
        p[i] = new int[wide];
        for(int j = 0; j < wide; j++) {
            p[i][j] = -1;
        }
    }
    return p;
}

/**
 * image cosists of layers, each layer has matrix
 * matrix has defined number of pixels wide and tall
 * */
class Layer
{

    private:
        int wide;
        int tall;

        int calculate_occurence(int element) {
            int count = 0;
            for(int i = 0; i < tall; i++) {
                for(int j = 0; j < wide; j++) {
                    if(matrix[i][j] == element) {
                        count++;
                    }
                }
            }

            return count;
        }

        int** build_matrix(std::vector<int> elements) {
            int **p = alloc_array(tall, wide);
            for(int i = 0; i <tall ; i++) {
                for(int j = 0; j <wide ; j++) {
                    p[i][j] = elements[i*wide+j];
                }
            }
            return p;
        }

    public:
        int **matrix;
        Layer(int pixels_wide, int pixels_tall,
            std::vector<int> elements)
        {
            wide = pixels_wide;
            tall = pixels_tall;
            matrix = build_matrix(elements);
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
};

int main() {
    std::ifstream file("day8.txt");
    std::string str;
    std::getline(file, str);
    std::stringstream ss(str);

    std::vector<int> elements;
    std::for_each(str.begin(), str.end(), [&](char c)
    {elements.push_back((int)c-ASCII_ZERO);});
   
    int elements_in_layer = PIXELS_WIDE*PIXELS_TALL;
    int number_of_layers = elements.size()/elements_in_layer;
   
    std::vector<Layer> image;
   
    int min = PIXELS_WIDE*PIXELS_TALL;
    int mul = 0;
   
    int** visible_pixels = ini_to_negative_value(PIXELS_TALL, PIXELS_WIDE);
   
    for(int i = 0; i < number_of_layers; i++) {
        int begin = i*elements_in_layer;
        int end = i == 0 ? i*elements_in_layer + elements_in_layer : i*elements_in_layer + 1 + elements_in_layer;
        Layer l(PIXELS_WIDE, PIXELS_TALL,
                extract_subvector(elements, begin, end));
        image.push_back(l);
       
        if(l.get_zero() < min) {
            min = l.get_zero();
            mul = l.get_one() * l.get_two();
        }
       
        for(int j = 0; j < PIXELS_TALL; j++) {
            for(int k = 0; k < PIXELS_WIDE; k++) {
                if(visible_pixels[j][k] < 0 && l.matrix[j][k] != 2) {
                    visible_pixels[j][k] = l.matrix[j][k];
                }
            }
        }
    }
   
    std::cout << "part1 = " << mul << std::endl;
   
    std::cout << "part2: " << std::endl;
    for(int j = 0; j < PIXELS_TALL; j++) {
        for(int k = 0; k < PIXELS_WIDE; k++) {
            if(visible_pixels[j][k] == 0)
                std::cout << " ";
            else
                std::cout << visible_pixels[j][k];
        }
        std::cout << std::endl;
    }
}