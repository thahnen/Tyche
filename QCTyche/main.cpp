#include <iostream>
#include <opencv2/opencv.hpp>
#include <royale.hpp>

int main() {
    std::cout << "Hello, World!" << std::endl;

    unsigned maj, min, pat, build;
    royale::getVersion(maj, min, pat, build);
    std::cout << "Royale-Version: " << maj << "." << min << "." << pat << "." << build << std::endl;

    return 0;
}