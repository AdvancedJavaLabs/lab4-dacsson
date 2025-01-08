#include "MapReduceAgr.h"
#include "Pipes.hh"
#include "Product.h"

// get negative key of revenue 
void AgrMapper::map(HadoopPipes::MapContext& context) {
  std::string line = context.getInputValue();

  std::vector<std::string> tokens = HadoopUtils::splitString(line, ",");

  if (tokens.size() != 3) { 
    std::cerr << "Lines doesnt contain needed tokens ( < 3) ";
    return; 
  }

  double price = std::stod(tokens[1]);
  int quantity = std::stoi(tokens[2]);
  std::string name = tokens[0];

  std::string value = name + "," + std::to_string(quantity);

  std::cout << "AGR MAP: " << tokens[2] << " " << tokens[1] << " " << tokens[0] << " | " << line << "|" << std::to_string(price * -1.0) << value << std::endl;

  context.emit(std::to_string(price * -1.0), value); 
}

void AgrReducer::reduce(HadoopPipes::ReduceContext& context) {
  long double revenue = 0.0;
  int quantity = 0;
  std::string name;
  
  /*
  if(context.getInputKey().empty() || !context.getInputValue().empty()) {
    std::cout << "Empty context" << std::endl;
    return;
  }
  */
  while(context.nextValue()) {
    std::cout << "AGR RED START: " << context.getInputKey() << " " << context.getInputValue() << std::endl;
    std::string value = context.getInputValue();
    std::vector<std::string> tokens = HadoopUtils::splitString(value, ",");
  
    name =tokens[0];
    quantity = std::stoi(tokens[1]);
    revenue = std::stod(context.getInputKey()) * -1.0;

  }
  /*
  while (context.nextValue()) {
    name =tokens[0];
    quantity = std::stoi(tokens[1]);
    revenue = std::stod(context.getInputKey()) * -1.0;

    //std::cout << "looking at: " << context.getInputKey() << " | " << totalQuantity << " " << totalRevenue << std::endl;
  }

  std::cout << "AGR RED: " << context.getInputKey() << " " << quantity << " " << revenue << " " << name;
  */
  context.emit(std::to_string(revenue), name + " " + std::to_string(quantity));
}
