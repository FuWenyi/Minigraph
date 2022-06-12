#ifndef MINIGRAPH_DATA_MNGR_H
#define MINIGRAPH_DATA_MNGR_H

#include <memory>

#include <folly/AtomicHashMap.h>

#include "utility/io/csr_io_adapter.h"
#include "utility/io/edge_list_io_adapter.h"

namespace minigraph {
namespace utility {
namespace io {

template <typename GID_T, typename VID_T, typename VDATA_T, typename EDATA_T>
class DataMngr {
  using GRAPH_BASE_T = graphs::Graph<GID_T, VID_T, VDATA_T, EDATA_T>;
  using CSR_T = graphs::ImmutableCSR<GID_T, VID_T, VDATA_T, EDATA_T>;
  using EDGE_LIST_T = graphs::EdgeList<GID_T, VID_T, VDATA_T, EDATA_T>;
  using MSG_T = graphs::Message<VID_T, VDATA_T, EDATA_T>;
  using VertexInfo = graphs::VertexInfo<VID_T, VDATA_T, EDATA_T>;

 public:
  std::unique_ptr<utility::io::CSRIOAdapter<GID_T, VID_T, VDATA_T, EDATA_T>>
      csr_io_adapter_;
  std::unique_ptr<
      utility::io::EdgeListIOAdapter<GID_T, VID_T, VDATA_T, EDATA_T>>
      edge_list_io_adapter_;
  std::unique_ptr<std::unordered_map<VID_T, std::vector<GID_T>*>>
      global_border_vertexes_ = nullptr;
  std::unique_ptr<std::unordered_map<VID_T, VertexInfo*>>
      global_border_vertexes_info_ = nullptr;

  DataMngr() {
    pgraph_by_gid_ =
        std::make_unique<folly::AtomicHashMap<GID_T, GRAPH_BASE_T*>>(1024);

    csr_io_adapter_ = std::make_unique<
        utility::io::CSRIOAdapter<GID_T, VID_T, VDATA_T, EDATA_T>>();

    edge_list_io_adapter_ = std::make_unique<
        utility::io::EdgeListIOAdapter<gid_t, vid_t, vdata_t, edata_t>>();
  };

  bool ReadGraph(const GID_T& gid, const CSRPt& csr_pt,
                 const GraphFormat& graph_format) {
    LOG_INFO("Read gid: ", gid);
    auto graph = new CSR_T;
    auto out = csr_io_adapter_->Read((GRAPH_BASE_T*)graph, graph_format, gid,
                                     csr_pt.meta_pt, csr_pt.data_pt);
    if (out) {
      pgraph_by_gid_->insert(std::make_pair(gid, (GRAPH_BASE_T*)graph));
    }
    return out;
  }

  bool ReadGraph(const GID_T& gid, const EdgeListPt& edge_list_pt,
                 const GraphFormat& graph_format = edge_list_csv) {
    LOG_INFO("Read gid: ", gid);
    auto graph = new EDGE_LIST_T;

    auto out = edge_list_io_adapter_->Read((GRAPH_BASE_T*)graph, graph_format,
                                           gid, edge_list_pt.edges_pt,
                                           edge_list_pt.v_label_pt);
    if (out) {
      pgraph_by_gid_->insert(std::make_pair(gid, (GRAPH_BASE_T*)graph));
    }
    return out;
  }

  bool WriteGraph(const GID_T& gid, const CSRPt& csr_pt,
                  const GraphFormat& graph_format) {
    auto graph = this->GetGraph(gid);
    LOG_INFO("Write gid: ", gid);
    return csr_io_adapter_->Write(*((GRAPH_BASE_T*)graph), graph_format,
                                  csr_pt.meta_pt, csr_pt.data_pt);
  }

