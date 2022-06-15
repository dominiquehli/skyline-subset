#include <cstring>
#include <iostream>
#include <vector>
#include <map>
#include <utility>
#include "dataset.h"
#include "method.h"
#include "timer.h"
#include "BSkyTreePMethod.h"
#include "BSkyTreeSMethod.h"
#include "SDIMethod.h"
#include "SDISubsetMethod.h"
#include "SFSMethod.h"
#include "SFSSubsetMethod.h"
#include "SaLSaMethod.h"
#include "SaLSaSubsetMethod.h"
using namespace std;

vector<string> DIMS; // Dimensionality.
int QUERIES; // Subset queries.
int SKYLINE; // Skyline size.
vector<int> AttList{}; // Dataset for BSkyTree.
vector<Point> Skyline{}; // Skyline for BSkyTree.

// Check data streams.
bool __CheckDataStreams(const char *, const char *, map<size_t, string> &);

// Run all algorithm with default settings.
template<class METHOD>
double __RunAlgorithm(Dataset<double, double> &);

// Algorithms.
void RunBSkyTreeP(map<size_t, string> &, vector<double> &);
void RunBSkyTreeS(map<size_t, string> &, vector<double> &);
void RunSDI(map<size_t, string> &, vector<double> &);
void RunSDISubset(map<size_t, string> &, vector<double> &);
void RunSFS(map<size_t, string> &, vector<double> &);
void RunSFSSubset(map<size_t, string> &, vector<double> &);
void RunSaLSa(map<size_t, string> &, vector<double> &);
void RunSaLSaSubset(map<size_t, string> &, vector<double> &);

int main(int argc, char **argv) {
  if (argc < 3) {
    cout << "Usage: card <PREFIX> <SUFFIX> [ALGORITHM]" << endl;
    return 0;
  }
  QUERIES = 0;
  SKYLINE = 0;
  map<size_t, string> inputs;
  map<string, vector<double>> results;
  __CheckDataStreams(argv[1], argv[2], inputs);
  if (argc > 3) {
    if (strcmp(argv[3], "BSkyTreeP") == 0) {
      RunBSkyTreeP(inputs, results["BSkyTreeP"]);
    } else if (strcmp(argv[3], "BSkyTreeS") == 0) {
      RunBSkyTreeS(inputs, results["BSkyTreeS"]);
    } else if (strcmp(argv[3], "SDI") == 0) {
      RunSDI(inputs, results["SDI"]);
    } else if (strcmp(argv[3], "SDI-Subset") == 0) {
      RunSDISubset(inputs, results["SDI-Subset"]);
    } else if (strcmp(argv[3], "SFS") == 0) {
      RunSFS(inputs, results["SFS"]);
    } else if (strcmp(argv[3], "SFS-Subset") == 0) {
      RunSFSSubset(inputs, results["SFS-Subset"]);
    } else if (strcmp(argv[3], "SaLSa") == 0) {
      RunSaLSa(inputs, results["SaLSa"]);
    } else if (strcmp(argv[3], "SaLSa-Subset") == 0) {
      RunSaLSaSubset(inputs, results["SaLSa-Subset"]);
    }
  } else {
    RunBSkyTreeP(inputs, results["BSkyTreeP"]);
    RunBSkyTreeS(inputs, results["BSkyTreeS"]);
    RunSDI(inputs, results["SDI"]);
    RunSDISubset(inputs, results["SDI-Subset"]);
    RunSFS(inputs, results["SFS"]);
    RunSFSSubset(inputs, results["SFS-Subset"]);
    RunSaLSa(inputs, results["SaLSa"]);
    RunSaLSaSubset(inputs, results["SaLSa-Subset"]);
  }
  vector<map<string, double>> outputs(DIMS.size());
  for (auto &r: results) {
    for (size_t i = 0; i < DIMS.size(); ++i) {
      outputs[i][r.first] = r.second[i];
    }
  }
  for (auto &x: outputs) {
    cout << "#";
    for (auto &it: x) {
      cout << "\t" << it.second << "\t" << it.first;
    }
    cout << endl;
  }
  return 0;
}

bool __CheckDataStreams(const char *prefix, const char *suffix, map<size_t, string> &inputs) {
  DIMS = {"2", "3", "4", "5", "6", "7", "8", "9", "10",
          "12", "14", "16", "18", "20", "22", "24"};
  for (auto &d: DIMS) {
    string filename = prefix;
    filename.append((d.size() == 1 ? "0" : "")).append(d).append("D");
    filename.append(suffix).append("K.csv");
    ifstream ifs(filename);
    if (ifs.good()) {
      inputs[std::stoi(d)] = filename;
    } else {
      cerr << "Invalid input file: " << filename << endl;
      ifs.close();
      return false;
    }
    ifs.close();
  }
  cout << inputs.size() << " input files: " << endl;
  for (auto &input: inputs) {
    cout << "  -- " << input.second << endl;
  }
  return true;
}

