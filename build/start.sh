#!/bin/bash

RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m' # No Color

printf "${GREEN}Building app.\n${NC}"
make

required_processes=("NameNode" "DataNode" "ResourceManager" "NodeManager" "SecondaryNameNode")

missing_processes=()

for process in "${required_processes[@]}"; do
  if ! jps | grep -q "$process"; then
    missing_processes+=("$process")
  fi
done

if [[ ${#missing_processes[@]} -gt 0 ]]; then
  printf "${RED}[ERROR] The following required processes are not running:\n${NC}"
  printf "- %s\n" "${missing_processes[@]}"
  jps
  exit 1
else
  printf "${GREEN}[INFO] All required processes are running.\n${NC}"
  jps
fi

# Remove prev output and bin
printf "${YELLOW}[INFO] Removing previous output and bin directory${NC}\n"
$HADOOP_HOME/bin/hdfs dfs -rmr output
$HADOOP_HOME/bin/hdfs dfs -rmr input/revcounter

# Put new binary 
$HADOOP_HOME/bin/hdfs dfs -put revcounter input

printf "${GREEN}[INFO] Running MapReduce job with pipes...\n${NC}"
$HADOOP_HOME/bin/mapred pipes -D hadoop.pipes.java.recordreader=true \
                 -D hadoop.pipes.java.recordwriter=true \
                 -input input/data                       \
                 -output output                         \
                 -program input/revcounter

printf "${GREEN}[INFO] Job is succefuly finished writing results to log.txt${NC}\n"
$HADOOP_HOME/bin/hdfs dfs -cat output/part-00000 > log.txt 

#cat log.txt 
awk '
{
    # Combine all fields except the last one into the category name
    category = $1;
    for (i = 2; i < NF; i++) {
        category = category " " $i;
    }

    # Process odd and even lines
    if (NR % 2 == 1) {
        value1 = $NF; # Last field of the odd line is value1
    } else {
        value2 = $NF; # Last field of the even line is value2
        printf ""category","value1","value2"\n", category, value1, value2; # Print the category and both values
    }
}' log.txt  log.txt > output1.txt

printf "${GREEN}[INFO] First step finished with: ${NC}\n"
cat output1.txt

$HADOOP_HOME/bin/hdfs dfs -rm input/output1.txt
$HADOOP_HOME/bin/hdfs dfs -put output1.txt input

printf "${YELLOW}[INFO] Cleaning after first step ${NC}\n"
$HADOOP_HOME/bin/hdfs dfs -rmr output2
$HADOOP_HOME/bin/hdfs dfs -rmr input/revagr

# Put new binary 
$HADOOP_HOME/bin/hdfs dfs -put revagr input


printf "${GREEN}[INFO] Launching second step ${NC}\n"

printf "${GREEN}[INFO] Running MapReduce job with pipes...\n${NC}"
$HADOOP_HOME/bin/mapred pipes -D hadoop.pipes.java.recordreader=true -D hadoop.pipes.java.recordwriter=true  -D mapred.output.key.comparator.class=org.apache.hadoop.mapred.lib.KeyFieldBasedComparator -D  mapred.text.key.comparator.options=-n -input input/output1.txt -output output2 -program input/revagr 

printf "${GREEN}[INFO] Job is succefuly finished writing results to result.txt${NC}\n"
$HADOOP_HOME/bin/hdfs dfs -cat output2/part-00000 > result.txt 

cat result.txt


