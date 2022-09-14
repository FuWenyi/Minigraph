#include "graphs/edge_list.h"
#include "graphs/immutable_csr.h"
#include "portability/sys_data_structure.h"
#include "portability/sys_types.h"
#include "utility/io/data_mngr.h"
#include "utility/io/edge_list_io_adapter.h"
#include "utility/paritioner/edge_cut_partitioner.h"
#include <gflags/gflags.h>
#include <sys/stat.h>
#include <iostream>
#include <string>

using CSR_T = minigraph::graphs::ImmutableCSR<gid_t, vid_t, vdata_t, edata_t>;
using GRAPH_BASE_T = minigraph::graphs::Graph<gid_t, vid_t, vdata_t, edata_t>;
using EDGE_LIST_T = minigraph::graphs::EdgeList<gid_t, vid_t, vdata_t, edata_t>;

int main(int argc, char* argv[]) {
  gflags::ParseCommandLineFlags(&argc, &argv, true);

  if (FLAGS_p) {
    assert(FLAGS_i != "" && FLAGS_o != "");
    std::cout << " #Partitioning: "
              << " input: " << FLAGS_i << " output: " << FLAGS_o
              << " init_model: " << FLAGS_init_model
              << " cores: " << FLAGS_cores << std::endl;
    std::string src_pt = FLAGS_i;
    std::string dst_pt = FLAGS_o;
    std::string init_model = FLAGS_init_model;
    std::size_t cores = FLAGS_cores;
    std::size_t num_partitions = FLAGS_n;
    std::size_t max_vid = FLAGS_vertexes;
    max_vid = (ceil(max_vid / 64) + 1) * 64;
    unsigned init_val = FLAGS_init_val;

    minigraph::utility::io::DataMngr<CSR_T> data_mngr;
    minigraph::utility::partitioner::EdgeCutPartitioner<CSR_T>
        edge_cut_partitioner(max_vid);

    edge_cut_partitioner.ParallelPartition(src_pt, ',', num_partitions, max_vid,
                                           cores, init_model, init_val);

    if (!data_mngr.IsExist(dst_pt + "minigraph_meta/")) {
      data_mngr.MakeDirectory(dst_pt + "minigraph_meta/");
    } else {
      remove((dst_pt + "minigraph_meta/").c_str());
      data_mngr.MakeDirectory(dst_pt + "minigraph_meta/");
    }
    if (!data_mngr.IsExist(dst_pt + "minigraph_data/")) {
      data_mngr.MakeDirectory(dst_pt + "minigraph_data/");
    } else {
      remove((dst_pt + "minigraph_data/").c_str());
      data_mngr.MakeDirectory(dst_pt + "minigraph_data/");
    }
    if (!data_mngr.IsExist(dst_pt + "minigraph_vdata/")) {
      data_mngr.MakeDirectory(dst_pt + "minigraph_vdata/");
    } else {
      remove((dst_pt + "minigraph_vdata/").c_str());
      data_mngr.MakeDirectory(dst_pt + "minigraph_vdata/");
    }
    if (!data_mngr.IsExist(dst_pt + "minigraph_border_vertexes/")) {
      data_mngr.MakeDirectory(dst_pt + "minigraph_border_vertexes/");
    } else {
      remove((dst_pt + "minigraph_border_vertexes/").c_str());
      data_mngr.MakeDirectory(dst_pt + "minigraph_border_vertexes/");
    }
    if (!data_mngr.IsExist(dst_pt + "minigraph_message/")) {
      data_mngr.MakeDirectory(dst_pt + "minigraph_message/");
    } else {
      remove((dst_pt + "minigraph_message/").c_str());
      data_mngr.MakeDirectory(dst_pt + "minigraph_message/");
    }

    auto fragments = edge_cut_partitioner.GetFragments();
    size_t count = 0;
    for (auto& iter_fragments : *fragments) {
      auto fragment = (CSR_T*)iter_fragments;
      std::string meta_pt =
          dst_pt + "minigraph_meta/" + std::to_string(count) + ".bin";
      std::string data_pt =
          dst_pt + "minigraph_data/" + std::to_string(count) + ".bin";
      std::string vdata_pt =
          dst_pt + "minigraph_vdata/" + std::to_string(count) + ".bin";
      data_mngr.csr_io_adapter_->Write(*fragment, csr_bin, false, meta_pt,
                                       data_pt, vdata_pt);
      count++;
    }

    LOG_INFO("WriteCommunicationMatrix.");
    auto pair_communication_matrix =
        edge_cut_partitioner.GetCommunicationMatrix();
    data_mngr.WriteCommunicationMatrix(
        dst_pt + "minigraph_border_vertexes/communication_matrix.bin",
        pair_communication_matrix.second, pair_communication_matrix.first);

    LOG_INFO("WriteVidMap.");
    auto vid_map = edge_cut_partitioner.GetVidMap();
    for (size_t i = 0; i < 34; i++) {
//      LOG_INFO("local: ", i, ", global: ", vid_map[i]);
    }
    data_mngr.WriteVidMap(max_vid, vid_map,
                          dst_pt + "minigraph_message/vid_map.bin");

    LOG_INFO("WriteGlobalBorderVidMap.");
    auto global_border_vid_map = edge_cut_partitioner.GetGlobalBorderVidMap();
    data_mngr.WriteBitmap(
        global_border_vid_map,
        dst_pt + "minigraph_message/global_border_vid_map.bin");
    LOG_INFO("End graph partition#");
  }
  gflags::ShutDownCommandLineFlags();
}