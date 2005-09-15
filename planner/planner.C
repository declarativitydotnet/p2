/*
 * @(#)$Id$
 *
 * Copyright (c) 2005 Intel Corporation. All rights reserved.
 *
 * This file is distributed under the terms in the attached INTEL-LICENSE file.
 * If you do not find these files, copies can be found by writing to:
 * Intel Research Berkeley, 2150 Shattuck Avenue, Suite 1300,
 * Berkeley, CA, 94704.  Attention:  Intel License Inquiry.
 * 
 * DESCRIPTION: Planner code
 *
 */

#ifndef __PLANNER_C__
#define __PLANNER_C__


#include "planner.h"
#include "planContext.h"
#include "rulePlanner.C"

Planner::Planner(Router::ConfigurationRef conf, Catalog* catalog, bool debug, str nodeID) 
  : _conf(conf)
{ 
  _catalog = catalog; 
  _debug = debug; 
  _nodeID = nodeID;
  _ruleCount = 0;
  _netPlanner = new NetPlanner(conf, nodeID); 
}

std::vector<RuleStrand*> 
Planner::generateRuleStrands(ECA_ContextRef ectxt)
{
  std::vector<RuleStrand*> toRet;
  // go through each eca rule, form a rule strand
  for (unsigned k = 0; k < ectxt->_ecaRules.size(); k++) {
    ECA_Rule* nextRule = ectxt->_ecaRules.at(k);
    strbuf b; b << _ruleCount++ << "-" << _nodeID;
    RuleStrand *rs = New RuleStrand(nextRule, str(b));
    toRet.push_back(rs);
    PlanContext* pc = new PlanContext(_conf, _catalog, rs, _nodeID);
    generateEcaElements(pc);    
    delete pc;
  }
  return toRet;
}

void Planner::setupNetwork(ref<Udp> udp)
{
  // call the netplanner to generate network in and out
  // mux and demux not generated
  _netPlanner->generateNetworkElements(udp);
}


void Planner::registerRuleStrand(RuleStrand* rs)
{
  // register a single rule strand 
}

void Planner::registerAllRuleStrands(std::vector<RuleStrand*> ruleStrands)
{
  // call netplanner to create the right mux/demuxes
  _netPlanner->registerAllRuleStrands(ruleStrands);
}



#endif