#Path to Matrix A: 
matrixA="../matrices/mock/A.txt"

#Path to Matrix B:
matrixB="../matrices/mock/B.txt"

#Path to Matrix C (output matrix):
matrixC="../matrices/mock/C_result.txt"

#Output should be generated
generate_output="true"

#Use method parallel
use_parallel="true"

# Path to statistics file
statistics_file="../matrices//mock/statistics.txt"

#Compile
./program $matrixA $matrixB $matrixC $generate_output $statistics_file $use_parallel