  bool WriteBorderVertexes(const std::unordered_map<VID_T, std::vector<GID_T>*>&
                               global_border_vertexes,
                           const std::string& border_vertexes_pt) {
    if (IsExist(border_vertexes_pt)) {
      remove(border_vertexes_pt.c_str());
    }
    std::ofstream border_vertexes_file(border_vertexes_pt,
                                       std::ios::binary | std::ios::app);
    size_t* buf_config = (size_t*)malloc(sizeof(size_t));
    buf_config[0] = global_border_vertexes.size();

    VID_T* buf_global_border_vertexes =
        (VID_T*)malloc(sizeof(VID_T) * global_border_vertexes.size());
    ((size_t*)buf_global_border_vertexes)[0] = global_border_vertexes.size();
    size_t i = 0;
    for (auto& iter : global_border_vertexes) {
      buf_global_border_vertexes[i++] = iter.first;
    }
    border_vertexes_file.write((char*)buf_config, sizeof(size_t));
    border_vertexes_file.write((char*)buf_global_border_vertexes,
                               sizeof(VID_T) * global_border_vertexes.size());
    free(buf_global_border_vertexes);
    free(buf_config);
    border_vertexes_file.close();
    return true;
  }

  bool WriteGraphDependencies(
      const std::unordered_map<VID_T, VertexDependencies<VID_T, GID_T>*>&
          global_border_vertexes_with_dependencies,
      const std::string output_pt) {
    if (IsExist(output_pt)) {
      remove(output_pt.c_str());
    }
    size_t num_border_vertexes =
        global_border_vertexes_with_dependencies.size();
    size_t* offset_who_need = (size_t*)malloc(
        sizeof(size_t) * global_border_vertexes_with_dependencies.size());
    size_t* offset_who_provide = (size_t*)malloc(
        sizeof(size_t) * global_border_vertexes_with_dependencies.size());

    VID_T* border_vertexes = (VID_T*)malloc(
        sizeof(VID_T) * global_border_vertexes_with_dependencies.size());
    size_t i = 0;
    size_t sum_who_need = 0, sum_who_provide = 0;
    size_t* num_gid_who_provide_for_each_vertex =
        (size_t*)malloc(sizeof(size_t) * num_border_vertexes);
    size_t* num_gid_who_need_for_each_vertex =
        (size_t*)malloc(sizeof(size_t) * num_border_vertexes);

    for (auto& iter : global_border_vertexes_with_dependencies) {
      border_vertexes[i] = iter.first;
      num_gid_who_provide_for_each_vertex[i] = iter.second->who_need_->size();
      num_gid_who_need_for_each_vertex[i] = iter.second->who_need_->size();
      if (i == 0) {
        offset_who_provide[0] = 0;
        offset_who_need[0] = 0;
      } else {
        offset_who_provide[i] =
            offset_who_provide[i - 1] + num_gid_who_provide_for_each_vertex[i];
        offset_who_need[i] =
            offset_who_need[i - 1] + num_gid_who_need_for_each_vertex[i];
      }
      sum_who_need += iter.second->who_need_->size();
      sum_who_provide += iter.second->who_provide_->size();
      i++;
    }
    GID_T* who_provide = (GID_T*)malloc(sizeof(GID_T) * sum_who_provide);
    GID_T* who_need = (GID_T*)malloc(sizeof(GID_T) * sum_who_need);
    size_t i_who_provide = 0, i_who_need = 0;
    for (auto& iter : global_border_vertexes_with_dependencies) {
      for (auto& iter_who_provide : *iter.second->who_provide_) {
        who_provide[i_who_provide++] = iter_who_provide;
      }
      for (auto& iter_who_need : *iter.second->who_need_) {
        who_need[i_who_need++] = iter_who_need;
      }
    }
    std::ofstream graph_dependencies_file(output_pt,
                                          std::ios::binary | std::ios::app);
    graph_dependencies_file.write((char*)&num_border_vertexes, sizeof(size_t));
    graph_dependencies_file.write((char*)&sum_who_provide, sizeof(size_t));
    graph_dependencies_file.write((char*)&sum_who_need, sizeof(size_t));
    graph_dependencies_file.write((char*)border_vertexes,
                                  sizeof(VID_T) * num_border_vertexes);
    graph_dependencies_file.write((char*)offset_who_provide,
                                  sizeof(size_t) * num_border_vertexes);
    graph_dependencies_file.write((char*)offset_who_need,
                                  sizeof(size_t) * num_border_vertexes);
    graph_dependencies_file.write((char*)who_provide,
                                  sizeof(GID_T) * sum_who_provide);
    graph_dependencies_file.write((char*)who_need,
                                  sizeof(GID_T) * sum_who_need);
    free(border_vertexes);
    free(num_gid_who_provide_for_each_vertex);
    free(num_gid_who_need_for_each_vertex);
    free(offset_who_provide);
    free(offset_who_need);
    free(who_provide);
    free(who_need);
    graph_dependencies_file.close();
    return true;
  }

