CXX = c++
CXXFLAGS = -O3 -std=c++11 -I. -I$(INCLUDE) -DMEASURE -DXDT -DXST -DXTT

BIN = bin
INCLUDE = include
METHODS = methods
SHARE = share
SHARE_BSKYTREE = $(SHARE)/BSkyTree
SHARE_BSKYTREE_CPP = $(SHARE_BSKYTREE)/BSkyTreeP.cpp \
                     $(SHARE_BSKYTREE)/BSkyTreeS.cpp \
                     $(SHARE_BSKYTREE)/Common.cpp \
                     $(SHARE_BSKYTREE)/DeleteSkyTree.cpp \
                     $(SHARE_BSKYTREE)/InsertSkyTree.cpp \
                     $(SHARE_BSKYTREE)/PivotSelection.cpp \
                     $(SHARE_BSKYTREE)/SkyTreeUtil.cpp


all: BNL BSkyTreeP BSkyTreeS SDI SDI-Subset SFS SFS-Subset SaLSa SaLSa-Subset

bin:
	mkdir -p $(BIN)

BNL: bin
	$(CXX) $(CXXFLAGS) -o $(BIN)/$@ $(METHODS)/$@/bench.cpp

BSkyTreeP: bin
	$(CXX) $(CXXFLAGS) -I$(SHARE_BSKYTREE) -o $(BIN)/$@ $(METHODS)/$@/bench.cpp $(SHARE_BSKYTREE_CPP)

BSkyTreeS: bin
	$(CXX) $(CXXFLAGS) -I$(SHARE_BSKYTREE) -o $(BIN)/$@ $(METHODS)/$@/bench.cpp $(SHARE_BSKYTREE_CPP)

SDI: bin
	$(CXX) $(CXXFLAGS) -o $(BIN)/$@ $(METHODS)/$@/bench.cpp

SDI-Subset: bin
	$(CXX) $(CXXFLAGS) -o $(BIN)/$@ $(METHODS)/$@/bench.cpp

SFS: bin
	$(CXX) $(CXXFLAGS) -o $(BIN)/$@ $(METHODS)/$@/bench.cpp

SFS-Subset: bin
	$(CXX) $(CXXFLAGS) -o $(BIN)/$@ $(METHODS)/$@/bench.cpp

SaLSa: bin
	$(CXX) $(CXXFLAGS) -o $(BIN)/$@ $(METHODS)/$@/bench.cpp

SaLSa-Subset: bin
	$(CXX) $(CXXFLAGS) -o $(BIN)/$@ $(METHODS)/$@/bench.cpp

clean:
	rm -f $(BIN)/*