template<class METHOD>
double __RunAlgorithm(Dataset<double, double> &dataset) {
  AttList.clear();
  Skyline.clear();
  GlobalAttList.clear();
  nGlobalAtt = 0;
  nGMeasure = 0;
  auto *method = new METHOD();
  method->assign(dataset);
  method->build();
  method->prepare();
  method->query();
  method->finalize();
  SKYLINE = method->skyline().size();
  double measure = method->DT * 1.0 / dataset.size();
  delete method;
  return measure;
}

void RunBSkyTreeP(map<size_t, string> &inputs, vector<double> &results) {
  cout << "Running BSkyTreeP..." << endl;
  for (auto &input: inputs) {
    cout << "  -- Input: " << input.second << endl;
    ifstream in(input.second);
    auto *dataset = new Dataset<double, double>(input.first, in);
    double dt = __RunAlgorithm<BSkyTreePMethod>(*dataset);
    delete dataset;
    cout << "  -- Skyline: " << SKYLINE << endl;
    cout << "  -- D/T: " << dt << endl;
    results.push_back(dt);
    in.close();
  }
}

void RunBSkyTreeS(map<size_t, string> &inputs, vector<double> &results) {
  cout << "Running BSkyTreeS..." << endl;
  for (auto &input: inputs) {
    cout << "  -- Input: " << input.second << endl;
    ifstream in(input.second);
    auto *dataset = new Dataset<double, double>(input.first, in);
    double dt = __RunAlgorithm<BSkyTreeSMethod>(*dataset);
    delete dataset;
    cout << "  -- Skyline: " << SKYLINE << endl;
    cout << "  -- D/T: " << dt << endl;
    results.push_back(dt);
    in.close();
  }
}

void RunSDI(map<size_t, string> &inputs, vector<double> &results) {
  cout << "Running SDI..." << endl;
  for (auto &input: inputs) {
    cout << "  -- Input: " << input.second << endl;
    ifstream in(input.second);
    auto *dataset = new Dataset<double, double>(input.first, in);
    double dt = __RunAlgorithm<SDIMethod<>>(*dataset);
    delete dataset;
    cout << "  -- Skyline: " << SKYLINE << endl;
    cout << "  -- D/T: " << dt << endl;
    results.push_back(dt);
    in.close();
  }
}

void RunSDISubset(map<size_t, string> &inputs, vector<double> &results) {
  cout << "Running SDI-Subset..." << endl;
  for (auto &input: inputs) {
    cout << "  -- Input: " << input.second << endl;
    ifstream in(input.second);
    auto *dataset = new Dataset<double, double>(input.first, in);
    double dt = __RunAlgorithm<SDISubsetMethod<>>(*dataset);
    delete dataset;
    cout << "  -- Skyline: " << SKYLINE << endl;
    cout << "  -- D/T: " << dt << endl;
    results.push_back(dt);
    in.close();
  }
}

void RunSFS(map<size_t, string> &inputs, vector<double> &results) {
  cout << "Running SFS..." << endl;
  for (auto &input: inputs) {
    cout << "  -- Input: " << input.second << endl;
    ifstream in(input.second);
    auto *dataset = new Dataset<double, double>(input.first, in);
    double dt = __RunAlgorithm<SFSMethod<>>(*dataset);
    delete dataset;
    cout << "  -- Skyline: " << SKYLINE << endl;
    cout << "  -- D/T: " << dt << endl;
    results.push_back(dt);
    in.close();
  }
}

void RunSFSSubset(map<size_t, string> &inputs, vector<double> &results) {
  cout << "Running SFS-Subset..." << endl;
  for (auto &input: inputs) {
    cout << "  -- Input: " << input.second << endl;
    ifstream in(input.second);
    auto *dataset = new Dataset<double, double>(input.first, in);
    double dt = __RunAlgorithm<SFSSubsetMethod<>>(*dataset);
    delete dataset;
    cout << "  -- Skyline: " << SKYLINE << endl;
    cout << "  -- D/T: " << dt << endl;
    results.push_back(dt);
    in.close();
  }
}

void RunSaLSa(map<size_t, string> &inputs, vector<double> &results) {
  cout << "Running SaLSa..." << endl;
  for (auto &input: inputs) {
    cout << "  -- Input: " << input.second << endl;
    ifstream in(input.second);
    auto *dataset = new Dataset<double, double>(input.first, in);
    double dt = __RunAlgorithm<SaLSaMethod<>>(*dataset);
    delete dataset;
    cout << "  -- Skyline: " << SKYLINE << endl;
    cout << "  -- D/T: " << dt << endl;
    results.push_back(dt);
    in.close();
  }
}

void RunSaLSaSubset(map<size_t, string> &inputs, vector<double> &results) {
  cout << "Running SaLSa-Subset..." << endl;
  for (auto &input: inputs) {
    cout << "  -- Input: " << input.second << endl;
    ifstream in(input.second);
    auto *dataset = new Dataset<double, double>(input.first, in);
    double dt = __RunAlgorithm<SaLSaSubsetMethod<>>(*dataset);
    delete dataset;
    cout << "  -- Skyline: " << SKYLINE << endl;
    cout << "  -- D/T: " << dt << endl;
    results.push_back(dt);
    in.close();
  }
}
