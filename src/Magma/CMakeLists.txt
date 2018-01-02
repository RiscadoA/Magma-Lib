file(GLOB_RECURSE MAGMA_STREAMS_SRC
    "*.hpp"
    "*.cpp"
)

add_library(Magma-Streams ${MAGMA_STREAMS_SRC})
