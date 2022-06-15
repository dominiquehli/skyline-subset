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

vector<string> CARD; // Cardinality.
int DIMS; // Dimensionality.
int QUERIES; // Subset queries.
int SKYLINE; // Skyline size.
vector<int> AttList{}; // Dataset for BSkyTree.
vector<Point> Skyline{}; // Skyline for BSkyTree.

// Check data streams.
bool __CheckDataStreams(const char *, vector<string> &);

// Run all algorithm with default settings.
template<class METHOD>
double __RunAlgorithm(Dataset<double, double> &);

// Run SubSDI algorithm with subset index.
template<class METHOD>
double __RunAlgorithmIndex(Dataset<double, double> &);

// Algorithms.
void RunBSkyTreeP(vector<string> &, vector<double> &);
void RunBSkyTreeS(vector<string> &, vector<double> &);
void RunSDI(vector<string> &, vector<double> &);
void RunSDISubset(vector<string> &, vector<double> &);
void RunSFS(vector<string> &, vector<double> &);
void RunSFSSubset(vector<string> &, vector<double> &);
void RunSaLSa(vector<string> &, vector<double> &);
void RunSaLSaSubset(vector<string> &, vector<double> &);

int main(int argc, char **argv) {
  if (argc < 3) {
    cout << "Usage: card <PREFIX> <DIMS> [ALGORITHM]" << endl;
    return 0;
  }
  DIMS = (int) strtoul(argv[2], nullptr, 10);
  QUERIES = 0;
  SKYLINE = 0;
  vector<string> inputs;
  map<string, vector<double>> results;
  __CheckDataStreams(argv[1], inputs);
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
  vector<map<string, double>> outputs(CARD.size());
  for (auto &r: results) {
    for (size_t i = 0; i < CARD.size(); ++i) {
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

bool __CheckDataStreams(const char *prefix, vector<string> &inputs) {
  CARD = {"1", "2", "3", "4", "5", "6", "7", "8", "9", "10"};
  for (auto &c: CARD) {
    string filename = prefix;
    filename.append((c == "10" ? "" : "0")).append(c).append("00K.csv");
    ifstream ifs(filename);
    if (ifs.good()) {
      inputs.push_back(filename);
    } else {
      cerr << "Invalid input file: " << filename << endl;
      ifs.close();
      return false;
    }
    ifs.close();
  }
  cout << inputs.size() << " input files: " << endl;
  for (auto &input: inputs) {
    cout << "  -- " << input << endl;
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
  QUERIES = method->ST;
  SKYLINE = method->skyline().size();
  double measure = method->DT * 1.0 / dataset.size();
  delete method;
  return measure;
}

void RunBSkyTreeP(vector<string> &inputs, vector<double> &results) {
  cout << "Running BSkyTreeP..." << endl;
  for (auto &input: inputs) {
    cout << "  -- Input: " << input << endl;
    ifstream in(input);
    auto *dataset = new Dataset<double, double>(DIMS, in);
    double dt = __RunAlgorithm<BSkyTreePMethod>(*dataset);
    delete dataset;
    cout << "  -- Skyline: " << SKYLINE << endl;
    cout << "  -- D/T: " << dt << endl;
    results.push_back(dt);
    in.close();
  }
}

void RunBSkyTreeS(vector<string> &inputs, vector<double> &results) {
  cout << "Running BSkyTreeS..." << endl;
  for (auto &input: inputs) {
    cout << "  -- Input: " << input << endl;
    ifstream in(input);
    auto *dataset = new Dataset<double, double>(DIMS, in);
    double dt = __RunAlgorithm<BSkyTreeSMethod>(*dataset);
    delete dataset;
    cout << "  -- Skyline: " << SKYLINE << endl;
    cout << "  -- D/T: " << dt << endl;
    results.push_back(dt);
    in.close();
  }
}

void RunSDI(vector<string> &inputs, vector<double> &results) {
  cout << "Running SDI..." << endl;
  for (auto &input: inputs) {
    cout << "  -- Input: " << input << endl;
    ifstream in(input);
    auto *dataset = new Dataset<double, double>(DIMS, in);
    double dt = __RunAlgorithm<SDIMethod<>>(*dataset);
    delete dataset;
    cout << "  -- Skyline: " << SKYLINE << endl;
    cout << "  -- D/T: " << dt << endl;
    results.push_back(dt);
    in.close();
  }
}

void RunSDISubset(vector<string> &inputs, vector<double> &results) {
  cout << "Running SDI-Subset..." << endl;
  for (auto &input: inputs) {
    cout << "  -- Input: " << input << endl;
    ifstream in(input);
    auto *dataset = new Dataset<double, double>(DIMS, in);
    double dt = __RunAlgorithm<SDISubsetMethod<>>(*dataset);
    delete dataset;
    cout << "  -- Skyline: " << SKYLINE << endl;
    cout << "  -- D/T: " << dt << endl;
    results.push_back(dt);
    in.close();
  }
}

void RunSFS(vector<string> &inputs, vector<double> &results) {
  cout << "Running SFS..." << endl;
  for (auto &input: inputs) {
    cout << "  -- Input: " << input << endl;
    ifstream in(input);
    auto *dataset = new Dataset<double, double>(DIMS, in);
    double dt = __RunAlgorithm<SFSMethod<>>(*dataset);
    delete dataset;
    cout << "  -- Skyline: " << SKYLINE << endl;
    cout << "  -- D/T: " << dt << endl;
    results.push_back(dt);
    in.close();
  }
}

void RunSFSSubset(vector<string> &inputs, vector<double> &results) {
  cout << "Running SFS-Subset..." << endl;
  for (auto &input: inputs) {
    cout << "  -- Input: " << input << endl;
    ifstream in(input);
    auto *dataset = new Dataset<double, double>(DIMS, in);
    double dt = __RunAlgorithm<SFSSubsetMethod<>>(*dataset);
    delete dataset;
    cout << "  -- Skyline: " << SKYLINE << endl;
    cout << "  -- D/T: " << dt << endl;
    results.push_back(dt);
    in.close();
  }
}

void RunSaLSa(vector<string> &inputs, vector<double> &results) {
  cout << "Running SaLSa..." << endl;
  for (auto &input: inputs) {
    cout << "  -- Input: " << input << endl;
    ifstream in(input);
    auto *dataset = new Dataset<double, double>(DIMS, in);
    double dt = __RunAlgorithm<SaLSaMethod<>>(*dataset);
    delete dataset;
    cout << "  -- Skyline: " << SKYLINE << endl;
    cout << "  -- D/T: " << dt << endl;
    results.push_back(dt);
    in.close();
  }
}

void RunSaLSaSubset(vector<string> &inputs, vector<double> &results) {
  cout << "Running SaLSaS-ubset..." << endl;
  for (auto &input: inputs) {
    cout << "  -- Input: " << input << endl;
    ifstream in(input);
    auto *dataset = new Dataset<double, double>(DIMS, in);
    double dt = __RunAlgorithm<SaLSaSubsetMethod<>>(*dataset);
    delete dataset;
    cout << "  -- Skyline: " << SKYLINE << endl;
    cout << "  -- D/T: " << dt << endl;
    results.push_back(dt);
    in.close();
  }
}
