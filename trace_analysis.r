plot_time_comm <- function(topology, bandwidth, latency){
  library(ggplot2)
  nb_procs <- c(16, 256, 625)
  #different sizes in bytes.
  sizes <- c(4, 10, 16, 64, 100, 256, 1000, 1024, 4096, 10000, 16384)
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
        file_name_end = sprintf("_%d_%d_%d_%d", bandwidth, latency, proc, size)
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
    plot(sizes[8:14], unlist(times_procs_default[i])[8:14],col=color2,ylim=range(c(y1,y2)),
         main=graph_title, xlab="Message size (bytes)", ylab="Time taken (seconds)",
         sub = sprintf("latency = %d us bandwidth = %d Mb/s", latency, bandwidth))
    lines(sizes[8:14], unlist(times_procs_default[i])[8:14], type="l",col=color2)
    points(sizes[8:14], unlist(times_procs_alg[i])[8:14],col=color1)
    lines(sizes[8:14], unlist(times_procs_alg[i])[8:14], type="l",col=color1)
    legend(x = "topleft", legend=c("Optimized algorithm", "MPI_ALLREDUCE"),
           col=c("red", "green"), lty=1:2, cex=0.8)
  }
  
  #Short messages
  for(i in 1:3) {
    graph_title = sprintf("Comparison of AllReduce with %d processors for the ", nb_procs[i])
    graph_title = paste(graph_title, topology, sep = "")
    color1 = "red"
    color2 = "green"
    y1 = times_procs_default[i]
    y2 = times_procs_alg[i]
    plot(sizes[1:7], unlist(times_procs_default[i])[1:7],col=color2,ylim=range(c(y1,y2)),
         main=graph_title, xlab="Message size (bytes)", ylab="Time taken (seconds)",
         sub = sprintf("latency = %dus bandwidth = %dMb/s", latency, bandwidth))
    lines(sizes[1:7], unlist(times_procs_default[i])[1:7], type="l",col=color2)
    points(sizes[1:7], unlist(times_procs_alg[i])[1:7],col=color1)
    lines(sizes[1:7], unlist(times_procs_alg[i])[1:7], type="l",col=color1)
    legend(x = "topleft", legend=c("Optimized algorithm", "MPI_ALLREDUCE"),
           col=c("red", "green"), lty=1:2, cex=0.8)
  }
}