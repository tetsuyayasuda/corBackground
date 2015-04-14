cmake_minimum_required(VERSION 2.8)

set(app corBackground)

### SFITS ###
set(SFITS_LIB sfitsio sllib z bz2 readline curses)


include_directories(include)

add_executable(${app}
  ${app}.cc
  src/readFitsBase.cc
  src/readFitsCor.cc
  src/readFitsTime.cc
  src/readFits.cc
)

target_link_libraries(${app} ${SFITS_LIB}
)
