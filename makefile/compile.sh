rm ./bomip1
g++ -O3 -fpermissive -Wno-deprecated -Dnullptr=NULL -DIL_STD -DILOUSESTL \
    -I /opt/ibm/ILOG/CPLEX_Studio_Academic124/cplex/include -I /opt/ibm/ILOG/CPLEX_Studio_Academic124/concert/include \
    -L /opt/ibm/ILOG/CPLEX_Studio_Academic124/concert/lib/x86-64_sles10_4.1/static_pic \
    -L /opt/ibm/ILOG/CPLEX_Studio_Academic124/cplex/lib/x86-64_sles10_4.1/static_pic \
    -I /opt/ibm/ILOG/CPLEX_Studio125/cplex/include -I /opt/ibm/ILOG/CPLEX_Studio125/concert/include \
    -L /opt/ibm/ILOG/CPLEX_Studio125/concert/lib/x86-64_sles10_4.1/static_pic \
    -L /opt/ibm/ILOG/CPLEX_Studio125/cplex/lib/x86-64_sles10_4.1/static_pic \
    -I /opt/ibm/ILOG/CPLEX_Studio124/cplex/include -I /opt/ibm/ILOG/CPLEX_Studio124/concert/include \
    -L /opt/ibm/ILOG/CPLEX_Studio124/concert/lib/x86-64_sles10_4.1/static_pic \
    -L /opt/ibm/ILOG/CPLEX_Studio124/cplex/lib/x86-64_sles10_4.1/static_pic \
    -o bomip1 *.cpp -lconcert -lilocplex -lcplex -lpthread
ls -l bomip1*