  std::unordered_map<VID_T, VertexDependencies<VID_T, GID_T>*>*
  ReadGraphDependencies(const std::string& input_pt) {
    std::ifstream graph_dependencies_file(input_pt,
                                          std::ios::binary | std::ios::app);
    size_t num_border_vertexes, sum_who_need, sum_who_provide;

    graph_dependencies_file.read((char*)&num_border_vertexes, sizeof(size_t));
    graph_dependencies_file.read((char*)&sum_who_provide, sizeof(size_t));
    graph_dependencies_file.read((char*)&sum_who_need, sizeof(size_t));

    VID_T* border_vertexes =
        (VID_T*)malloc(sizeof(VID_T) * num_border_vertexes);
    graph_dependencies_file.read((char*)border_vertexes,
                                 sizeof(VID_T) * num_border_vertexes);
    size_t* offset_who_need =
        (size_t*)malloc(sizeof(size_t) * num_border_vertexes);
    size_t* offset_who_provide =
        (size_t*)malloc(sizeof(size_t) * num_border_vertexes);

    graph_dependencies_file.read((char*)offset_who_provide,
                                 sizeof(size_t) * num_border_vertexes);
    graph_dependencies_file.read((char*)offset_who_need,
                                 sizeof(size_t) * num_border_vertexes);

    GID_T* who_provide = (GID_T*)malloc(sizeof(GID_T) * sum_who_provide);
    GID_T* who_need = (GID_T*)malloc(sizeof(GID_T) * sum_who_need);
    memset(who_provide, 0, sizeof(GID_T) * sum_who_provide);
    memset(who_need, 0, sizeof(GID_T) * sum_who_need);
    graph_dependencies_file.read((char*)who_provide,
                                 sizeof(GID_T) * sum_who_provide);
    graph_dependencies_file.read((char*)who_need, sizeof(GID_T) * sum_who_need);
    auto global_border_vertexes_with_dependencies =
        new std::unordered_map<VID_T, VertexDependencies<VID_T, GID_T>*>;
    global_border_vertexes_with_dependencies->reserve(num_border_vertexes);

    for (size_t i = 0; i < num_border_vertexes; i++) {
      VertexDependencies<VID_T, GID_T>* vd =
          new VertexDependencies<VID_T, GID_T>(border_vertexes[i]);
      if (i < num_border_vertexes - 1) {
        size_t scope = offset_who_provide[i + 1] - offset_who_provide[i];
        for (size_t j = 0; j < scope; j++) {
          vd->who_provide_->push_back(who_provide[offset_who_provide[i] + j]);
        }
        scope = offset_who_need[i + 1] - offset_who_need[i];
        for (size_t j = 0; j < scope; j++) {
          vd->who_need_->push_back(who_need[offset_who_need[i] + j]);
        }
      } else {
        size_t scope = sum_who_provide - offset_who_provide[i];
        for (size_t j = 0; j < scope; j++) {
          vd->who_provide_->push_back(who_provide[offset_who_provide[i] + j]);
        }
        scope = sum_who_need - offset_who_need[i];
        for (size_t j = 0; j < scope; j++) {
          vd->who_need_->push_back(who_need[offset_who_need[i] + j]);
        }
      }
      global_border_vertexes_with_dependencies->insert(
          std::make_pair(border_vertexes[i], vd));
    }
    free(border_vertexes);
    free(offset_who_provide);
    free(offset_who_need);
    free(who_provide);
    free(who_need);
    graph_dependencies_file.close();
    // for (auto& iter : *global_border_vertexes_with_dependencies) {
    //   iter.second->ShowVertexDependencies();
    // }
    return global_border_vertexes_with_dependencies;
  }

