ADD_LIBRARY(myrobomaster SHARED
  #  SaveDepth.cpp
    ArmorDetector.cpp
    ArmorSimple.cpp
    )
TARGET_LINK_LIBRARIES(myrobomaster ${THIRD_PARITY_LIBS})
