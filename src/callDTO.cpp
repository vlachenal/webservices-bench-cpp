/*
 * Copyright © 2017 Vincent Lachenal
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#include "callDTO.hpp"

#include <map>
#include <mutex>
#include <sstream>


std::map<std::string,CallDTO> calls;
std::mutex lock;


// Functions +
CallDTO
initializeCall(int32_t reqSeq, const std::string& protocol, const std::string& method) {
  CallDTO call;
  if(reqSeq > 0) {
    call.requestSeq = reqSeq;
    call.protocol = &protocol;
    call.method = &method;
    call.initialized = true;
    call.serverStart = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    call.ok = false;
  }
  return call;
}

/*!
 * Compute call key
 *
 * \return the call key
 */
std::string
computeKey(const CallDTO& call) {
  std::ostringstream oss;
  oss << *call.protocol << '-' << *call.method << '-' << call.requestSeq;
  return oss.str();
}

void
registerCall(CallDTO& call) {
  if(call.initialized) {
    call.serverEnd = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    std::lock_guard<std::mutex> guard(lock);
    calls[computeKey(call)] = call;
  }
}

CallDTO
mergeCall(const CallDTO& call) {
  auto iter = calls.find(computeKey(call));
  if(iter == calls.end()) {
    return CallDTO();
  } else {
    CallDTO& regCall = iter->second;
    regCall.clientStart = call.clientStart;
    regCall.clientEnd = call.clientEnd;
    regCall.ok = call.ok;
    regCall.errMsg = call.errMsg;
    return regCall;
  }
}

void
cleanCalls() {
  std::lock_guard<std::mutex> guard(lock);
  calls.clear();
}
// Functions -


// Constructors +
CallDTO::CallDTO():
  initialized(false),
  requestSeq(-1),
  protocol(NULL),
  method(NULL),
  clientStart(-1),
  serverStart(-1),
  serverEnd(-1),
  clientEnd(-1),
  ok(false),
  errMsg() {
  // Nothing to do
}

CallDTO::CallDTO(const CallDTO& other):
  initialized(other.initialized),
  requestSeq(other.requestSeq),
  protocol(other.protocol),
  method(other.method),
  clientStart(other.clientStart),
  serverStart(other.serverStart),
  serverEnd(other.serverEnd),
  clientEnd(other.clientEnd),
  ok(other.ok),
  errMsg(other.errMsg) {
  // Nothing to do
}
// Constructors -


// Destructor +
CallDTO::~CallDTO() {
  // Nothing to do
}
// Destructor -
