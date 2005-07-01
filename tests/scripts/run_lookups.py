#!/usr/bin/env python2
# -*- Mode: python -*-
#
# DESCRIPTION: Setup and run n chord nodes.
#
#
import getopt
import os
import sys
import time
import random
import signal
import threading

def print_usage():
    print
    print "Usage: run_lookups -p <port> [-t <sleep_time>] [-v <vantages>] [-s <seed>] lookup_exec output_dir ip0 [ ip1 [...]]"
    print

def parse_cmdline(argv): 
    global log
    shortopts = "v:p:s:t:"
    flags = {"seed" : random.random()*sys.maxint, "vantages" : 1, "port" : 0, "sleep_time" : 1}
    opts, args = getopt.getopt(argv[1:], shortopts)
    for o, v in opts:
        if   o == "-s": flags["seed"]        = int(v)
        elif o == "-v": flags["vantages"]    = int(v)
        elif o == "-p": flags["port"]        = int(v)
        elif o == "-n": flags["num_lookups"] = int(v)
        elif o == "-t": flags["sleep_time"]  = int(v)
    if args[1]: 
        log = open(args[1]+"/lookups.log", 'w')
    return flags, args

def run_lookup(lookup_exec, seed, node, out):
    try:
        rv = os.system(r"%s NONE %d %s %s %s >> %s 2>&1" \
                        % (lookup_exec, seed, "simple_lookup:%s%d" % (node, random.random()*sys.maxint), node, node, out))
    except: 
        print >> log, "EXCEPTION RUN LOOKUP: %s\n" % str(sys.exc_info()[:2])

if __name__ == "__main__":
    try:
        flags, args = parse_cmdline(sys.argv)
    except:
        print_usage()
        sys.exit(3)
    if len(args) < 3 or not int(flags["port"]):
        print_usage()        
        sys.exit(3)

    seed       = int(flags["seed"])
    port       = int(flags["port"])
    vantages   = int(flags["vantages"])
    sleep_time = int(flags["sleep_time"])
    ips        = args[2:]

    print "IPS: ", ips

    while 1:
        try:
            for v in range(vantages):
                if len(ips) == 1: run_lookup(args[0], seed, "%s:%s" % (ips[0], int(port)+int(v)), args[1]+"/lookups.log")
                else: run_lookup(args[0], seed, "%s:%s" % (ips[v], port), args[1]+"/lookups.log")
            seed += 1
            time.sleep(sleep_time)
        except:
       	    print >> log, "EXCEPTION WHILE LOOP: %s\n" % str(sys.exc_info()[:2])


