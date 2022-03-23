cmake_minimum_required(VERSION 3.15)
project(Skyline-Subset)

set(CMAKE_CXX_STANDARD 11)

add_definitions(-DMEASURE -DXDT -DXST -DXTT)

include_directories(.)

set(BENCH .)
set(BENCH_HEADERS
        ${BENCH}/include/bench.h
        ${BENCH}/include/dataset.h
        ${BENCH}/include/method.h
        ${BENCH}/include/subset.h
        ${BENCH}/include/subspace.h
        ${BENCH}/include/timer.h
        )
set(BENCH_INCLUDE ${BENCH}/include)
set(BENCH_METHODS ${BENCH}/methods)

# Share

set(BENCH_SHARE ${BENCH}/share)
set(BENCH_SHARE_BSKYTREE ${BENCH_SHARE}/BSkyTree)
set(BENCH_SOURCE_BSKYTREE
        ${BENCH_SHARE_BSKYTREE}/BSkyTreeP.cpp
        ${BENCH_SHARE_BSKYTREE}/BSkyTreeS.cpp
        ${BENCH_SHARE_BSKYTREE}/Common.cpp
        ${BENCH_SHARE_BSKYTREE}/DeleteSkyTree.cpp
        ${BENCH_SHARE_BSKYTREE}/InsertSkyTree.cpp
        ${BENCH_SHARE_BSKYTREE}/PivotSelection.cpp
        ${BENCH_SHARE_BSKYTREE}/SkyTreeUtil.cpp
        )

# BNL
add_executable(BNL ${BENCH_METHODS}/BNL/bench.cpp ${BENCH_HEADERS})
target_include_directories(BNL PUBLIC ${BENCH_INCLUDE})

# BSkyTreeP
add_executable(BSkyTreeP ${BENCH_METHODS}/BSkyTreeP/bench.cpp ${BENCH_HEADERS} ${BENCH_SOURCE_BSKYTREE})
target_include_directories(BSkyTreeP PUBLIC ${BENCH_INCLUDE} ${BENCH_SHARE_BSKYTREE})

# BSkyTreeS
add_executable(BSkyTreeS ${BENCH_METHODS}/BSkyTreeS/bench.cpp ${BENCH_HEADERS} ${BENCH_SOURCE_BSKYTREE})
target_include_directories(BSkyTreeS PUBLIC ${BENCH_INCLUDE} ${BENCH_SHARE_BSKYTREE})

# SDI
add_executable(SDI ${BENCH_METHODS}/SDI/bench.cpp ${BENCH_HEADERS})
target_include_directories(SDI PUBLIC ${BENCH_INCLUDE})

# SDI-Subset
add_executable(SDI-Subset ${BENCH_METHODS}/SDI-Subset/bench.cpp ${BENCH_HEADERS})
target_include_directories(SDI-Subset PUBLIC ${BENCH_INCLUDE})

# SFS
add_executable(SFS ${BENCH_METHODS}/SFS/bench.cpp ${BENCH_HEADERS})
target_include_directories(SFS PUBLIC ${BENCH_INCLUDE})

# SFS-Subset
add_executable(SFS-Subset ${BENCH_METHODS}/SFS-Subset/bench.cpp ${BENCH_HEADERS})
target_include_directories(SFS-Subset PUBLIC ${BENCH_INCLUDE})

# SaLSa
add_executable(SaLSa ${BENCH_METHODS}/SaLSa/bench.cpp ${BENCH_HEADERS})
target_include_directories(SaLSa PUBLIC ${BENCH_INCLUDE})

# SaLSa-Subset
add_executable(SaLSa-Subset ${BENCH_METHODS}/SaLSa-Subset/bench.cpp ${BENCH_HEADERS})
target_include_directories(SaLSa-Subset PUBLIC ${BENCH_INCLUDE})
