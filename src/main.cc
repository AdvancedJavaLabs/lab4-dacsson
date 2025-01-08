#include <string>
#include <stdint.h>

#include "Pipes.hh"
#include "TemplateFactory.hh"

#include "MapReduce.h"

int main(int argc, char* argv[]) {
  return HadoopPipes::runTask(
    HadoopPipes::TemplateFactory<RevenueCountMapper, RevenueCountReducer>()
  );
} 
