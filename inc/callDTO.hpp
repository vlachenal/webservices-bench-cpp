/*
 * Copyright © 2017 Vincent Lachenal
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#ifndef CALL_DTO_HPP
#define CALL_DTO_HPP

#include <string>


/*!
 * Call DTO
 *
 * \author Vincent Lachenal
 */
struct CallDTO {

  // Attributes +
  /*! Is initiliazed */
  bool initialized;

  /*! Request sequence */
  int32_t requestSeq;

  /*! Protocol */
  const std::string* protocol;

  /*! Method */
  const std::string* method;

  /*! Client start in ms */
  uint64_t clientStart;

  /*! Server start in ms */
  uint64_t serverStart;

  /*! Server end in ms */
  uint64_t serverEnd;

  /*! Client end in ms */
  uint64_t clientEnd;

  /*! Call status */
  bool ok;

  /*! Call error message */
  std::string errMsg;
  // Attributes -


  // Constructors +
  /*!
   * \ref CallDTO default constructor
   */
  CallDTO();

  /*!
   * \ref CallDTO default constructor
   *
   * \param other the \ref CallDTO to copy
   */
  CallDTO(const CallDTO& other);
  // Constructors -


  // Destructor +
  /*!
   * \ref CallDTO destructor
   */
  virtual ~CallDTO();
  // Destructor -

};

/*!
 * Initialize call if request sequence is greater than 0
 *
 * \param reqSeq the request sequence
 * \param protocol the protocol
 * \param method the method
 */
extern CallDTO initializeCall(int32_t reqSeq, const std::string& protocol, const std::string& method);

/*!
 * Register call if it has been initialized
 *
 * \param call the call to register
 */
extern void registerCall(CallDTO& call);

/*!
 * Register call if it has been initialized
 *
 * \param call the call to register
 *
 * \return the merged call
 */
extern CallDTO mergeCall(const CallDTO& call);

/*!
 * Clean registered calls
 */
extern void cleanCalls();

#endif // CALL_DTO_HPP
