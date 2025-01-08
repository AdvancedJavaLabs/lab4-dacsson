#include "MapReduce.h"
#include "Pipes.hh"
#include "Product.h"

bool is_valid_int(const std::string& s)
{
    return std::all_of(std::begin(s), std::end(s), 
                         [](unsigned char c) { 
                           return std::isdigit(c); 
                       });
}

void RevenueCountMapper::map(HadoopPipes::MapContext& context) {
  std::string line = context.getInputValue();

  std::vector<std::string> tokens = HadoopUtils::splitString(line, ",");

  if(!is_valid_int(tokens[1])) return;

  double price = std::stod(tokens[3]);
  int quantity = std::stoi(tokens[4]);
  std::string name = tokens[2];

  std::string value = std::to_string(price) + "," + std::to_string(quantity);

  std::cout << "emitting: " << name << " " << value << " | " << tokens[3] << " " << tokens[4] << " | " << line <<  std::endl;

  context.emit(name, value); 
}

void RevenueCountReducer::reduce(HadoopPipes::ReduceContext& context) {
  long double totalRevenue = 0.0;
  int totalQuantity = 0;
  while (context.nextValue()) {
    std::string value = context.getInputValue();
    std::vector<std::string> tokens = HadoopUtils::splitString(value, ",");
    if (is_valid_int(tokens[1])) {
      totalRevenue += (std::stod(tokens[0]) * std::stoi(tokens[1]));
      totalQuantity += std::stoi(tokens[1]);
    }

    std::cout << "looking at: " << context.getInputKey() << " | " << totalQuantity << " " << totalRevenue << std::endl;
  }

  std::cout << "reducing: " << context.getInputKey() << " " << totalQuantity << " " << totalRevenue;
  context.emit(context.getInputKey(), std::to_string(totalRevenue));
  context.emit(context.getInputKey(), std::to_string(totalQuantity));
}
