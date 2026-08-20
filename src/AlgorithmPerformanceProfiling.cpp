


#include <iostream>

int main(int argc, char* argv[]) {
  std::cout << "Hello world source2" << std::endl;

  for (int index = 1; index < argc; ++index) {
    std::cout << "Argumento " << index << ": " << argv[index] << std::endl;
  }

  return 0;
}
