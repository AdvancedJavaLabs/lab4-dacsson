#ifndef REVMAPPERAGR_H
#define REVMAPPERAGR_H 

#include "Pipes.hh"
#include "StringUtils.hh"
#include <iostream>
#include <algorithm>

class AgrMapper : public HadoopPipes::Mapper {
public:
  AgrMapper(HadoopPipes::TaskContext& context) {}
  
  void map(HadoopPipes::MapContext& context);
};

class AgrReducer : public HadoopPipes::Reducer {
public:
  AgrReducer(HadoopPipes::TaskContext& context) {}

  void reduce(HadoopPipes::ReduceContext& context);
};

#endif 