  bool WriteCommunicationMatrix(const std::string& output_pt,
                                bool* communication_matrix,
                                const size_t num_graphs) {
    std::ofstream communication_matrix_file(output_pt, std::ios::binary);

    communication_matrix_file.write((char*)&num_graphs, sizeof(size_t));
    communication_matrix_file.write((char*)communication_matrix,
                                    sizeof(bool) * num_graphs * num_graphs);
    communication_matrix_file.close();
    return true;
  }

  bool* ReadCommunicationMatrix(const std::string& input_pt) {
    std::ifstream communication_matrix_file(input_pt, std::ios::binary);
    size_t num_graphs = 0;
    communication_matrix_file.read((char*)&num_graphs, sizeof(size_t));
    bool* communication_matrix =
        (bool*)malloc(sizeof(bool) * num_graphs * num_graphs);
    communication_matrix_file.read((char*)communication_matrix,
                                   sizeof(bool) * num_graphs * num_graphs);
    for (size_t i = 0; i < num_graphs; i++) {
      for (size_t j = 0; j < num_graphs; j++) {
        std::cout << *(communication_matrix + i * num_graphs + j) << ", ";
      }
      std::cout << std::endl;
    }
    communication_matrix_file.close();
    return communication_matrix;
  }

  std::unordered_map<VID_T, std::vector<GID_T>*>* ReadBorderVertexes(
      const std::string& border_vertexes_pt) {
    auto global_border_vertexes =
        new std::unordered_map<VID_T, std::vector<GID_T>*>();
    std::ifstream border_vertexes_file(border_vertexes_pt,
                                       std::ios::binary | std::ios::app);
    size_t global_border_vertexes_size;

    border_vertexes_file.read((char*)&global_border_vertexes_size,
                              sizeof(size_t));
    VID_T* buf_global_border_vertexes =
        (VID_T*)malloc(sizeof(VID_T*) * global_border_vertexes_size);
    border_vertexes_file.read((char*)buf_global_border_vertexes,
                              sizeof(VID_T) * global_border_vertexes_size);

    for (size_t i = 0; i < global_border_vertexes_size; i++) {
      global_border_vertexes->insert(
          std::make_pair(buf_global_border_vertexes[i], nullptr));
    }
    LOG_INFO("LOAD global_border_vertexes: ", global_border_vertexes->size());
    return global_border_vertexes;
  }

  GRAPH_BASE_T* GetGraph(const GID_T& gid) {
    if (pgraph_by_gid_->count(gid)) {
      return pgraph_by_gid_->find(gid)->second;
    } else {
      return nullptr;
    }
  }

  void EraseGraph(const GID_T& gid) {
    if (pgraph_by_gid_->count(gid)) {
      auto graph = (CSR_T*)pgraph_by_gid_->find(gid)->second;
      graph->CleanUp();
      // delete graph;
      pgraph_by_gid_->erase(gid);
    }
  };

  void CleanUp() {
    for (auto& iter : *pgraph_by_gid_) {
      iter.second->CleanUp();
      pgraph_by_gid_->erase(iter.first);
    }
  }

  void MakeDirectory(const std::string& pt) {
    std::string dir = pt;
    int len = dir.size();
    if (dir[len - 1] != '/') {
      dir[len] = '/';
      len++;
    }
    std::string temp;
    for (int i = 1; i < len; i++) {
      if (dir[i] == '/') {
        temp = dir.substr(0, i);
        if (access(temp.c_str(), 0) != 0) {
          if (mkdir(temp.c_str(), 0777) != 0) {
            VLOG(1) << "failed operaiton.";
          }
        }
      }
    }
  }

  bool IsExist(const std::string& pt) const {
    struct stat buffer;
    return (stat(pt.c_str(), &buffer) == 0);
  }

 private:
  std::unique_ptr<MSG_T> global_msg_ = nullptr;
  std::unique_ptr<folly::AtomicHashMap<GID_T, GRAPH_BASE_T*>> pgraph_by_gid_ =
      nullptr;
};

}  // namespace io
}  // namespace utility
}  // namespace minigraph
#endif  // MINIGRAPH_DATA_MNGR_H
