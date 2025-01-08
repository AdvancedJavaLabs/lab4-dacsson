#ifndef REVMAPPER_H
#define REVMAPPER_H 

#include "Pipes.hh"
#include "StringUtils.hh"
#include <iostream>
#include <algorithm>

class RevenueCountMapper : public HadoopPipes::Mapper {
public:
  RevenueCountMapper(HadoopPipes::TaskContext& context) {}
  
  void map(HadoopPipes::MapContext& context);
};

class RevenueCountReducer : public HadoopPipes::Reducer {
public:
  RevenueCountReducer(HadoopPipes::TaskContext& context) {}

  void reduce(HadoopPipes::ReduceContext& context);
};

#endif 
