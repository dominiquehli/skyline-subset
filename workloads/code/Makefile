BIN = ../bin
INCLUDE = ../../include
METHODS = ../../methods
SHARE = ../../share

CXX = c++
CXXFLAGS = -O3 -std=c++11 -I. -I$(INCLUDE) \
           -I$(METHODS)/BSkyTreeP \
           -I$(METHODS)/BSkyTreeS \
           -I$(METHODS)/SDI \
           -I$(METHODS)/SFS \
           -I$(METHODS)/SaLSa \
           -I$(METHODS)/SDI-Subset \
           -I$(METHODS)/SFS-Subset \
           -I$(METHODS)/SaLSa-Subset \
           -I$(SHARE_BSKYTREE)

SHARE_BSKYTREE = $(SHARE)/BSkyTree
SHARE_BSKYTREE_CPP = $(SHARE_BSKYTREE)/BSkyTreeP.cpp \
                     $(SHARE_BSKYTREE)/BSkyTreeS.cpp \
                     $(SHARE_BSKYTREE)/Common.cpp \
                     $(SHARE_BSKYTREE)/DeleteSkyTree.cpp \
                     $(SHARE_BSKYTREE)/InsertSkyTree.cpp \
                     $(SHARE_BSKYTREE)/PivotSelection.cpp \
                     $(SHARE_BSKYTREE)/SkyTreeUtil.cpp

all: card-dt card-rt dims-dt dims-rt

bin:
	mkdir -p $(BIN)

card-dt: bin
	$(CXX) $(CXXFLAGS) -DMEASURE -DXDT -DXSQ -o $(BIN)/$@ $@.cpp $(SHARE_BSKYTREE_CPP)

card-rt: bin
	$(CXX) $(CXXFLAGS) -o $(BIN)/$@ $@.cpp $(SHARE_BSKYTREE_CPP)

dims-dt: bin
	$(CXX) $(CXXFLAGS) -DMEASURE -DXDT -DXSQ -o $(BIN)/$@ $@.cpp $(SHARE_BSKYTREE_CPP)

dims-rt: bin
	$(CXX) $(CXXFLAGS) -o $(BIN)/$@ $@.cpp $(SHARE_BSKYTREE_CPP)

clean:
	rm -f $(BIN)/card-dt
	rm -f $(BIN)/card-rt
	rm -f $(BIN)/dims-dt
	rm -f $(BIN)/dims-rt
