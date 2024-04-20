#include <sys/stat.h>
#include <fstream>
#include <iostream>

int main(int argc, char* argv[]) {
  double total_size = (double)(7158 * 1024) * 1024;
  std::ifstream data_file("./ssd/workspace_4/minigraph_data/2.bin", std::ios::binary | std::ios::app);
  char *buf_graph_ = (char *)malloc(total_size);
  std::cout << "read\n"; 
  data_file.read(buf_graph_, total_size);
  data_file.close();
  return 0;
}