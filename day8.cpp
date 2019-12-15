#include <sstream>
#include <fstream>
#include <iostream>
#include <vector>

/**
 * image cosists of layers, each layer has matrix
 * matrix has defined number of pixels wide and tall
 * */
class Image
{

    public:
        Image(int pixels_wide, int pixels_tall,
            std::vector<int> elements) 
        {
            pixels_wide = pixels_wide;
            pixels_tall = pixels_tall;
        }

    private:
        int pixels_wide;
        int pixels_tall;
        int **matrix;

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

    std::cout << str << std::endl;
}