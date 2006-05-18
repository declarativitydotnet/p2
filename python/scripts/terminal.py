import dfparser
from libp2python import *
import sys
import os
import getopt
import time

DATAFLOW_NAME = "Terminal"

class Terminal(Element):
  def __init__(self, name, myaddress):
      Element.__init__(self,name, 1, 1)
      self.mode      = "terminal"
      self.myaddress = myaddress
      self.address   = None 
      self.program   = None
  def class_name(self): return "Terminal"
  def processing(self): return "h/h"
  def flow_code(self):  return "-/-"
  def print_usage(self):
      print "===== Welcome to the P2 Overlog/Dataflow Terminal ====="
      print "This terminal is setup to edit the %s dataflow." % self.name()
      print "Create an overlog or dataflow program by accumulating overlog/dataflow text"
      print "from within the \"edit\" or \"input\" mode (described below)."
      print ""
      print "The terminal has the following modes:"
      print "terminal: Accepts commands or mode change commands."
      print "input:    Enter a filename to be read in and"
      print "          added to the program."
      print "edit:     Adds the typed text to the program."
      print "address:  Enter the node address to which the"
      print "          program is to be sent." 
      print "The default mode is the terminal mode. To exit a given"
      print "mode back into the terminal type a single \".\" in the prompt." 
      print "You can only enter a given node from the terminal mode."
      print ""
      print "The following commands can be issued in any mode."
      print "print: Print to stdout, the current overlog program."
      print "clear: Clear the current dataflow script."
      print "osend: Sends the current overlog program to the last node address"
      print "       entered in the address mode." 
      print "ssend: Sends the current dataflow scirpt to the last node address"
      print "       entered in the address mode." 
      print "exit:  Exit the terminal"
  def initialize(self): 
      if not self.program:
          self.program = ""
          self.print_usage()
          self.set_delay(0, self.delay_callback) 
      else:
          self.set_delay(0, lambda: self.push_program(0)) 
      return 0
  def push_program(self, node):
      if node < self.nodes:
          print "\tprogram push to node %s:%d" % (self.address, self.port+node)
          if self.send(self.myaddress, self.address+":"+str(self.port+node), 
                       "overlog", self.program) == 0:
              return
          if node+1 < self.nodes: 
              self.set_delay(flags["delay"], lambda: self.push_program(node+1)) 
          else:
              self.set_delay(10, lambda: sys.exit(0)) 
  def callback(self):
      self.set_delay(0, self.delay_callback) 
  def delay_callback(self):
      # Read string from terminal and send it in a tuple
      line = raw_input("%s >> " % self.mode) 
      if   line[0:5] == "print":
          print self.program
      elif line[0:5] == "clear":
          self.program = ""
      elif line[0:5] == "osend" or line[0:5] == "ssend":
          if not self.address:
              print "ERROR: no address entered!!!"
              self.print_usage()
          else:
              if line[0] == "o":
                  v = self.send(self.myaddress, self.address, "overlog", self.program)
              else: 
                  v = self.send(self.myaddress, self.address, "script", self.program)
              if v > 0:
                  self.set_delay(1, self.delay_callback) 
              return
      elif line[0:4] == "exit":
          sys.exit(0)
      elif line == "." and self.mode != "terminal": 
          self.mode = "terminal" 
      elif self.mode == "terminal" and line[0:7] == "edit":
          self.mode = "edit" 
      elif self.mode == "terminal" and line[0:5] == "input":
          self.mode = "input" 
      elif self.mode == "terminal" and line[0:7] == "address":
          self.mode = "address" 
      elif self.mode == "input":
          try:
              file = open(line, 'r') 
              self.program += file.read()
              print "File %s text added to overlog program." % line
          except:
              print "ERROR: open file error on file", line
      elif self.mode == "address":
          self.address = line
          print "Address entered: ", self.address
      elif self.mode == "edit":
          self.program += line + "\n" 
      else:
          print "ERROR: unknown command or mode entered."
          self.print_usage()
      self.set_delay(0.1, self.delay_callback) 
  def push(self, port, tp, cb):
      # Received status of some sent tuple
      nodeID = Val_Str.cast(tp.at(1))
      status = Val_Int32.cast(tp.at(2))
      mesg   = Val_Str.cast(tp.at(3))

      print "=== RECEIVED STATUS FROM NODE %s ===" % nodeID
      if status == 0: print "Overlog installation successful!"
      else: print "Overlog installation failure!"
      print mesg
      return 1
  def send(self, src, dest, type, program):
      tuple = Tuple.mk()
      payload = Tuple.mk()
      tuple.append(Val_Str.mk(dest))
      payload.append(Val_Str.mk(type))
      payload.append(Val_Str.mk(dest))
      payload.append(Val_Str.mk(src))
      payload.append(Val_Str.mk(program))
      payload.freeze()
      tuple.append(Val_Tuple.mk(payload))
      tuple.freeze()
      return self.py_push(0, tuple, self.callback)
  

