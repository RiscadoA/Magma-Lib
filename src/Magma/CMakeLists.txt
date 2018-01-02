file(GLOB_RECURSE MAGMA_STREAMS_SRC
    "*.hpp"
    "*.cpp"
)

add_library(Magma-Lib ${MAGMA_STREAMS_SRC})
