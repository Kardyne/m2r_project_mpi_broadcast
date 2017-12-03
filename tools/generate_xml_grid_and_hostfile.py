#!/usr/bin/env python2.7
import sys
import os
import math

# Link parameters
link_latency = "10us"
link_bandwidth = 10
link_bandwidth_unit = "Gbps"

# XML generation functions
def issueHead():
        head = ("<?xml version='1.0'?>\n"
                "<!DOCTYPE platform SYSTEM \"http://simgrid.gforge.inria.fr/simgrid/simgrid.dtd\">\n"
                "<platform version=\"4\">\n\n")

        config_clause = ("<!--  WARNING:  This <config></config> clause below\n"
                       "makes it so that NO COMPUTATION TIME is simulated. This is because\n"
                       "in this module, for pedagogic purposes, we don't want to muddy the\n"
                       "(simulation) waters with computational times. As a results, this\n"
                       "XML platform file may not be suitable for running other\n"
                       "simulations, unless you remove the <config></config> clause.\n"
                       "-->\n"
                       "<config>\n"
                       "<prop id=\"smpi/simulate-computation\" value=\"0\"></prop>\n"
                       "<prop id=\"smpi/running-power\" value=\"200000000000\"></prop>\n"
                       "</config>\n\n")

        AS_head = "<AS id=\"AS0\" routing=\"Full\">\n"

        return head + config_clause + AS_head


def issueTail():
	return "</AS>\n</platform>\n"

def issueLink(x1, y1, x2, y2):
	return "  link id=\"link-"+str(x1)+","+str(y1)+"-"+str(x2)+","+str(y2)+"\" latency=\""+str(link_latency)+"\" bandwidth=\""+str(link_bandwidth)+link_bandwidth_unit+"\"/>\n"

def issueLink3(x, y, bw):
	return "  <link id=\"link-"+str(x)+"-"+str(y)+"\" latency=\""+str(link_latency)+"\" bandwidth=\""+str(bw)+link_bandwidth_unit+"\"/>\n"

def issueHost(index1, index2):
	return "  <host id=\"host-"+str(index1)+","+str(index2)+".hawaii.edu\" speed=\"200Gf\"/>\n"

def issueRouteHead(x1, y1, x2, y2):
	return "  <route src=\"host-"+str(x1)+","+str(y1)+".hawaii.edu\" dst=\"host-"+str(x2)+","+str(y2)+".hawaii.edu\">\n"

def issueRouteTail():
	return "  </route>\n"

def issueRouteLink(x1, y1, x2, y2):
	return "\t<link_ctn id=\"link-"+str(x1)+","+str(y1)+"-"+str(x2)+","+str(y2)+"\"/>\n"

######################################################################
# Parse command-line arguments
if (len(sys.argv) != 5):
	print >> sys.stderr, "Usage:a"+sys.argv[0]+" <num hosts>\n"
	print >> sys.stderr, "  Will generate a grid_<num hosts>.xml and grid_hostfile_<num hosts>.txt file\n"
	exit(1)

num_rows = int(sys.argv[1])
num_columns = int(sys.argv[2])
num_hosts = num_rows*num_columns
host_path = sys.argv[3]
platform_path = sys.argv[4]

###############################################################
# Generate GRID XML file
filename = platform_path+"grid_"+str(num_rows)+"_"+str(num_columns)+".xml"
fh = open(filename, 'w')
fh.write(issueHead())

# Create hosts
for i in range(0,num_rows):
	for j in range(0,num_columns):
		fh.write(issueHost(i, j))

# Create links
for i in range(0,num_rows):
	for j in range(0,num_columns):
		fh.write(issueLink(i,j,(i+1)%num_rows,j))
		fh.write(issueLink(i,j,i,(j+1)%num_columns))

# Create routes between adjacent nodes
for i in range(0,num_rows):
	for j in range(0,num_columns):
		# Route to the next node in the row
		fh.write(issueRouteHead(i,j,(i+1)%num_rows,j))
		fh.write(issueRouteLink(i,j,(i+1)%num_rows,j))
		fh.write(issueRouteTail())

		# Route to the next node in the column
		fh.write(issueRouteHead(i,j,i,(j+1)%num_columns))
		fh.write(issueRouteLink(i,j,i,(j+1)%num_columns))
		fh.write(issueRouteTail())

fh.write(issueTail())
fh.close()
print >> sys.stderr, "Grid XML platform file created: "+filename

###############################################################
## Generate host file
filename = host_path+"grid_hostfile_"+str(num_rows)+","+str(num_columns)+".txt"
fh = open(filename, 'w')

for i in range(0,num_rows):
	for j in range(0,num_columns):
		fh.write("host-"+str(i)+","+str(j)+".hawaii.edu\n")

fh.close()
print >> sys.stderr, "Hostfile created: "+filename