def print_usage():
    print
    print "Usage: terminal.py [-d delay] [-f <input_file> -n <nodes> -a <ip_address> -p <start_port>] <terminal_ip_address> <terminal_port>\n"
    print

def parse_cmdline(argv):
    shortopts = "d:f:n:a:p:"
    flags = {"delay" : 20, "debug" : False, "input" : None, "nodes" : 0, "ip" : None, "port" : 0}
    opts, args = getopt.getopt(argv[1:], shortopts)
    for o, v in opts:
        if o   == "-f": flags["input"] = v
        elif o == "-n": flags["nodes"] = int(v)
        elif o == "-a": flags["ip"]    = v
        elif o == "-p": flags["port"]  = int(v)
        elif o == "-d": flags["delay"]  = int(v)
        else:
            print_usage()
            exit(3)
    return flags, args

def get_test_stub():
    stub = r"""dataflow %s {
      TimedPushSource("source", 0) -> Print("output") -> Discard("output_discard");
      TimedPushSource("input", 0) -> Discard("input_discard");
      }
      .   # END OF DATAFLOW DEFINITION""" % DATAFLOW_NAME
    return stub

def get_stub(port):
    stub = r"""dataflow %s {
      let udp = Udp2("udp", %s);

      TimedPushSource("dummy_source", 0)            ->
      Sequence("output", 1, 1)                      ->
      Frag("fragment", 1)                           ->
      PelTransform("package", "$0 pop swallow pop") ->
      MarshalField("marshal", 1)                    ->
      StrToSockaddr("addr_conv", 0)                 -> 
      udp -> UnmarshalField("unmarshal", 1) ->
      PelTransform("unpackage", "$1 unboxPop") ->
      Defrag("defragment", 1) ->
      PelTransform("get_payload", "$2 unboxPop") ->
      TimedPullPush("input", 0) ->
      Discard("dummy_discard");
      }
      .   # END OF DATAFLOW DEFINITION""" % (DATAFLOW_NAME, port)
    return stub

def gen_stub(plumber, port):
    stub = get_stub(port) 

    dfparser.compile(plumber, stub)

    if dfparser.dataflows.has_key(DATAFLOW_NAME):
      m = dfparser.dataflows[DATAFLOW_NAME]
      m.eval_dataflow()
      return m.conf
    print "DATAFLOW COMPILATION PROBLEM"
    return None

if __name__ == "__main__":
    try:
      flags, args = parse_cmdline(sys.argv)
    except:
      print "EXCEPTION"
      print_usage()
      sys.exit(3)
    if len(args) < 2:
      print_usage()
      sys.exit(3)
  
    eventLoopInitialize()
  
    address  = args[0]
    port     = int(args[1])
  
    plumber = Plumber()
    stub    = gen_stub(plumber, port)

    if plumber.install(stub) != 0:
        print "** Stub Failed to initialize correct spec\n"

    edit   = plumber.new_dataflow_edit(DATAFLOW_NAME);
    input  = edit.find("input");
    output = edit.find("output");

    term = edit.addElement(Terminal(DATAFLOW_NAME, address+":"+str(port)))
    edit.hookUp(input, 0, term, 0)
    edit.hookUp(term, 0, output, 0)

    if flags["input"] and flags["nodes"] and flags["ip"] and flags["port"]:
        try:
            file = open(flags["input"], 'r') 
            term.element().program = file.read()
            print "File %s text added to overlog program." % flags["input"]
        except:
            print "ERROR: open file error on file", flags["input"]
        term.element().nodes   = flags["nodes"]
        term.element().address = flags["ip"]
        term.element().port    = flags["port"]

    if plumber.install(edit) != 0:
        print "Edit Correctly initialized.\n"

    # plumber.toDot("terminal.dot")
    # os.system("dot -Tps terminal.dot -o terminal.ps")
    # os.remove("terminal.dot")
    # Run the plumber
    eventLoop()
