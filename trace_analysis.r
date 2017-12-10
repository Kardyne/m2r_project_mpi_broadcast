plot_time_comm <- function(topology, bandwidth, latency){
  library(ggplot2)
  nb_procs <- c(16, 256, 625)
  #different sizes in bytes.
  sizes <- c(4, 10, 16, 64, 100, 256, 1000, 1024, 4096, 10000, 16384, 65536,100000,
             262144,1000000,1048576)
  times_procs_default <- vector("list", 3)
  times_procs_alg <- vector("list", 3)
  
  #Index which represents at which processor number we are (the first, the second or the third)
  i = 1
  for(proc in nb_procs){
    comm_time_default <- numeric()
    comm_time_alg <- numeric()
    for(size in sizes){
      for(option in c("-d", "")){
        file_name_start = paste("./processed_data/smpi_simgrid_", topology, sep = "")
        file_name_end = sprintf("_%d_%d_%d_%d", latency, bandwidth, proc, size)
        file_name = paste(file_name_start, file_name_end, sep = "")
        file_name = paste(file_name, option, ".csv", sep = "")
        df_link = read.csv(file_name, header=F, strip.white=T)
        
        #df_link[1,6] contains the information on when the last communication was made
        last_comm = df_link[1,6]
        if(option == "-d"){
          comm_time_default <- c(comm_time_default, last_comm)
        }
        else {
          comm_time_alg <- c(comm_time_alg, last_comm)
        }
      }
    }
    times_procs_default[[i]] <- comm_time_default
    times_procs_alg[[i]] <- comm_time_alg
    i = i + 1
  }
  
  #Big messages
  for(i in 1:3) {
    graph_title = sprintf("Comparison of AllReduce with %d processors for the ", nb_procs[i])
    graph_title = paste(graph_title, topology, sep = "")
    color1 = "red"
    color2 = "green"
    y1 = times_procs_default[i]
    y2 = times_procs_alg[i]
    plot(sizes[9:16], unlist(times_procs_default[i])[9:16],col=color2,ylim=range(c(y1,y2)),
         main=graph_title, xlab="Message size (bytes)", ylab="Time taken (seconds)",
         sub = sprintf("latency = %d us bandwidth = %d Mb/s", latency, bandwidth))
    lines(sizes[9:16], unlist(times_procs_default[i])[9:16], type="l",col=color2)
    points(sizes[9:16], unlist(times_procs_alg[i])[9:16],col=color1)
    lines(sizes[9:16], unlist(times_procs_alg[i])[9:16], type="l",col=color1)
    legend(x = "bottomright", legend=c("Optimized algorithm", "MPI_ALLREDUCE"),
           col=c("red", "green"), lty=1:2, cex=0.8)
    #The multiplicator factor of the processors in hockney model
    mult_proc = 0
    if(topology == "ring") {
      mult_proc = nb_procs[i] - 1
    }
    else {
      mult_proc = 2*sqrt(nb_procs[i]) - 2
    }
    print("Linear regression for optimized algorithm for number of processors:")
    print(nb_procs[i])
    lmResult <- lm(unlist(times_procs_default[i])[9:12] ~ sizes[9:12])
    a = unname(coef(lmResult)[2])
    b = unname(coef(lmResult)[1])
    print(a)
    print("Estimated bandwidth on the linear part of the graph")
    print(8*10^(-6)*mult_proc/a)
    print("Mb/s")
    print("Estimated latency on the linear part of the graph")
    print((10^6)*b/mult_proc)
    print("us")
  }
  
  #Short messages
  for(i in 1:3) {
    graph_title = sprintf("Comparison of AllReduce with %d processors for the ", nb_procs[i])
    graph_title = paste(graph_title, topology, sep = "")
    color1 = "red"
    color2 = "green"
    y1 = times_procs_default[i]
    y2 = times_procs_alg[i]
    plot(sizes[1:8], unlist(times_procs_default[i])[1:8],col=color2,ylim=range(c(y1,y2)),
         main=graph_title, xlab="Message size (bytes)", ylab="Time taken (seconds)",
         sub = sprintf("latency = %dus bandwidth = %dMb/s", latency, bandwidth))
    lines(sizes[1:8], unlist(times_procs_default[i])[1:8], type="l",col=color2)
    points(sizes[1:8], unlist(times_procs_alg[i])[1:8],col=color1)
    lines(sizes[1:8], unlist(times_procs_alg[i])[1:8], type="l",col=color1)
    legend(x = "topleft", legend=c("Optimized algorithm", "MPI_ALLREDUCE"),
           col=c("red", "green"), lty=1:2, cex=0.8)
  }
  
  #linear regression on a size of message for the numbers of processors
  time_message_size <- numeric()
  for(i in 1:3) {
    time_message_size = c(time_message_size, unlist(times_procs_alg[i])[9])
  }
  print("linear regression on a fixed message size for different number of processors")
  print(lm(time_message_size ~ nb_procs - 1))
  plot(y = time_message_size, x = nb_procs - 1)
}